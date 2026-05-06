
#include 	"uart.h"
#include	"stm32f446re.h"

char buffer[MAX_LENGTH];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;

void uart_send_char(uint8_t c){
	while(!(*((volatile uint32_t *)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 7))){}; // Wait until TXE is 1 (Tx data register empty)
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET)) = c;
}
void uart_send_string(const char* str){
	while((*str)!='\0'){
		uart_send_char(*str);
		str++;
	}
}
void USART2_IRQHandler(void){
	if( *((volatile uint32_t*)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 5) ){ // Check if interrupt fired because data has been received
		buffer[head]= *((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET));
		head = (head + 1) % MAX_LENGTH; // reset circular buffer to beginning

		// Interrupt cleared automatically by hardware after reading clear after reading
	} // refactored to circular buffer
}
uint8_t uart_receive_string(char* str, uint8_t maxlen){
	static uint8_t index = 0;
	uint8_t complete = 0;
	if(head != tail)
	{
		if (buffer[tail] == '\r' || buffer[tail] == '\n')
		{
			if (index > 0)
			{
				str[index]   = '\0';
				index = 0;
				complete = 1;
			}
			tail = (tail + 1) % MAX_LENGTH;
		}
		else
		{
			if (index < MAX_LENGTH - 1)
			{
				str[index++] = buffer[tail];
			}
			tail = (tail + 1) % MAX_LENGTH;
		}
	}
	return complete;
}
void init_uart(void){
		//Enable clock for USART
		*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_APB1ENR_OFFSET))	|= (0x1 << 17);
		// Uart on pin PA2 Tx
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	&= ~(0x3 << 2*2); //Alternate function
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	|= (0x2 << 2*2); //Alternate function
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	&= ~(0xF << 4*2); //AF 7 for Tx
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	|= (0x7 << 4*2); //AF 7 for Tx
		// Uart on pin PA3 Rx
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	&= ~(0x3 << 2*3); //Alternate function
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) |= (0x2 << 2*3); //Alternate Function mode (MODER bits [7:6])
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	&= ~(0xF << 4*3); //AF 7 for Tx
		*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))  |= (0x7 << 4*3); // AF7 (AFRL bits [15:12])

		// USART2 Config
		*((volatile uint32_t *)(USART2_BASE_ADDR + USART_BRR_OFFSET))	|= (0x8B); // Baudrate 115200
		*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET))	|= (0x1 << 3); //UART Enable TE - bit 3
		*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET)) 	|= (0x1 << 2); //UART Enable RE - bit 2
		//	Enable Interrupt RXNEIE
		*((volatile uint32_t*)(USART2_BASE_ADDR + USART_CR1_OFFSET)) 	|= 	(0x1 << 5);
		// 	Enable NVIC
		*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER1_OFFSET))		|= (0x1 << USART2_POS);
		*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET))	|= (0x1 << 13); //UART Enable UE - bit 13
}
