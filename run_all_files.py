import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

if __name__ == "__main__":
    base = os.getcwd() + "/ETROC_output/"
    filenames = glob.glob(base + "output_run_90181*_rb0.dat")
    for f in filenames:
        file_index = f.split(base)[1].split("output_run_")[1].split("_rb0.dat")[0]
        print(file_index)
        os.system(f"bash full_conversion_chain.sh {file_index}")