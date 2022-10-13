#!/bin/bash
# This scrip builds the Random-Walk NMR Simulator program usin CMAKE
# Before build, all requirements must be installed in the system
# Check "rwnmr_requirements_installation_guide" in "requirements" directory for a complete guidance

# Get system available processors
procs=$(nproc --all)

# Source code path
ROOT_DIR=${pwd}
SRC_DIR='./src/'
APP_EXECUTABLE='rwnmr'

# Assign compilation mode from command line args
TARGET_MODE="Release"
TARGET_DIR="./build/release"
BUILD_MODE="false"
TEST_MODE="false"

for i in "$@"; do
    case $i in
		-r|--release)
			BUILD_MODE="true"
			TARGET_MODE="Release"
			TARGET_DIR="./build/release"
			APP_EXECUTABLE="rwnmr"
			shift
			;;
		-d|--debug)
			BUILD_MODE="true"
			TARGET_MODE="Debug"
			TARGET_DIR="./build/debug"
			APP_EXECUTABLE="rwnmr-dbg"
			shift
			;;
		-t|--test)
			TEST_MODE="true"
			shift
			;;
		*)
			BUILD_MODE="true"
			TARGET_MODE="Release"
			TARGET_DIR="./build/release"
			APP_EXECUTABLE="rwnmr"
			;;
	esac
done

if [ ${BUILD_MODE} == "true" ] || [ ${TEST_MODE} == "false" ] 
then
	## BUILDING RWNMR
	echo "::Building RWNMR (${TARGET_MODE}) with ${procs} processors..."
	# Configure the build
	cmake -S ${SRC_DIR} -B ${TARGET_DIR} -D CMAKE_BUILD_TYPE=${TARGET_MODE}
	# Actually build the binaries
	cmake --build ${TARGET_DIR} -j${procs}

	# Create symbolic link in root directory
	ln -sf ${TARGET_DIR}/rwnmr-app ${APP_EXECUTABLE}
fi


if [ ${TEST_MODE} == "true" ] 
then
	if [ -f "${TARGET_DIR}/rwnmr-app" ] 
	then
		echo ""
		echo "::Testing RWNMR"
		cd ${TARGET_DIR}
		ctest
		cd ${ROOT_DIR}
	else
		echo "rwnmr-app was not built yet."
	fi
fi
