#!/bin/bash

old=hello
new=bitset

old=`grep '_help.output' .gitlab-ci.yml | grep -v git | sed 's/-//g;s/ //g'`
new=`git branch | grep '*' | cut -d' ' -f2`_help.output

echo "change GitLab/CI *_help.output from "$old' to '$new
cat .gitlab-ci.yml | sed "s/$old/$new/g" > tmp.md
mv tmp.md .gitlab-ci.yml

