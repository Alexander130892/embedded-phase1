
#include 	"uart.h"
#include	"stm32f446re.h"

static char buffer[UART_MAX_LENGTH];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

status_t uart_init(void){
	// Enable clocks
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_APB1ENR_OFFSET))	|=  (1u << 17);		//Enable clock for USART

	// USART2 Config
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_BRR_OFFSET))	|= (UART_BAUDRATE_115200);
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET))	|= (0x1 << 3); //UART Enable TE - bit 3
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET)) 	|= (0x1 << 2); //UART Enable RE - bit 2
	//	Enable Interrupt RXNEIE
	*((volatile uint32_t*)(USART2_BASE_ADDR + USART_CR1_OFFSET)) 	|= 	(0x1 << 5);
	// 	Enable NVIC
	*((volatile uint32_t*)(NVIC_BASE_ADDR + NVIC_ISER1_OFFSET))		|= (0x1 << USART2_POS);
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_CR1_OFFSET))	|= (0x1 << 13); //UART Enable UE - bit 13
	return STATUS_OK;
}
status_t uart_gpio_init(void){
	// Enable clocks
	*((volatile uint32_t *)(RCC_BASE_ADDR + RCC_AHB1ENR_OFFSET)) 	|= 	(1u << 0);		//  Bit 0,1 for GPIOA
	// Uart on pin PA2 Tx
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	&= ~(0x3 << 2*2); //Alternate function
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	|= (0x2 << 2*2); //Alternate function
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	&= ~(0xF << 4*2); //AF 7 for Tx
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	|= (UART_ALT_FUNC << 4*2); //AF 7 for Tx
	// Uart on pin PA3 Rx
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET))	&= ~(0x3 << 2*3); //Alternate function
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_MODER_OFFSET)) |= (0x2 << 2*3); //Alternate Function mode (MODER bits [7:6])
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))	&= ~(0xF << 4*3); //AF 7 for Tx
	*((volatile uint32_t *)(GPIOA_BASE_ADDR + GPIOx_AFRL_OFFSET))  |= (UART_ALT_FUNC << 4*3); // AF7 (AFRL bits [15:12])
	return STATUS_OK;
}
status_t uart_send_char(uint8_t c){
	uint32_t timeout = 0;
	while(!(*((volatile uint32_t *)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 7))){// Wait until TXE is 1 (Tx data register empty)
		timeout++;
		if(timeout > MAX_TIMEOUT) return UART_TIMEOUT;
	}
	*((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET)) = c;
	return STATUS_OK;
}
status_t uart_send_string(const char* str){
	status_t status = STATUS_OK;
	while((*str)!='\0'){
		status = uart_send_char(*str);
		str++;
		if (status){
			return status;
		}
	}
	return status;
}
void USART2_IRQHandler(void){
	if( *((volatile uint32_t*)(USART2_BASE_ADDR + USART_SR_OFFSET)) & (0x1 << 5) ){ // Check if interrupt fired because data has been received
		buffer[head]= *((volatile uint32_t *)(USART2_BASE_ADDR + USART_DR_OFFSET));
		head = (head + 1) % UART_MAX_LENGTH; // reset circular buffer to beginning

		// Interrupt cleared automatically by hardware after reading clear after reading
	} // refactored to circular buffer
}
bool uart_receive_string(char* str, uint8_t maxlen){
	static uint8_t index = 0;
	bool complete = false;
	if(head != tail)
	{
		if (buffer[tail] == '\r' || buffer[tail] == '\n')
		{
			if (index > 0)
			{
				str[index]   = '\0';
				index = 0;
				complete = true;
			}
			tail = (tail + 1) % UART_MAX_LENGTH;
		}
		else
		{
			if (index < UART_MAX_LENGTH - 1)
			{
				str[index++] = buffer[tail];
			}
			tail = (tail + 1) % UART_MAX_LENGTH;
		}
	}
	return complete;
}

