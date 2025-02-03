import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

if __name__ == "__main__":
    base = os.getcwd() + "/ETROC_output_box_setup/"
    # filenames = glob.glob(base + "output_run_90181*_rb0.dat")
    filenames = glob.glob(base + "output_run_8090184*.dat")
    iter = 0
    for f in filenames:
        file_index = f.split(base)[1].split("output_run_")[1].split("_rb0.dat")[0]
        current_file = f"./MergedData/run_{file_index}.root"
        print(current_file)
        if not os.path.exists(current_file):
            os.system(f"bash full_conversion_chain.sh {file_index}")
        iter += 1