# ******************************************************
# Script to create variables of git revision for sources.
#
# Required environment variables:
# PROJECT_DIR=  RMCIOS implementation root dir
# INTERFACE_DIR = RMCIOS-interface dir
# SRC_DIR = compile source directory
#
# Generated environment variables:
# ISO8601DATETIME
# IREVISION - RMCIOS interface revision
# SREVISION - Source revision (for SRC_DIR)
# REVISION  - Project revision (all sources and scripts)
# *******************************************************

# Variable for build revision id
REVISION_ID=$(git show -s --format=%h)
REVISION="Rev:$REVISION_ID"

# Variable for interface revision id
cd $PROJECT_DIR
cd $INTERFACE_DIR
IREVISION_ID=$(git show -s --format=%h)
IREVISION="iRev:$IREVISION_ID"

# Variable for source revision id
cd $PROJECTDIR
cd $SRC_DIR
SREVISION_ID=$(git show -s --format=%h)
SREVISION="sRev:$SREVISION_ID"
cd $PROJECTDIR

# Generate ISO8601 date string
ISO8601DATETIME=$(date --iso-8601=seconds)

# set version string
VERSION_STR="$REVISION\\,Built:$ISO8601DATETIME\\,$SREVISION\\,$IREVISION"

