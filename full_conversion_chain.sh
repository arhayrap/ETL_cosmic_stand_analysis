# cd /home/aram/ETL_cosmic_stand_analysis/
# python3 convert_data.py --input_file_index $1

cd /home/aram/ETL_cosmic_stand_analysis/Lecroy/Conversion/
python3 recoLoop.py --input_file_index $1

# cd /home/aram/ETL_cosmic_stand_analysis/Lecroy/Merging/
# python3 merge_scope_etroc.py --input_file_index $1 --min 0.2 --max 0.9 --threshold 0.5

# cd /home/aram/cosmic_data_analysis/Analysis/
# export filename="run_${1}.root"
# root -b -q "Analysis.C(\"${filename}\")"
