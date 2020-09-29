#include <stdint.h>

/* Clock related registers */
#define CLK_CKDIVR  (*((volatile uint64_t *) 0x0050C0))
#define CLK_PCKENR  (*((volatile uint64_t *) 0x0050C3))

/* GPIOB related registers */
#define PB_ODR      (*((volatile uint64_t *) 0x005005))
#define PB_IDR      (*((volatile uint64_t *) 0x005006))
#define PB_DDR      (*((volatile uint64_t *) 0x005007))
#define PB_CR1      (*((volatile uint64_t *) 0x005008))
#define PB_CR2      (*((volatile uint64_t *) 0x005009))

/* GPIOC related registers */
#define PC_ODR      (*((volatile uint64_t *) 0x00500A))
#define PC_IDR      (*((volatile uint64_t *) 0x00500B))
#define PC_DDR      (*((volatile uint64_t *) 0x00500C))
#define PC_CR1      (*((volatile uint64_t *) 0x00500D))
#define PC_CR2      (*((volatile uint64_t *) 0x00500E))

/* USART related registers */
#define USART_SR    (*((volatile uint64_t *) 0x005230))
#define USART_DR    (*((volatile uint64_t *) 0x005231))
#define USART_BRR1  (*((volatile uint64_t *) 0x005232))
#define USART_BRR2  (*((volatile uint64_t *) 0x005233))
#define USART_CR1   (*((volatile uint64_t *) 0x005234))
#define USART_CR2   (*((volatile uint64_t *) 0x005235))
#define USART_CR3   (*((volatile uint64_t *) 0x005236))
#define USART_CR4   (*((volatile uint64_t *) 0x005237))

/* AWU related registers */
#define AWU_CSR     (*((volatile uint64_t *) 0x0050F0))
#define AWU_APR     (*((volatile uint64_t *) 0x0050F1))
#define AWU_TBR     (*((volatile uint64_t *) 0x0050F2))

/************************************************************
 *  PIN     *   Function                                    *
 * **********************************************************
 *  PC0     *   "SET". AT Command mode or transmission mode *
 ************************************************************
 *  PC1     *   "CS". Turn on/off JDY-40 RF module          *
 ************************************************************
 *  PC2     *   "UART-RX". Receive data from the JDY-40     *
 ************************************************************
 *  PC3     *   "UART-TX". Transmit data to the JDY-40      *
 ************************************************************
 *  PB1     *   "OC". Turn on/off the optocoupler           *
 ************************************************************
 *  PB7     *   "EN". Enable the DC/DC boost converter      *
 ************************************************************/

int main(void)
{
    CLK_CKDIVR = 0x02;      /* HSIDIV is 0b11 so the 16 MHz clock is divived by 8 */
    CLK_PCKENR = 0x60;      /* Enable USART to communicate with RF module
                             * Enable Auto-WakeUp (AWU) which gets the CPU out of active-halt sleep mode */ 
   
    /* Port C configuration */     
    PC_DDR = 0x0B;          /* PC0, PC1 and PC3 are output (1) and PC2 is input (0) */      
    PC_CR1 = 0x0B;          /* PC0, PC1 and PC1 are Push-pull (1). TODO: check for PC2 floating or pull-up */
    PC_CR2 = 0x00;          /* Do not use interrupt and output speed of up to 2 MHz */

    /* Port B configuration */
    PB_DDR = 0x41;          /* PB1 and PB7 are output pins */ 
    PB_CR1 = 0x41;          /* PB1 and PB7 are push-pull pins */
    PB_CR2 = 0x00;          /* Do not use interrupt and output speed of up to 2 MHz */

    /* USART_DIV should be 208 assuming a clock of 2 MHz frequency */ 
    USART_CR1 = 0x00;       /* Disable USART, 8 data bits, no parity verification */
    USART_CR2 = 0x0C;       /* Enable the receiver and transmitter. Do not use interupts */
    USART_CR3 = 0x00;       /* Send 1 stop bit. TODO: Check CPOL and CPHA bits */

    AWU_APR = 0x3E;         
    AWU_TBR = 0x0F;         /* Maximum amount of interval (17.79sec - 103.5sec) */    
    AWU_CSR = 0x10;         /* Enable the AWU */
    __asm__("HALT\n");      /* Enter active-halt mode */ 

      

    return 0;       /*  We should never reach this point */
}
