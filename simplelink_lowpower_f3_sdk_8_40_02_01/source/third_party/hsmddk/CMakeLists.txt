# Copyright (c) 2022-2023, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
cmake_minimum_required(VERSION ${TI_MIN_CMAKE_VERSION})

include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_common.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc27xx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeSources_cc35xx.cmake)

ti_init_package(NAMESPACE ThirdPartyHSMDDKLib)

foreach(TI_PLATFORM ${SUPPORTED_PLATFORMS})
    if(${TI_PLATFORM} STREQUAL "cc27xx")
        ti_init_package(NAMESPACE ThirdPartyHSMDDKLib PACKAGE_NAME CC27XX)

        ti_add_package_dependency(DriverlibCC27XX)
        ti_add_library(
            hsmddk_${TI_PLATFORM}
            OUTPUT_NAME hsmddk_${TI_PLATFORM}
            SOURCES ${SOURCES_CC27XX}
            ARCH m33f
            ADD_SDK_INCLUDE_PATH source
            LINK_PUBLIC CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
            Driverlib::${TI_PLATFORM}
        )

        ti_add_library(
            hsmddk_${TI_PLATFORM}_its
            OUTPUT_NAME hsmddk_${TI_PLATFORM}_its
            SOURCES ${SOURCES_ITS_CC27XX}
            ARCH m33f
            ADD_SDK_INCLUDE_PATH source
            LINK_PUBLIC CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
            Driverlib::${TI_PLATFORM}
        )

        if (${TI_TOOLCHAIN_NAME} MATCHES "ticlang|gcc")
            target_compile_options(hsmddk_${TI_PLATFORM}_its PRIVATE -mcmse)
        elseif (${TI_TOOLCHAIN_NAME} MATCHES "iar")
            target_compile_options(hsmddk_${TI_PLATFORM}_its PRIVATE --cmse)
        endif ()


        ti_export_package()
    elseif(${TI_PLATFORM} MATCHES "cc35")
        ti_init_package(NAMESPACE ThirdPartyHSMDDKLib PACKAGE_NAME CC35XX)

        ti_add_package_dependency(DriverlibCC35XX)
        ti_add_library(
            hsmddk_${TI_PLATFORM}
            OUTPUT_NAME hsmddk_${TI_PLATFORM}
            SOURCES ${SOURCES_CC35XX}
            ARCH m33f
            ADD_SDK_INCLUDE_PATH source
            LINK_PUBLIC CMakeCommon::${TI_TOOLCHAIN_NAME}_${ARCH_${TI_PLATFORM}}
            Driverlib::${TI_PLATFORM}
        )

        ti_export_package()
    endif()
endforeach()
