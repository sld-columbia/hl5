// See LICENSE for license details.

#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include "util.h"

#define SYS_write 64
#define SYS_exit 93
#define SYS_stats 1234

#undef strcmp

/******************************************************************************
*
* Copyright (C) 2002 - 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xuartns550_l.c
* @addtogroup uartns550_v3_3
* @{
*
* This file contains low-level driver functions that can be used to access the
* device.  The user should refer to the hardware device specification for more
* details of the device operation.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b jhl  04/24/02 First release
* 1.11a sv   03/20/07 Updated to use the new coding guidelines.
* 2.00a sdm  09/22/09 Converted all register accesses to 32 bit access.
* 2.00a ktn  10/20/09 Converted all register accesses to 32 bit access.
*		      Updated to use HAL Processor APIs. _m is removed from the
*		      name of all the macro definitions.
* 3.3	nsk  04/13/15 Fixed Clock Divisor Enhancement.
*		      (CR 857013)
* 3.4   sk   11/10/15 Used UINTPTR instead of u32 for Baseaddress CR# 867425.
*                     Changed the prototypes of XUartNs550_SendByte,
*                     XUartNs550_RecvByte, XUartNs550_SetBaud APIs.
* </pre>
*
******************************************************************************/


/***************************** Include Files *********************************/

//#include "xuartns550_l.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/


/****************************************************************************/
/**
*
* This function sends a data byte with the UART. This function operates in the
* polling mode and blocks until the data has been put into the UART transmit
* holding register.
*
* @param	BaseAddress contains the base address of the UART.
* @param	Data contains the data byte to be sent.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void inline XUartNs550_SendByte(UINTPTR BaseAddress, u8 Data)
{
	/*
	 * Wait til we know that the byte can be sent, the 550 does not have any
	 * way to tell how much room is in the FIFO such that we must wait for
	 * it to be empty
	 */
	while (!XUartNs550_IsTransmitEmpty(BaseAddress));

	/*
	 * Write the data byte to the UART to be transmitted
	 */
	XUartNs550_WriteReg(BaseAddress, XUN_THR_OFFSET, (u32)Data);
}

/****************************************************************************/
/**
*
* This function receives a byte from the UART. It operates in a polling mode
* and blocks until a byte of data is received.
*
* @param	BaseAddress contains the base address of the UART.
*
* @return	The data byte received by the UART.
*
* @note		None.
*
*****************************************************************************/
u8 XUartNs550_RecvByte(UINTPTR BaseAddress)
{
	/*
	 * Wait for there to be data received
	 */
	while (!XUartNs550_IsReceiveData(BaseAddress));

	/*
	 * Return the next data byte the UART received
	 */
	return (u8) XUartNs550_ReadReg(BaseAddress, XUN_RBR_OFFSET);
}


/****************************************************************************/
/**
*
* Set the baud rate for the UART.
*
* @param	BaseAddress contains the base address of the UART.
* @param	InputClockHz is the frequency of the input clock to the device
*		in Hertz.
* @param	BaudRate is the baud rate to be set.
*
* @return	None.
*
* @note		None.
*
*****************************************************************************/
void XUartNs550_SetBaud(UINTPTR BaseAddress, u32 InputClockHz, u32 BaudRate)
{

	u32 BaudLSB;
	u32 BaudMSB;
	u32 LcrRegister;
	u32 Divisor;

	/*
	 * Determine what the divisor should be to get the specified baud
	 * rater based upon the input clock frequency and a baud clock prescaler
	 * of 16
	 */
	Divisor = ((InputClockHz +((BaudRate * 16UL)/2)) /
			(BaudRate * 16UL));
	/*
	 * Get the least significant and most significant bytes of the divisor
	 * so they can be written to 2 byte registers
	 */
	BaudLSB = Divisor & XUN_DIVISOR_BYTE_MASK;
	BaudMSB = (Divisor >> 8) & XUN_DIVISOR_BYTE_MASK;

	/*
	 * Get the line control register contents and set the divisor latch
	 * access bit so the baud rate can be set
	 */
	LcrRegister = XUartNs550_GetLineControlReg(BaseAddress);
	XUartNs550_SetLineControlReg(BaseAddress,
					LcrRegister | XUN_LCR_DLAB);

	/*
	 * Set the baud Divisors to set rate, the initial write of 0xFF is to
	 * keep the divisor from being 0 which is not recommended as per the
	 * NS16550D spec sheet
	 */
	XUartNs550_WriteReg(BaseAddress, XUN_DRLS_OFFSET, 0xFF);
	XUartNs550_WriteReg(BaseAddress, XUN_DRLM_OFFSET, BaudMSB);
	XUartNs550_WriteReg(BaseAddress, XUN_DRLS_OFFSET, BaudLSB);

	/*
	 * Clear the Divisor latch access bit, DLAB to allow nornal
	 * operation and write to the line control register
	 */
	XUartNs550_SetLineControlReg(BaseAddress, LcrRegister);
}
/** @} */

