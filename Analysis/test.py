import uproot
import awkward as ak
import numpy as np
import pandas as pd
from hist import Hist
import matplotlib.pyplot as plt

def mod(array):
    count = np.zeros(len(array))
    for i in range(len(array)):
        count[i]+=1
    return array[np.argmax(count)]

def create_1D_histogram(array, nbins, name, label):
    minimum = np.min(array)
    maximum = np.max(array[array!=np.inf])
    print(minimum, maximum)
    histogram = Hist( Hist.new.Reg(nbins, minimum, maximum, name = name, label = label) )
    histogram.fill(ak.flatten(array))
    return histogram

if __name__ == "__main__":
    file = uproot.open("../MergedData_no_clock/check.root")
    pulse = file["pulse"]
    nhits = pulse["nhits"].array()[:,0]
    row = pulse["row"].array()
    col = pulse["col"].array()
    cal = pulse["cal_code"].array()
    toa = pulse["toa_code"].array() * 3.125 / cal
    tot = (2 * pulse["tot_code"].array() - np.floor(pulse["tot_code"].array() / 32)) * (3.125 / cal)
    print(tot)
    cal_mask = np.zeros((16,16))
    cal_selection = (row >= 0).tolist()
    for i in range(0,16):     # row
        for j in range(0,16): # col
            flat_cal = ak.flatten(cal[(row==i)&(col==j)])
            cal_mask[i][j] = mod(flat_cal)
            # print(cal_selection[(row == i) & (col == j)])
            # print(cal[(row == i) & (col == j)] == cal_mask[i][j])
    print(cal_mask)
    # cal_selection = list()
    # for i in range(len(row)):
    #     sel = list()
    #     for j in range(nhits[i]):
    #         sel.append(cal[i][j] == cal_mask[row[i][j]][col[i][j]])
    #     cal_selection.append(sel)

    # cal_selection[(row == i) & (col == j)] = cal_selection[(row == i) & (col == j)] & (cal[(row == i) & (col == j)] == cal_mask[i][j])
    # tot[(row == i) & (col == j)] = tot[(row == i) & (col == j)][selection]
    # toa[(row == i) & (col == j)] = toa[(row == i) & (col == j)][selection]
    # cal[(row == i) & (col == j)] = cal[(row == i) & (col == j)][selection]
    # print(cal_mask)
    # print(cal_selection)
    # cal_selection = (row == cal_row) & (col == cal_col) & (cal == cal_mask)
    """
    tot_hist = create_1D_histogram(tot[cal_selection], 100, "tot", "TOT")
    toa_hist = create_1D_histogram(toa[cal_selection], 100, "toa", "TOA")
    cal_hist = create_1D_histogram(cal[cal_selection], 100, "cal", "CAL")
    
    tot_hist.plot()
    # plt.yscale("log")
    plt.savefig("TOT.png")
    plt.close()

    toa_hist.plot()
    # plt.yscale("log")
    plt.savefig("TOA.png")
    plt.close()

    cal_hist.plot()
    # plt.yscale("log")
    plt.savefig("CAL.png")
    plt.close()
    """