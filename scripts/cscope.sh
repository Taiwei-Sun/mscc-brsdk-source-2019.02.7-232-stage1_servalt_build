#!/bin/bash

codeRootPath=/root/mscc_stage1_servalt_defconfig_build

mkdir -p $codeRootPath/.cscope

cd $codeRootPath/.cscope
echo "generete cscope.files"
find $codeRootPath -name '*.c' -or -name '*.cxx' -or -name '*.hxx' -or -name '*.h' -or -name '*.cpp' -or -name '*.S' > cscope.files
echo "cscope.files ok"
echo "generete  cscope.out"
cscope -qbRk
echo "cscope.out ok"

cd -

exit 0