extern volatile uint64_t tohost;
extern volatile uint64_t fromhost;

static uintptr_t handle_frontend_syscall(uintptr_t which, uint64_t arg0, uint64_t arg1, uint64_t arg2)
{
  volatile uint64_t magic_mem[8] __attribute__((aligned(64)));
  magic_mem[0] = which;
  magic_mem[1] = arg0;
  magic_mem[2] = arg1;
  magic_mem[3] = arg2;
  __sync_synchronize();

  tohost = (uintptr_t)magic_mem;
  while (fromhost == 0)
    ;
  fromhost = 0;

  __sync_synchronize();
  return magic_mem[0];
}

#define NUM_COUNTERS 2
static uintptr_t counters[NUM_COUNTERS];
static char* counter_names[NUM_COUNTERS];

static int handle_stats(int enable)
{
  int i = 0;
#define READ_CTR(name) do { \
    while (i >= NUM_COUNTERS) ; \
    uintptr_t csr = read_csr(name); \
    if (!enable) { csr -= counters[i]; counter_names[i] = #name; } \
    counters[i++] = csr; \
  } while (0)

  READ_CTR(mcycle);
  READ_CTR(minstret);

#undef READ_CTR
  return 0;
}

void __attribute__((noreturn)) tohost_exit(uintptr_t code)
{
  tohost = (code << 1) | 1;
  while (1);
}

uintptr_t handle_trap(uintptr_t cause, uintptr_t epc, uintptr_t regs[32])
{
  if (cause != CAUSE_MACHINE_ECALL)
    tohost_exit(1337);
  else if (regs[17] == SYS_exit)
    tohost_exit(regs[10]);
  else if (regs[17] == SYS_stats)
    regs[10] = handle_stats(regs[10]);
  else
    regs[10] = handle_frontend_syscall(regs[17], regs[10], regs[11], regs[12]);

  return epc + ((*(unsigned short*)epc & 3) == 3 ? 4 : 2);
}

static uintptr_t syscall(uintptr_t num, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2)
{
  register uintptr_t a7 asm("a7") = num;
  register uintptr_t a0 asm("a0") = arg0;
  register uintptr_t a1 asm("a1") = arg1;
  register uintptr_t a2 asm("a2") = arg2;
  asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a7));
  return a0;
}

void exit(int code)
{
  syscall(SYS_exit, code, 0, 0);
  while (1);
}

void setStats(int enable)
{
  syscall(SYS_stats, enable, 0, 0);
}

void printstr(const char* s)
{
  syscall(SYS_write, 1, (uintptr_t)s, strlen(s));
}

void __attribute__((weak)) thread_entry(int cid, int nc)
{
  // multi-threaded programs override this function.
  // for the case of single-threaded programs, only let core 0 proceed.
  while (cid != 0);
}

int __attribute__((weak)) main(int argc, char** argv)
{
  // single-threaded programs override this function.
  printstr("Implement main(), foo!\n");
  return -1;
}

