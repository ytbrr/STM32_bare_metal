#include "uart.h"

#define GPIOAEN 				(1U << 0)
#define UART2EN					(1U << 17)

#define SR_TXE 					(1U << 7)
#define SR_RXNE					(1U << 5)

#define CR1_TE					(1U << 3)
#define CR1_RE					(1U << 2)

#define CR1_UE					(1U << 3)

#define SYS_FREQ				16000000
#define APB1_CLK				16000000

#define UART_BAUDRATE			115200

static void set_BaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


void UART2_Write(int ch);

int __IO_putchar(int ch)
{
	UART2_Write(ch);
	return ch;
}

void uart2_rxtx_init(void)
{
	/* configure UART GPIO pin */
	/* enable clock access to GPIOA and set PA2 mode to alternate*/
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);
	/* set PA2 alternate function type to UART_TX AF07*/
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= ~(1U << 11);

	/* enable clock access to GPIOA and set PA3 mode to alternate*/
	GPIOA->MODER &= ~(1U << 6);
	GPIOA->MODER |= (1U << 7);
	/* set PA3 alternate function type to UART_RX AF07*/
	GPIOA->AFR[0] |= (1U << 12);
	GPIOA->AFR[0] |= (1U << 13);
	GPIOA->AFR[0] |= (1U << 14);
	GPIOA->AFR[0] &= ~(1U << 15);


	/* configure UART module */
	/* enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;
	/* transfer direction */
	set_BaudRate(USART2, APB1_CLK, UART_BAUDRATE);
	USART2->CR1 = (CR1_RE | CR1_TE);

	/* enable UART module */
	USART2->CR1 |= (CR1_UE);
}

void uart2_tx_init(void)
{
	/* configure UART GPIO pin */
	/* enable clock access to GPIOA and set PA2 mode to alternate*/
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER &= ~(1U << 4);
	GPIOA->MODER |= (1U << 5);
	/* set PA2 alternate function type to UART_TX AF07*/
	GPIOA->AFR[0] |= (1U << 8);
	GPIOA->AFR[0] |= (1U << 9);
	GPIOA->AFR[0] |= (1U << 10);
	GPIOA->AFR[0] &= !(1U << 11);

	/* configure UART module */
	/* enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;
	/* transfer direction */
	set_BaudRate(USART2, APB1_CLK, UART_BAUDRATE);
	USART2->CR1 = CR1_TE;

	/* enable UART module */
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void)
{
	/* make sure receive data register is not empty*/
	while (!(USART2->SR & SR_RXNE)) {}
	/* Read data*/
	return USART2->DR;
}

void UART2_Write(int ch)
{
	/*transmit data register should be empty*/
	while (!(USART2->SR & SR_TXE)) {}
	/* write to transmit data register*/
	USART2->DR = (ch & 0xff);
}

static void set_BaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return (PeriphClk + (BaudRate/2U))/BaudRate;
}

