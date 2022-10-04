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
TEST_MODE="false"

for i in "$@"; do
	case $1 in
		-r|--release)
			TARGET_MODE="Release"
			TARGET_DIR="./build/release"
			shift
			;;
		-d|--debug)
			TARGET_MODE="Debug"
			TARGET_DIR="./build/debug"
			shift
			;;
		-t|--build-and-test)
			TARGET_MODE="Release"
			TARGET_DIR="./build/release"
			TEST_MODE="true"
			shift
			;;
		*)
			TARGET_MODE="Release"
			TARGET_DIR="./build/release"
			;;
	esac
done

## BUILDING RWNMR
echo "::Building RWNMR (${TARGET_MODE}) with ${procs} processors..."
# Configure the build
cmake -S ${SRC_DIR} -B ${TARGET_DIR} -D CMAKE_BUILD_TYPE=${TARGET_MODE}
# Actually build the binaries
cmake --build ${TARGET_DIR} -j${procs}

if [ ${TARGET_MODE} == "Release" ] 
then
	# Create symbolic link in root directory
	ln -sf ${TARGET_DIR}/rwnmr-app ${APP_EXECUTABLE}

	if [ ${TEST_MODE} == "true" ]
	then
		echo ""
		echo "::Now testing build"
		cd ${TARGET_DIR}
		ctest
		cd ${ROOT_DIR}
	fi
fi

