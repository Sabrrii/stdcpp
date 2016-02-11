#!/bin/bash

#usagei: force checkout of remote branches and make

for b in `git branch -a | grep origin | rev | cut -d'/' -f1 | rev`
do
  git checkout $b -f
  make
done

