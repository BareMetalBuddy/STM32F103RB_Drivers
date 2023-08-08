#include <stdint.h>
#include "timx.h"
#include "gpio.h"
#include "rcc.h"
#include "nvic.h"
#include "usart.h"
#include "bsp.h"
#include "stdio.h"
#include "cli.h"
#include "rtc.h"
#include "pwr.h"
#include "iwdt.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

GPIO_Handler_t status_led;
GPIO_Handler_t navigation_led;

GPIO_Handler_t modem_tx;
GPIO_Handler_t modem_rx;
GPIO_Handler_t cli_tx;
GPIO_Handler_t cli_rx;

TIMx_Handler_t timx_handler;
USART_Handler_t usart2;
USART_Handler_t usart1;

RTC_Handler rtc;
IWDT_handler iwdt;

/*
 * Prototypes
 * */
void command_line_interface(void);
void execute_command(char *line);

void cmd_help(int argc, char *argv[]);
void cmd_led(int argc, char *argv[]);
void cmd_apn(int argc, char *argv[]);
void cmd_connection(int argc, char *argv[]);
void cmd_protocol(int argc, char *argv[]);

Command command_table[] = {
    {"help", cmd_help},
    {"led", cmd_led},
	{"apn",cmd_apn},
	{"connection",cmd_connection},
	{"protocol",cmd_protocol},
    {NULL, NULL}
};

/*
 * Printf implementation
 * */
int __io_putchar(int ch)
{
	usart_tx(&usart1,(uint8_t)ch);
	return ch;
}


int main(void)
{
	RCC_GPIOA_CLOCK_ENABLE();

	gpio_init(&status_led,GPIOA_BASE_ADDRESS,GPIO_pin_5,GPIO_General_purpose_output_push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&status_led);

	gpio_init(&navigation_led,GPIOA_BASE_ADDRESS,GPIO_pin_1,GPIO_General_purpose_output_push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&navigation_led);

	gpio_write_pin(&status_led,0);
	gpio_write_pin(&navigation_led,0);

	RCC_TIM2_CLOCK_ENABLE();

	timx_init(&timx_handler,TIM2_BASE_ADDRESS,TIM2_PRESCALER_VALUE_FOR_100MS,TIM2_RELOAD_VALUE_FOR_100MS); // values for 100ms
	timx_config(&timx_handler);
	timx_update_interrupt(&timx_handler, 1);

	// in order to configure pins TX and RX , the selection for the register GPIOx_CRL and GPIOx_CRH have to be:
	// TX -> alternative function push pull
	// RX -> input float or input pull up
	gpio_init(&modem_tx,GPIOA_BASE_ADDRESS,GPIO_pin_2,GPIO_Alternate_function_output_Push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&modem_tx);
	gpio_init(&modem_rx,GPIOA_BASE_ADDRESS,GPIO_pin_3,GPIO_Floating_input,GPIO_Input_mode);
	gpio_config(&modem_rx);

	RCC_USART2_CLOCK_ENABLE();  // enable clock access to UART2
	usart_init(&usart2,USART2_BASE_ADDRESS,BAUDRATE,0,0,0,0);
	usart_config(&usart2);

	gpio_init(&cli_tx,GPIOA_BASE_ADDRESS,GPIO_pin_9,GPIO_Alternate_function_output_Push_pull,GPIO_Output_mode_max_speed_10_MHz);
	gpio_config(&cli_tx);
	gpio_init(&cli_rx,GPIOA_BASE_ADDRESS,GPIO_pin_10,GPIO_Floating_input,GPIO_Input_mode);
	gpio_config(&cli_rx);

	RCC_USART1_CLOCK_ENABLE();  // enable clock access to USART1
	usart_init(&usart1,USART1_BASE_ADDRESS,BAUDRATE,0,0,0,0);
	usart_config(&usart1);
	usart_interrupt_enable(&usart1);

	NVIC_SET_INTERRUPT_PRIORITY_USART1(2);
	NVIC_ENABLE_INTERRUPT_USART1();

	uint8_t input_data_from_user[MAX_CMDLINE_LENGTH];

	uint8_t index = 0;
	input_data_from_user[index] = '\0';
	/////////////////////////////////////////////////////////////////
	RCC_BACKUP_INTERFACE_CLOCK_ENABLE();
	RCC_POWER_INTERFACE_CLOCK_ENABLE();
	PWR_DISABLE_BKD_WR_PROTECTION();
	////////////////////////////////////////////////////////////////
	RCC_RESET_BACKUP_DOMAIN();
	RCC_RESET_BACKUP_DOMAIN_NOT_ACTIVATED();
	/////////////////////////////////////

	RCC_LSE_OSC_ENABLE(); 		 		// external low speed Enable
	while(RCC_LSE_OSC_READY() == 0);	// external Low Speed oscillator Ready?
	RCC_RTC_CLOCK_SOURCE_SEL(1); 		// Select LSE as Source
	RCC_RTC_CLOCK_ENABLE(); 			// RTC Clock Enable

	/////////////////////////////
	rtc_init(&rtc,RTC_BASE_ADDRESS);
	rtc_set_time(&rtc,(uint32_t)200);

	/////////////////////////////
	iwdt_init(&iwdt, IWDT_BASE_ADDRESS);
	iwdt_config(&iwdt, IWDT_PRESCALER_1_6_SEC);
	iwdt_start_counter(&iwdt);

	uint8_t counter = 0;
	printf("Hardware init ok\n");


    while(1)
    {
    	while(usart_available(&usart1)>0){
    		input_data_from_user[index] = usart_read(&usart1);
    		if(input_data_from_user[index] != '\r'){
    			index++;
    		}
    		else
    		{
    			input_data_from_user[index] = '\0';
    			index = 0;
    			if (strlen((const char *)input_data_from_user) > 0) {
    				execute_command((char *)input_data_from_user);
    			}
    		}
    	}
    	if((timx_handler.timx->TIMx_SR) & ((uint16_t)(0x0001))){
    		timx_handler.timx->TIMx_SR &= ~((uint16_t)(0x0001));
    		gpio_toogle_pin(&navigation_led);
    		counter++;
    		if(counter>=10){
    			uint32_t t = rtc_get_time(&rtc);
    			printf("rtc value: %lu\n",t);
    			counter = 0;
    		}
    		iwdt_reset(&iwdt);
    	}
    }
}


