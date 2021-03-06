#!/bin/sh

solver=~/bin/nonogram
export TESSDATA_PREFIX=/home/fvbakel/git/tessdata_best
export MODEL_DIR=/home/fvbakel/git/nonogram/models
if [ -n "$1" ]; then
  echo "Processing directory: $1"
  process_dir=$1
else
  echo "First parameter not supplied."
  exit
fi

if [ -n "$2" ]; then
  echo "Processing file filter: $2"
  file_filter=$2
else
  echo "File filter not supplied."
  exit
fi

shift
shift

FILES=`find -L ${process_dir} -name "${file_filter}"`
for file in $FILES
do
    CMD="${solver} -f ${file} ${@}"
    echo $CMD
    $CMD
done
