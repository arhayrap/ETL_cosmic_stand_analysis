import matplotlib.pyplot as plt
import hist
from hist import Hist
import os
import numpy as np
import awkward as ak
import uproot

if __name__ == "__main__":
    base = "~/ETL_cosmic_stand_analysis/MergedData/"
    offsets = [3,4,5,6,7,8,9]
    nhits_sum = []
    nhits_sum_cal = []
    mean_tot = []
    mean_tot_cal = []
    num_shades = len(offsets)
    colors = plt.cm.Reds(np.linspace(0.1, 1.0, num_shades))  # 3 shades from light to dark red
    print(colors)
    for i, o in enumerate(offsets):
        file = uproot.open(f"{base}run_809118{o}16.root")
        arr = file["pulse"].arrays(["nhits", "Clock", "toa_code", "tot_code", "cal_code"])
        cal = ak.flatten(arr["cal_code"])
        condition = (cal > 140) & (cal < 190)
        print("step 0")
        nhits = arr["nhits"][:,0]
        Clock = arr["Clock"][:,0]
        nhits_sum.append(np.sum(arr["nhits"]))
        nhits_sum_cal.append(np.sum(condition))
        print("step 1")
        toa = (ak.flatten(arr["toa_code"])*3.125/cal)
        tot = ((2*ak.flatten(arr["tot_code"])+np.floor(ak.flatten(arr["tot_code"])/32))*3.125/cal)
        mean_tot.append(np.mean(tot))
        mean_tot_cal.append(np.mean(tot[condition]))

        '''
        fig, ax = plt.subplots(1, 1, figsize = (500, 500))
        # clock_vs_toa = Hist(
        #     hist.new.Reg(30, 0, 20, name="TOA", label = f"TOA"),
        #     hist.new.Reg(30, -20, 5, name="Clock", label = f"Clock")
        # )
        draw_clock = ak.flatten([[Clock[c]]*nhits[c] for c in range(len(Clock))])
        draw_clock = draw_clock[condition]
        print(len(toa), len(draw_clock))
        print(draw_clock)
        ax.hist2d(toa.to_numpy(), draw_clock.to_numpy())
        # clock_vs_toa.fill(toa, draw_clock)
        ax.set_ylabel("Summary number of hits")
        ax.set_xlabel("Offset [vth]")
        plt.show()
        fig.save_fig(f"toa_vs_clock_{o}.png")
        plt.close()

        tot_hist = Hist( hist.new.Reg(30, 0, 20, name="TOT", label = f"TOT") )
        tot_hist.fill(tot, weight=1.0/len(tot))
        tot_hist.plot(label = f"Offset: {o} vth", linewidth=3)
        '''

        del arr
        del toa
        del tot
        del cal
        del Clock
        del nhits
        file.close()
        del file

    # plt.legend()
    # plt.show()
    print("start plotting 1")
    fig, ax = plt.subplots(1, 1, figsize = (10, 5))
    ax.plot(offsets, nhits_sum, label = "No cuts", linewidth=3)
    ax.plot(offsets, nhits_sum_cal, label = "140 < CAL <190", linewidth=3)
    ax.set_ylabel("Summary number of hits")
    ax.set_xlabel("Offset [vth]")
    plt.legend()
    plt.show()
    fig.savefig("sum_nhits_vs_offset.png")
    print("start plotting 2")
    fig, ax = plt.subplots(1, 1, figsize = (10, 5))
    ax.plot(offsets, mean_tot, label = "No cuts", linewidth=3)
    ax.plot(offsets, mean_tot_cal, label = "140 < CAL <190", linewidth=3)
    ax.set_ylabel("Mean TOT [ns]")
    ax.set_xlabel("Offset [vth]")
    plt.legend()
    plt.show()
    fig.savefig("mean_tot_vs_offset.png")
