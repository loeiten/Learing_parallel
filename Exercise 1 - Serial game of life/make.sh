#!/bin/sh

# determine compiler options (check first if the environment
# variable CPP_COMPILER is set):
if [ ! -n "$CPP_COMPILER" ]; then
  # base CPP_COMPILER on the current machine type:
  case `uname -s` in
	Linux)
		CPP_COMPILER=g++
		CPP_COMPILER_OPTIONS="-Wall -O3 -std=c++11"
		;;
	AIX)
		CPP_COMPILER=xlC
		CPP_COMPILER_OPTIONS="-O"
		;;
	SunOS)
		CPP_COMPILER=CC
		CPP_COMPILER_OPTIONS="-O3"
		;;
	*)
		# GNU's gcc is available on most systems...
		C_COMPILER=gcc
		C_COMPILER_OPTIONS="-Wall -O3"
		;;
    esac
# else: CPP_COMPILER and CPP_COMPILER_OPTIONS are set in start-up file
fi
	
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
