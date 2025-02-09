# scp daq@timingdaq03:/home/daq/ETROC2_Test_Stand/module_test_sw/ETROC_output/* ./ETROC_output_box_setup_1/
# scp daq@timingdaq03:/media/daq/onn.\\\ Disk/ETROC_output/* ./ETROC_output_box_setup/
# scp ahayrape@cmslpc-el9.fnal.gov:/eos/uscms/store/group/cmstestbeam/2024_11_Cosmics/tests/ETROC_output_02_06_2025/*.dat ./ETROC_output_box_setup/
cd ./ETROC_LecroyScope/
for i in {1..8}; do
scp daq@timingdaq03:/home/daq/LecroyMount/C${i}--Cosmics_8090184000*.trc .
done;
