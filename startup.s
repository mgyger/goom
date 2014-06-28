@ Copyright 2014 Mark Owen
@ http://www.quinapalus.com
@ E-mail: goom@quinapalus.com
@ 
@ This file is free software: you can redistribute it and/or modify
@ it under the terms of version 2 of the GNU General Public License
@ as published by the Free Software Foundation.
@ 
@ This file is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@ GNU General Public License for more details.
@ 
@ You should have received a copy of the GNU General Public License
@ along with this file.  If not, see <http://www.gnu.org/licenses/> or
@ write to the Free Software Foundation, Inc., 51 Franklin Street,
@ Fifth Floor, Boston, MA  02110-1301, USA.

.syntax unified
.cpu cortex-m3
.thumb

.extern main
.extern CT16B0handler
.global inton
.global intoff

.equ ram   ,0x10000000
.equ ramtop,0x10002000

.org 0
.word ramtop @ initial SP
.word main+1  @ reset vector +1 for Thumb mode
.word 0      @ other vectors
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0

.word 0 @ WAKEUP0_IRQn
.word 0 @ WAKEUP1_IRQn
.word 0 @ WAKEUP2_IRQn
.word 0 @ WAKEUP3_IRQn
.word 0 @ WAKEUP4_IRQn
.word 0 @ WAKEUP5_IRQn
.word 0 @ WAKEUP6_IRQn
.word 0 @ WAKEUP7_IRQn
.word 0 @ WAKEUP8_IRQn
.word 0 @ WAKEUP9_IRQn
.word 0 @ WAKEUP10_IRQn
.word 0 @ WAKEUP11_IRQn
.word 0 @ WAKEUP12_IRQn
.word 0 @ WAKEUP13_IRQn
.word 0 @ WAKEUP14_IRQn
.word 0 @ WAKEUP15_IRQn
.word 0 @ WAKEUP16_IRQn
.word 0 @ WAKEUP17_IRQn
.word 0 @ WAKEUP18_IRQn
.word 0 @ WAKEUP19_IRQn
.word 0 @ WAKEUP20_IRQn
.word 0 @ WAKEUP21_IRQn
.word 0 @ WAKEUP22_IRQn
.word 0 @ WAKEUP23_IRQn
.word 0 @ WAKEUP24_IRQn
.word 0 @ WAKEUP25_IRQn
.word 0 @ WAKEUP26_IRQn
.word 0 @ WAKEUP27_IRQn
.word 0 @ WAKEUP28_IRQn
.word 0 @ WAKEUP29_IRQn
.word 0 @ WAKEUP30_IRQn
.word 0 @ WAKEUP31_IRQn
.word 0 @ WAKEUP32_IRQn
.word 0 @ WAKEUP33_IRQn
.word 0 @ WAKEUP34_IRQn
.word 0 @ WAKEUP35_IRQn
.word 0 @ WAKEUP36_IRQn
.word 0 @ WAKEUP37_IRQn
.word 0 @ WAKEUP38_IRQn
.word 0 @ WAKEUP39_IRQn
.word 0 @ I2C_IRQn
.word CT16B0handler+1 @ TIMER_16_0_IRQn
.word 0 @ TIMER_16_1_IRQn
.word CT32B0handler+1 @ TIMER_32_0_IRQn
.word 0 @ TIMER_32_1_IRQn
.word 0 @ SSP0_IRQn
.word 0 @ UART_IRQn
.word 0 @ USB_IRQn
.word 0 @ USB_FIQn
.word 0 @ ADC_IRQn
.word 0 @ WDT_IRQn
.word 0 @ BOD_IRQn
.word 0 @ EINT3_IRQn
.word 0 @ EINT2_IRQn
.word 0 @ EINT1_IRQn
.word 0 @ EINT0_IRQn
.word 0 @ SSP1_IRQn

inton:
 cpsie i
 bx r14

intoff:
 cpsid i
 bx r14
