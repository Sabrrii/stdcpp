#!/bin/bash

#usage: $0 "commit message" file1_to_spread.ext file2.ext ...

cb=`git branch | grep '*' | sed 's/\*//'`
ob=`git branch | grep -v '*' | grep -v 'dev_'`

##ob=`echo $ob | cut -d' ' -f1`

comment=\"$1\"
shift

echo 'current branch='$cb' is the reference for files'
echo 'other stable branches='$ob

#case of almost generic file, e.g. README.md 
has_readme=` echo $* | grep 'README.md' | wc -l`
has_gitlabCI=` echo $* | grep '\.gitlab-ci\.yml' | wc -l`

##exit

for b in $ob
do
  #goto other branch
  git checkout $b -f
  #get files from reference branch
  git checkout $cb -- $*
  #compile/run/doc ... to update
  make
  #almost generic: run script to specify
  if((has_readme==1))
  then
    ./README.sh
  fi
  if((has_gitlabCI==1))
  then
    ./gitlab-ci.sh
  fi
  #commit
  git commit -am "$comment"
  #push to origin, e.g. GitLab
  git push
done

#go back to reference branch
git checkout $cb -f

