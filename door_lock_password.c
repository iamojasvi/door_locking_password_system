/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t data_pins[8];
char str[4]={'0','0','0','0'}; //defining array for user input
char password[4]={'1','1','1','1'}; //setting our password as '1111'
int i=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void LCD_init(void);
void LCD_command(uint8_t value);
void LCD_data(uint8_t value);
void LCD_string(uint8_t *str);
void Keys_Detect(void);
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

 data_pins[0]=GPIO_PIN_0; //LCD Pins
 data_pins[1]=GPIO_PIN_1;
 data_pins[2]=GPIO_PIN_2;
 data_pins[3]=GPIO_PIN_3;
 data_pins[4]=GPIO_PIN_4;
 data_pins[5]=GPIO_PIN_5;
 data_pins[6]=GPIO_PIN_6;
 data_pins[7]=GPIO_PIN_7;
/* Initialize LCD */
HAL_Delay(1000);
LCD_init();
HAL_Delay(1000);
LCD_string((uint8_t *)"Hello, sir");
HAL_Delay(1000);
 LCD_command(0x38); // Set 8 bit mode
LCD_command(0XC0);
// Moving the cursor to second line
LCD_command(0x01);
LCD_string((uint8_t *)"Enter Password:");
HAL_Delay(1000);
LCD_command(0x01);
Keys_Detect(); //first digit
HAL_Delay(1000);
Keys_Detect(); //second digit
HAL_Delay(1000);
Keys_Detect(); //third digit
HAL_Delay(1000);
Keys_Detect(); //fourth digit
HAL_Delay(1000);
LCD_command(0X01);
if (str[0]=='1' && str[1]=='1' && str[2]=='1' && str[3]=='1'){ //password logic
LCD_string((uint8_t *)"Correct Password!");
HAL_GPIO_WritePin(GPIOB,led_correct_Pin, GPIO_PIN_SET);
HAL_Delay(1000);
LCD_command(0x01);
LCD_string((uint8_t *)"Unlocking door!");
HAL_Delay(2000);
HAL_GPIO_WritePin(GPIOB,led_correct_Pin, GPIO_PIN_RESET);
LCD_command(0x01);
}
else if(str!=password){
LCD_string((uint8_t *)"Wrong Password!");
HAL_GPIO_WritePin(GPIOB,led_wrong_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB,C6_Pin, GPIO_PIN_SET); //buzzer goes off
HAL_Delay(10000);
HAL_GPIO_WritePin(GPIOB, C6_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB,led_wrong_Pin, GPIO_PIN_RESET);
LCD_command(0x01);
}
/* Infinite loop */
/* USER CODE BEGIN WHILE */
while (1)
{
 /* Polling for Switch Changes */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D0_Pin|D1_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|RS_Pin|RW_Pin
                          |EN_Pin|D2_Pin|D3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R1_Pin|R2_Pin|R3_Pin|C6_Pin
                          |led_correct_Pin|led_wrong_Pin|R4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : D0_Pin D1_Pin D4_Pin D5_Pin
                           D6_Pin D7_Pin RS_Pin RW_Pin
                           EN_Pin D2_Pin D3_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin|RS_Pin|RW_Pin
                          |EN_Pin|D2_Pin|D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin C6_Pin
                           led_correct_Pin led_wrong_Pin R4_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|C6_Pin
                          |led_correct_Pin|led_wrong_Pin|R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : C1_Pin C2_Pin C3_Pin C4_Pin */
  GPIO_InitStruct.Pin = C1_Pin|C2_Pin|C3_Pin|C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Keys_Detect(void)
{
/* Setting Row 1 ad High and others LOW */
HAL_GPIO_WritePin(GPIOB, R1_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, R2_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R3_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R4_Pin, GPIO_PIN_RESET);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET){
/* 1 */
LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='1';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C1_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET){
/* 2 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='2';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C2_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET){
/* 3 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='3';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C3_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET){
/* A */
 LCD_command(0x01);
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i] = 'A';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C4_Pin));}
return;
}
/* Setting Row 2 ad High and others LOW */
HAL_GPIO_WritePin(GPIOB, R1_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R2_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, R3_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R4_Pin, GPIO_PIN_RESET);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET){
/* 4 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='4';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C1_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET){
/* 5 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='5';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C2_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET){
/* 6 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='6';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C3_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET){
/* B */
 LCD_command(0x01);
 LCD_command(0x38); // Set 8 bit mode
 LCD_command(0XC0); // Moving the cursor to second line
 LCD_string((uint8_t *)"#");
 str[i] = 'B';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C4_Pin));}
