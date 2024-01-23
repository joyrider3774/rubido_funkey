#!/bin/sh

make clean
make TARGET=funkey

mkdir -p opk
cp rubido.elf opk/rubido.elf
cp -r ./rubidofs opk/rubidofs
cp Rubido.funkey-s.desktop opk/Rubido.funkey-s.desktop

mksquashfs ./opk Rubido.opk -all-root -noappend -no-exports -no-xattrs

rm -r opk