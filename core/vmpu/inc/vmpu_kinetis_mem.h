/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
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
#ifndef __VMPU_FREESCALE_K64_MEM_H__
#define __VMPU_FREESCALE_K64_MEM_H__

#ifndef UVISOR_MAX_ACLS
#define UVISOR_MAX_ACLS 16
#endif/*UVISOR_MAX_ACLS*/

/* default permission mask for the background MPU region
 *    | # | Bus master | Supervisor | User  |
 *    |---|------------|------------|-------|
 *    | 0 | core       | r/w        | -     |
 *    | 1 | debugger   | r/w/x      | r/w/x |
 *    | 2 | dma        | -          | -     |
 *    | 3 | enet       | -          | -     |
 *    | 4 | usb        | NA         | -     |
 *    | 5 | core       | NA         | -     |
 */


// the following macros are serviceable only for MPU regions 0 - 3
#define MPU_BUS_MASTER_CORE 0
#define MPU_BUS_MASTER_DEBUGGER 1
#define MPU_BUS_MASTER_DMA 2
#define MPU_BUS_MASTER_ENET 3

#define MPU_RGDn_WORD2_Mx_UM_R_SM_RWX       0x00000004U
#define MPU_RGDn_WORD2_Mx_UM_RW_SM_RWX      0x00000006U
#define MPU_RGDn_WORD2_Mx_UM_RWX_SM_RWX     0x00000007U
#define MPU_RGDn_WORD2_Mx_UM_NONE_SM_RW     0x00000010U
#define MPU_RGDn_WORD2_Mx_UM_NONE_SM_NONE   0x00000018U

#define MPU_RGDn_WORD2_Mx(bus_master, perm) (perm << (bus_master * 6))

#define UVISOR_TACL_CORE_BACKGROUND MPU_RGDn_WORD2_Mx(MPU_BUS_MASTER_CORE, MPU_RGDn_WORD2_Mx_UM_NONE_SM_RW)
#define UVISOR_TACL_DEBUGGER_BACKGROUND ( \
    MPU_RGDn_WORD2_Mx(MPU_BUS_MASTER_CORE, MPU_RGDn_WORD2_Mx_UM_NONE_SM_NONE) | \
    MPU_RGDn_WORD2_Mx(MPU_BUS_MASTER_DEBUGGER, MPU_RGDn_WORD2_Mx_UM_RWX_SM_RWX) \
)

extern int vmpu_mem_add(uint8_t box_id, void* start, uint32_t size, UvisorBoxAcl acl);
extern void vmpu_mem_switch(uint8_t src_box, uint8_t dst_box);
extern uint32_t vmpu_fault_find_acl_mem(uint8_t box_id, uint32_t fault_addr, uint32_t size);
extern void vmpu_mem_init(void);
extern int vmpu_mem_push_page_acl(uint32_t start_addr, uint32_t end_addr);

#endif/*__VMPU_FREESCALE_K64_MEM_H__*/
