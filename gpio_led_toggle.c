/***********************************************************************************************************************
    @file    gpio_led_toggle.c
    @author  FAE Team
    @date    25-May-2023
    @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/

/* Define to prevent recursive inclusion */
#define _GPIO_LED_TOGGLE_C_

/* Files include */
#include <stdio.h>
#include "platform.h"
#include "gpio_led_toggle.h"
#include "adc_internalvoltagesensor.h"

/**
  * @addtogroup MM32F0140_LibSamples
  * @{
  */

/**
  * @addtogroup GPIO
  * @{
  */

/**
  * @addtogroup GPIO_LED_Toggle
  * @{
  */

/* Private typedef ****************************************************************************************************/

/* Private define *****************************************************************************************************/

/* Private macro ******************************************************************************************************/

/* Private variables **************************************************************************************************/

/* Private functions **************************************************************************************************/


/***********************************************************************************************************************
  * @brief
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/


uint8_t LedDigit = 0;
uint16_t Digit1_Value = 0;
uint16_t Digit2_Value = 0;

uint32_t SegTimer = 0;





const uint8_t SegData[] = {
	0xFC, // 0
	0x60, // 1
	0xDA, // 2
	0xF2, // 3
	0x66, // 4
	0xB6, // 5
	0xBE, // 6
	0xE4, // 7
	0xFE, // 8
	0xF6, // 9

	0xFC, // 'O'   : 10
	0x1C, // 'L'   : 11
};




void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOD, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOC, ENABLE);
	
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15| GPIO_Pin_8| GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_0 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_10| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14| GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
	

    //GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
    //GPIO_WriteBit(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7, Bit_SET);
}

/***********************************************************************************************************************
  * @brief
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void GPIO_IO_Toggle(GPIO_TypeDef *GPIOn, uint16_t PINn)
{
    if (Bit_RESET == GPIO_ReadOutputDataBit(GPIOn, PINn))
    {
        GPIO_SetBits(GPIOn, PINn);
    }
    else
    {
        GPIO_ResetBits(GPIOn, PINn);
    }
}

/***********************************************************************************************************************
  * @brief
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void GPIO_LED_Toggle_Sample(void)
{
    printf("\r\nTest1 %s", __FUNCTION__);

    

    while (1)
    {
        //GPIO_IO_Toggle(GPIOA, GPIO_Pin_15);
        //GPIO_IO_Toggle(GPIOB, GPIO_Pin_3);
       // GPIO_IO_Toggle(GPIOB, GPIO_Pin_4);
       // GPIO_IO_Toggle(GPIOB, GPIO_Pin_5);

		//GPIO_IO_Toggle(GPIOB, GPIO_Pin_1);  

        PLATFORM_DelayMS(100);
    }
}


void SegLEDTimer(void)
{
	// enter per 1msec 



	if( SegTimer ){
		SegTimer--;
	}
	else{
		
		SegTimer = 9;


		#if 0
		if( Digit1_Value <999 ){
			Digit1_Value++;
		}
		else{
			Digit1_Value = 0;
		}

		
		if( Digit2_Value <999 ){
			Digit2_Value++;
		}
		else{
			Digit2_Value = 0;
		}

		#endif
	}
}




void Set_SegLED( uint8_t dat, uint8_t digit )
{
	uint8_t Seg = 0;
	
	//Seg = SegData[dat];
	
	

	if( digit == 0 ){

		#if 0
		// Seg LOW is ON, Digit High is ON
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);		// 1G
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);		// 1E   
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);		// 1D
		GPIO_ResetBits(GPIOA, GPIO_Pin_13);		// 1C   
		GPIO_ResetBits(GPIOA, GPIO_Pin_14);		// 1A   
		GPIO_SetBits(GPIOD, GPIO_Pin_3);		// 1B
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);		// 1F
		#endif
		
		if( dat == 0xFF ){
			Seg = 0;
		}
		else		
		if( dat == 0xFE ){
			Seg = 2;
		}
		else		
		if( dat == 'O' ){
			Seg = SegData[10];
		}
		else		
		if( dat == 'L' ){
			Seg = SegData[11];
		}
		else{
			if( dat <= 9 ){
				Seg = SegData[dat];
			}	
		}
		
		if( Seg & 0x80 )  	GPIO_ResetBits(GPIOA, GPIO_Pin_14);		// 1A   
		else				GPIO_SetBits(GPIOA, GPIO_Pin_14);		// 1A   

		if( Seg & 0x40 )  	GPIO_ResetBits(GPIOD, GPIO_Pin_3);		// 1B   
		else				GPIO_SetBits(GPIOD, GPIO_Pin_3);		// 1B

		if( Seg & 0x20 )  	GPIO_ResetBits(GPIOA, GPIO_Pin_13);		// 1C 
		else				GPIO_SetBits(GPIOA, GPIO_Pin_13);		// 1C 
		
		if( Seg & 0x10 )  	GPIO_ResetBits(GPIOA, GPIO_Pin_12);		// 1D
		else				GPIO_SetBits(GPIOA, GPIO_Pin_12);		// 1D 

		if( Seg & 0x08 )  	GPIO_ResetBits(GPIOA, GPIO_Pin_11);		// 1E   
		else				GPIO_SetBits(GPIOA, GPIO_Pin_11);		// 1E   

		if( Seg & 0x04 )  	GPIO_ResetBits(GPIOD, GPIO_Pin_2);		// 1F
		else				GPIO_SetBits(GPIOD, GPIO_Pin_2);		// 1F

		if( Seg & 0x02 )  	GPIO_ResetBits(GPIOA, GPIO_Pin_8);		// 1G
		else				GPIO_SetBits(GPIOA, GPIO_Pin_8);		// 1G
		
		  
	}
	else
	if( digit == 1 ){
		#if 0

		GPIO_SetBits(GPIOB, GPIO_Pin_15);		// 2A
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);		// 2B
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);		// 2F
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);		// 2C
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);		// 2D
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);		// 2E
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);		// 2G

		#endif

		if( dat == 0xFF ){
			Seg = 0;
		}
		else		
		if( dat == 0xFE ){
			Seg = 2;
		}
		else{
			if( dat <= 9 ){
				Seg = SegData[dat];
			}	
		}
		
		if( Seg & 0x80 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_15);		// 2A
		else				GPIO_SetBits(GPIOB, GPIO_Pin_15);		// 2A

		if( Seg & 0x40 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_14);		// 2B
		else				GPIO_SetBits(GPIOB, GPIO_Pin_14);		// 2B

		if( Seg & 0x20 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		// 2C
		else				GPIO_SetBits(GPIOB, GPIO_Pin_12);		// 2C


		if( Seg & 0x10 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_10);		// 2D
		else				GPIO_SetBits(GPIOB, GPIO_Pin_10);		// 2D

		if( Seg & 0x08 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_2);		// 2E
		else				GPIO_SetBits(GPIOB, GPIO_Pin_2);		// 2E

		if( Seg & 0x04 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_13);		// 2F
		else				GPIO_SetBits(GPIOB, GPIO_Pin_13);		// 2F 

		if( Seg & 0x02 )  	GPIO_ResetBits(GPIOB, GPIO_Pin_1);		// 2G
		else				GPIO_SetBits(GPIOB, GPIO_Pin_1);		// 2G
		

		
		
	}
}



#define SEG_FULL_ON   0

void SegLED_Proc(void)
{
	uint8_t buf1 = 0;
	uint8_t buf10 = 0;
	uint8_t buf100 = 0;

	#define DIGIT1		0
	#define DIGIT2		1

	uint16_t buf16_2, buf16_1;
	
	LedDigit++;
	if( LedDigit > 5 ) LedDigit = 0;

	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);	// LED1-100 OFF
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);	// LED1-10 OFF
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);	// LED1-1 OFF
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);	// LED2-100 OFF
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);	// LED2-10 OFF
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);	// LED2-1 OFF

#if 0 
	// Seg LOW is ON, Digit High is ON
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);		// 1G
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);		// 1E   
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);		// 1D
	GPIO_ResetBits(GPIOA, GPIO_Pin_13);		// 1C   
	GPIO_ResetBits(GPIOA, GPIO_Pin_14);		// 1A   
	GPIO_SetBits(GPIOD, GPIO_Pin_3);		// 1B
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);		// 1F


	GPIO_SetBits(GPIOB, GPIO_Pin_15);		// 2A
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);		// 2B
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);		// 2F
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);		// 2C
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);		// 2D
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);		// 2E
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);		// 2G
	
#endif
	buf16_1 = Digit1_Value;
	buf16_2 = Digit2_Value;

	switch( LedDigit ){
		case 0:


			#if SEG_FULL_ON

			Set_SegLED(8, DIGIT1);
			GPIO_SetBits(GPIOA, GPIO_Pin_15);   
			break;
			

			#endif

			if( buf16_1 >= 0xFFFF ){
				Set_SegLED('L', DIGIT1);
				GPIO_SetBits(GPIOA, GPIO_Pin_15);     // LED1 -D1
			}
			else{
				buf1 = (buf16_1 %10);
				Set_SegLED(buf1, DIGIT1);
				GPIO_SetBits(GPIOA, GPIO_Pin_15);     // LED1 -D1
			}
			break;

			
		case 1:	

#if SEG_FULL_ON

			Set_SegLED(8, DIGIT1);
			GPIO_SetBits(GPIOB, GPIO_Pin_3); 
			break;
			
#endif



			
			if( buf16_1 >= 0xFFFF ){
				Set_SegLED('O', DIGIT1);
				GPIO_SetBits(GPIOB, GPIO_Pin_3);
			}
			else{
				if( (buf16_1/10) == 0 ){
					buf10 = 0xFF; // blank
				}
				else{
					buf10 = (buf16_1 %100)/10;
				}
			
				Set_SegLED(buf10, DIGIT1);
				GPIO_SetBits(GPIOB, GPIO_Pin_3);
			}
			
			break;

			
		case 2:				// 100

			
#if SEG_FULL_ON
			
			Set_SegLED(8, DIGIT1);
			GPIO_SetBits(GPIOB, GPIO_Pin_4);	
			break;
						
#endif
			

			if( buf16_1 >= 0xFFFF ){
				Set_SegLED(0xFF, DIGIT1);
				GPIO_SetBits(GPIOB, GPIO_Pin_4);
			}
			else{
			

				buf100 = (buf16_1 /100);

				
				
				if( buf100 == 0 ) buf100 = 0xFF; // Blank
				
				Set_SegLED(buf100, DIGIT1);
				GPIO_SetBits(GPIOB, GPIO_Pin_4);


			}
			break;
			
		case 3:						// 1


#if SEG_FULL_ON
			
			Set_SegLED(8, DIGIT2);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			break;
						
#endif



			(buf16_2 &= ~0x8000);
			
			buf1 = (buf16_2 %10);
			Set_SegLED(buf1, DIGIT2);
			
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			break;
			
		case 4:						// 10


#if SEG_FULL_ON
						
			Set_SegLED(8, DIGIT2);
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
			break;
									
#endif


			(buf16_2 &= ~0x8000);
				
			buf10 = (buf16_2 %100)/10;
			Set_SegLED(buf10, DIGIT2);
			
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
			break;
			
		case 5:						// 100


#if SEG_FULL_ON
						
			Set_SegLED(8, DIGIT2);
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
			break;
									
#endif


		

#if 1
			if( buf16_2 & 0x8000 ){
				buf100 = 0xFE;  // minus
				Set_SegLED(buf100, DIGIT2);
				GPIO_SetBits(GPIOB, GPIO_Pin_7);
			}
			else{

				buf100 = (Digit2_Value /100);
				if( buf100 == 0 ) buf100 = 0xFF; // Blank
				Set_SegLED(buf100, DIGIT2);
				GPIO_SetBits(GPIOB, GPIO_Pin_7);
			}
			

#else		
			buf100 = (Digit2_Value /100);
			Set_SegLED(buf100, DIGIT2);
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
#endif			
			break;
			
	}

	


}



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/********************************************** (C) Copyright MindMotion **********************************************/

