/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t move_mode = 0 ;
uint8_t flag = 0;
#define move_delay 400
#define move_speed 4
#define MyMap(x,in_min,in_max,out_min,out_max) (x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min
void Rbt_Init(void);
uint16_t angle(uint8_t angle);
void Rbt_attention(void);
void move_forward(void);
void move_behind(void);
void move_right(void);
void move_left(void);
void move_swing(void);
void move_stretch(void);
void move_sleep(void);
void move_dance(void);
void OLED_Init(void);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint16_t angle(uint8_t angle)
{
  return (MyMap(angle,0,180.f,0.025f,0.125f))*TIM2->ARR;
}

void Change_Mode(uint8_t mode)
	
{  
	move_mode = mode;
	if(move_mode == 3 || move_mode == '3')
	 {//前进
		//OLED_DrawBMP(0,0,128,8,BMP2);
		  move_forward();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
		OLED_NewFrame();
    	OLED_DrawImage(0, 0, &normalImg, OLED_COLOR_NORMAL);
    	OLED_ShowFrame();
	  }
	  else if(move_mode == 4 || move_mode == '4')
	  {//后�??
		//   OLED_DrawBMP(0,0,128,8,BMP2);
		  move_behind();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
		  OLED_NewFrame();
    	  OLED_DrawImage(0, 0, &normalImg, OLED_COLOR_NORMAL);
    	  OLED_ShowFrame();
	  }
	  else if(move_mode == 6 || move_mode == '6')
	  {//左转
		  move_left();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
		  OLED_NewFrame();
    	  OLED_DrawImage(0, 0, &leftlookImg, OLED_COLOR_NORMAL);
    	  OLED_ShowFrame();
	  }
	  else if(move_mode == 5 || move_mode == '5')
	  {//右转
		//OLED_DrawBMP(0,0,128,8,BMP3);
		  move_right();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
		  OLED_NewFrame();
    	  OLED_DrawImage(0, 0, &rightlookImg, OLED_COLOR_NORMAL);
    	  OLED_ShowFrame();
	  }
	  else if(move_mode == 2 || move_mode == '2')
	  {//立正
		//OLED_DrawBMP(0,0,128,8,BMP1);
          Rbt_attention();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
	  }
	  else if(move_mode == 7 || move_mode == '7')
	  {//摇摆
		//   OLED_DrawBMP(0,0,128,8,BMP2);
		  move_swing();
		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		  flag=0;
	  }
	  else if(move_mode == '8' || move_mode == 8)
	  {//坐下招手
		  if(flag==0)
		  {
			//OLED_DrawBMP(0,0,128,8,BMP2);
			  move_stretch();
			//OLED_DrawBMP(0,0,128,8,BMP5);
			  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			  flag=1;
		  }
	  }
	  else if(move_mode == 9 || move_mode == '9')
	      {//休眠
              move_sleep();
			//   OLED_DrawBMP(0,0,128,8,BMP6);
			  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			  flag=0;
		  }
      else if(move_mode==0x0A)
	  {//唱歌跳舞//语音模块特有指令
              move_dance();
              HAL_Delay(200);
            //OLED_DrawBMP(0,0,128,8,BMP2);
              HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    /* USER CODE END WHILE */
	  }

}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
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
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
  Rbt_Init();
  Rbt_attention();
  HAL_Delay(20); 
  OLED_Init();
  OLED_NewFrame();
  OLED_DrawImage(0, 0, &normalImg, OLED_COLOR_NORMAL);
  OLED_ShowFrame();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  { 
	HAL_UART_Receive(&huart1, &move_mode, 1, HAL_MAX_DELAY);
	Change_Mode(move_mode);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Rbt_Init(void)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(15));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(160));
    HAL_Delay(100);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(167));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(20));
	HAL_Delay(100);
}
void Rbt_attention(void)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
    HAL_Delay(100);
}
void move_forward(void)
{   
	int i=0;
	for(i=0;i<5;i++)
	{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
 
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	HAL_Delay(move_delay);
	}
}
 
void move_behind(void)
{
	int i=0;
	for(i=0;i<5;i++)
	{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
 
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(45));
	HAL_Delay(move_delay);
	}
}
 
void move_right(void)
{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	HAL_Delay(move_delay);
}
 
void move_left(void)
{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(135));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(135));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(45));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(45));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	HAL_Delay(move_delay);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	HAL_Delay(move_delay);
}
 
void move_swing(void)
{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(50));
	HAL_Delay(200);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(130));
	HAL_Delay(200);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(50));
	HAL_Delay(200);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(50));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(130));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(130));
	HAL_Delay(200);

}
 
void move_stretch(void){
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
	if(TIM2->CCR1<angle(155) && TIM2->CCR3>angle(25)){
		for(uint8_t i=0;i<70;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90+i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90-i));
			HAL_Delay(move_speed);
		}
		HAL_Delay(1000);
		for(uint8_t i=0;i<70;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(160-i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(20+i));
			HAL_Delay(move_speed);
		}
		for(uint8_t i=0;i<65;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90+i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90-i));
			HAL_Delay(move_speed);
		}
		for(uint8_t i=0;i<20;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90-i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90+i));
			HAL_Delay(move_speed);
		}
		HAL_Delay(1000);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(140));
		HAL_Delay(1000);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(178));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(140));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(178));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(140));
		HAL_Delay(500);
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(70));
	}
}
 
void move_sleep(void){
	if(TIM2->CCR3<angle(160) && TIM2->CCR1>angle(20)){
		for(uint8_t i=0;i<75;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90-i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90+i));
			HAL_Delay(move_speed);
		}
	}
	if(TIM2->CCR2<angle(160) && TIM2->CCR4>angle(20)){
		for(uint8_t i=0;i<75;i++){
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90+i));
			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90-i));
			HAL_Delay(move_speed);
		}
	}
 
}
void move_dance(void)
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90));
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90));
    if(TIM2->CCR1<angle(160) && TIM2->CCR2<angle(160)){//当在目标位置之外，则执行
        for(uint8_t i=0;i<75;i++){
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(90+i));
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(90+i));
            HAL_Delay(move_speed);
        }
        for(uint8_t i=0;i<75;i++){
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1,angle(165-i));
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2,angle(165-i));
            HAL_Delay(move_speed);
        }
    }
    if(TIM2->CCR3>angle(20) && TIM2->CCR4>angle(20)){
        for(uint8_t i=0;i<75;i++){
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(90-i));
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(90-i));
            HAL_Delay(move_speed);
        }
        for(uint8_t i=0;i<75;i++){
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3,angle(15+i));
            __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4,angle(15+i));
            HAL_Delay(move_speed);
        }
    }
}


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
