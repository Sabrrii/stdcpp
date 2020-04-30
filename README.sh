#!/bin/bash

old=hello
new=bitset

old=`cat README.md | grep '/stdcpp/badges/' | head -n 1 | sed 's/^.*badges//' | cut -d'/' -f2`
new=`git branch | grep '*' | cut -d' ' -f2`

echo "change GitLab/CI pipeline status from "$old' to '$new
cat README.md | sed "s/$old/$new/g" > tmp.md
mv tmp.md README.md

