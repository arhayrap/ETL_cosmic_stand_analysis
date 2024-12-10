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

raw_path = "/home/daq/LecroyMount/"  # PATH TO THE SCOPE RAW DATA FOLDER
d = 1.2 # max(signal[s]) # (max(signal[s]) - min(signal[s]))

argument_holder = argparse.ArgumentParser(description = "Argument parser")
argument_holder.add_argument("--plotting", action="store_true", default = False, help = "Plot the fits of the clock waveform.")
argument_holder.add_argument("--run_number", action="store", help = "The run number.", type = int)
argument_holder.add_argument("--min", action="store", help = "The lowest bound of the fit.", type = float)
argument_holder.add_argument("--max", action="store", help = "The highest bound of the fit.", type = float)
argument_holder.add_argument("--threshold", action="store", help = "The applied threshold.", type = float)
argument_holder.add_argument('--input_file_index', action='store', default='', help="Binary file")

args = argument_holder.parse_args()

# clk_bound_max = 1.0 # V
# clk_bound_min = 0.2 # V
# clk_threshold = 0.5 # V

clk_bound_max = args.max # V
clk_bound_min = args.min # V
clk_threshold = args.threshold # V

def linear(x, a, b):
    return a*x + b

def linear_fit_rising_edge(signal, time_axis, frequency, sampling_rate, which_peak = "first"):
    # Calculate the time period
    # Provide frequency in Herz
    time_period = (1 / frequency) * 10**9
    # print(time_period)
    half_period_samples = int((time_period / 2) * sampling_rate)
    
    # rising_edges_i = []
    maxes = []
    mins = []
    slopes = []
    biases = []
    rising_edges_mV = []
    rising_edges_t  = []
    N_peaks_before_max = []
    N_peaks_before_min = []
    N_peaks_after = []
    peak_index_min = []
    peak_index_max = []
    n_points_rising_edge = []
    n_fit_points = []

    for s in range(len(signal)): # /////////////////////////////////////////////////////////////// LOOP OVER THE EVENTS (WAVEFORMS)
        print(f"{s} / {len(signal)}")
        bias  = []
        slope = []
        
        # Find peaks
        peaks = []
        
        amp_step   = d * 1 / 1000
        
        height_min = clk_bound_min # d * prop_min / 100 # min(signal[s]) + d * prop_min / 100
        height_max = clk_bound_max # d - d * prop_max / 100 # max(signal[s]) - d * prop_max / 100
        
        p_min, _ = find_peaks(-signal[s], height = (-1*height_min, -1*min(signal[s])), distance=int(len(signal[s])/12))
        p_max, _ = find_peaks(-signal[s], height = (-1*max(signal[s]), -1*height_max), distance=int(len(signal[s])/12))
        print(p_min, p_max)
        N_peaks_before_max.append(len(p_max))
        N_peaks_before_min.append(len(p_min))
        if p_max[0] < p_min[0]:
            # peaks.append(p_max[1])
            p_max = p_max[1::]
        if p_max[-1] < p_min[-1]:
            # peaks.append(p_max[0])
            p_min = p_min[0:-1]
        # print(p_min)
        # print(p_max)
        N_peaks_after.append(len(p_max))
        
        amp_step = d * 1 / 1000
        
        r_e_start = []
        r_e_end   = []
        rising_edge_mV = []
        rising_edge_t = []
        n_f_points = []
        print(min(len(p_max), len(p_min)))
        # print(f"The length of the max and the min: {len(p_max)}, {len(p_min)}.")
        for p in range(min(len(p_max), len(p_min))): # /////////////////////////////////////////////////////////////// LOOP OVER THE PEAKS
            # height_min = d * prop_min / 100 # signal[s][p_min][p] + d[p] * prop_min / 100
            # height_max = d - d * prop_max / 100 # signal[s][p_max][p] - d[p] * prop_max / 100
            start = p_min[p]
            end   = p_max[p]
            print(start)
            print(end)
            print(len(time_axis))
            print(height_min, height_max)
            print()
            rising_edge_start = start
            for i in range(start, end - 1):
                if signal[s][i] < signal[s][i+1] and abs(signal[s][i] - signal[s][i+1]) > amp_step and signal[s][i] >= height_min:
                    rising_edge_start = i
                    print("---------------------------->    ", rising_edge_start)
                    break
            r_e_start.append(rising_edge_start)
            rising_edge_end = rising_edge_start
            for i in range(rising_edge_start, end):
                print(signal[s][i])
                if signal[s][i] >= height_max:
                    rising_edge_end = i
                    print("---------------------------->    ", rising_edge_end)
                    break
            r_e_end.append(rising_edge_end)
            print(r_e_start)
            print(r_e_end)
            rising_edge_mV.append(signal   [s][r_e_start[p]:r_e_end[p]])
            rising_edge_t .append(time_axis[s][r_e_start[p]:r_e_end[p]])
            # popt, _ = curve_fit(linear, [rising_edge_t[p][0], rising_edge_t[p][-1]], [rising_edge_mV[p][0], rising_edge_mV[p][-1]])
            popt, _ = curve_fit(linear, rising_edge_t[p], rising_edge_mV[p])
            n_f_points.append(len(signal[s][r_e_start[p]:r_e_end[p]]))
            slope.append(popt[0])
            bias.append(popt[1])
        rising_edges_mV.append(list(rising_edge_mV))
        rising_edges_t.append(list(rising_edge_t))
        slopes.append(slope)
        biases.append(bias)
        maxes.append(r_e_end)
        mins.append(r_e_start)
        peak_index_min.append(p_min)
        peak_index_max.append(p_max)
        n_fit_points.append(n_f_points)

    return slopes, biases, rising_edges_mV, rising_edges_t, N_peaks_after, peak_index_min, peak_index_max, n_fit_points