void execute_command(char *line)
{
    char *argv[MAX_CMDLINE_ARGS];
    int argc = 0;

    char *token = strtok(line, " ");
    while (token != NULL && argc < MAX_CMDLINE_ARGS) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    for (Command *command = command_table; command->name != NULL; command++) {
        if (strcmp(argv[0], command->name) == 0) {
            command->function(argc, argv);
            return;
        }
    }

    printf("unknown command: %s\n", argv[0]);
}

void cmd_help(int argc, char *argv[]) {
    printf("Available commands:\n");
    for (Command *command = command_table; command->name != NULL; command++) {
        printf("  %s\n", command->name);
    }
}

void cmd_led(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Use: led [on|off]\n");
        return;
    }
    if (strcmp(argv[1], "on") == 0) {
    	gpio_write_pin(&status_led,1);
    } else if (strcmp(argv[1], "off") == 0) {
    	gpio_write_pin(&status_led,0);
    } else {
        printf("Use: led [on|off]\n");
    }
}

void cmd_apn(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Use: apn [value]\n");
		return;
	}
	if(strlen(argv[1])>0){
		printf("apn: %s\n",(char*)argv[1]);
	}else{
		printf("error apn value\n");
	}
}

void cmd_connection(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Use: connection [ip port]\n");
		return;
	}
	if(strlen(argv[1])>0 && strlen(argv[2])>0){
		printf("ip: %s\n",(char*)argv[1]);
		printf("port: %s\n",(char*)argv[2]);
	}else{
		printf("error");
	}
}

void cmd_protocol(int argc, char *argv[])
{

}


void USART1_IRQHandler()
{
	usart_receive(&usart1);
}

