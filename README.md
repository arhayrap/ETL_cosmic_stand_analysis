# ETL_cosmic_stand_analysis
The directory is used for the analysis of the data collected by a cosmic stand based in Fermilab, 
built to test modules for CMS endcap MIP timing detector (ETL) modules. The current version is able to 
test only one module at a time using cosmic radiation.

It consists of:
two silicon photomultipliers with a lyso crystal, information from which are used 
to select cosmic muons which are passing through the setup with a strait trajectory, 

a PCB board which has a wirebonded 2x2 LGAD sensor, the information from which is used 
as a reference in terms of timing performance. 

a multichannel photomultiplier (MCP), the information of which is used for triggering of the 
setup

and the ETL module under test.

The main script which is used to perform the data conversion and merging is `run_all_files.py` script.
The script accepts the following arguments to perform different operations:
file - A file which contains the run numbers.

--index - An individual run number. \n
--shift - A shift applied to the oscilloscope id. \n
--power_mode - The power mode at which the setup has been running. \n
--offset - The offset above baseline that has been applied. \n 
--l1a_delay - The delay of signal arrival in DAC units. \n 
--download - Download the ETROC and oscilloscope data from the DAQ computer if they are not downloaded yet. \n
--convert - Convert the downloaded information (mention which ones with the parameters below). \n
--etroc - Convert the downloaded etroc data. \n
--scope - Convert the downloaded scope data. \n
--merge - Merge the converted data from the etroc and scope. \n
--clock_fit_min - The minima of the clock fitting range. \n
--clock_fit_max - The maxima of the clock fitting range. \n
--clock_fit_thr - The threshold of the clock signal fit. \n
--pattern - The prefix of the run number (the first 4 numbers of the run number). \n
--daq - The address of the DAQ computer (daq@timingdaq03). \n

.

The output data, for each corresponding operation is stored in the following folders:
```
--etroc: ./ETROC_output_box_setup/
--scope: This step saves two types of output files: the waveforms (./Lecroy/Conversion/CONVERTED/) and the resonstructed timestamps of the waveforms (./Lecroy/Conversion/RECONSTRUCTED/) for different channels of the oscilloscope.
```
The output information from the oscilloscope during each run has been taken with 30 iterations, each iteration contains 100 events. Therefore after processing each 30 consecutive batches of scope data the converted outputs are combined into the following repositories:
```
Waveforms: ./Scope_data_combined_conv/
Timestamps: ./Scope_data_combined_reco/
```

The `--merge` option will merge the combined scope and etroc data and will perform the calculation of the timestamps for the clock signal waveform provided by the KCU. The output of this procedure is saved in the following folder: "MergedData".

To perform a quick analysis of the merged data the Analysis.C macro can be used which reads the provided root file and makes histograms of the time of arrival (TOA), time over threshold (TOT), the calibration value etc. Also it performs the calculation of the \DeltaT between the ETROC and the MCP for all as well as for each individual pixel. The created histograms are saved in `./ETL_cosmic_stand_analysis/Analysis/output_files` folder as root files.

An example of the execution of the command:
```
python3 run_all_files.py --power_mode default --offset 3 --etroc --scope --merge --file ../filelist.txt --download
```