def linear(x, a, b):
    return a*x + b

def add_clock(tree, frequency = 40*10**6, plotting = False): # Hz
    correction_offset = tree['timeoffsets'].array() # [0:n_of_e]
    channel = tree['channel'].array() # [0:n_of_e]
    time = tree['time'].array() # [0:n_of_e]
    nSamples = len(time)
    # clocks = channel[:,2]
    # triggers = channel[:,1]
    clocks = channel[:,7]
    triggers = channel[:,7]
    times = np.array(time[:,0])*10**9
    clock = np.array(clocks)
    print(clock)
    slopes, biases, rising_edges_mV, rising_edges_t, n_points_rising_edge, n_peaks_before_truncation_max, n_peaks_before_truncation_min, n_peaks_after_truncation, peak_index_min, peak_index_max = linear_fit_rising_edge(clock, times, frequency, 1)
    # slopes, biases, rising_edges_mV, rising_edges_t, n_peaks_after_truncation, peak_index_min, peak_index_max, n_fit_points = linear_fit_rising_edge(clock, times, frequency, 1)
    amp_fraction = 20 # %
    clock_timestamps = []
    clock_timestamp_differences = []
    amps = []
    min_rising_edge_t = []
    max_rising_edge_t = []
    min_rising_edge_mV = []
    max_rising_edge_mV = []
    for s in range(len(slopes)): # loop over the events
        print(f"{s} / {len(slopes)}")
        t_stamps = []
        a = []
        mins_t = []
        maxes_t = []
        mins_mV = []
        maxes_mV = []
        clock_timestamp_difference = []

        if plotting:
            plt.figure(figsize = (15, 12))
            plt.plot(times[s], clock[s])
            plt.scatter(times[s], clock[s], marker = "o", color = "orange")

        for i in range(len(slopes[s])): # loop over the peaks
            minima = min(rising_edges_mV[s][i])
            maxima = max(rising_edges_mV[s][i])
            min_index = np.argmin(rising_edges_mV[s][i])
            max_index = np.argmax(rising_edges_mV[s][i])
            mins_t.append(rising_edges_t[s][i][min_index])
            maxes_t.append(rising_edges_t[s][i][max_index])
            mins_mV.append(rising_edges_mV[s][i][min_index])
            maxes_mV.append(rising_edges_mV[s][i][max_index])
            # d = 1.2 # rising_edges_mV[s][0][max_index] - rising_edges_mV[s][0][min_index]
            height_min = clk_bound_min # d * prop_min / 100 # min(signal[s]) + d * prop_min / 100
            height_max = clk_bound_max # d - d * prop_max / 100 # max(signal[s]) - d * prop_max / 100
            amp = clk_threshold # height_min + np.abs(height_max - height_min) * 10 / 100.0
            a.append(amp)
            clock_timestamp = ((amp - biases[s][i]) / slopes[s][i]) + correction_offset[:,7][s]*10**9
            t_stamps.append(clock_timestamp)

            if plotting:
                plt.plot(times[s], [minima]*len(clock[s]), "--", color="blue")
                plt.plot(times[s], [maxima]*len(clock[s]), "--", color="red")
                plt.plot(times[s][peak_index_min[s]], clock[s][peak_index_min[s]], "*", color="blue", markersize = 20)
                plt.plot(times[s][peak_index_max[s]], clock[s][peak_index_max[s]], "*", color="red", markersize = 20)
                plt.plot(rising_edges_t[s][i], rising_edges_mV[s][i], "*", color = "blue")
                plt.plot(rising_edges_t[s][i], slopes[s][i] * rising_edges_t[s][i] + biases[s][i], color = "red")

            for t in range(len(peak_index_min[s])):
                # d = np.abs(clock[s][peak_index_max[s][t]] - clock[s][peak_index_min[s][t]])
                # plt.plot(times[s], [clock[s][peak_index_min[s][t]] + d * (prop_min / 100)]*len(clock[s]), "--", color="gray") # Lower limit
                # plt.plot(times[s], [clock[s][peak_index_max[s][t]] - d * (prop_max / 100)]*len(clock[s]), "--", color="gray") # Upper limit
                if plotting:
                    # plt.plot(times[s], [max(clock[s]) * (prop_max / 100)]*len(clock[s]), "--", color="gray") # Lower limit
                    # plt.plot(times[s], [max(clock[s]) - max(clock[s]) * (prop_max / 100)]*len(clock[s]), "--", color="gray") # Upper limit
                    plt.plot(times[s], [clk_bound_min]*len(clock[s]), "--", color="gray") # Lower limit
                    plt.plot(times[s], [clk_bound_max]*len(clock[s]), "--", color="gray") # Upper limit
            
        clock_timestamps.append(t_stamps)
        amps.append(a)
        min_rising_edge_t.append(mins_t)
        max_rising_edge_t.append(maxes_t)
        min_rising_edge_mV.append(mins_mV)
        max_rising_edge_mV.append(maxes_mV)

        if len(clock_timestamps[s]) > 1:
            for p in range(1, len(clock_timestamps[s])):
                # clock_timestamp_difference.append(np.abs(clock_timestamps[s][p] - clock_timestamps[s][p-1]))
                clock_timestamp_difference.append(np.abs(clock_timestamps[s][p] - clock_timestamps[s][p-1]))
                if plotting:
                    plt.plot(np.linspace(clock_timestamps[s][p-1], clock_timestamps[s][p-1] + (np.abs(clock_timestamps[s][p] - clock_timestamps[s][p-1])), 100), [amps[s][p]]*100, color = "black")
                    plt.plot([clock_timestamps[s][p-1], clock_timestamps[s][p-1] + (np.abs(clock_timestamps[s][p] - clock_timestamps[s][p-1]))], [amps[s][p-1]]*2, color = "brown", marker = "o", linestyle = "-", linewidth = 4)
                    plt.plot([clock_timestamps[s][p-1], clock_timestamps[s][p]], [amps[s][p-1], amps[s][p]], color = "black", marker = "o", linestyle = "-", linewidth = 2)
        else:
            clock_timestamp_difference.append(-10)

        
        if plotting:
            plt.savefig(f"./peaks_fit/plot_{s}.png")
            plt.close()
        
        clock_timestamp_differences.append(clock_timestamp_difference)
        
    return clock_timestamps,clock_timestamp_differences, n_peaks_after_truncation, n_fit_points

