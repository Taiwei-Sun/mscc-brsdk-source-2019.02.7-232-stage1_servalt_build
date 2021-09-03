#!/bin/bash



copy_to_path="/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/"

all_modify_files=`git status |grep 'modified:' |cut --fields=4 --delimiter=' '`

for i in $all_modify_files
do
    echo "cp $i $copy_to_path$i"
    cp $i $copy_to_path$i
done

exit 0


for i; do 
    echo "cp $i $copy_to_path$i" 
    cp $i $copy_to_path$i
done

exit 0

# store arguments in a special array 
args=("$@") 
# get number of elements 
ELEMENTS=${#args[@]} 
 
# echo each element in array  
# for loop 
for (( i=0;i<$ELEMENTS;i++)); do 
    echo ${args[${i}]} 
done