static void init_tls()
{
  register void* thread_pointer asm("tp");
  extern char _tls_data;
  extern __thread char _tdata_begin, _tdata_end, _tbss_end;
  size_t tdata_size = &_tdata_end - &_tdata_begin;
  memcpy(thread_pointer, &_tls_data, tdata_size);
  size_t tbss_size = &_tbss_end - &_tdata_end;
  memset(thread_pointer + tdata_size, 0, tbss_size);
}

void _init(int cid, int nc)
{
  init_tls();
  thread_entry(cid, nc);

  // only single-threaded programs should ever get here.
  int ret = main(0, 0);

  char buf[NUM_COUNTERS * 32] __attribute__((aligned(64)));
  char* pbuf = buf;
  for (int i = 0; i < NUM_COUNTERS; i++)
    if (counters[i])
      pbuf += sprintf(pbuf, "%s = %d\n", counter_names[i], counters[i]);
  if (pbuf != buf)
    printstr(buf);

  exit(ret);
}

#undef putchar
int putchar(int ch)
{
	XUartNs550_SendByte(0x60000000, ch);

  /* static __thread char buf[64] __attribute__((aligned(64))); */
  /* static __thread int buflen = 0; */

  /* buf[buflen++] = ch; */

  /* if (ch == '\n' || buflen == sizeof(buf)) */
  /* { */
  /*   syscall(SYS_write, 1, (uintptr_t)buf, buflen); */
  /*   buflen = 0; */
  /* } */

  return 0;
}

void printhex(uint64_t x)
{
  char str[17];
  int i;
  for (i = 0; i < 16; i++)
  {
    str[15-i] = (x & 0xF) + ((x & 0xF) < 10 ? '0' : 'a'-10);
    x >>= 4;
  }
  str[16] = 0;

  printstr(str);
}

static inline void printnum(void (*putch)(int, void**), void **putdat,
                    unsigned long long num, unsigned base, int width, int padc)
{
  unsigned digs[sizeof(num)*CHAR_BIT];
  int pos = 0;

  while (1)
  {
    digs[pos++] = num % base;
    if (num < base)
      break;
    num /= base;
  }

  while (width-- > pos)
    putch(padc, putdat);

  while (pos-- > 0)
    putch(digs[pos] + (digs[pos] >= 10 ? 'a' - 10 : '0'), putdat);
}

static unsigned long long getuint(va_list *ap, int lflag)
{
  if (lflag >= 2)
    return va_arg(*ap, unsigned long long);
  else if (lflag)
    return va_arg(*ap, unsigned long);
  else
    return va_arg(*ap, unsigned int);
}

static long long getint(va_list *ap, int lflag)
{
  if (lflag >= 2)
    return va_arg(*ap, long long);
  else if (lflag)
    return va_arg(*ap, long);
  else
    return va_arg(*ap, int);
}

