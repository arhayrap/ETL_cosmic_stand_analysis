# def ReocoAllScope(laserMode=False, LGADChannel=2, LGADThreshold=50):
import time
from datetime import datetime
# import numpy as np
# from numpy import loadtxt
import getpass
import os
import subprocess
import socket
import sys
import glob
import shutil
import ROOT
import argparse
import numpy as np

#### Use setup script in home area (setup.sh)

raw_path = "/home/aram/Efficiency_study/scope_binaries/"
converted_path = "./CONVERTED/"
reco_path ="./RECONSTRUCTED/"

parser = argparse.ArgumentParser(description='Run info.')

parser.add_argument('--singleMode',metavar='Single event mode', type=str, default = 0, help='Single event acquisition (default 0, 1 for yes)', required=False)
parser.add_argument('--rb', metavar='The index of the readout board', type=str, default = 0, help='', required=False)
parser.add_argument('--shift', metavar='The shift for the file index', type=str, default = 0, help='', required=False)
parser.add_argument('--input_file_index',metavar='The input file index', type=str, default = 0, help='')

args = parser.parse_args()

useSingleEvent =False
if int(args.singleMode) > 0:
    useSingleEvent=True
    print("Using single event mode.")

def RunEntriesScope(FileLocation, LGADChannels, LGADThreshold):
    list_hits=[]
    list_coinc=[]
    print(f"File location: {FileLocation}")
    f = ROOT.TFile.Open(FileLocation)
    if hasattr(f, 'pulse'):
        TotalEntries = f.pulse.GetEntries()
        for chan in LGADChannels:
            EntriesWithLGADHits = f.pulse.GetEntries("amp[%i]>%i"%(chan,LGADThreshold))
            CoincRate = float(EntriesWithLGADHits)/TotalEntries
            list_hits.append(EntriesWithLGADHits)
            list_coinc.append(CoincRate)
        return list_coinc, list_hits, TotalEntries
    else:
        return -1

LGADChannels=[0,1,2,7]
Threshold=15

# while True:

file_index = int(args.input_file_index)
# ETROC_path = "../../ETROC_output_box_setup/"
ETROC_path = "/home/aram/Efficiency_study/etroc_binaries/"
pattern = str(args.input_file_index).split("*")[0]
print(pattern)
# all_indices = np.array([int(x.split('output_run_')[1].split('_rb0.dat')[0]) for x in glob.glob('%s/output_run_90181*_rb0.dat'%ETROC_path)])
all_indices = np.array([int(x.split('output_run_')[1].split(f'_rb{args.rb}.dat')[0]) for x in glob.glob(f'{ETROC_path}/output_run_{pattern}*_rb{args.rb}.dat')])
all_indices = np.sort(all_indices)
print(all_indices)
mask = (all_indices>file_index)
if (sum(mask) != 0):
    next_index = all_indices[mask][0]
else:
    next_index = int(args.input_file_index)+1
# next_index = all_indices[(all_indices<file_index)][-1]
print(file_index, next_index)

shift = int(args.shift)
next_index+=shift
file_index+=shift

next_index = file_index+1
SetRawFiles = range(file_index, next_index)
print(SetRawFiles)
print(len(SetRawFiles))

if len(SetRawFiles) != 0:
    print(f"Number of files to be converted: {len(SetRawFiles)}")

for run in SetRawFiles:
    RecoPath = '%s/converted_run%i.root' % (converted_path,run)
    # RawPath = 'C8--Cosmics_%i.trc' % run
    RawPath = 'C2--Cosmics_%i.trc' % run

    print('lsof -f --../../ETROC_LecroyScope/%s |grep -Eoi %s' % (RawPath, RawPath))
    if os.path.exists(RecoPath):
        print('Run %i already converted. Doing reco stage two' % run)
    ConversionCmd = "python3 ./conversion.py --runNumber %i" % (run)
    os.system(ConversionCmd)
    '''
    elif not os.popen('lsof -f -- /home/aram/Efficiency_study/scope_binaries/ |grep -Eoi %s' % (RawPath)).read().strip() == RawPath:
        print('Converting run ', run)
        if not useSingleEvent: 
            print("using conversion")
            ConversionCmd = "python3 %s/../../Lecroy/Conversion/conversion.py --runNumber %i" % (os.getcwd(), run)
        else:
            print("using one event conversion")
            ConversionCmd = "python3 %s/../../Lecroy/Conversion/conversion_one_event.py --runNumber %i" % (os.getcwd(), run)
        os.system(ConversionCmd)
    '''
    if useSingleEvent: continue
    print('Doing dattoroot for run %i' % run)
    
    OutputFile = '%s/run_scope%i.root' % (reco_path, run)
    # DattorootCmd = '../../TimingDAQ/NetScopeStandaloneDat2Root --correctForTimeOffsets --input_file=%s/converted_run%i.root --output_file=%s --config=../../TimingDAQ/config/LecroyScope_v12.config --save_meas'  % (converted_path,run,OutputFile)
    DattorootCmd = '../../NetScopeStandaloneDat2Root --correctForTimeOffsets --input_file=%s/converted_run%i.root --output_file=%s --config=../../LecroyScope_v12_test.config --save_meas'  % (converted_path,run,OutputFile)
    os.system(DattorootCmd)
    can_be_later_merged = False
    
    try:
        CoincRate, EntriesWithLGADHits, TotalEntries = RunEntriesScope(OutputFile, LGADChannels, Threshold) # lgad channel starting from zero 
        can_be_later_merged = True
    except Exception as error:
        print(repr(error))
    print("Run %i: Total entries are %i"%(run,TotalEntries))
    for i,chan in enumerate(LGADChannels):
        print('\t Channel %i coincidence:  %.1f%% (%i hits)'  % (chan+1, 100.*CoincRate[i], EntriesWithLGADHits[i]))
        print("\n")

    print(TotalEntries)

# print('Now moving the converted and raw data to backup')
filebase = "./RECONSTRUCTED/run_scope"
files_to_combine = [f"{filebase}{i}.root" for i in range(file_index, next_index)]
# files_to_combine = [f"{filebase}{i}.root" for i in range(next_index, file_index)]
os.system(f"rm ../../Scope_data_combined_reco/run_{file_index-shift}.root")
combine_command = f"hadd -k ../../Scope_data_combined_reco/run_{file_index-shift}.root {' '.join(files_to_combine)}"
print(combine_command)
os.system(combine_command)
filebase = "./CONVERTED/converted_run"
files_to_combine = [f"{filebase}{i}.root" for i in range(file_index, next_index)]
# files_to_combine = [f"{filebase}{i}.root" for i in range(next_index, file_index)]
os.system(f"rm ../../Scope_data_combined_conv/converted_run{file_index-shift}.root")
combine_command = f"hadd -k ../../Scope_data_combined_conv/converted_run{file_index-shift}.root {' '.join(files_to_combine)}"
print(combine_command)
os.system(combine_command)
