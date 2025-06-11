#!/bin/bash

#set -e
#set -o pipefail

WorkDir="./"
SrcDir="./"
CMAKEDIR="/c/Simple_link_toolchain/cmake-3.26.5-windows-x86_64/bin"
MAKEDIR="/c/Simple_link_toolchain/make"
#GCCDIR="/c/Simple_link_toolchain/gcc-arm-none-eabi-10.3-2021.10/bin"
GCCDIR="/c/Simple_link_toolchain/arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-arm-none-eabi/bin"
TICLANGDIR="/c/ti/ccs2011/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS/bin"
export PATH=$PATH:$CMAKEDIR:$MAKEDIR:$GCCDIR:$TICLANGDIR

bl_ver=0
app_ver=0
set_ver=1

# Firmware Versioning
FW_MAJOR_VER=${fwMajor:-${FW_MAJOR_VER:-0}}
FW_MINOR_VER=${fwMinor:-${FW_MINOR_VER:-0}}
FW_PATCH_VER=${fwPatch:-${FW_PATCH_VER:-0}}
FW_BUILD_NUM=${fwBuild:-${FW_BUILD_NUM:-0}}

# Print version numbers
echo -e "Major version:   \e[32m\e[1m${FW_MAJOR_VER}\e[0m"
echo -e "Minor version:   \e[32m\e[1m${FW_MINOR_VER}\e[0m"
echo -e "Patch version:   \e[32m\e[1m${FW_PATCH_VER}\e[0m"
echo -e "Build Number :   \e[32m\e[1m${FW_BUILD_NUM}\e[0m"

# Validate the FW Version Format - Major[0~9].Minor[0~99].Patch[0~999]-Build[0~9999]
declare -A version_limits=( ["FW_MAJOR_VER"]=9 ["FW_MINOR_VER"]=99 ["FW_PATCH_VER"]=999 )

for var in "${!version_limits[@]}"; do
    value=${!var}
    limit=${version_limits[$var]}
    if ! [[ "$value" =~ ^[0-9]+$ ]] || [ "$value" -lt 0 ] || [ "$value" -gt $limit ]; then
        echo -e "\n\e[31mError: Invalid input: $var must be a number between 0 and $limit and must follow the Semantic Versioning 2.0.0 Format.\e[0m\n"
        exit 1
    fi
done

echo -e "\n\nFirmware Version:   \e[32m\e[1m${FW_MAJOR_VER}.${FW_MINOR_VER}.${FW_PATCH_VER}-${FW_BUILD_NUM}\e[0m\n\n"

# Detect the operating system
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    envos="linux"
    export PATH=/opt/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi/bin:$PATH
    echo "OS is Linux"
elif [[ "$OSTYPE" == "msys" ]]; then
    envos="win"
    echo "OS is Windows Git Bash"
    export PATH=$PATH:/c/Simple_link_toolchain/nrf-command-line-tools/bin
else
    echo "Unsupported OS"
    exit 1
fi


# Default to build image for copperhead dev board
if [ "$1" == "-c" ]; then
    rm -fr ${WorkDir}/build/Debug
    rm -fr ${WorkDir}/build/Release
    echo "Deleted folder ${WorkDir}/build/Debug"
    echo "Deleted folder ${WorkDir}/build/Release"
    exit 0
fi

SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR="../../../../../../simplelink_lowpower_f3_sdk_8_40_02_01"
#SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR=/d/kent/try_run/simplelink_lowpower_f3_sdk_8_40_02_01
SYSCONFIG_TOOL=/c/ti/ccs2011/ccs/utils/sysconfig_1.23.0/sysconfig_cli.sh
mkdir -p ${WorkDir}/generatedcode
${SYSCONFIG_TOOL} --compiler ticlang --product ${SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR}/.metadata/product.json --output ${WorkDir}/generatedcode freertos/basic_ble.syscfg
#${SYSCONFIG_TOOL} --compiler ticlang --product ${SIMPLELINK_LOWPOWER_F3_SDK_INSTALL_DIR}/.metadata/product.json --listGeneratedFiles --listReferencedFiles --output . ../../freertos/basic_ble.syscfg


buildtype="Release"
if [ "$1" == "-d" ]; then
    buildtype="Debug"
fi

# Detect number of available CPU cores
if [ $envos == "linux" ]; then
    num_cores=$(nproc)
elif [ $envos == "win" ]; then
    if command -v wmic &> /dev/null; then
        #need update later, in shell, there is not findstr
        #num_cores=$(wmic cpu get NumberOfCores | findstr /r /v "^$" | findstr /r /v "NumberOfCores")
        num_cores=1
    elif command -v powershell &> /dev/null; then
        num_cores=$(powershell -Command "& { (Get-WmiObject -Class Win32_Processor | Measure-Object -Property NumberOfCores -Sum).Sum }")
    else
        num_cores=1
    fi
else
    num_cores=1
fi

echo "Using $num_cores cores for build"

# kent change, should move this to pipeline yaml file
# Update file timestamps to avoid clock skew warnings
#find . -type f -exec touch {} +

if [ $envos == "win" ]; then
    #cmake -G"MinGW Makefiles" -B ${SrcDir}/build/$buildtype -S ${SrcDir} -DCMAKE_C_COMPILER_FORCED=ON -DCMAKE_CXX_COMPILER_FORCED=ON -DCMAKE_BUILD_TYPE=$buildtype -DCMAKE_MAKE_PROGRAM=make -DFW_MAJOR_VER=$FW_MAJOR_VER -DFW_MINOR_VER=$FW_MINOR_VER -DFW_PATCH_VER=$FW_PATCH_VER -DFW_BUILD_NUM=$FW_BUILD_NUM
    cmake -G"MinGW Makefiles" -B ${SrcDir}/build/$buildtype -S ${SrcDir} -DCMAKE_BUILD_TYPE=$buildtype -DCMAKE_MAKE_PROGRAM=make -DFW_MAJOR_VER=$FW_MAJOR_VER -DFW_MINOR_VER=$FW_MINOR_VER -DFW_PATCH_VER=$FW_PATCH_VER -DFW_BUILD_NUM=$FW_BUILD_NUM
fi
if [ $envos == "linux" ]; then
    cmake -B ${SrcDir}/build/$buildtype -S ${SrcDir} -DCMAKE_BUILD_TYPE=$buildtype -DCMAKE_MAKE_PROGRAM=make -DFW_MAJOR_VER=$FW_MAJOR_VER -DFW_MINOR_VER=$FW_MINOR_VER -DFW_PATCH_VER=$FW_PATCH_VER -DFW_BUILD_NUM=$FW_BUILD_NUM
fi

cmake --build ${SrcDir}/build/$buildtype -j$num_cores
if [ $? != 0 ]; then
    echo "Build failed"
    exit 1
fi
echo "Build succeed"

