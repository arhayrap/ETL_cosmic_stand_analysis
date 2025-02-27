# BASE=`pwd`

# cd ${BASE}
# python3 convert_data.py --input_file_index $1

# cd ${BASE}/Lecroy/Conversion/
# python3 recoLoop_1.py --input_file_index $1 --rb 0 --shift 0

# cd ${BASE}/Lecroy/Merging/
# python3 merge_scope_etroc.py --input_file_index $1 --min 0.2 --max 0.9 --threshold 0.5

# cd ${BASE}/Analysis/
# export filename="run_${1}.root"
# root -b -q "Analysis.C(\"${filename}\")"
