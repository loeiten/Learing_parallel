#!/bin/sh

CPP_COMPILER=g++
CPP_COMPILER_OPTIONS="-Wall -O3 -std=c++11"
	
files=`ls *.cxx`

for file in $files; do
  # Removes the .cpp
  # Notice the use of backtick ` (so not a string)
  # sed 's/replace_this/by_this/' is the substitute command
  # \ means turn off special meaning of character that follows
  # $ does the action on the last element (if there is several)
  stem=`echo $file | sed 's/\.cxx$//'`
  # Print what is after echo on screen
  # newline
  echo
  echo $CPP_COMPILER $CPP_COMPILER_OPTIONS -o $stem.app $file -lm
  # Does this
  $CPP_COMPILER $CPP_COMPILER_OPTIONS -o $stem.app $file -lm
  # -s displays how many blocks used
  ls -s $stem.app
done
