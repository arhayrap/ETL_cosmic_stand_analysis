# rm /home/aram/cosmic_data_analysis/Scope_data_combined/run_$1.root

cd /home/aram/cosmic_data_analysis/
python3 convert_data.py --input_file_index $1

# cd /home/aram/cosmic_data_analysis/Lecroy/Conversion/
# python3 recoLoop.py --input_file_index $1

# cd /home/aram/cosmic_data_analysis/Lecroy/Merging/
# python3 merge_scope_etroc_FNAL_Aram_Copy_2.py --input_file_index $1 --min 0.2 --max 0.9 --threshold 0.4

# python3 merge_scope_etroc_FNAL_Aram_Copy_2.py --input_file_index $1 --min 0.2 --max 0.9 --threshold -0.4 --plotting --niters 1 --nevents 100

# python3 merge_scope_etroc.py --input_file_index $1 --min 0.2 --max 0.9 --threshold 0.5 --data_part $2
# cd /home/aram/cosmic_data_analysis/Analysis/
# export filename="run_${1}.root"
# root -b -q "Analysis.C(\"${filename}\")"
