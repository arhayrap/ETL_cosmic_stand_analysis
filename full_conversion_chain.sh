rm /home/aram/cosmic_data_analysis/Scope_data_combined/run_$1.root
python3 convert_data.py --input_file_index $1
cd /home/aram/cosmic_data_analysis/Lecroy/Conversion/
python3 recoLoop.py --input_file_index $1
cd /home/aram/cosmic_data_analysis/Lecroy/Merging/
python3 merge_scope_etroc.py --input_file_index $1
cd /home/aram/cosmic_data_analysis/Analysis/
export filename="run_${1}.root"
root -b -q -l "Analysis.C(\"${filename}\")"
