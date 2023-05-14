/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32l476xx.h"
#include "helper_functions.h"
#include "7seg.h"
#include "keypad.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// Define pins for 7seg
#define SEG_gpio GPIOC
#define DIN_pin 3
#define CS_pin 2
#define CLK_pin 1

// Define pins for keypad
#define COL_gpio GPIOA
#define COL_pin 6       // 6 7 8 9
#define ROW_gpio GPIOB
#define ROW_pin 3       // 3 4 5 6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int check_press(int i, int j){
	return check_keypad_input_one(ROW_gpio, COL_gpio, ROW_pin, COL_pin, i, j);
}
void display(int a, int b){
	display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, a, b);
}
int keypad_space[16][2]= { {0,0}, {0,1}, {0,2}, {0,3},
						   {1,0}, {1,1}, {1,2}, {1,3},
						   {2,0}, {2,1}, {2,2}, {2,3},
						   {3,0}, {3,1}, {3,2}, {3,3}};

int get_keyboard_input(){
	for(int i=0;i<16;i++){
		int a = keypad_space[i][0];
		int b = keypad_space[i][1];
		if(check_press(a,b)){
			while(check_press(a,b)!=0) continue;
			return keypad[a][b];
		}
	}
	return -1;
}

void input_num(int *num, int *operator){
	*num = 0;
	int input = 0;
	int times = 0;
	int negative = 0;
	while(1){
		if(negative == 1 && times == 0)
			send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 1,10);
		else
			display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, *num, num_digits(*num));
		input = get_keyboard_input();

		if (input == -1 || (times > 2 && input >= 0 && input < 10 )){
			// no input or number bigger than 999
			continue;
		}else if (times <= 2 && input >= 0 && input < 10 ){
			// acceptable number
			if(negative == 1){
				*num = *num * 10 - input;
			}else {
				*num = *num * 10 + input;
			}
			times++;
		}else if(input == 14){
			// clear
			*num = 0;
			times = 0;
		}else if(input == 11 && times == 0){
			negative = 1;
		}else{
			// operator
			*operator = input;
			return;
		}
	}
}

void calculate(int *num1, int *num2, int *operator){
	int input = 0;
	int result ;
	switch(*operator){
		case 10:
			result = *num1 + *num2; break;
		case 11:
			result = *num1 - *num2; break;
		case 12:
			result = *num1 * *num2; break;
		case 13:
			result = *num1 / *num2; break;
	}
	while(input != 14 && result != 0){
		input = get_keyboard_input();
		display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, result, num_digits(result));
	}
	return;
}

void part1(){
	int num_1 = 0, num_2 = 0;
	int operator_1 = 0, operator_2 = 0;
	int status = 1;
	while(1){
		if (status == 1){
			input_num(&num_1, &operator_1);
			status = 2;
		}else if(status == 2){
			while(operator_2 != 15){
				// operator != '='
				input_num(&num_2, &operator_2);
			}
			status = 3;
		}else if(status == 3){
			calculate(&num_1, &num_2, &operator_1);
			status = 1;
			operator_1 = 0;
			operator_2 = 0;
		}
	}
}

int input_checkError(int *num, int *operator){
	int input = 0;
	int times = 0;
	while(1){
		display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, *num, num_digits(*num));
		input = get_keyboard_input();
		if (input == -1 || (times > 2 && input >= 0 && input < 10 )){
			// no input or number bigger than 999
			continue;
		}else if (input >= 0 && input < 10 ){
			// acceptable number
			*num = *num * 10 + input;
			times++;
		}else if(input == 14){
			// clear
			*num = 0;
			times = 0;
		}else if(times ==0 && (input >= 10 && input <= 13)){
			// if enter a operator which is not neg at first time
			return -2;
		}else if(input == 15){
			*operator = input;
			return -1;
		}
		else{
			// operator
			*operator = input;
			return 0;
		}
	}
}

int BODMAS(int list[]){
	int length = 0;
	while(list[length] !=15)
		length++;
	int tmp[20] = {0};

	int tmp_i = 0;
	for(int i = 0; i<=length; i++){
		tmp[tmp_i] = list[i];
		if(list[i+1] == 12){
			tmp[tmp_i] *= list[i+2];
			i+=2;
		}else if(list[i+1] == 13){
			tmp[tmp_i] /= list[i+2];
			i+=2;
		}
		tmp_i++;
	}

	int num = tmp[0];
	length = 0;
	while(tmp[length] !=15)
		length++;

	for(int i = 2; i<=length; i+=2){
		if(tmp[i-1] == 10)
			num += tmp[i];
		else if (tmp[i-1] == 11)
			num-= tmp[i];
	}
	return num;
}

void part3(){
	while(1){
		int result = 0;
		int state = 0, error = 0;
		// state == -1 : end
		// state == -2 : error

		int list[20] = {0};
		int times = 0;

		// get a num and an operator at a time
		while(state != -1){
			state = input_checkError( &list[times], &list[times+1]);
			times+=2;
			if(state == -2) error = 1;
		}

		if(error == 1)
			result = -1;
		else if(state == -1 && error == 0)
			result = BODMAS(list);

		int input = 0;
		while(input != 14 && result != 0){
			input = get_keyboard_input();
			display_number(SEG_gpio, DIN_pin, CS_pin, CLK_pin, result, num_digits(result));
		}
	}
}

void input_multi(){
	int input = 0, num = 0;
	int first = 1;
	while(1){
		if(first == 1)
			// show nothing if the first time
			display(0, 0);
		else
			display(num, num_digits(num));

		for(int i=0;i<16;i++){
			int a = keypad_space[i][0];
			int b = keypad_space[i][1];
			if(check_press(a,b)){
				// if one been pressed
				input = keypad[a][b];

				if(input == 14)// clear
					return;
				if(num_digits(num)>=8) break;
				if(first == 1) first--;

				int keyboard_state[16] = {0};
				int press = 1;
				while(press == 1){
					press = 0;
					for(int j=0;j<16;j++){
						int c = keypad_space[j][0];
						int d = keypad_space[j][1];
						if(check_press(c,d)){
							if(keypad[c][d]==14) return;
							keyboard_state[keypad[c][d]] = 1;
							press = 1;
						}
					}
				}
				int tmp = 0;
				for(int j = 0; j<16; j++){
					if(keyboard_state[j] == 1)
						tmp+=j;
				}

				for(int j = 0; j<num_digits(tmp); j++){
					num *=10;
				}
				num+=tmp;
				break;
			}
		}
	}
}
void part4(){
	while(1)
		input_multi();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	if(init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin) != 0){
		// Fail to init 7seg
		return -1;
	}
	if(init_keypad(ROW_gpio, COL_gpio, ROW_pin, COL_pin) != 0){
		// Fail to init keypad
		return -1;
	}

	// Set Decode Mode to non-decode mode
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE,0xFF);
 	 // Set Scan Limit to digit 0 only
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT,0xFF);
	 // Wakeup 7seg
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN,0x01);

	part1();
//	part3();
//	part4();

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
