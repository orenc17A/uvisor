/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <uvisor.h>
#include "semaphore.h"

int semaphore_init(UvisorSemaphore * semaphore, int32_t count)
{
#if (__ARM_FEATURE_CMSE == 3)
    int result;
    asm volatile (
        "mov r0, %[semaphore]\n"
        "mov r1, %[count]\n"
        "blxns %[init]\n"
        "mov %[result], r0\n"
        : [result] "=r" (result)
        : [init] "r" (((uint32_t) __uvisor_config.lib_hooks->semaphore_init) & ~1UL),
          [semaphore] "r" (UVISOR_GET_NS_ALIAS(semaphore)),
          [count] "r" (count)
    );
    return result;
#else
    /* If privileged mode, die because of uVisor programmer error. Semaphores
     * must be initialized from outside of uVisor. */
    assert(__get_IPSR() == 0U);
    return __uvisor_config.lib_hooks->semaphore_init(semaphore, count);
#endif
}

int semaphore_pend(UvisorSemaphore * semaphore, uint32_t timeout_ms)
{
#if (__ARM_FEATURE_CMSE == 3)
    int result;
    asm volatile (
        "mov r0, %[semaphore]\n"
        "mov r1, %[timeout_ms]\n"
        "blxns %[pend]\n"
        "mov %[result], r0\n"
        : [result] "=r" (result)
        : [pend] "r" (((uint32_t) __uvisor_config.lib_hooks->semaphore_pend) & ~1UL),
          [semaphore] "r" (UVISOR_GET_NS_ALIAS(semaphore)),
          [timeout_ms] "r" (timeout_ms)
    );
    return result;
#else
    /* If privileged mode, die because of uVisor programmer error. Semaphores
     * can't be pended upon from inside uVisor. */
    assert(__get_IPSR() == 0U);
    return __uvisor_config.lib_hooks->semaphore_pend(semaphore, timeout_ms);
#endif
}

int semaphore_post(UvisorSemaphore * semaphore)
{
#if (__ARM_FEATURE_CMSE == 3)
    int result;
    asm volatile (
        "mov r0, %[semaphore]\n"
        "blxns %[init]\n"
        "mov %[result], r0\n"
        : [result] "=r" (result)
        : [init] "r" (((uint32_t) __uvisor_config.priv_sys_hooks->priv_uvisor_semaphore_post) & ~1UL),
          [semaphore] "r" (UVISOR_GET_NS_ALIAS(semaphore))
    );
    return result;
#else
    /* We call the flash based hook directly (instead of the RAM-based hook
     * `g_priv_sys_hooks`) because this code will also be called from
     * unprivileged code (which doesn't have access to uVisor-private SRAM). */
    return __uvisor_config.priv_sys_hooks->priv_uvisor_semaphore_post(semaphore);
#endif
}