static void vprintfmt(void (*putch)(int, void**), void **putdat, const char *fmt, va_list ap)
{
  register const char* p;
  const char* last_fmt;
  register int ch, err;
  unsigned long long num;
  int base, lflag, width, precision, altflag;
  char padc;

  while (1) {
    while ((ch = *(unsigned char *) fmt) != '%') {
      if (ch == '\0')
        return;
      fmt++;
      putch(ch, putdat);
    }
    fmt++;

    // Process a %-escape sequence
    last_fmt = fmt;
    padc = ' ';
    width = -1;
    precision = -1;
    lflag = 0;
    altflag = 0;
  reswitch:
    switch (ch = *(unsigned char *) fmt++) {

    // flag to pad on the right
    case '-':
      padc = '-';
      goto reswitch;
      
    // flag to pad with 0's instead of spaces
    case '0':
      padc = '0';
      goto reswitch;

    // width field
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      for (precision = 0; ; ++fmt) {
        precision = precision * 10 + ch - '0';
        ch = *fmt;
        if (ch < '0' || ch > '9')
          break;
      }
      goto process_precision;

    case '*':
      precision = va_arg(ap, int);
      goto process_precision;

    case '.':
      if (width < 0)
        width = 0;
      goto reswitch;

    case '#':
      altflag = 1;
      goto reswitch;

    process_precision:
      if (width < 0)
        width = precision, precision = -1;
      goto reswitch;

    // long flag (doubled for long long)
    case 'l':
      lflag++;
      goto reswitch;

    // character
    case 'c':
      putch(va_arg(ap, int), putdat);
      break;

    // string
    case 's':
      if ((p = va_arg(ap, char *)) == NULL)
        p = "(null)";
      if (width > 0 && padc != '-')
        for (width -= strnlen(p, precision); width > 0; width--)
          putch(padc, putdat);
      for (; (ch = *p) != '\0' && (precision < 0 || --precision >= 0); width--) {
        putch(ch, putdat);
        p++;
      }
      for (; width > 0; width--)
        putch(' ', putdat);
      break;

    // (signed) decimal
    case 'd':
      num = getint(&ap, lflag);
      if ((long long) num < 0) {
        putch('-', putdat);
        num = -(long long) num;
      }
      base = 10;
      goto signed_number;

    // unsigned decimal
    case 'u':
      base = 10;
      goto unsigned_number;

    // (unsigned) octal
    case 'o':
      // should do something with padding so it's always 3 octits
      base = 8;
      goto unsigned_number;

    // pointer
    case 'p':
      static_assert(sizeof(long) == sizeof(void*));
      lflag = 1;
      putch('0', putdat);
      putch('x', putdat);
      /* fall through to 'x' */

    // (unsigned) hexadecimal
    case 'x':
      base = 16;
    unsigned_number:
      num = getuint(&ap, lflag);
    signed_number:
      printnum(putch, putdat, num, base, width, padc);
      break;

    // escaped '%' character
    case '%':
      putch(ch, putdat);
      break;
      
    // unrecognized escape sequence - just print it literally
    default:
      putch('%', putdat);
      fmt = last_fmt;
      break;
    }
  }
}

int printf(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  vprintfmt((void*)putchar, 0, fmt, ap);

  va_end(ap);
  return 0; // incorrect return value, but who cares, anyway?
}

int sprintf(char* str, const char* fmt, ...)
{
  va_list ap;
  char* str0 = str;
  va_start(ap, fmt);

  void sprintf_putch(int ch, void** data)
  {
    char** pstr = (char**)data;
    **pstr = ch;
    (*pstr)++;
  }

  vprintfmt(sprintf_putch, (void**)&str, fmt, ap);
  *str = 0;

  va_end(ap);
  return str - str0;
}

void* memcpy(void* dest, const void* src, size_t len)
{
  if ((((uintptr_t)dest | (uintptr_t)src | len) & (sizeof(uintptr_t)-1)) == 0) {
    const uintptr_t* s = src;
    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = *s++;
  } else {
    const char* s = src;
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = *s++;
  }
  return dest;
}

void* memset(void* dest, int byte, size_t len)
{
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = byte;
  }
  return dest;
}

size_t strlen(const char *s)
{
  const char *p = s;
  while (*p)
    p++;
  return p - s;
}

size_t strnlen(const char *s, size_t n)
{
  const char *p = s;
  while (n-- && *p)
    p++;
  return p - s;
}

int strcmp(const char* s1, const char* s2)
{
  unsigned char c1, c2;

  do {
    c1 = *s1++;
    c2 = *s2++;
  } while (c1 != 0 && c1 == c2);

  return c1 - c2;
}

char* strcpy(char* dest, const char* src)
{
  char* d = dest;
  while ((*d++ = *src++))
    ;
  return dest;
}

long atol(const char* str)
{
  long res = 0;
  int sign = 0;

  while (*str == ' ')
    str++;

  if (*str == '-' || *str == '+') {
    sign = *str == '-';
    str++;
  }

  while (*str) {
    res *= 10;
    res += *str++ - '0';
  }

  return sign ? -res : res;
}