return;
}
/* Setting Row 3 ad High and others LOW */
HAL_GPIO_WritePin(GPIOB, R1_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R2_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R3_Pin, GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB, R4_Pin, GPIO_PIN_RESET);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET){
/* 7 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='7';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C1_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET){
/* 8 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='8';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C2_Pin));}
return;
}
else if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET){
/* 9 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='9';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C3_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET){
/* # */
 LCD_command(0x38); // Set 8 bit mode
 LCD_command(0XC0); // Moving the cursor to second line
 LCD_string((uint8_t *)"#");
 str[i] = 'C';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C4_Pin));}
return;
}
/* Setting Row 4 ad High and others LOW */
HAL_GPIO_WritePin(GPIOB, R1_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R2_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R3_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, R4_Pin, GPIO_PIN_SET);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C1_Pin) == GPIO_PIN_SET){
/* * */
 LCD_command(0x01);
 LCD_command(0x38); // Set 8 bit mode
 LCD_command(0XC0); // Moving the cursor to second line
 LCD_string((uint8_t )"");
 str[i] = '*';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C1_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C2_Pin) == GPIO_PIN_SET){
/* 0 */
 LCD_command(0x38); // Set 8 bit mode
 LCD_string((uint8_t *)"#");
 str[i]='0';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C2_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C3_Pin) == GPIO_PIN_SET){
/* # */
 LCD_command(0x01);
 LCD_command(0x38); // Set 8 bit mode
 LCD_command(0XC0); // Moving the cursor to second line
 LCD_string((uint8_t *)"#");
 str[i] = '#';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C3_Pin));}
return;
}
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET)
{
HAL_Delay(10);
if(HAL_GPIO_ReadPin(GPIOB, C4_Pin) == GPIO_PIN_SET){
/* D */
 LCD_command(0x01);
 LCD_command(0x38); // Set 8 bit mode
 LCD_command(0XC0); // Moving the cursor to second line
 LCD_string((uint8_t *)"#");
 str[i]='D';
 i++;
while (HAL_GPIO_ReadPin (GPIOB, C4_Pin));}
return;
}
}
/***************/
/* Initialization of LCD in 8-bit mode */
void LCD_init(void)
{
HAL_Delay(100); // Give LCD time to intialize
LCD_command(0x38); // Setting 8-bit mode
 LCD_command(0x38);
 LCD_command(0x38);
 LCD_command(0x0C); // Display ON and Cursor OFF
 LCD_command(0x06); // Auto Increment Cursor
LCD_command(0x01); // Clear Display
LCD_command(0x80); // Cursor at home position
}
void LCD_command(uint8_t value)
{
 HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
 HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);
// witing to data to D0, D1, D2, D3, D4, D5, D6, D7
for (int i = 0; i < 8; i++)
{
 HAL_GPIO_WritePin(GPIOA, data_pins[i], ((value >> i) & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
 }
// Pulse the Enable pin
HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
 HAL_Delay(1);
 HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
 HAL_Delay(1); //hi to lo transition to enable r/w operation
 HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
 HAL_Delay(1); // commands need > 37us to settle
}
void LCD_data(uint8_t value)
{
HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
 HAL_GPIO_WritePin(RW_GPIO_Port, RW_Pin, GPIO_PIN_RESET);
 for (int i = 0; i < 8; i++)
{
 HAL_GPIO_WritePin(GPIOA, data_pins[i], ((value >> i) & 0x01)?GPIO_PIN_SET:GPIO_PIN_RESET);
 }
// Pulse the Enable pin
 HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
 HAL_Delay(1);
 HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_SET);
 HAL_Delay(1); // enable pulse must be >450ns
 HAL_GPIO_WritePin(GPIOA, EN_Pin, GPIO_PIN_RESET);
 HAL_Delay(1); // commands need > 37us to settle
}
void LCD_string(uint8_t *str)
{
int i = 0;
while(str[i] != '\0')
{
LCD_data(str[i]);
i++;
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */