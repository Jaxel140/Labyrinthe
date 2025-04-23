/**
 *******************************************************************************
 * @file 	main.c
 * @author 	jjo
 * @date 	Mar 29, 2024
 * @brief	Fichier principal de votre projet sur carte Nucléo STM32G431KB
 *******************************************************************************
 */

#include "config.h"
#include "stm32g4_sys.h"
#include "stm32g4_adc.h"
#include "stm32g4_systick.h"
#include "stm32g4_gpio.h"
#include "stm32g4_uart.h"
#include "stm32g4_utils.h"
#include "tft_ili9341/stm32g4_ili9341.h"

#include <stdio.h>

#define BLINK_DELAY		100	//ms
uint8_t x = 110 ;
uint8_t y = 11;


void write_LED(bool b)
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_PIN, b);
}

bool char_received(uart_id_t uart_id)
{
	if( BSP_UART_data_ready(uart_id) )	/* Si un caractère est reçu sur l'UART 2*/
	{
		/* On "utilise" le caractère pour vider le buffer de réception */
		BSP_UART_get_next_byte(uart_id);
		return true;
	}
	else
		return false;
}

void heartbeat(void)
{
	while(! char_received(UART2_ID) )
	{
		write_LED(true);
		HAL_Delay(50);
		write_LED(false);
		HAL_Delay(1500);
	}
}

void Button_init(void){
BSP_GPIO_pin_config(GPIOA,GPIO_PIN_7,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
	BSP_GPIO_pin_config(GPIOA,GPIO_PIN_6,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
	BSP_GPIO_pin_config(GPIOA,GPIO_PIN_5,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
	BSP_GPIO_pin_config(GPIOA,GPIO_PIN_4,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
}

void Joystick_init(void){
BSP_ADC_init();
BSP_GPIO_pin_config(GPIOA,GPIO_PIN_0,GPIO_MODE_ANALOG,GPIO_NOPULL,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
BSP_GPIO_pin_config(GPIOA,GPIO_PIN_1,GPIO_MODE_ANALOG,GPIO_NOPULL,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
BSP_GPIO_pin_config(GPIOB,GPIO_PIN_6,GPIO_MODE_INPUT,GPIO_PULLUP,GPIO_SPEED_MEDIUM,GPIO_NO_AF);
}

void Joystick_test(void){
uint8_t x_value = BSP_ADC_getValue(ADC_1);
uint8_t y_value = BSP_ADC_getValue(ADC_2);

uint8_t button_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);
ILI9341_Init();


char buffer[32];


sprintf(buffer, "X: %d", x_value);
ILI9341_Puts(x_value, 20, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

sprintf(buffer, "Y: %d", y_value);
ILI9341_Puts(10, y_value, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);

sprintf(buffer, "Bouton: %s", button_state ? "Relache" : "Presse");
ILI9341_Puts(10, 80, buffer, &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

void Button_test(void){


}



/**
  * @brief  Point d'entrée de votre application
  */
int main(void)
{
	/* Cette ligne doit rester la première de votre main !
	 * Elle permet d'initialiser toutes les couches basses des drivers (Hardware Abstraction Layer),
	 * condition préalable indispensable à l'exécution des lignes suivantes.
	 */
	HAL_Init();
	Button_init();
	ILI9341_Init();
	Joystick_init();


	/* Initialisation des périphériques utilisés dans votre programme */
	BSP_GPIO_enable();
	BSP_UART_init(UART2_ID,115200);

	/* Indique que les printf sont dirigés vers l'UART2 */
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	/* Initialisation du port de la led Verte (carte Nucleo) */
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH,GPIO_NO_AF);

	/* Hello student */
	printf("Hi <Student>, can you read me?\n");


	//heartbeat();

	/* Tâche de fond, boucle infinie, Infinite loop,... quelque soit son nom vous n'en sortirez jamais */
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_DrawCircle(100,100,20,ILI9341_COLOR_BLUE);
	ILI9341_DrawLine(20,20,100,20,ILI9341_COLOR_RED);
	ILI9341_DrawLine(20,20,20,100,ILI9341_COLOR_RED);
	ILI9341_Putc(110,20,'x',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Putc(15,110,'y',&Font_11x18,ILI9341_COLOR_BLUE,ILI9341_COLOR_WHITE);
	ILI9341_Puts(200,200,"chaine", &Font_11x18, ILI9341_COLOR_BROWN, ILI9341_COLOR_WHITE);

	while (1)
	{
		Joystick_test();
		HAL_Delay(4000);
			//write_LED(true);		/* write_LED? Faites un ctrl+clic dessus pour voir... */
			//HAL_Delay(BLINK_DELAY);	/* ... ça fonctionne aussi avec les macros, les variables. C'est votre nouveau meilleur ami */
			//write_LED(false);
		}

	}

