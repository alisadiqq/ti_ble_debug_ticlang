cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER    tiarmclang)
set(CMAKE_CXX_COMPILER  tiarmclang)
set(AS                  tiarmasm)
set(AR                  tiarmar)
set(OBJCOPY             tiarmobjcopy)
set(OBJDUMP             tiarmobjdump.exe)
set(SIZE                tiarmsize)
set(CMAKE_C_COMPILER_LINKER  tiarmlnk)

#set(TICLANG_ARMCOMPILER "/c/ti/ccs2011/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS")
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Project Infomation
project( app_basic_ble_ticlang )
enable_language(ASM)
enable_language(C)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_VERBOSE_MAKEFILE ON)

# STDLIB
set (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

if (NOT DEFINED FW_MAJOR_VER)
    set(FW_MAJOR_VER 0)
endif()

if (NOT DEFINED FW_MINOR_VER)
    set(FW_MINOR_VER 0)
endif()

if (NOT DEFINED FW_PATCH_VER)
    set(FW_PATCH_VER 0)
endif()

if (NOT DEFINED FW_BUILD_NUM)
    set(FW_BUILD_NUM 0)
endif()

message("MAJOR=${FW_MAJOR_VER}")
message("MINOR=${FW_MINOR_VER}")
message("PATCH=${FW_PATCH_VER}")
message("BUILD_NUM=${FW_BUILD_NUM}")

set (TISDK "${CMAKE_SOURCE_DIR}/../../../../..")
#set (TISDK "/d/kent/BAT/TI_gcc/simplelink_lowpower_f3_sdk_8_40_02_01")


add_compile_definitions( 
    ICALL_NO_APP_EVENTS 
    CC23X0 
    NVOCMP_NWSAMEITEM=1 
    FLASH_ONLY_BUILD 
    NVOCMP_NVPAGES=6 
    FREERTOS 
    NVOCMP_POSIX_MUTEX
	
	PERIPHERAL_CFG=4
	HOST_CONFIG=PERIPHERAL_CFG

	FW_MAJOR_VER=${FW_MAJOR_VER}
	FW_MINOR_VER=${FW_MINOR_VER}
	FW_PATCH_VER=${FW_PATCH_VER}
	FW_BUILD_NUM=${FW_BUILD_NUM}
)

include_directories(    
	./
	"${CMAKE_CURRENT_SOURCE_DIR}/app"
    "${CMAKE_CURRENT_SOURCE_DIR}/app/Profiles"
	"${CMAKE_CURRENT_SOURCE_DIR}/common/startup"
    "${CMAKE_CURRENT_SOURCE_DIR}/generatedcode"
    ${TISDK}/source/third_party/mcuboot/boot/bootutil/include
    ${TISDK}/source/third_party/mcuboot/boot/ti/source/mcuboot_app
    ${TISDK}/source/ti/common/cc26xx/flash_interface
    ${TISDK}/source/ti/drivers/nvs
    ${TISDK}/source/ti/devices/cc23x0r5/driverlib
    ${TISDK}/source
    ${TISDK}/source/ti
	${TISDK}/source/ti/drivers
    ${TISDK}/source/ti/ble5stack_flash
    ${TISDK}/source/ti/ble5stack_flash/controller/cc26xx/inc
    ${TISDK}/source/ti/ble5stack_flash/inc
    ${TISDK}/source/ti/ble5stack_flash/rom
    ${TISDK}/source/ti/ble5stack_flash/common/cc26xx
    ${TISDK}/source/ti/ble5stack_flash/icall/inc
    ${TISDK}/source/ti/ble5stack_flash/hal/src/target/_common
    ${TISDK}/source/ti/ble5stack_flash/common/cc26xx/npi/stack
    ${TISDK}/source/ti/ble5stack_flash/hal/src/inc
    ${TISDK}/source/ti/ble5stack_flash/heapmgr
    ${TISDK}/source/ti/ble5stack_flash/profiles/dev_info
    ${TISDK}/source/ti/ble5stack_flash/profiles/simple_profile
    ${TISDK}/source/ti/ble5stack_flash/icall/src/inc
    ${TISDK}/source/ti/ble5stack_flash/npi/src
    ${TISDK}/source/ti/ble5stack_flash/osal/src/inc
    ${TISDK}/source/ti/ble5stack_flash/services/src/saddr
    ${TISDK}/source/ti/ble5stack_flash/services/src/sdata
    ${TISDK}/source/ti/common/nv
    ${TISDK}/source/ti/common/cc26xx
    ${TISDK}/source/ti/ble5stack_flash/icall/src
    ${TISDK}/source/ti/bleapp/profiles/health_thermometer
    ${TISDK}/source/ti/bleapp/services/health_thermometer
    ${TISDK}/source/ti/drivers/rcl
    ${TISDK}/source/ti/posix/ticlang
    ${TISDK}/source/third_party/freertos/include
    ${TISDK}/source/third_party/freertos/portable/GCC/ARM_CM0
    ${TISDK}/kernel/freertos
)

file(GLOB SRC_FILES 
    ./*.c
    ./generatedcode/*.c
    app/*.c
    app/Profiles/*.c
    common/Startup/main_freertos.c
    common/Startup/osal_icall_ble.c
	
    #${TISDK}/source/ti/common/cc26xx/flash_interface/external/flash_interface_ext_rtos_NVS.c
    ${TISDK}/source/ti/common/nv/crc.c
    ${TISDK}/source/ti/common/nv/nvocmp.c

    ${TISDK}/source/ti/ble5stack_flash/rom/agama_r1/rom_init.c
    ${TISDK}/source/ti/ble5stack_flash/icall/stack/ble_user_config_stack.c
    ${TISDK}/source/ti/ble5stack_flash/icall/src/icall_cc23x0.c
    ${TISDK}/source/ti/ble5stack_flash/icall/src/icall_user_config.c
    ${TISDK}/source/ti/ble5stack_flash/icall/app/ble_stack_api.c
    ${TISDK}/source/ti/ble5stack_flash/icall/app/ble_user_config.c
    ${TISDK}/source/ti/ble5stack_flash/icall/app/icall_api_lite.c
    ${TISDK}/source/ti/ble5stack_flash/icall/src/icall_POSIX.c
	
	source/empty.c
	source/ble5stack/basic_ble/ble_app_util/src/bleapputil_init.c
	source/ble5stack/basic_ble/ble_app_util/src/bleapputil_process.c
	source/ble5stack/basic_ble/ble_app_util/src/bleapputil_stack_callbacks.c
	source/ble5stack/basic_ble/ble_app_util/src/bleapputil_task.c
	
	source/ble5stack/basic_ble/menu_module/menu_module.c
	source/ble5stack/basic_ble/profiles/simple_gatt/simple_gatt_profile.c
	source/ble5stack/basic_ble/services/dev_info/dev_info_service.c
)

link_directories(
	#${TISDK}/source/
	#${TISDK}/kernel/rtos/
)

# --- Post-Build Step (Generate HEX File using TI's armhex) ---
# You might need to adjust the path to armhex based on your CCS installation
find_program(ARMHEX tiarmhex
HINTS "C:/Simple_link_toolchain/ti-cgt-armllvm_4.0.2.LTS/bin"
PATH_SUFFIXES bin
)

#--- Post-Build Step (Optional: Generate Binary File using TI's armobjcopy) ---
find_program(ARMOBJCOPY tiarmobjcopy
 HINTS "C:/Simple_link_toolchain/ti-cgt-armllvm_4.0.2.LTS/bin"
 PATH_SUFFIXES bin
)

set(LD_FLAGS_1   "-Wl,-u,_c_int00 -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,-x -Wl,-c ")
set(LD_FLAGS_2   "-L${TISDK}/source -L${CMAKE_SOURCE_DIR}/generatedcode ${CMAKE_SOURCE_DIR}/generatedcode/ti_utils_build_linker.cmd.genlibs ${CMAKE_SOURCE_DIR}/freertos/ticlang/lpf3_app_freertos.cmd")
set(LD_FLAGS_3   "-Wl,-m,bin/${CMAKE_PROJECT_NAME}.map -Wl,--rom_model -Wl,--warn_sections") # -L${TICLANG_ARMCOMPILER}/lib/")

set(LD_FLAGS     "${LD_FLAGS_1} ${LD_FLAGS_2} ${LD_FLAGS_3}" CACHE INTERNAL "linker flags")

set(MCPU_FLAGS "-mthumb -mcpu=cortex-m0plus -march=thumbv6m -mlittle-endian")
set(VFP_FLAGS "-mfloat-abi=soft")
set(SPEC_FLAGS "")
set(WARN_IGN_FLAGS "-Wno-int-conversion -Wno-incompatible-pointer-types -Wno-implicit-function-declaration ")
set(CFLAG_EXT1 "@${CMAKE_SOURCE_DIR}/generatedcode/ti_utils_build_compiler.opt @${TISDK}/source/ti/ble5stack_flash/config/build_components.opt @${TISDK}/source/ti/ble5stack_flash/config/factory_config.opt")

set(CMAKE_C_FLAGS   "${MCPU_FLAGS} ${VFP_FLAGS} ${CFLAG_EXT1} -std=gnu99 -std=c99 -gdwarf-3 -ffunction-sections" CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS "${MCPU_FLAGS} ${VFP_FLAGS} ${CFLAG_EXT1} -std=gnu99 -std=c99 -gdwarf-3 -ffunction-sections" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_ASM_FLAGS "${MCPU_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
set(CMAKE_EXE_LINKER_FLAGS "-v ${MCPU_FLAGS} ${VFP_FLAGS} ${LD_FLAGS} " CACHE INTERNAL "exe link flags")

SET(CMAKE_C_FLAGS_DEBUG "-g"   CACHE INTERNAL "c debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "-g" CACHE INTERNAL "cxx debug compiler flags")
SET(CMAKE_ASM_FLAGS_DEBUG "-g"     CACHE INTERNAL "asm debug compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-Oz -g"   CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-Oz -g" CACHE INTERNAL "cxx release compiler flags")
SET(CMAKE_ASM_FLAGS_RELEASE "-g"     CACHE INTERNAL "asm release compiler flags")

# Generate the target
add_executable (${CMAKE_PROJECT_NAME}.elf  ${SRC_FILES})

# this way does not work, it will treat libc.a as -lc
#target_link_options(${CMAKE_PROJECT_NAME}.elf PRIVATE -llibc.a)

# For TICLANG, -llibc.a must be at last of link command, otherwise it will link fail, 
# If add -llibc.a in link flag, then -llibc.a will be in front of object files, and finnally it will link fail.
# after many times try, only this way works
# use cmake find feature to find libc.a and add libc.a in target_link_libraries.
# Find static lib, suffix .a
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(STAND_C_LIB libc.a)

# Link the library to the target
target_link_libraries (${CMAKE_PROJECT_NAME}.elf ${STAND_C_LIB})


#Generate the hex file
add_custom_target (${CMAKE_PROJECT_NAME}.hex ALL arm-none-eabi-objcopy -R  ".bl_app_share_ram" -Oihex "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.hex" DEPENDS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf)
#add_custom_target (${CMAKE_PROJECT_NAME}.bin ALL arm-none-eabi-objcopy -R  ".bl_app_share_ram" -Obinary "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf" "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.bin" DEPENDS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf)

# Make clean-all
add_custom_target (clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
