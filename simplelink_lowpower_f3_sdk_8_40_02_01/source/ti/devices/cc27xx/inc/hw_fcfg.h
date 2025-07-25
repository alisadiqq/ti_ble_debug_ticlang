/******************************************************************************
*  Copyright (c) 2022-2024 Texas Instruments Incorporated. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
*  3) Neither the name of the copyright holder nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
******************************************************************************
*  \file       hw_fcfg.h
*
*  \brief      Factory Configuration (FCFG) header file.
******************************************************************************/
#ifndef __HW_FCFG_H__
#define __HW_FCFG_H__

#include <stdint.h>
#include "hw_device.h"
#include "hw_ccfg.h"

// *** Define datatypes used in FCFG ***

// \note Struct written to avoid automatic padding. Packing directives not needed
// \note Bitfields: lsb->msb, packing of similar base types, no word boundary crossing
typedef struct {
    // Critical Trim (done early in boot sequence or requires special handling)
    struct {    // [0]: length 80 B
        // SRAM repair information (* hardcoded into ASM to be at offset 0, do not move! *)
        struct {    // length 28B
            // Value SRAMREP_MAGICWORD tells SRAM repair in ASM that criticalTrim is valid
            uint32_t magicWord;
            // Number of bits to shift through SRAM repair chain (first 4b are read back)
            uint8_t numBits;
            // Data to shift through SRAM repair chain (MSB first, max 184b)
            uint8_t data[23];
        } sramRepair;
        // HFOSC trim (CKMD.TRIM0)
        struct {    // length: 4B
            union {
                uint16_t val16;
                struct {
                    uint16_t coarse : 5;
                    uint16_t mid : 4;
                    uint16_t mode : 1;
                    uint16_t res0 : 6;
                };
            } initial;
            union {
                uint16_t val16;
                struct {
                    uint16_t coarse : 5;
                    uint16_t mid : 4;
                    uint16_t mode : 1;
                    uint16_t res0 : 6;
                };
            } final;
        } hfoscTrim;
        // Flash trims
        struct {  // length 24 B
            // Flash 1T wait states (default: 2 WS trimmed)
            struct {    // length: 2B
                uint8_t initial;
                uint8_t final;
            } flws1t;
            // Flash 2T wait states (default: 5 WS untrimmed, 2 WS trimmed)
            struct {    // length: 2B
                uint8_t initial;
                uint8_t final;
            } flws2t;
            uint32_t ptrmc0;
            uint32_t b0trmc1;
            uint32_t b0trmc0;
            uint32_t b1trmc1;
            uint32_t b1trmc0;
        } flashTrim;
        // PMU trim: GLDO/BIAS/BGAP/DigLDO
        struct {  // length 8B
            union {
                uint32_t val32;
                struct {
                    uint32_t vref       : 4;
                    uint32_t vbg        : 6;
                    uint32_t vddsbod    : 5;
                    uint32_t bgtrimen   : 1;    // Latch vddsbod (0 initially, set to 1 in later trim steps)
                    uint32_t iref       : 5;
                    uint32_t tsense     : 2;
                    uint32_t spare      : 1;
                    uint32_t vddrokhyst : 1;
                    uint32_t dissahyst  : 1;
                    uint32_t gldocompdis: 1;
                    uint32_t gldodisana : 1;
                    uint32_t res0       : 4;
                };
            } timmute0;
            union {
                uint32_t val32;
                struct {
                    uint32_t vddr               : 5;
                    uint32_t vddrsl             : 5;
                    uint32_t dis_hs_dcdc_comp   : 1;
                    uint32_t en_low_iqq_gldo    : 1;
                    uint32_t bod                : 4;
                    uint32_t udig               : 4;
                    uint32_t dig                : 4;
                    uint32_t coarse             : 4;
                    uint32_t delta              : 3;
                    uint32_t res0               : 1;
                };
            } timmute1;
        } pmuTrim;
        // Systick reload value that determines minimum time for each trim step in clock cycles
        uint8_t sysTickRvrTrimStep;
        // Flag to control whether BOD reset is disabled during VDDR/BGAP trim stepping
        uint8_t disableBodDuringTrim : 1;
        // Settling delay after VDDR/BGAP = enableBodDelaySteps*sysTickRvrTrimStep+1 clock cycles
        uint8_t enableBodDelaySteps : 5;
        // Flag to control whether analog reset error detection is enabled before applying critical trims
        uint8_t enableAnalogResetErrorDetectorBeforeTrim : 1;
        uint8_t res0 : 1;
        // Flash verify waitstates
        uint8_t flashVerifyWaitStates : 4;
        uint8_t res2 : 4;
        // Padding to get record size to multiple of 16 B
        uint8_t res1[9];
        // CRC32 integrity check critical trim
        uint32_t crc32;
    } criticalTrim;

    // Paperspin options     [80]: length 8 B
    // Defines peripheral/feature availability and accessible memory
    uint32_t hwOpts[2];


    // Device permissions   [88]: length 4 B
    // This is maximally-restrictive combined with similar field in CCFG
    struct {
        #define FCFG_PERMISSION_ALLOW  0xAU
        #define FCFG_PERMISSION_FORBID 0x0U
        // (all other value other than ALLOW are interpreted as FORBID)
        uint32_t allowReturnToFactory : 4;
        uint32_t allowFakeStby        : 4;
        uint32_t allowToolsClientMode : 4;
        uint32_t allowChipErase       : 4;
        uint32_t allowFlashProgram    : 4;
        uint32_t allowFlashVerify     : 4;
        uint32_t allowEnergyTrace     : 4;
        uint32_t allowDebugPort       : 4;
    } permissions;


    // Miscellaneous fields
    // [92]: length 4B
    struct {
        // SACI timeout is infinite when 0, else (2^saciTimeoutExp)*64 ms
        // Ccfg timeout applied instead if CCfg.saciTimeoutOverride==1
        uint32_t saciTimeoutExp     : 3;
            #define XCFG_MISC_SACITOEXP_8SEC        7
            #define XCFG_MISC_SACITOEXP_1SEC        4
            #define XCFG_MISC_SACITOEXP_INFINITE    0
        uint32_t res1               : 1;
        uint32_t allowMainAppErase  : 4;    // Extension of permissions above
        uint32_t res0               : 24;
    } misc;


    // Device information
    struct {    // [96]: length 48B
        // 64b device-unique UUID (non-sequential across parts)
        uint8_t uuid[8];
        // 48b device-unique BLE address
        uint8_t bleAddr[6];
        // Padding
        uint8_t res0[2];
        // 64b device-unique IEEE MAC address
        uint8_t macAddr[8];
        // 128b die identifier (lot #, wafer #, die X/Y, date, etc)
        uint8_t dieId[16];
        // Padding (previously devId but this HW-defined value is now readable through an MMR)
        uint32_t res1;
        // 32b PARTID (Part/Variant/majRev/minorRev)
        union {
            uint32_t val32;
            struct {
                // Random bit pattern to uniquely identify part (with devId)
                uint32_t part : 16;
                // Random bit pattern to uniquely identify package/memory variant
                uint32_t variant : 8;
                // Minor revision for orderable part (starts at 0)
                uint32_t minorRev : 4;
                // Major revision for orderable part: PCB/SW change (starts at 1)
                uint32_t majorRev : 3;
                // 0: partId not yet valid; 1: partId valid
                uint32_t partIdValid : 1;
            };
        } partId;
    } deviceInfo;


    // Flash protection     [144]: length 32 B
    // This is maximally-restrictive combined with similar field in CCFG
    struct {
        struct {
            // Sticky-0 bits written to VIMS.WEPRA (sectors 0-31, 1/bit)
            uint32_t mainSectors0_31;
            // Sticky-0 bits written to VIMS.WEPRB(0) (sectors 32-255, 8/bit)
            uint32_t mainSectors32_255;
            // Sticky-0 bits written to VIMS.WEPRB1 (sectors 256-511, 8/bit)
            uint32_t mainSectors256_511;
            // Sticky-0 bit written to VIMS.WEPRAUX
            union {
                uint32_t auxSectors;
                struct {
                    uint32_t ccfgSector  : 1;
                    uint32_t fcfgSector  : 1;
                    uint32_t engrSector  : 1;
                    uint32_t scfgSector  : 1;
                    uint32_t vlogSector  : 1;
                    uint32_t res1        : 27;
                };
            };
        } writeEraseProt;
        struct {
            union {
                uint32_t val32;
                struct {
                    // Sticky-0 bits written to VIMS.RDPRMN (0-4 2K blocks at beginning of main)
                    uint32_t mainSectors    : 4;
                    // Stick-0 bits written to VIMS.RDPREGR (entire sector)
                    uint32_t engrSector     : 1;
                        #define FLASH_MAIN_READPROT_2KB_BLOCKS(x) ( (~(x)) & 0x0F )
                    uint32_t res0           : 27;
                };
            };
            union {
                uint16_t nmnSectors;
                struct {
                    // Sticky-0 bits written to VIMS.RDPRNMN (watermark at end-x*16B)
                    uint16_t ccfgSector     : 6;
                        #define XCFG_READPROT_16B_BLOCKS(x) ( (~(x)) & 0x3F )
                    uint16_t res2           : 2;
                    uint16_t vlogSector     : 1;
                    uint16_t res3           : 7;
                };
            };
            union {
                uint16_t trmSectors;
                struct {
                    // Sticky-0 bits written to VIMS.RDPRTRM (watermark at end-x*16B)
                    uint16_t fcfgSector     : 6;
                        #define XCFG_READPROT_16B_BLOCKS(x) ( (~(x)) & 0x3F )
                    uint16_t res4           : 2;
                    uint16_t scfgSector     : 1;
                    uint16_t res5           : 7;
                };
            };
        } readProt;
        // Reserved/padding to get 16 B alignment
        uint32_t res0[2];
    } flashProt;

    // *******************************************************
    // ***         General Trims (copy list format)        ***
    // *******************************************************
    // [176]:  128 words
    uint32_t generalTrims[FCFG_GENERALTRIMS_SIZE];
        // Simple macros to assist in initializing copy lists
        // NOTE: Addresses to CPYLIST_CPY must fulfill (a & 0x0FF00003) == 3.
        //       The memory map ensures this for SRAM and peripherals.
        //       There is no check as ELF only supports {symbol + const offset}
        #define CPYLST_EOL               0
        #define CPYLST_NOP               0x10000000
        #define CPYLST_WAIT(x)           ( 0x10000000 + (((x)<<2)&0x000FFFFC) )
        #define CPYLST_CPY(a, n)         ( ((uint32_t)(a)) + (((n)<<20)&0x0FF00000) )
        #define CPYLST_CPYFULLADDR(a)    (((uint32_t)(a)) + 1)
        #define CPYLST_JUMP(a)           (((uint32_t)(a)) + 2)
        #define CPYLST_CALL(a)           (((uint32_t)(a)) + 3)


    // *******************************************************
    // ***        Extended Application Trims               ***
    // *******************************************************
    // Extended application trims.
    // [End-336]: length 64B
    struct appTrimsExt_struct {
        struct appTrimsExtCc27xx_s {     // length: 64B
            // Unused space
            uint8_t res[40];
            struct {    // length: 4B
                // Measured value on 1.8V for ODP core NMOS IDSAT
                uint16_t odpNmos;
                // Measured value on 1.8V for ODP core PMOS IDSAT
                uint16_t odpPmos;
            } odpVal1p8v;
            struct {    // length: 4B
                // Measured value on 1.2V for ODP core NMOS IDSAT
                uint16_t odpNmos;
                // Measured value on 1.2V for ODP core PMOS IDSAT
                uint16_t odpPmos;
            } odpVal1p2v;
            // Value for the FWT MP production program revision
            struct {    // length: 4B
                uint32_t revBranch      : 4;
                uint32_t revPatch       : 8;
                uint32_t revMinor       : 8;
                uint32_t revMajor       : 6;
                uint32_t variantCode    : 6;
            } tfwMp;
            // Value for the FWT FT production program revision
            struct {    // length: 4B
                uint32_t revBranch      : 4;
                uint32_t revPatch       : 8;
                uint32_t revMinor       : 8;
                uint32_t revMajor       : 6;
                uint32_t variantCode    : 6;
            } tfwFt;
            // Value for the MP production test program revision
            struct {    // length 4B
                uint32_t mp1Rev         : 8;
                uint32_t mp2Rev         : 8;
                uint32_t mp3Rev         : 8;
                uint32_t res            : 8;
            } ateMpRev;
            // Value for the FT production test program revision
            struct {    // length 4B
                uint32_t ft1Rev         : 8;
                uint32_t ft2Rev         : 8;
                uint32_t ft3Rev         : 8;
                uint32_t trimState      : 8;
            } ateFtRev;
        } cc27xx;
    } appTrimsExt;


    // *******************************************************
    // ***            Application Trims                    ***
    // *******************************************************
    // Application trims (individual fields are not referenced
    // by ROM). Note that the Application Trims section is
    // copied to SRAM by the SACI command SC_MODE_REQ_TOOLS_CLIENT.
    // [End-272]: length 128B
    struct appTrims_struct {
        // Revision of appTrims (defines layout)
        uint8_t revision;
        // Offset range to copy to SRAM in tools client mode to appTrims-n*16B.
        uint8_t nToolsClientOffset;
        // Pad to 32b alignment
        uint8_t res[2];

        struct appTrimsCc27xx_s {     // length: 124B
            // Trim value for LRFDRFE:PA2.TRIM
            struct {    // length: 2B
                uint16_t trim_mode0     : 5;
                uint16_t zero0          : 3;
                uint16_t trim_mode1     : 5;
                uint16_t zero1          : 3;
            } pa2_0;
            // Trim value for LRFDRFE:ATSTREFH.IREFTRIM
            struct {    // length: 2B
                uint16_t zero0          : 10;
                uint16_t irefTrim       : 5;
                uint16_t zero1          : 1;
            } atstRefH;
            // Trim value for LRFDRFE:LNA.TRIM
            struct {    // length: 2B
                uint16_t zero0          : 4;
                uint16_t trim           : 4;
                uint16_t zero1          : 8;
            } lna;
            // Trim value for LRFDRFE:IFAMPRFLDO.TRIM
            struct {    // length: 2B
                uint16_t zero           : 9;
                uint16_t trim           : 7;
            } ifampRfLdo;
            // Trim value for LRFDRFE:DIVLDO.VOUTTRIM
            struct {    // length: 2B
                uint16_t zero0          : 8;
                uint16_t voutTrim       : 7;
                uint16_t zero1          : 1;
            } divLdo;
            // Trim value for LRFDRFE:TDCLDO.VOUTTRIM
            struct {    // length: 2B
                uint16_t zero0          : 8;
                uint16_t voutTrim       : 7;
                uint16_t zero1          : 1;
            } tdcLdo;
            // Trim values for LRFDRFE:DCOLDO0
            struct {    // length: 2B
                uint16_t zero0          : 4;
                uint16_t firstTrim      : 4;
                uint16_t secondTrim     : 6;
                uint16_t zero1          : 2;
            } dcoLdo0;
            // Trim value for LRFDRFE:IFADCALDO.TRIMOUT
            struct {    // length: 2B
                uint16_t zero0          : 8;
                uint16_t trimout        : 6;
                uint16_t zero1          : 2;
            } ifadcAldo;
            // Trim value for LRFDRFE:IFADCDLDO.TRIMOUT
            struct {    // length: 2B
                uint16_t zero0          : 8;
                uint16_t trimout        : 6;
                uint16_t zero1          : 2;
            } ifadcDldo;
            // Trim value for LRFDRFE:DCO.TAILRESTRIM
            struct {    // length: 2B
                uint16_t zero0          : 3;
                uint16_t tailresTrim    : 4;
                uint16_t zero1          : 9;
            } dco;
            // Trim value for LRFDRFE:IFADCQUANT.QUANTTHR
            struct {    // length: 2B
                uint16_t quantThr       : 3;
                uint16_t zero           : 13;
            } ifadcQuant;
            // Trim values for LRFDRFE:IFADC0
            struct {    // length: 2B
                uint16_t zero0          : 2;
                uint16_t aafcap         : 2;
                uint16_t int2Adj        : 4;
                uint16_t zero1          : 2;
                uint16_t ditheren       : 2;
                uint16_t dithertrim     : 3;
                uint16_t zero2          : 1;
            } ifadc0;
            // Trim value for LRFDRFE:IFADC1.TRIM
            struct {    // length: 2B
                uint16_t zero0          : 9;
                uint16_t trim           : 6;
                uint16_t nrz            : 1;
            } ifadc1;
            // Trim values for LRFDRFE:IFADCLF
            struct {    // length: 2B
                uint16_t int3           : 4;
                uint16_t ff1            : 4;
                uint16_t ff2            : 4;
                uint16_t ff3            : 4;
            } ifadclf;
            // Trim value for LRFDRFE:IFADCQUANT.QUANTTHR high-bandwidth
            struct {    // length: 2B
                uint16_t quantThr       : 3;
                uint16_t zero           : 13;
            } ifadcQuantWbw;
            // Trim values for LRFDRFE:IFADC0 high-bandwidth
            struct {    // length: 2B
                uint16_t zero0          : 2;
                uint16_t aafcap         : 2;
                uint16_t int2Adj        : 4;
                uint16_t zero1          : 2;
                uint16_t ditheren       : 2;
                uint16_t dithertrim     : 3;
                uint16_t zero2          : 1;
            } ifadc0Wbw;
            // Trim value for LRFDRFE:IFADC1.TRIM high-bandwidth
            struct {    // length: 2B
                uint16_t zero0          : 9;
                uint16_t trim           : 6;
                uint16_t nrz            : 1;
            } ifadc1Wbw;
            // Trim values for LRFDRFE:IFADCLF high-bandwidth
            struct {    // length: 2B
                uint16_t int3           : 4;
                uint16_t ff1            : 4;
                uint16_t ff2            : 4;
                uint16_t ff3            : 4;
            } ifadclfWbw;
            // Trim values for RTRIM, synth LDO and HFXT Peak Detector
            struct {
                // RTRIM Min Offset.
                // Value represents the positive offset in RTRIM at -40 vs 25C. The calculated trim
                // adjustment will be added to nominal trim when temperature crosses -40 +TTHRL x 2^k.
                uint32_t rtrimMinOffset  : 2;
                // RTRIM Max Offset.
                // Value represents the positive offset in RTRIM at 125C vs 25C. The calculated trim adjustment
                // will be added to nominal trim when temperature crosses 125 - TTHRH x 2^k.
                uint32_t rtrimMaxOffset  : 2;
                // DIVLDO Min Offset.
                // Value represents 0, 10 (50mV), 20 (100mV), or 30 (150mV) positive trim LSB offset at -40 as
                // compared to 25C. The calculated trim adjustment will be added to nominal trim when temperature
                // crosses -40 + TTHRL x 2^k.
                uint32_t divLdoMinOffset : 2;
                // DIVLDO Max Offset.
                // Value represents 0, 10 (50mV), 20 (100mV), or 30 (150mV) positive trim LSB offset at 125C as
                // compared to 25C. The calculated trim adjustment will be added to nominal trim when temperature
                // crosses 125 - TTHRH x 2^k.
                uint32_t divLdoMaxOffset : 2;
                // TDCLDO Min Offset.
                // Value represents 0, 10 (50mV), 20 (100mV), or 30 (150mV) positive trim LSB offset at -40 as
                // compared to 25C. The calculated trim adjustment will be added to nominal trim when temperature
                // crosses -40 + TTHRL x 2^k.
                uint32_t tdcLdoMinOffset : 2;
                // TDCLDO Max Offset.
                // Value represents 0, 10 (50mV), 20 (100mV), or 30 (150mV) positive trim LSB offset at 125C as
                // compared to 25C. The calculated trim adjustment will be added to nominal trim when temperature
                // crosses 125 - TTHRH x 2^k.
                uint32_t tdcLdoMaxOffset : 2;
                // Temperature Threshold Low.
                // Value sets the temperature threshold for correction as -40C + x * 2^k, where k is TBD.
                // At temperatures below the threshold, a nominal trim value will be adjusted.
                uint32_t tThrl           : 2;
                // Temperature Threshold High.
                // Value sets the temperature threshold for correction as -40C + x * 2^k, where k is TBD.
                // At temperatures above the threshold, a nominal trim value will be adjusted.
                uint32_t tThrh           : 2;
                // Peak detector variation measured in production
                uint32_t hfxtPdError     : 8;
                // DCOLDO First Min Offset.
                // Value represents 0, 2 (20mV), 5 (50mV), or 7 (70mV) positive trim LSB offset at -40C as
                // compared to 25C. The calculated trim adjustment will be added linearly to nominal trim when
                // temperature is below 25C. The scaling of value to trim codes may change at later stage, and may not
                // be identical to scaling at high temp.
                uint32_t dcoLdoFirstMinOffset   : 2;
                // DCOLDO First Max Offset.
                // Value represents 0, 2 (20mV), 5 (50mV), or 7 (70mV) positive trim LSB offset at 125C as
                // compared to 25C. The calculated trim adjustment will be added linearly to nominal trim when
                // temperature is above 25C. The scaling of value to trim codes may change at later stage, and may not
                // be identical to scaling at low temp.
                uint32_t dcoLdoFirstMaxOffset   : 2;
                // DCOLDO Second Min Offset.
                // Value represents 0, 5 (25mV), 10 (50mV), or 15 (75mV) positive trim LSB offset at -40C as
                // compared to 25C. The calculated trim adjustment will be added linearly to nominal trim when
                // temperature is below 25C. The scaling of value to trim codes may change at later stage, and may not
                // be identical to scaling at high temp.
                uint32_t dcoLdoSecondMinOffset  : 2;
                // DCOLDO Second Max Offset.
                // Value represents 0, 5 (25mV), 10 (50mV), or 15 (75mV) positive trim LSB offset at 125C as
                // compared to 25C. The calculated trim adjustment will be added linearly to nominal trim when
                // temperature is above 25C. The scaling of value to trim codes may change at later stage, and may not
                // be identical to scaling at low temp.
                uint32_t dcoLdoSecondMaxOffset  : 2;
            } lrfdrfeExtTrim1;
            // Trim values for adjusting AGC and RSSI
            struct {
                // RSSI offset temperature compensation.
                // The COMP is added to RSSIOFFSET before programming RSSI offset to HW register.
                // Use for all PHYs.
                // Max compensation at 125C is -6 to +5.5 dB. Max compensation at -40C is -3.5 to 4 dB.
                // Add to LRFDRFE:RSSIOFFSET.
                // Formula: COMP = (temp-25)*signed(RSSITCOMP)/128
                uint32_t rssiTcomp      : 4;
                // AGC magn target temperature compensation.
                // The COMP is added to MAGN target before programming HW register (SPARE<x>).
                // Use for PHYs using the generic AGC.
                // Max compensation at 125C is -6 to +5.5 dB. Max compensation at -40C is -3.5 to 4 dB.
                // Add to LRFDRFE:SPARE<x>.
                // Formula: COMP = (temp-25)*signed(magnTcomp)/128
                uint32_t magnTcomp      : 4;
                // Fab dependent offset to add to AGC magnitude target (SPARE<x>).
                // Use for PHYs using the generic AGC. Add to LRFDRFE:SPARE<x>.
                // Formula: COMP = signed(magnOffset)
                uint32_t magnOffset     : 4;
                // Reserved
                uint32_t rfu            : 4;
                // AGC threshold temperature compensation.
                // The COMP is added to AGC threshold before programming HW register (SPARE<x>).
                // Use for PHYs with fast AGC (high gain / low gain type).
                // Formula: COMP = (temp-25)*signed(agcThrTcomp)/128
                uint32_t agcThrTcomp    : 4;
                // Fab dependent offset to add to AGC threshold target.
                // Use for PHYs with fast AGC (high gain / low gain type).
                // Add to LRFDRFE:SPARE<x>.
                // Formula: COMP = signed(agcThrOffset)
                uint32_t agcThrOffset   : 4;
                // Adjust the low gain setting with this signed number.
                // Use for PHYs with fast AGC (high gain / low gain type).
                // Add to LRFDRFE:SPARE0[3:0], check for overflow/underflow.
                // Formula: COMP = signed(lowGainOffset)
                uint32_t lowGainOffset  : 4;
                // Adjust the high gain setting with this signed number.
                // Use for PHYs with fast AGC (high gain / low gain type).
                // Add to LRFDRFE:SPARE0[7:4], check for overflow/underflow.
                // Formula: COMP = signed(lowGainOffset)
                uint32_t highGainOffset : 4;
            } lrfdrfeExtTrim0;
            // Trim values for front end 0
            struct {    // length: 2B
                // RSSI measured for front end 0 in production test.
                // Value is read by RF Core FW during RF Core initialization
                uint16_t offset         : 8;
                uint16_t trimCompleteN  : 1;
                uint16_t zero           : 7;
            } fend0Rssi;
            // Trim values for synth divider 0
            struct {    // length: 2B
                // Trim value for IQ mismatch compensation.
                // Value is read by RF Core FW during RF Core initialization
                uint16_t iqmc           : 16;
            } syntDiv0;
            // Additional Trim values for LRFDRFE:PA2.TRIM
            struct { // length: 2B
                uint16_t trim_mode2     : 5;
                uint16_t zero0          : 3;
                uint16_t trim_mode3     : 5;
                uint16_t zero1          : 3;
            } pa2_1;
            // Trim values for LRFDRFE:IFAMP.AAFCAP
            struct {    // length: 1B
                uint8_t zero            : 4;
                uint8_t aafcap          : 4;
            } ifamprfldo;
            // Trim values for LRFDRFE:IFAMP.AAFCAP high-bandwidth
            struct {    // length: 1B
                uint8_t zero            : 4;
                uint8_t aafcap          : 4;
            } ifamprfldoWbw;
            // Trim value for AUX Diode voltage at 30C
            struct {    // length: 4B
                uint16_t auxDiodeGnd;
                uint16_t auxDiodeVoltage;
            } auxDiodeCal30C;
            // Trim value for AUX Diode voltage at 125C
            struct {    // length: 4B
                uint16_t auxDiodeGnd;
                uint16_t auxDiodeVoltage;
            } auxDiodeCal125C;
            // Misc values
            struct {    // length: 4B
                uint32_t lfoscPpmRtn         : 8;
                uint32_t lfoscPpmTempMid     : 8;
                uint32_t lfoscPpmTempExt     : 8;
                uint32_t batMonTempSenseDelta125 : 8;
            } misc0;
            // Unused space
            uint8_t res2[16];
            // ADC offset for four modes
            struct {    // length: 4B
                uint32_t adcOffsetVdds       : 8;
                uint32_t adcOffsetExtref     : 8;
                uint32_t adcOffsetIntref2P5V : 8;
                uint32_t adcOffsetIntref1P4V : 8;
            } adcOffset;
            // ADC gain for VDDS and Extref modes
            struct {    // length: 4B
                uint32_t adcGainVdds         : 16;
                uint32_t adcGainExtref       : 16;
            } adcGainWord0;
            // ADC gain for Intref 2.5V and Intref 1.4V modes
            struct {    // length: 4B
                uint32_t adcGainIntref2P5V   : 16;
                uint32_t adcGainIntref1P4V   : 16;
            } adcGainWord1;
            // AFOSC Trims for the 80MHz set frequency
            struct {    // length: 2B
                uint16_t coarse : 5;
                uint16_t mid    : 4;
                uint16_t mode   : 1;
                uint16_t res0   : 6;
            } afosc80MHZ;
            // AFOSC Trims for the 90.3MHz set frequency
            struct {    // length: 2B
                uint16_t coarse : 5;
                uint16_t mid    : 4;
                uint16_t mode   : 1;
                uint16_t res0   : 6;
            } afosc90MHZ;
            // AFOSC Trims for the 98.3MHz set frequency
            struct {    // length: 2B
                uint16_t coarse : 5;
                uint16_t mid    : 4;
                uint16_t mode   : 1;
                uint16_t res0   : 6;
            } afosc98MHZ;
            // Unused space
            uint8_t res3[2];
            // Coefficients for AUX Diode temperature to voltage
            struct {    // length: 8B
                int16_t coeffP2;
                int16_t coeffP1;
                int16_t coeffP0;
                uint16_t coeffP2Shift   :5;
                uint16_t coeffP1Shift   :5;
                uint16_t coeffP0Shift   :5;
                uint16_t res0           :1;
            } auxDiodeCoeff;
            // Measured voltage output from AUXTEMPSENSE at 30C
            struct {    // length: 4B
                uint16_t auxTempSens30C;
                uint16_t zero;
            } auxTempSens;
            // Unused space
            uint8_t res4[8];
            // Measured I2V resistor error values
            struct {    // length: 4B
                uint32_t i2v20k   : 8;
                uint32_t i2v50k   : 8;
                uint32_t i2v100k  : 8;
                uint32_t i2v1m    : 8;
            } i2vCompact;
        } cc27xx;
    } appTrims;

    // Bootloader configuration
    struct {    // [End-144]: length 8B
        // Pointer to default bootloader VTOR table
        const void *pBldrVtor;
            #define XCFG_BC_PBLDR_FORBID   ((void*)((uint32_t*)0xFFFFFFFCU))
            #define XCFG_BC_PBLDR_UNDEF    ((void*)((uint32_t*)0xFFFFFFFFU))
            #define FCFG_BC_PBLDR_VALID(x) ((x) < XCFG_BC_PBLDR_FORBID)
        // Parameter passed to bootloader
        union {
            uint32_t val32;
            // Serial ROM bootloader parameters (defined in CCFG.h)
            serialRomBldrParam_t bldrParam;
        } bldrParam;
    } bootCfg;


    // [End-136] length: 4B
    uint32_t hsmSizeCfg;
        #define FCFG_HSM_SIZE_128KB     (VIMS_CFG_HSMSZ_SIZE_128)
        #define FCFG_HSM_SIZE_96KB      (VIMS_CFG_HSMSZ_SIZE_96)


    // CRC across hwOpts through res1 (after criticalTrim to here)
    // [End-132]: length 4B
    uint32_t crc32;

    // Lifecycle management
    struct {    // [End-128]: length 16B
        // 128b field updated incrementally (32b at a time) as lifecycle increments
        uint32_t states[4];
            // SET => 32b word has a value within a Hamming distance <=1 of (FCFG_SET32)
            // UNSET => all others (2T erased state is random)
            #define FCFG_SET32        0x41008002
            #define FCFG_UNSET32      0x41FFFFFF
            // Lifecycle BDAY1ST: FA unlock key integrity check fails
            // Other lifecycles: Fa unlock key integrity check OK + states words as below
            #define FCFG_LC_TESTPT  {FCFG_UNSET32, FCFG_UNSET32, FCFG_UNSET32, FCFG_UNSET32}
            #define FCFG_LC_TESTFT  {FCFG_SET32, FCFG_UNSET32, FCFG_UNSET32, FCFG_UNSET32}
            #define FCFG_LC_ENGRDEV {FCFG_SET32, FCFG_SET32, FCFG_UNSET32, FCFG_UNSET32}
            #define FCFG_LC_PRODDEV {FCFG_SET32, FCFG_SET32, FCFG_SET32, FCFG_UNSET32}
            #define FCFG_LC_RETEST  {FCFG_SET32, FCFG_SET32, FCFG_SET32, FCFG_SET32}
    } lifecycle;

    // *******************************************************
    // *** Read protection security barrier (16 B aligned) ***
    // *******************************************************
    // HSM config
    // [End-112]: length 64B
    struct {
        // 256b hash of the TI owned public RSA key
        uint8_t publicKeyHash[32];
        // 128b aesKey for HSM decryption
        uint8_t aesKey[32];
    } hsmUpdateKeys;


    // FA unlock password information
    // [End-48]: length 48B
    struct faUnlock_struct {
        // SHA256 hash of FA unlock password
        uint8_t hash[32];
        // 32b unique test ID used to lookup FA unlock password
        uint8_t testId[4];
        // CRC32 integrity check of (corrected) hash+testId
        uint32_t crc32;
        // Parity bits to do SECDED correction of errors in hash/testId/crc32
        uint8_t parity[5];
        // Indicates validity of struct (only in corrected RAM-copy, no purpose in flash)
        uint8_t isValid;
        // Padding
        uint8_t res0[2];
    } faUnlock;
} fcfg_t;

#ifndef DRIVERLIB_NS // FCFG is not accessible in non-secure mode.
/**! \brief Define to access FCFG struct pointer from application code.
 *
 * This definition can be used to access member elements with the `->`
 * operator.
 *
 *  \note The FCFG section is only accessible from secure context.
 */
#define fcfg ((const fcfg_t *)FCFG_BASE)
#endif // DRIVERLIB_NS

#endif // __HW_FCFG_H__
