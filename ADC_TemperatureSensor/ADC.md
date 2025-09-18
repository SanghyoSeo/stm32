# ADC_TemperatureSensor

<img width="456" height="215" alt="image" src="https://github.com/user-attachments/assets/bdf43d46-f0e0-499c-aa1d-556351a37c74" />
<br><br><br>


<img width="1586" height="1125" alt="image" src="https://github.com/user-attachments/assets/229b2c75-e75b-407f-bfc4-82a4533b4f13" />
<br>
<img width="1571" height="1125" alt="image" src="https://github.com/user-attachments/assets/11dca187-8508-4ad0-bebf-a0ce77c49322" />

<br>

<br>
<img width="1225" height="274" alt="temp_010" src="https://github.com/user-attachments/assets/78be6ebb-bf14-4f3d-aee2-b632df80b4ab" />
<br>
<img width="853" height="832" alt="temp_011" src="https://github.com/user-attachments/assets/8d605061-129a-41e3-8a16-14c6708e42e5" />
<br>
<img width="853" height="832" alt="temp_012" src="https://github.com/user-attachments/assets/e6accd8e-6208-4d61-9460-ccd2de35d717" />
<br>



```c
/* USER CODE BEGIN PV */
const float AVG_SLOPE = 4.3E-03;
const float V25 = 1.43;
const float ADC_TO_VOLT = 3.3 / 4096;
/* USER CODE END PV */
```


```c
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
```


```c
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  if (ch == '\n')
    HAL_UART_Transmit (&huart2, (uint8_t*) "\r", 1, 0xFFFF);
  HAL_UART_Transmit (&huart2, (uint8_t*) &ch, 1, 0xFFFF);

  return ch;
}
```

```c
  /* USER CODE BEGIN 2 */
  uint16_t adc1;

  float vSense;
  float temp;

  if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
  {
	  Error_Handler();
  }

  if(HAL_ADC_Start(&hadc1) != HAL_OK)
  {
	  Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_ADC_PollForConversion(&hadc1, 100);
	  adc1 = HAL_ADC_GetValue(&hadc1);
	  //printf("ADC1_temperature : %d \n",adc1);

	  vSense = adc1 * ADC_TO_VOLT;
	  temp = (V25 - vSense) / AVG_SLOPE + 25.0;
	  printf("temperature : %d, %f \n",adc1, temp);

	  HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
```
