import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os
import time
import argparse

argParser = argparse.ArgumentParser(description = "Argument parser")
argParser.add_argument('--file', action='store', default="none", help="A file which contains the run numbers.")
argParser.add_argument('--shift', action='store', default="0", help="A shift applied to the oscilloscope id.")
argParser.add_argument('--power_mode', action='store', default="default", choices = ["default", "low", "medium", "high"], help="The power mode at which the setup has been running.")
argParser.add_argument('--offset', action='store', default=0, help="The offset above baseline that has been applied.")
argParser.add_argument('--download', action='store_true', default=False, help="Download the ETROC and oscilloscope data from the DAQ computer.")
args = argParser.parse_args()

etroc_data_base = "./ETROC_output_box_setup/"
scope_data_base = "./ETROC_LecroyScope/"

def get_index_list(pattern_number):
    index_list = []
    etroc_pattern = f"output_run_{pattern_number}*.dat"
    filenames = glob.glob(etroc_data_base + etroc_pattern)
    index_list = [int(f.split(etroc_data_base)[1].split("output_run_")[1].split("_rb0.dat")[0]) for f in filenames]
    sorted_indices = np.sort(index_list)
    scope_filenames = glob.glob(f"./{scope_data_base}/C1*{pattern_number}*")
    scope_index_list = np.sort([int(i.split("C1--Cosmics_")[1].split(".trc")[0]) for i in scope_filenames])
    filtered_runs = []
    n_scope_files = []
    for i in range(len(sorted_indices)):
        print(sorted_indices[i])
        if (i == 0):
            n_scope_files.append(np.sum(scope_index_list<=sorted_indices[i]))
        else:
            n_scope_files.append(np.sum((scope_index_list<=sorted_indices[i]) & (scope_index_list>sorted_indices[i-1])))
        if n_scope_files[i] == 30:
            filtered_runs.append(sorted_indices[i])
    return filtered_runs

if __name__ == "__main__":
    power_mode_id = {"default": 16, "low": 10, "medium": 12, "high": 14}
    start_time = time.time()
    pattern_number = f"809018{args.offset}{power_mode_id[args.power_mode]}00000"
    etroc_pattern = f"output_run_{pattern_number}*.dat"
    index_list = []
    if (args.file == "none"):
        index_list = get_index_list(pattern_number)
    else:
        index_list = open(args.file, "r").read().split("\n")
    print(index_list)

    if args.download:
        os.system(f"scp daq@timingdaq03:/home/daq/ETROC2_Test_Stand/module_test_sw/ETROC_output/*{pattern_number}* ./ETROC_output_box_setup/")
        for i in range(1,9):
            os.system(f"scp daq@timingdaq03:/home/daq/LecroyMount/C{i}--Cosmics_{pattern_number}*.trc ./ETROC_LecroyScope/")

    iter = 0
    for file_index in index_list:
        current_file = f"./MergedData/run_{file_index}.root"
        print(current_file)
        if not os.path.exists(current_file):
            os.system(f"bash full_conversion_chain.sh {file_index} {args.shift}")
        iter += 1
    end_time = time.time()

    print(f"Number of processed files: {iter}.")
    print(f"Duration of the processing: {end_time-start_time} [s].")
