import uproot
import matplotlib.pyplot as plt
import numpy as np
import os.path as path
from scipy.optimize import curve_fit
import awkward as ak
import argparse
import glob
import time
import pdb
import os

def linear(x, a, b):
    return a*x + b

def find_cross_x(array, start, direction):
    minima = np.min(array)
    maxima = np.max(array)
    min_scale = np.abs(maxima - minima)/10.0
    max_scale = np.abs(maxima - minima)*9.0/10.0
    crossing_point = start
    i = start
    while i < len(array):
        if i != 0 and (array[i] - array[i - 1]) > min_scale and direction < 0 and (array[i] - min(array)) > min_scale and (array[i] - min(array)) < max_scale:
            crossing_point = i
            break
        if i != 0 and (array[i - 1] - array[i]) > min_scale and direction < 0 and (array[i] - min(array)) > min_scale and (array[i] - min(array)) < max_scale:
            crossing_point = i
            break
        else:
            i+=direction
    return crossing_point

def add_clock(tree):
    correction_offset = tree['timeoffsets'].array()
    channel = tree['channel'].array()
    time = tree['time'].array()
    nSamples = len(time)
    clocks = channel[:,6]
    triggers = channel[:,7]
    times = np.array(time[:,0])*10**9
    clock = np.array(clocks)
    minima = np.tile(np.min(clock, axis=1).reshape(-1,1), (1, len(clock[0])))
    maxima = np.tile(np.max(clock, axis=1).reshape(-1,1), (1, len(clock[0])))
    amp_fraction = 20 # %
    amp = minima + np.abs(minima - maxima)*amp_fraction/100

    min_scale = np.abs(maxima - minima)/10.0

    clock_diff = np.diff(clock, append=0)
    clock_diff_mask = clock_diff > min_scale
    # true after indices
    check_prior_fall = clock_diff < -min_scale
    prior_indices = np.argmax(check_prior_fall, axis=1)

    prior_fall_mask = np.arange(check_prior_fall.shape[1]) >= prior_indices[:, None]

    global_mask = clock_diff_mask & prior_fall_mask

    times = np.where(global_mask, times, 0)
    clock = np.where(global_mask, clock, 0)
    # delete 0 values for each row
    times = ak.Array([sublist[sublist != 0] for sublist in times])
    clock = ak.Array([sublist[sublist != 0] for sublist in clock])

    # print(times)
    time_slope = times[:,1] - times[:,0]
    clock_slope = clock[:,1] - clock[:,0]
    slope = clock_slope / time_slope
    ybias = clock[:,0] - slope*times[:,0]

    # calculate 20% of the amplitude
    amp = (minima + np.abs(minima - maxima)*amp_fraction/100)[:,0]
    clock_timestamp = np.array((amp - ybias) / slope) + correction_offset[:, 6]*10**9
    return clock_timestamp

def merge_trees(files, trees, output_file):
    ts = [uproot.open(files[t])[tree] for t, tree in enumerate(trees)]
    datas = [t.arrays() for t in ts]

    merged_data  = {}
    common_keys  = []
    other_keys_1 = []
    other_keys_2 = []

    for data in datas:
        for key in data.fields:
            if key not in merged_data.keys():
                # print(key, len(data[key]))
                merged_data[key] = data[key]

    print(merged_data.keys(), len(merged_data.keys()))

    with uproot.recreate(output_file) as output:
        try:
            output[trees[0]] = {key: merged_data[key] for key in merged_data.keys()}
            print(output[trees[0]].num_entries)
            print("Merging done!")
        except:
            for key in merged_data.keys():
                print(key, len(merged_data[key]))
            print("The number of the events doesn't match and cannot be merged.")


if __name__ == "__main__":

    argParser = argparse.ArgumentParser(description = "Argument parser")

    argParser.add_argument('--input_file_index', action='store', default='', help="Binary file")

    args = argParser.parse_args()

    base = "/home/aram/cosmic_data_analysis"
    f_index = args.input_file_index
    print(f_index, "\n")
    prev_status = 0

    reco_tree   = f"{base}/Scope_data_combined/run_{f_index}.root"
    etroc_tree  = f"{base}/ETROC_output/output_run_{f_index}_rb0.root"
    merged_file = f"{base}/MergedData/run_{f_index}.root"

    reco = (path.isfile(reco_tree))
    etroc = (path.isfile(etroc_tree))
    status = (reco & etroc)

    print( "                                    File")
    print(f"Acquisition from the KCU done:    {etroc}")
    print(f"Conversion done:                  {reco}")
    print(f"Merged file wasn't created:      {not path.isfile(merged_file)}")
    print()

    if status:
        time.sleep(10)
        merge_trees([reco_tree, etroc_tree], ["pulse", "pulse"], merged_file)
        time.sleep(1)
    else:
        if not reco:
            print("The reconstructed scope file has not been made!")
        if not etroc:
            print("The ETROC file has not been made!")
