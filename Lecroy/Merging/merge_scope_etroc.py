import uproot
import matplotlib.pyplot as plt
import numpy as np
import os.path as path
from scipy.optimize import curve_fit
from scipy.signal import find_peaks
import awkward as ak
import argparse
import glob
import time
import pdb
import os
import sys


d = 1.2 # max(signal[s]) # (max(signal[s]) - min(signal[s]))

argument_holder = argparse.ArgumentParser(description = "Argument parser")
argument_holder.add_argument("--plotting", action="store_true", default = False, help = "Plot the fits of the clock waveform.")
argument_holder.add_argument("--run_number", action="store", help = "The run number.", type = int)
argument_holder.add_argument("--min", action="store", help = "The lowest bound of the fit.", type = float)
argument_holder.add_argument("--max", action="store", help = "The highest bound of the fit.", type = float)
argument_holder.add_argument("--threshold", action="store", help = "The applied threshold.", type = float)
argument_holder.add_argument("--nevents", action="store", help = "The number of events in a batch.", default = 5000, type = int)
argument_holder.add_argument('--input_file_index', action='store', default='', help="The run number of the merging files.")
argument_holder.add_argument('--n_peaks', action='store', default=2, help="Number of expected clock peaks.")
argument_holder.add_argument('--niters', action='store', default=-1, help="Number of iterations.")

args = argument_holder.parse_args()
# Constants from arguments
CLK_BOUND_MAX = args.max
CLK_BOUND_MIN = args.min
CLK_THRESHOLD = args.threshold
f_index = args.input_file_index
NEVENTS = int(args.nevents)
expected_number_of_peaks = int(args.n_peaks)
n_iterations = int(args.niters)
print(expected_number_of_peaks)

def linear(x, a, b):
    return a*x + b

def linear_fit_rising_edge(signal, time_axis, frequency, sampling_rate, which_peak="first"):
    """
    Fits the rising edges of the waveform.
    Args:
        signal (list of numpy.ndarray): List of waveforms.
        time_axis (list of numpy.ndarray): Corresponding time axes for the waveforms.
        frequency (float): Frequency in Hz.
        sampling_rate (float): Sampling rate in samples per second.
    Returns:
        dict: A dictionary with slopes, biases, and other fit parameters.
    """   
    time_period_ns = (1 / frequency) * 1e9
    half_period_samples = int((time_period_ns / 2) * sampling_rate)

    results = {
        "slopes": [], "biases": [], "rising_edges_mV": [], "rising_edges_t": [],
        "N_peaks_after": [], "peak_index_min": [], "peak_index_max": [], "n_fit_points": []
    }

    for waveform_idx, waveform in enumerate(signal):
        # Positive signal
        waveform = np.abs(waveform - 1.2)
        p_min, _ = find_peaks(waveform, height=(min(waveform), CLK_BOUND_MIN), distance=int(len(waveform)/4))
        p_max, _ = find_peaks(-waveform, height=(-max(waveform), -CLK_BOUND_MAX), distance=int(len(waveform)/4))

        if len(p_max) > 0 and len(p_min) > 0:
            if p_max[0] < p_min[0]:
                p_max = p_max[1:]
            if p_max[-1] < p_min[-1]:
                p_min = p_min[:-1]

        results["N_peaks_after"].append(len(p_max))
        results["peak_index_min"].append(p_min)
        results["peak_index_max"].append(p_max)

        rising_edges_mV = []
        rising_edges_t = []
        slopes = []
        biases = []
        n_fit_points = []

        for start, end in zip(p_min, p_max):

            rising_edge_start = next(
                (i for i in range(start, end - 1) if (CLK_BOUND_MIN - abs(waveform[i])) < 0),
                start
            ) - 1

            rising_edge_end = next(
                (i for i in range(rising_edge_start, end) if (CLK_BOUND_MAX - abs(waveform[i])) < 0),
                end
            ) - 1

            edge_mV = waveform[rising_edge_start:rising_edge_end]
            edge_t = time_axis[waveform_idx][rising_edge_start:rising_edge_end]

            if len(edge_t) > 1:
                popt, _ = curve_fit(linear, edge_t, edge_mV)
                slopes.append(popt[0])
                biases.append(popt[1])
                n_fit_points.append(len(edge_t))
            else:
                slopes.append(0)
                biases.append(0)
                n_fit_points.append(0)

            rising_edges_mV.append(edge_mV)
            rising_edges_t.append(edge_t)

        results["slopes"].append(slopes)
        results["biases"].append(biases)
        results["rising_edges_mV"].append(rising_edges_mV)
        results["rising_edges_t"].append(rising_edges_t)
        results["n_fit_points"].append(n_fit_points)

    return results


