#!/bin/bash

PROJECT_DIR=$PWD
scriptdir=`dirname "$BASH_SOURCE"`
cd $scriptdir

FILENAME=RMCIOS
: ${CC=gcc}
: ${OUTPUT_FILE=$FILENAME.bin}
INTERFACE_DIR=RMCIOS-interface
SRC_DIR=RMCIOS-Linux-module

# Settings
PROJECTDIR=$(pwd)

SOURCES="RMCIOS-Linux.c"
SOURCES+=" string-conversion.c"
SOURCES+=" RMCIOS-system/RMCIOS-system.c" 
SOURCES+=" RMCIOS-interface/RMCIOS-functions.c"
SOURCES+=" RMCIOS-base-module/*.c"
SOURCES+=" RMCIOS-std-module/*.c"
SOURCES+=" RMCIOS-Linux-module/*.c" 

OUTPUT_DIR=..
OUTPUT_FILE=$FILENAME.bin

. version_str.sh
 
# Compiler flags
CFLAGS="-s -Os -flto -lm -lrt"
CFLAGS+=" -I $PROJECTDIR/RMCIOS-interface/"
CFLAGS+=" -I $PROJECTDIR/RMCIOS-system/"
CFLAGS+=" -I $PROJECTDIR/RMCIOS-std_module/"
CFLAGS+=" -I $PROJECTDIR/RMCIOS-base_module/"
CFLAGS+=" -DVERSION_STR=\"$VERSION_STR\""

# Compile command/s
echo $SOURCES -o $OUTPUT_DIR/$OUTPUT_FILE $CFLAGS
gcc $SOURCES -o $OUTPUT_DIR/$OUTPUT_FILE $CFLAGS

# Execute program
#echo "Running $OUTPUT_DIR/$OUTPUT_FILE"
#$OUTPUT_DIR/$OUTPUT_FILE 
#cd %OUTPUT_DIR%
#read -rsp $'Press enter to continue...\n'

cd $PROJECTDIR

