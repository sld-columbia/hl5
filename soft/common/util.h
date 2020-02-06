// See LICENSE for license details.

#ifndef __UTIL_H
#define __UTIL_H

//--------------------------------------------------------------------------
// Macros

// Set HOST_DEBUG to 1 if you are going to compile this for a host
// machine (ie Athena/Linux) for debug purposes and set HOST_DEBUG
// to 0 if you are compiling with the smips-gcc toolchain.

#ifndef HOST_DEBUG
#define HOST_DEBUG 0
#endif

// Set PREALLOCATE to 1 if you want to preallocate the benchmark
// function before starting stats. If you have instruction/data
// caches and you don't want to count the overhead of misses, then
// you will need to use preallocation.

#ifndef PREALLOCATE
#define PREALLOCATE 0
#endif


#include <stdint.h>



typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


typedef char char8;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint64_t u64;
typedef int sint32;

typedef intptr_t INTPTR;
typedef u32 UINTPTR;


static inline void Xil_Out32(UINTPTR Addr, u32 Value)
{
	volatile u32 *LocalAddr = (volatile u32 *)Addr;
	*LocalAddr = Value;
}

static inline u32 Xil_In32(UINTPTR Addr)
{
	return *(volatile u32 *) Addr;
}

/*
 * Offset from the device base address to the IP registers.
 */
#define XUN_REG_OFFSET 0x1000

/** @name Register Map
 *
 * Register offsets for the 16450/16550 compatible UART device.
 * @{
 */
#define XUN_RBR_OFFSET	(XUN_REG_OFFSET) /**< Receive buffer, read only */
#define XUN_THR_OFFSET	(XUN_REG_OFFSET) /**< Transmit holding register */
#define XUN_IER_OFFSET	(XUN_REG_OFFSET + 0x04) /**< Interrupt enable */
#define XUN_IIR_OFFSET	(XUN_REG_OFFSET + 0x08) /**< Interrupt id, read only */
#define XUN_FCR_OFFSET	(XUN_REG_OFFSET + 0x08) /**< Fifo control, write only */
#define XUN_LCR_OFFSET	(XUN_REG_OFFSET + 0x0C) /**< Line Control Register */
#define XUN_MCR_OFFSET	(XUN_REG_OFFSET + 0x10) /**< Modem Control Register */
#define XUN_LSR_OFFSET	(XUN_REG_OFFSET + 0x14) /**< Line Status Register */
#define XUN_MSR_OFFSET	(XUN_REG_OFFSET + 0x18) /**< Modem Status Register */
#define XUN_DRLS_OFFSET	(XUN_REG_OFFSET + 0x00) /**< Divisor Register LSB */
#define XUN_DRLM_OFFSET	(XUN_REG_OFFSET + 0x04) /**< Divisor Register MSB */
/* @} */

/*
 * The following constant specifies the size of the FIFOs, the size of the
 * FIFOs includes the transmitter and receiver such that it is the total number
 * of bytes that the UART can buffer
 */
#define XUN_FIFO_SIZE			16


/**
 * @name Interrupt Enable Register (IER) mask(s)
 * @{
 */
#define XUN_IER_MODEM_STATUS	0x00000008 /**< Modem status interrupt */
#define XUN_IER_RX_LINE		0x00000004 /**< Receive status interrupt */
#define XUN_IER_TX_EMPTY	0x00000002 /**< Transmitter empty interrupt */
#define XUN_IER_RX_DATA		0x00000001 /**< Receiver data available */
/* @} */

/**
 * @name Interrupt ID Register (INT_ID) mask(s)
 * @{
 */
#define XUN_INT_ID_MASK		 0x0000000F /**< Only the interrupt ID */
#define XUN_INT_ID_FIFOS_ENABLED 0x000000C0 /**< Only the FIFOs enable */
/* @} */

/**
 * @name FIFO Control Register mask(s)
 * @{
 */
#define XUN_FIFO_RX_TRIG_MSB	0x00000080 /**< Trigger level MSB */
#define XUN_FIFO_RX_TRIG_LSB	0x00000040 /**< Trigger level LSB */
#define XUN_FIFO_TX_RESET	0x00000004 /**< Reset the transmit FIFO */
#define XUN_FIFO_RX_RESET	0x00000002 /**< Reset the receive FIFO */
#define XUN_FIFO_ENABLE		0x00000001 /**< Enable the FIFOs */
#define XUN_FIFO_RX_TRIGGER	0x000000C0 /**< Both trigger level bits */
/* @} */

/**
 * @name Line Control Register(LCR) mask(s)
 * @{
 */
