import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os
import time
import argparse

argParser = argparse.ArgumentParser(description = "Argument parser")
argParser.add_argument('--file', action='store', default="none", help="A file which contains the run numbers.")
args = argParser.parse_args()

if __name__ == "__main__":
    start_time = time.time()
    base = "./ETROC_output_box_setup/"
    pattern = "output_run_8090184000*.dat"
    index_list = []
    if (args.file == "none"):
        filenames = glob.glob(base + pattern)
        index_list = f.split(base)[1].split("output_run_")[1].split("_rb0.dat")[0]
    else:
        index_list = open(args.file, "r").read().split("\n")
    iter = 0
    for file_index in index_list:
        current_file = f"./MergedData/run_{file_index}.root"
        print(current_file)
        if not os.path.exists(current_file):
            os.system(f"bash full_conversion_chain.sh {file_index}")
        iter += 1
    end_time = time.time()
    print(f"Number of processed files: {iter}.")
    print(f"Duration of the processing: {end_time-start_time} [s].")
