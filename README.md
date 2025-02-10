# ETL_cosmic_stand_analysis
The directory is used for the analysis of the data collected by a cosmic stand based in Fermilab, 
built to test modules for CMS endcap MIP timing detector (ETL) modules. The current version is able to 
test only one module at a time using cosmic radiation.

It consists of:
two silicon photomultipliers with a lyso crystal, information from which are used 
to select cosmic muons which are passing through the setup with a strait trajectory, 

a PCB board which has a wirebonded 2x2 LGAD sensor, the information from which is used 
as a reference in terms of timing performance. 

a multichannel photomultiplier, the information of which is used for triggering of the 
setup

and the ETL module under test.

The collected data is being downloaded from the computer using which the data acquisition is 
performed by running the `update_ETROC_data.sh` script:
```
bash update_ETROC_data.sh
```
.
The `full_conversion_chain.sh` script is responsible for performing the conversion of the 
binary files collected from the oscilloscope and KCU data acquisition board, merging and 
analyzing of the converted data. It takes the run number as an input and performs the 
conversion and the analyis for that particular run. The script is ran by the `run_all_files.py` 
script which selects the run numbers based on the filenames in the `ETROC_output_box_setup` folder 
or by a provided index list using `--file` argument which takes as an input a filename which contains 
run numbers. To start the processing of the downloaded binary data run the following command:
`python3 run_all_files.py --file {filename}`
.
