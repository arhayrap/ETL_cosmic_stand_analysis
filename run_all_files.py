import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os
import time
import argparse

argParser = argparse.ArgumentParser(description = "Argument parser")
argParser.add_argument('--file', action='store', default="none", help="A file which contains the run numbers.")
argParser.add_argument('--index', action='store', default="none", help="The run number.")
argParser.add_argument('--shift', action='store', default="0", help="A shift applied to the oscilloscope id.")
argParser.add_argument('--power_mode', action='store', default="default", choices = ["default", "low", "medium", "high"], help="The power mode at which the setup has been running.")
argParser.add_argument('--offset', action='store', default=0, help="The offset above baseline that has been applied.")
argParser.add_argument('--l1a_delay', action='store', default=18, help="The delay of signal arrival in DAC units.")
argParser.add_argument('--download', action='store_true', default=False, help="Download the ETROC and oscilloscope data from the DAQ computer.")
argParser.add_argument('--convert', action='store_true', default=False, help="Convert the downloaded information (mention which ones with the parameters below).")
argParser.add_argument('--etroc', action='store_true', default=False, help="Convert the downloaded etroc data.")
argParser.add_argument('--scope', action='store_true', default=False, help="Convert the downloaded scope data.")
argParser.add_argument('--merge', action='store_true', default=False, help="Merge the converted data from the etroc and scope.")
argParser.add_argument('--clock_fit_min', action='store', default=0.2, help="The minima of the clock fitting range.")
argParser.add_argument('--clock_fit_max', action='store', default=0.9, help="The maxima of the clock fitting range.")
argParser.add_argument('--clock_fit_thr', action='store', default=0.5, help="The threshold of the clock signal fit.")
argParser.add_argument('--pattern', action='store', default="8090", help="The prefix of the run number.")
argParser.add_argument('--daq', action='store', default="daq@timingdaq03", help="The address of the DAQ computer.")

args = argParser.parse_args()

etroc_data_base = "./ETROC_output_box_setup/"
scope_data_base = "./ETROC_LecroyScope/"
merged_data_base = "./MergedData/"
daq_etroc_data_location = "/home/daq/ETROC2_Test_Stand/module_test_sw/ETROC_output/"
daq_scope_data_location = "/home/daq/LecroyMount/"

def get_index_list(pattern_number, check_scope = False):
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
        run_number = int(str(sorted_indices[i]).split(pattern_number)[1])
        if (run_number < 3000): continue
        if (i == 0):
            n_scope_files.append(np.sum(scope_index_list<=sorted_indices[i]))
        else:
            n_scope_files.append(np.sum((scope_index_list<=sorted_indices[i]) & (scope_index_list>sorted_indices[i-1])))
        print(sorted_indices[i])
        if n_scope_files[-1] == 30 and check_scope:
            filtered_runs.append(sorted_indices[i])
        if not check_scope:
            filtered_runs.append(sorted_indices[i])
    return filtered_runs

if __name__ == "__main__":
    power_mode_id = {"default": 16, "low": 10, "medium": 12, "high": 14}
    start_time = time.time()
    pattern_number = f"{args.pattern}{args.l1a_delay}{args.offset}{power_mode_id[args.power_mode]}00000"
    etroc_pattern = f"output_run_{pattern_number}*.dat"
    current_dir = os.getcwd()
    index_list = []
    if (args.file == "none" and args.index == "none"):
        index_list = get_index_list(pattern_number)
    elif (args.file == "none" and args.index != "none"):
        index_list = [int(args.index)]
    elif (args.file != "none" and args.index != "none"):
        raise Exception("Please provide a filelist or a run number.")
    elif (args.file != "none" and args.index == "none"):
        index_list = open(args.file, "r").read().split("\n")
    print(index_list)

    if args.download:
        if (args.file == "none" and args.index == "none"):
            command = f"scp -r {args.daq}:{daq_etroc_data_location}*{pattern_number}* {etroc_data_base}"
            print(command)
            os.system(command)
            for i in range(1,9):
                pattern_number = f"{args.pattern}{args.l1a_delay}{args.offset}{power_mode_id[args.power_mode]}00000"
                command = f"scp -r {args.daq}:{daq_scope_data_location}C{i}--Cosmics_{pattern_number}*.trc {scope_data_base}"
                print(command)
                os.system(command)
        else:
            for index in range(len(index_list)):
                index = int(index)
                print(f"Downloading the data for run: {index_list[index]}.")
                command = f"scp -r {args.daq}:{daq_etroc_data_location}*{{"+f"{int(index_list[index])}..{int(index_list[index])+30}"+f"}}* {etroc_data_base}"
                print(command)
                os.system(command)
                for i in range(1,9):
                    pattern_number = f"{args.pattern}{args.l1a_delay}{args.offset}{power_mode_id[args.power_mode]}00000"
                    command = f"scp -r {args.daq}:{daq_scope_data_location}C{i}--Cosmics_{{"+f"{int(index_list[index])}..{int(index_list[index])+30}"+f"}}.trc {scope_data_base}"
                    print(command)
                    os.system(command)

    iter = 0
    for file_index in index_list:
        current_file = f"{merged_data_base}run_{file_index}.root"
        print(current_file)
        if not os.path.exists(current_file):
            if (args.etroc):
                os.system(f"python3 convert_data.py --input_file_index {file_index}")
            if (args.scope):
                os.chdir("./Lecroy/Conversion/")
                os.system(f"python3 recoLoop_1.py --input_file_index {file_index} --rb 0 --shift 0")
                os.chdir("../../")
            if (args.merge):
                os.chdir("./Lecroy/Merging/")
                os.system(f"python3 merge_scope_etroc.py --input_file_index {file_index} --min {args.clock_fit_min} --max {args.clock_fit_max} --threshold {args.clock_fit_thr}")
                os.chdir("../../")
        iter += 1
    if (args.etroc):
        hadd_inputs = [f"{etroc_data_base}output_run_{j}_rb0.root" for j in index_list]
        command = f"hadd -k -f {etroc_data_base}combined_pm_{power_mode_id[args.power_mode]}_offset_{args.offset}.root {' '.join(hadd_inputs)}"
        print(command)
        os.system(command)
    # if (args.scope):
    #     hadd_inputs = [f"{etroc_data_base}output_run_{j}_rb0.root" for j in index_list]
    #     command = f"hadd -k -f {etroc_data_base}combined_pm_{power_mode_id[args.power_mode]}_offset_{args.offset}.root {' '.join(hadd_inputs)}"

    end_time = time.time()
    print(f"Number of processed files: {iter}.")
    print(f"Duration of the processing: {end_time-start_time} [s].")
