/******************************************************************************
*  Filename:       hw_types.h
*
*  Description:    Common types and macros.
*
*  Copyright (c) 2015 - 2023, Texas Instruments Incorporated
*  All rights reserved.
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
*  3) Neither the name of the ORGANIZATION nor the names of its contributors may
*     be used to endorse or promote products derived from this software without
*     specific prior written permission.
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
*
******************************************************************************/

#ifndef __HW_TYPES_H__
#define __HW_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

//*****************************************************************************
//
// Common driverlib types
//
//*****************************************************************************
typedef void (* FPTR_VOID_VOID_T) (void);
typedef void (* FPTR_VOID_UINT8_T) (uint8_t);

//*****************************************************************************
//
// This symbol forces simple driverlib functions to be inlined in the code
// instead of using function calls.
//
//*****************************************************************************
#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif

//*****************************************************************************
//
// C99 types only allows bitfield defintions on certain datatypes.
//
//*****************************************************************************
typedef unsigned int  __UINT32;

//*****************************************************************************
//
// Macros for direct hardware access.
//
// If using these macros the programmer should be aware of any limitations to
// the address accessed i.e. if it supports word and/or byte access.
//
//*****************************************************************************
// Word (32 bit) access to address x
// Read example  : my32BitVar = HWREG(base_addr + offset) ;
// Write example : HWREG(base_addr + offset) = my32BitVar ;
#define HWREG(x)                                                              \
        (*((volatile unsigned long *)(x)))

// Half word (16 bit) access to address x
// Read example  : my16BitVar = HWREGH(base_addr + offset) ;
// Write example : HWREGH(base_addr + offset) = my16BitVar ;
#define HWREGH(x)                                                             \
        (*((volatile unsigned short *)(x)))

// Byte (8 bit) access to address x
// Read example  : my8BitVar = HWREGB(base_addr + offset) ;
// Write example : HWREGB(base_addr + offset) = my8BitVar ;
#define HWREGB(x)                                                             \
        (*((volatile unsigned char *)(x)))

// Word (32 bit) read to address x in the LRF module. On some device revisions,
// back-to-back memory reads to LRF does not read the correct value, this is
// worked around by always inserting 2 NOPs before loading from address x.
#define HWREG_READ_LRF(x) ({ \
    uint32_t tmpHwregReadLrfVal; \
    __asm volatile("nop\n\t" \
                   "nop\n\t" \
                   "ldr %[val], [%[adr]]" \
                   : [val] "=r" (tmpHwregReadLrfVal) \
                   : [adr] "r" (x)); \
    tmpHwregReadLrfVal; })

// Half word (16 bit) access to address x in the LRF module. On some device revisions,
// back-to-back memory reads to LRF does not read the correct value, this is
// worked around by always inserting 2 NOPs before loading from address x.
#define HWREGH_READ_LRF(x) ({ \
    uint16_t tmpHwregReadLrfVal; \
    __asm volatile("nop\n\t" \
                   "nop\n\t" \
                   "ldrh %[val], [%[adr]]" \
                   : [val] "=r" (tmpHwregReadLrfVal) \
                   : [adr] "r" (x)); \
    tmpHwregReadLrfVal; })

// Byte (8 bit) access to address x in the LRF module. On some device revisions,
// back-to-back memory reads to LRF does not read the correct value, this is
// worked around by always inserting 2 NOPs before loading from address x.
#define HWREGB_READ_LRF(x) ({ \
    uint8_t tmpHwregReadLrfVal; \
    __asm volatile("nop\n\t" \
                   "nop\n\t" \
                   "ldrb %[val], [%[adr]]" \
                   : [val] "=r" (tmpHwregReadLrfVal) \
                   : [adr] "r" (x)); \
    tmpHwregReadLrfVal; })

// Utility macro used by HWREGx_WRITE_LRF macros
#define ASM_4_NOPS() \
    __asm volatile("nop\n\t" \
                   "nop\n\t" \
                   "nop\n\t" \
                   "nop\n\t")

// Word (32 bit) write to address x in the LRF module. On some device revisions,
// back-to-back memory writes may not go through, this is worked around by always
// inserting 4 NOPs before writing to address x.
#define HWREG_WRITE_LRF(x) ASM_4_NOPS(); HWREG(x)

// Half word (16 bit) write to address x in the LRF module. On some device revisions,
// back-to-back memory writes may not go through, this is worked around by always
// inserting 4 NOPs before writing to address x.
#define HWREGH_WRITE_LRF(x) ASM_4_NOPS(); HWREGH(x)

// Byte (8 bit) write to address x in the LRF module. On some device revisions,
// back-to-back memory writes may not go through, this is worked around by always
// inserting 4 NOPs before writing to address x.
#define HWREGB_WRITE_LRF(x) ASM_4_NOPS(); HWREGB(x)

#endif // __HW_TYPES_H__