#define XUN_LCR_DLAB		0x00000080 /**< Divisor latch access */
#define XUN_LCR_SET_BREAK	0x00000040 /**< Cause a break condition */
#define XUN_LCR_STICK_PARITY	0x00000020 /**< Stick Parity */
#define XUN_LCR_EVEN_PARITY	0x00000010 /**< 1 = even, 0 = odd parity */
#define XUN_LCR_ENABLE_PARITY	0x00000008 /**< 1 = Enable, 0 = Disable parity*/
#define XUN_LCR_2_STOP_BITS	0x00000004 /**< 1= 2 stop bits,0 = 1 stop bit */
#define XUN_LCR_8_DATA_BITS	0x00000003 /**< 8 Data bits selection */
#define XUN_LCR_7_DATA_BITS	0x00000002 /**< 7 Data bits selection */
#define XUN_LCR_6_DATA_BITS	0x00000001 /**< 6 Data bits selection */
#define XUN_LCR_LENGTH_MASK	0x00000003 /**< Both length bits mask */
#define XUN_LCR_PARITY_MASK	0x00000018 /**< Both parity bits mask */
/* @} */

/**
 * @name Mode Control Register(MCR) mask(s)
 * @{
 */
#define XUN_MCR_LOOP		0x00000010 /**< Local loopback */
#define XUN_MCR_OUT_2		0x00000008 /**< General output 2 signal */
#define XUN_MCR_OUT_1		0x00000004 /**< General output 1 signal */
#define XUN_MCR_RTS		0x00000002 /**< RTS signal */
#define XUN_MCR_DTR		0x00000001 /**< DTR signal */
/* @} */

/**
 * @name Line Status Register(LSR) mask(s)
 * @{
 */
#define XUN_LSR_RX_FIFO_ERROR	0x00000080 /**< An errored byte is in FIFO */
#define XUN_LSR_TX_EMPTY	0x00000040 /**< Transmitter is empty */
#define XUN_LSR_TX_BUFFER_EMPTY 0x00000020 /**< Transmit holding reg empty */
#define XUN_LSR_BREAK_INT	0x00000010 /**< Break detected interrupt */
#define XUN_LSR_FRAMING_ERROR	0x00000008 /**< Framing error on current byte */
#define XUN_LSR_PARITY_ERROR	0x00000004 /**< Parity error on current byte */
#define XUN_LSR_OVERRUN_ERROR	0x00000002 /**< Overrun error on receive FIFO */
#define XUN_LSR_DATA_READY	0x00000001 /**< Receive data ready */
#define XUN_LSR_ERROR_BREAK	0x0000001E /**< Errors except FIFO error and
						break detected */
/* @} */

#define XUN_DIVISOR_BYTE_MASK	0x000000FF

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

/****************************************************************************/
/**
* Read a UART register.
*
* @param	BaseAddress contains the base address of the device.
* @param	RegOffset contains the offset from the 1st register of the
*		device to select the specific register.
*
* @return	The value read from the register.
*
* @note		C-Style signature:
*		u32 XUartNs550_ReadReg(u32 BaseAddress, u32 RegOffset);
*
******************************************************************************/
#define XUartNs550_ReadReg(BaseAddress, RegOffset) \
	Xil_In32((BaseAddress) + (RegOffset))

/****************************************************************************/
/**
* Write to a UART register.
*
* @param	BaseAddress contains the base address of the device.
* @param	RegOffset contains the offset from the 1st register of the
*		device to select the specific register.
* @param	RegisterValue is the value to be written to the regsiter.
*
* @return	None.
*
* @note		C-Style signature:
*		u32 XUartNs550_WriteReg(u32 BaseAddress, u32 RegOffset,
*						u32 RegisterValue);
*
******************************************************************************/
#define XUartNs550_WriteReg(BaseAddress, RegOffset, RegisterValue) \
	Xil_Out32((BaseAddress) + (RegOffset), (RegisterValue))

/****************************************************************************/
/**
* Get the UART Line Status Register.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	The value read from the register.
*
* @note		C-Style signature:
*		u32 XUartNs550_GetLineStatusReg(u32 BaseAddress);
*
******************************************************************************/
#define XUartNs550_GetLineStatusReg(BaseAddress)   \
	XUartNs550_ReadReg((BaseAddress), XUN_LSR_OFFSET)

/****************************************************************************/
/**
* Get the UART Line Status Register.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	The value read from the register.
*
* @note		C-Style signature:
*		u32 XUartNs550_GetLineControlReg(u32 BaseAddress);
*
******************************************************************************/
#define XUartNs550_GetLineControlReg(BaseAddress)  \
	XUartNs550_ReadReg((BaseAddress), XUN_LCR_OFFSET)

/****************************************************************************/
/**
* Set the UART Line Status Register.
*
* @param	BaseAddress contains the base address of the device.
* @param	RegisterValue is the value to be written to the register.
*
* @return	None.
*
* @note		C-Style signature:
*		void XUartNs550_SetLineControlReg(u32 BaseAddress,
*				u32 RegisterValue);
*
******************************************************************************/
#define XUartNs550_SetLineControlReg(BaseAddress, RegisterValue) \
	XUartNs550_WriteReg((BaseAddress), XUN_LCR_OFFSET, (RegisterValue))

/****************************************************************************/
/**
* Enable the transmit and receive interrupts of the UART.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XUartNs550_EnableIntr(u32 BaseAddress);,
*
******************************************************************************/
#define XUartNs550_EnableIntr(BaseAddress)				\
	XUartNs550_WriteReg((BaseAddress), XUN_IER_OFFSET,		\
			 XUartNs550_ReadReg((BaseAddress), XUN_IER_OFFSET) | \
			 (XUN_IER_RX_LINE | XUN_IER_TX_EMPTY | XUN_IER_RX_DATA))

