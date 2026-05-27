/**************************************************************************//**
 * @file     cmsis_compiler.h
 * @brief    CMSIS compiler generic header file
 * @version  V5.0.1
 * @date     30. January 2017
 ******************************************************************************/
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CMSIS_COMPILER_H
#define __CMSIS_COMPILER_H

#include <stdint.h>

/*
 * ARM Compiler 4/5
 */
#if   defined ( __CC_ARM )
  #include "cmsis_armcc.h"


/*
 * ARM Compiler 6 (armclang)
 */
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #include "cmsis_armclang.h"


/*
 * GNU Compiler
 */
#elif defined ( __GNUC__ )
  #include "cmsis_gcc.h"


/*
 * Clang/LLVM Compiler
 */
#elif defined ( __clang__ )
  #define __ASM                     _asm
  #define __INLINE                  inline
  #define __STATIC_INLINE           static inline
  #define __NO_RETURN
  #define __USED                    __attribute__((used))
  #define __WEAK                    __attribute__((weak))
  #define __UNALIGNED_UINT32(x)     (((struct T_UINT32 *)(x))->v)
  #define __ALIGNED(x)              __attribute__((aligned(x)))
  #define __PACKED                  __attribute__((packed))
  #define __PACKED_STRUCT           struct __attribute__((packed))


/*
 * IAR Compiler
 */
#elif defined ( __ICCARM__ )
  #ifndef   __ASM
    #define __ASM                     __asm
  #endif
  #ifndef   __INLINE
    #define __INLINE                  inline
  #endif
  #ifndef   __STATIC_INLINE
    #define __STATIC_INLINE           static inline
  #endif
  #include <cmsis_iar.h>


/*
 * TASKING Compiler
 */
#elif defined ( __TASKING__ )
  /*
   * The CMSIS functions have been implemented as intrinsics in the compiler.
   * Please use "carm -?i" to get an up to date list of all intrinsics,
   * Including the CMSIS ones.
   */


#else
  #error Unknown compiler.
#endif


#endif /* __CMSIS_COMPILER_H */
