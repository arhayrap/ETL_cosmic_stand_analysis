import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

if __name__ == "__main__":
    index_list = open("index_list.txt", "r").read().split("\n")
    print(index_list)
    iter = 0
    for file_index in index_list:
        current_file = f"./MergedData/run_{file_index}.root"
        print(current_file)
        if not os.path.exists(current_file):
            os.system(f"bash full_conversion_chain.sh {file_index}")
        iter += 1