/****************************************************************************/
/**
* Disable the transmit and receive interrupts of the UART.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	None.
*
* @note		C-Style signature:
*		void XUartNs550_DisableIntr(u32 BaseAddress);,
*
******************************************************************************/
#define XUartNs550_DisableIntr(BaseAddress)				\
	XUartNs550_WriteReg((BaseAddress), XUN_IER_OFFSET,		\
			XUartNs550_ReadReg((BaseAddress), XUN_IER_OFFSET) & \
			~(XUN_IER_RX_LINE | XUN_IER_TX_EMPTY | XUN_IER_RX_DATA))

/****************************************************************************/
/**
* Determine if there is receive data in the receiver and/or FIFO.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	TRUE if there is receive data, FALSE otherwise.
*
* @note		C-Style signature:
*		int XUartNs550_IsReceiveData(u32 BaseAddress);,
*
******************************************************************************/
#define XUartNs550_IsReceiveData(BaseAddress)				\
	(XUartNs550_GetLineStatusReg(BaseAddress) & XUN_LSR_DATA_READY)

/****************************************************************************/
/**
* Determine if a byte of data can be sent with the transmitter.
*
* @param	BaseAddress contains the base address of the device.
*
* @return	TRUE if a byte can be sent, FALSE otherwise.
*
* @note		C-Style signature:
*		int XUartNs550_IsTransmitEmpty(u32 BaseAddress);,
*
******************************************************************************/
#define XUartNs550_IsTransmitEmpty(BaseAddress)			\
	(XUartNs550_GetLineStatusReg(BaseAddress) & XUN_LSR_TX_BUFFER_EMPTY)

/************************** Function Prototypes ******************************/

void XUartNs550_SendByte(u32 BaseAddress, u8 Data);

u8 XUartNs550_RecvByte(u32 BaseAddress);

void XUartNs550_SetBaud(u32 BaseAddress, u32 InputClockHz, u32 BaudRate);



// Set SET_STATS to 1 if you want to carve out the piece that actually
// does the computation.

#if HOST_DEBUG
#include <stdio.h>
static void setStats(int enable) {}
#else
extern void setStats(int enable);
#endif


#define static_assert(cond) switch(0) { case 0: case !!(long)(cond): ; }

static void printArray(const char name[], int n, const int arr[])
{
/* #if HOST_DEBUG */
  int i;
  printf( " %10s :", name );
  for ( i = 0; i < n; i++ )
    printf( " %3d ", arr[i] );
  printf( "\n" );
/* #endif */
}

static void printDoubleArray(const char name[], int n, const double arr[])
{
#if HOST_DEBUG
  int i;
  printf( " %10s :", name );
  for ( i = 0; i < n; i++ )
    printf( " %g ", arr[i] );
  printf( "\n" );
#endif
}

static int verify(int n, const volatile int* test, const int* verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n/2*2; i+=2)
  {
    int t0 = test[i], t1 = test[i+1];
    int v0 = verify[i], v1 = verify[i+1];
    if (t0 != v0) return i+1;
    if (t1 != v1) return i+2;
  }
  if (n % 2 != 0 && test[n-1] != verify[n-1])
    return n;
  return 0;
}

static int verifyDouble(int n, const volatile double* test, const double* verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n/2*2; i+=2)
  {
    double t0 = test[i], t1 = test[i+1];
    double v0 = verify[i], v1 = verify[i+1];
    int eq1 = t0 == v0, eq2 = t1 == v1;
    if (!(eq1 & eq2)) return i+1+eq1;
  }
  if (n % 2 != 0 && test[n-1] != verify[n-1])
    return n;
  return 0;
}

static void __attribute__((noinline)) barrier(int ncores)
{
  static volatile int sense;
  static volatile int count;
  static __thread int threadsense;

  __sync_synchronize();

  threadsense = !threadsense;
  if (__sync_fetch_and_add(&count, 1) == ncores-1)
  {
    count = 0;
    sense = threadsense;
  }
  else while(sense != threadsense)
    ;

  __sync_synchronize();
}

static uint64_t lfsr(uint64_t x)
{
  uint64_t bit = (x ^ (x >> 1)) & 1;
  return (x >> 1) | (bit << 62);
}

#ifdef __riscv
#include "encoding.h"
#endif

#define stringify_1(s) #s
#define stringify(s) stringify_1(s)
#define stats(code, iter) do { \
    unsigned long _c = -read_csr(mcycle), _i = -read_csr(minstret); \
    code; \
    _c += read_csr(mcycle), _i += read_csr(minstret); \
    if (cid == 0) \
      printf("\n%s: %ld cycles, %ld.%ld cycles/iter, %ld.%ld CPI\n", \
             stringify(code), _c, _c/iter, 10*_c/iter%10, _c/_i, 10*_c/_i%10); \
  } while(0)

#endif //__UTIL_H
