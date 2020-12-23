#!/bin/bash

for dir in ./*/* ; 
do 
  if [ -d "$dir" ]
  then (cd "$dir" && echo "$dir" && make clean); 
  fi
done
