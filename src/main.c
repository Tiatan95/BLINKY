/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
int main(void)
{
  HAL_Init();
  /* Configure the System clock to have a frequency of 80 MHz */
  SystemClock_Config();
  /* Configure the User LED */
  BSP_LED_Init(LED2);
  BSP_LED_Off(LED2);
  /* Infinite loop */
  uint32_t PrevTicks = HAL_GetTick();

  enum {S,O,R}state;
  enum {Space,Dash,Dot} mstate;
  state = S;
  mstate = Space;
  int count = 0;
  int scount = 0;
  int tick_comparison;
  while (1)
  {
	uint32_t CurrentTicks = HAL_GetTick();
	switch(state)
	{
		case S:
			if((CurrentTicks-PrevTicks) >= 1000 && count <= 5)
			{
				PrevTicks = CurrentTicks;
				BSP_LED_Toggle(LED2);
				count++;
				if(count > 5)
				{
					state = O;
					count = 0;
					scount++;
					if(scount >= 2)
					{
						state = R;
					}
				}
			}
			break;
		case O:
			if(mstate == Dash)
			{
				tick_comparison = 3000;
			}
			if(mstate == Space)
			{
				tick_comparison = 1000;
			}
			if((CurrentTicks-PrevTicks) >= tick_comparison && count <=5)
			{
				PrevTicks = CurrentTicks;
				BSP_LED_Toggle(LED2);
				count++;
				if(count > 5)
				{
					state = S;
					count = 0;
				}
				if(mstate == Dash)
				{
					mstate = Space;
				}
				else if(mstate == Space)
				{
					mstate = Dash;
				}
			}
			break;
		case R:
			if((CurrentTicks-PrevTicks) >= 4000)
			{
				state = S;
				scount = 0;
				mstate = Space;
				count = 0;
			}
	}
	/*
	if ((CurrentTicks-PrevTicks)>=1000)
	{
		PrevTicks = CurrentTicks;
		BSP_LED_Toggle(LED2);
	}
	*/
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	/* MSI is enabled after System reset, activate PLL with MSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLP = 7;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		while(1);
	}
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
	/* Initialization Error */
	while(1);
	}
}
