/*
 * MIT License
 *
 * Copyright (c) 2020 Yuuki Takano <ytakanoster@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Copyright (c) 2016-2019 Raspberry Pi (Trading) Ltd.
 * Copyright (c) 2016 Stephen Warren <swarren@wwwdotorg.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

.section .init, "x"
.global _start
.global el2_setup

// #define NOEL3

#ifdef raspi4
#define BCM2711 1
#define GIC     1
#endif

#define BIT(x) (1 << (x))

#if BCM2711
#define LOCAL_CONTROL       0xff800000
#define LOCAL_PRESCALER     0xff800008
#else
#define LOCAL_CONTROL       0x40000000
#define LOCAL_PRESCALER     0x40000008
#endif
#define GIC_DISTB           0xff841000
#define GIC_CPUB            0xff842000

#if BCM2711
#define OSC_FREQ            54000000
#else
#define OSC_FREQ            19200000
#endif

#define SCR_RW              BIT(10)
#define SCR_HCE             BIT(8)
#define SCR_SMD             BIT(7)
#define SCR_RES1_5          BIT(5)
#define SCR_RES1_4          BIT(4)
#define SCR_NS              BIT(0)
#define SCR_VAL \
    (SCR_RW | SCR_HCE | SCR_RES1_5 | SCR_RES1_4 | SCR_NS)

#define CPUECTLR_EL1        S3_1_C15_C2_1
#define CPUECTLR_EL1_SMPEN  BIT(6)

#define SPSR_EL3_D          BIT(9)
#define SPSR_EL3_A          BIT(8)
#define SPSR_EL3_I          BIT(7)
#define SPSR_EL3_F          BIT(6)
#define SPSR_EL3_MODE_el3H  9
#define SPSR_EL3_VAL \
    (SPSR_EL3_D | SPSR_EL3_A | SPSR_EL3_I | SPSR_EL3_F | SPSR_EL3_MODE_el3H)

#define L2CTLR_EL1          S3_1_C11_C0_2


#define GICC_CTRLR          0x0
#define GICC_PMR            0x4
#define IT_NR               0x8 // Number of interrupt enable registers (256 total irqs)
#define GICD_CTRLR          0x0
#define GICD_IGROUPR        0x80

_start:
completed:
//    bl      mmu_init
//    bl      entry
	// set stack before _start
    ldr     x1, =__stack_firm_start
    mov     sp, x1

	bl      main
.L5:
    wfe
    b       .L5
.global _start_el2
_start_el2:
	bl      led_test
_start_el1:
	bl      led_test
.global _el3_el2
#define MODE_AARCH64_EL2H 0x9
#define MODE_AARCH64_EL1H 0x5
_el3_el2:
    ldr     x1, =__stack_el2_start
    msr     sp_el2, x1
	mov   x0, #0x4b1    // RW=1, SMD=1, RES=1, NS=1
	msr   scr_el3, x0
	adr   x0, _start_el2
	msr   elr_el3, x0
	mov   x0, #MODE_AARCH64_EL2H
	msr   spsr_el3, x0
	eret
.global _el3_el1
_el3_el1:
    ldr     x1, =__stack_el1_start
    msr     sp_el1, x1

	mov x0, #3 << 20
	msr cpacr_el1, x0           /* Enable FP/SIMD at EL1 */

	/*=============================================================*/
	/*      Initialize sctlr_el1                                   */
	/*=============================================================*/
	mov x0, xzr
	orr x0, x0, #(1 << 29)          /* Checking http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHDIEBD.html */
	orr x0, x0, #(1 << 28)          /* Bits 29,28,23,22,20,11 should be 1 (res1 on documentation) */
	orr x0, x0, #(1 << 23)
	orr x0, x0, #(1 << 22)
	orr x0, x0, #(1 << 20)
	orr x0, x0, #(1 << 11)
	msr sctlr_el1, x0

	/*=============================================================*/
	/*      Initialize scr_el3                                     */
	/*=============================================================*/
	mrs x0, scr_el3
	orr x0, x0, #(1<<10)        /* Lower EL is 64bits */
	msr scr_el3, x0

	/*=============================================================*/
	/*      Initialize spsr_el3                                    */
	/*=============================================================*/
	mov x0, xzr
	mov x0, #0b00101            /* EL1 */
	orr x0, x0, #(1 << 8)       /* Enable SError and External Abort. */
	orr x0, x0, #(1 << 7)       /* IRQ interrupt Process state mask. */
	orr x0, x0, #(1 << 6)       /* FIQ interrupt Process state mask. */
	msr spsr_el3, x0

	/*=============================================================*/
	/*      Initialize elr_el3                                     */
	/*=============================================================*/
	adr x0, _start_el1
	msr elr_el3, x0

	eret
	