def add_clock(tree, frequency=40e6, plotting=False):
    """
    Process clock signals and extract timestamps.
    Args:
        tree (dict): Input data tree containing waveforms and metadata.
        frequency (float): Clock frequency in Hz.
        plotting (bool): Whether to enable plotting.
    Returns:
        tuple: Processed clock timestamps, timestamp differences, and other metadata.
    """
    correction_offset = tree['timeoffsets']
    channel = ak.to_numpy(tree['channel'])
    time = tree['time']
    clock = channel[:, 7, :]
    times = np.array(time[:, 0]) * 1e9

    results = linear_fit_rising_edge(clock, times, frequency, 1)

    clock_timestamps = []
    clock_timestamp_differences = []

    for event_idx, slopes in enumerate(results["slopes"]):
        event_timestamps = []

        if plotting:
            plt.figure(figsize=(15, 10))
            plt.plot(times[event_idx], clock[event_idx], label="Clock Signal")
            plt.title(f"Event {event_idx} Clock Signal and Fits")
            plt.xlabel("Time (ns)")
            plt.ylabel("Amplitude (mV)")

        for i, slope in enumerate(slopes):
            amp = CLK_THRESHOLD
            timestamp = ((amp - results["biases"][event_idx][i]) / slope) + correction_offset[:, 7][event_idx] * 1e9
            event_timestamps.append(timestamp)

            if plotting:
                rising_edge_t = results["rising_edges_t"][event_idx][i]
                rising_edge_mV = results["rising_edges_mV"][event_idx][i]
                plt.plot(
                    rising_edge_t,
                    slope * np.array(rising_edge_t) + results["biases"][event_idx][i],
                    label=f"Fit {i}",
                )
                # Add a black vertical line at the calculated timestamp
                plt.axvline(x=timestamp, color="black", linestyle="--", label=f"Timestamp {i}")

                plt.scatter(rising_edge_t, rising_edge_mV, label=f"Rising Edge {i}")

        clock_timestamps.append(event_timestamps)

        if len(event_timestamps) > 1:
            differences = [
                abs(event_timestamps[j] - event_timestamps[j - 1])
                for j in range(1, len(event_timestamps))
            ]
            clock_timestamp_differences.append(differences)
        else:
            clock_timestamp_differences.append([-10])

        if plotting:
            plt.legend()
            plt.savefig(f"./plots/event_{event_idx}.png")
            plt.close()

    return clock_timestamps, clock_timestamp_differences

def merge_trees(files, trees, output_file):
    ts = [uproot.open(files[t])[tree] for t, tree in enumerate(trees)]
    ts_Nevents= [tree.num_entries for tree in ts]
    batch_size = NEVENTS
    merged_data = {key: [] for key in set(ts[2].keys()).union(ts[2].keys())}
    iter = 0 
    for start in range(0, ts_Nevents[0], batch_size):
        if iter > n_iterations and n_iterations != -1: break
        stop = min(start + batch_size, ts_Nevents[0])  # Make sure we don't exceed the total number of events
        print(f"Loading events from {start} to {stop - 1}...")
        datas = [t.arrays(entry_start=start,entry_stop=stop) for t in ts]
        for key in ts[0].keys():
            merged_data[key]= datas[0][key]
        for key in ts[1].keys():
            merged_data[key] = datas[1][key]
        for key in ts[2].keys():
            merged_data[key] = datas[2][key]
            
        clock, t_diff = add_clock(datas[2], plotting = args.plotting)
        merged_data["Clock"] = clock
        merged_data["t_diff"] = t_diff

        print(merged_data.keys(), len(merged_data.keys()))

        with uproot.recreate(output_file+f"_i{iter}.root") as output:
            try:
                output[trees[0]] = {key: merged_data[key] for key in merged_data.keys()}
                print(output[trees[0]].num_entries)
                print("Merging done!")
            except:
                for key in merged_data.keys():
                    print(key, len(merged_data[key]))
                    print("The number of the events doesn't match and cannot be merged.")
        iter+=1
    
if __name__ == "__main__":
    """
    Process clock signals and extract timestamps.
    The clock signal expected here is a negative signal , which is coming directly from the converted Scope file in Volts (no Scope Reco in mV)
    """
    
    base = "../../"
    reco_tree   = f"{base}/Scope_data_combined_reco/run_{f_index}.root"
    etroc_tree  = f"{base}/ETROC_output_box_setup/output_run_{f_index}_rb0.root"
    conv_tree   = f"{base}/Scope_data_combined_conv/converted_run{f_index}.root"
    merged_file = f"{base}/MergedData/run_{f_index}.root"
    
    merge_trees([etroc_tree, reco_tree, conv_tree], ["pulse", "pulse", "pulse"], merged_file)
    print("Merging done!")
    