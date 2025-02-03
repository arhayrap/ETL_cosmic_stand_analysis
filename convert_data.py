import os
import sys
import argparse

base = os.getcwd()

def convert(input_data):
    output_data = input_data.split(".")[0] + ".json"
    print(f"{base}/ETROC_output_box_setup/{input_data}")
    os.system(f"python3 data_dumper.py --input_file {base}/ETROC_output_box_setup/{input_data}") #  --output_file {base}/{output_data}")
    input_data = input_data.split(".")[0] + ".json"
    output_data = output_data.split(".")[0] + ".root"
    os.system(f"python3 root_dumper.py --input_file {base}/ETROC_output_box_setup/{input_data} --output_file {base}/ETROC_output_box_setup/{output_data}")

if __name__ == "__main__":

    argParser = argparse.ArgumentParser(description = "Argument parser")

    argParser.add_argument('--input_file_index', action='store', default='', help="Binary file")

    args = argParser.parse_args()
    input_file = f"output_run_{args.input_file_index}_rb0.dat"
    convert(input_file)
