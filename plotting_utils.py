import uproot
import argparse
import hist
import awkward as ak
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import os

base = f"{os.getcwd()}/ETROC_output"

if __name__ == "__main__":
    argParse = argparse.ArgumentParser(description = "Argument parser")
    
    argParse.add_argument("--input_file", action = "store", default = "", help = "Root file to investigate")
    args = argParse.parse_args()
    
    input_folder = args.input_file.split(".")[0]
    f = uproot.reading.open(f"{base}/{input_folder}.root")
    tree = f["pulse"]
    arrays = tree.arrays(library = "ak")
    
    tbin = 3.125 / arrays["cal_code"]
    toa = tbin * arrays["toa_code"]
    tot = (2 * arrays["tot_code"] - np.floor(arrays["tot_code"] / 32)) * tbin
    cal = arrays["cal_code"]
    rows = arrays["row"]
    cols = arrays["col"]
    
    toa_hists = []
    tot_hists = []
    cal_hists = []
    
    os.system(f"mkdir -p plot_dump")
    os.system(f"mkdir -p plot_dump/{input_folder}/")
    nhits_hist = hist.Hist(hist.axis.Regular(256, 0, 256, name = "nhits"))
    hitmap       = hist.Hist(hist.axis.Regular(16, 0, 15, name = "row"), hist.axis.Regular(16, 0, 15, name = "col"))
    hitmap.fill(ak.flatten(rows[arrays["tot_code"]<200]), ak.flatten(cols[arrays["tot_code"]<200]))
    print(f"nhits: {arrays['nhits']}")
    nhits_hist.fill(arrays["nhits"][:, 0])
    tot_vs_pixel = hist.Hist(hist.axis.Regular(256, 0, 256, name = "pixel index"), hist.axis.Regular(100, 0, 50, name = "tot"))
    tot_vs_pixel_zoom = hist.Hist(hist.axis.Regular(40, 60, 100, name = "pixel index"), hist.axis.Regular(100, 0, 50, name = "tot"))
    for row in range(16):
        for col in range(16):
            print(f"Row: {row}, Col: {col}.")
            output_folder = f"./plot_dump/{input_folder}/pixel_{row}_{col}/"
            os.system(f"mkdir -p {output_folder}")
            toa_hist = hist.Hist(hist.axis.Regular(50, 0, 20, name = "toa"))
            tot_hist = hist.Hist(hist.axis.Regular(50, 0, 20, name = "tot"))
            cal_hist = hist.Hist(hist.axis.Regular(400, 0, 400, name = "cal"))
            print((rows == row) & (cols == col))
            print(toa[(rows == row) & (cols == col)])
            toa_hist.fill(ak.flatten(toa[(rows == row) & (cols == col)]))
            tot_hist.fill(ak.flatten(tot[(rows == row) & (cols == col)]))
            cal_hist.fill(ak.flatten(cal[(rows == row) & (cols == col)]))
            pixel_index = row*16 + col
            tot_vs_pixel.fill([pixel_index]*len(ak.flatten(tot[(rows == row) & (cols == col)])), ak.flatten(tot[(rows == row) & (cols == col)]))
            tot_vs_pixel_zoom.fill([pixel_index]*len(ak.flatten(tot[(rows == row) & (cols == col)])), ak.flatten(tot[(rows == row) & (cols == col)]))
            # toa_hists.append(toa_hist)
            # tot_hists.append(tot_hist)
            # cal_hists.append(cal_hist)
            toa_hist.plot()
            plt.title("TOA")
            plt.xlabel("TOA [ns]")
            plt.ylabel("Count")
            plt.savefig(f"{output_folder}/toa.png")
            plt.close()
            tot_hist.plot()
            plt.title("TOT")
            plt.xlabel("TOT [ns]")
            plt.ylabel("Count")
            plt.savefig(f"{output_folder}/tot.png")
            plt.close()
            cal_hist.plot()
            plt.title("CAL")
            plt.xlabel("CAL")
            plt.ylabel("Count")
            plt.savefig(f"{output_folder}/cal.png")
            plt.close()
    
    # fig, ax = plt.subplots(figsize=(8, 6))
    # Set the z-axis to log scale
    # pcm = tot_vs_pixel.plot2d(ax=ax, cmin=1)
    # Adding a colorbar
    # cbar = fig.colorbar(pcm, ax=ax, label="Counts")

    tot_vs_pixel.plot()
    # plt.plot()
    plt.title("tot vs pixel")
    # plt.set_zscale('log')
    # tot_vs_pixel.xaxis.set_major_locator(plt.MaxNLocator(256))
    plt.xticks(range(0,64))
    plt.savefig(f"./plot_dump/{input_folder}/tot_vs_pixel.png")
    plt.close()

    tot_vs_pixel_zoom.plot()
    plt.title("tot vs pixel")
    # tot_vs_pixel.xaxis.set_major_locator(plt.MaxNLocator(256))
    plt.xticks(range(60,100))
    plt.savefig(f"./plot_dump/{input_folder}/tot_vs_pixel_zoom.png")
    plt.close()

    nhits_hist.plot()
    plt.title("Number of hits")
    plt.savefig(f"./plot_dump/{input_folder}/nhits.png")
    plt.close()
    
    hitmap.plot()
    plt.title("Hitmap")
    plt.savefig(f"./plot_dump/{input_folder}/hitmap.png")
    plt.close()

