#!/bin/bash
FILE="$(zenity --file-selection --title='Select your File')"

zenity --question --text="operation?" --ok-label="compress" --cancel-label="decompress"
ANS=$?
if [ "$ANS" -eq 0 ]; then
    ../huffman.elf ${FILE} -c
else
    ../huffman.elf ${FILE} -d
fi