if __name__ == "__main__":

    base = "/home/aram/cosmic_data_analysis"
    f_index = args.input_file_index
    print(f_index, "\n")
    prev_status = 0
    height_min = clk_bound_min # d * prop_min / 100 # min(signal[s]) + d * prop_min / 100
    height_max = clk_bound_max # d - d * prop_max / 100 # max(signal[s]) - d * prop_max / 100

    # reco_tree   = f"{base}/Scope_data_combined_reco/run_{f_index}.root"
    # etroc_tree  = f"{base}/ETROC_output/output_run_{f_index}_rb0.root"
    conv_tree   = f"{base}/Scope_data_combined_conv/converted_run9018100002460.root"
    # merged_file = f"{base}/MergedData/run_{f_index}.root"
    tree = uproot.open(conv_tree)["pulse"].arrays()
    clock_signal = tree["channel"][:, 7]
    time = tree["time"][:, 0]
    print(len(clock_signal), len(clock_signal[0]))
    print(len(time), len(time[0]))
    print(clock_signal)
    # reco = (path.isfile(reco_tree))
    # etroc = (path.isfile(etroc_tree))
    # status = (reco & etroc)
    s = 0
    p_min, _ = find_peaks(-clock_signal[s], height = (-1*height_min, -1*min(clock_signal[s])), distance=int(len(clock_signal[s])/12))
    p_max, _ = find_peaks(-clock_signal[s], height = (-1*max(clock_signal[s]), -1*height_max), distance=int(len(clock_signal[s])/12))
    print(p_min)
    print(p_max)
    plt.plot(time[0], clock_signal[0])
    # plt.scatter(range(), p_min, color = "blue")
    # plt.scatter(p_max, color = "red")
    plt.show()
    
    

