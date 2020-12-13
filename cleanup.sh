#!/bin/bash

for dir in ./lab*/* ; 
do 
  if [ -d "$dir" ]
  then (cd "$dir" && echo "$dir" && make clean); 
  fi
done
