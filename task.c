/* task.c */

#include <stdio.h>
#include "platform.h"
#include "Gpio_led_toggle.h"
#include "Adc_internalvoltagesensor.h"

#include <stdlib.h>


#define		TEST_FW_1	1
#define		TEST_FW_2	0	



uint32_t TaskTimer10msec = 0;
uint32_t TaskTimer100msec = 0;
uint32_t TaskTimer1sec = 0;


uint16_t CurrentADC = 0;
uint16_t TempADC = 0;
uint16_t PA6_ADC = 0;
uint8_t task1secflag = 0;


uint8_t CurrentUpdateConter = 0;
uint16_t CurrentBuf[32];

uint32_t TaskTimer_PeakCap = 0;

typedef struct {
	int temp;
	uint16_t v_value;
}temp_t;

const temp_t temp_tbl[] = {

	 {-40,			    320 } , 
	 {-39,              319 } ,
	 {-38,              319 } ,
	 {-37,              318 } ,
	 {-36,              317 } ,
	 {-35,              317 } ,
	 {-34,              316 } ,
	 {-33,              315 } ,
	 {-32,              314 } ,
	 {-31,              313 } ,
	 {-30,              312 } ,
	 {-29,              311 } ,
	 {-28,              310 } ,
	 {-27,              309 } ,
	 {-26,              307 } ,
	 {-25,              306 } ,
	 {-24,              305 } ,
	 {-23,              303 } ,
	 {-22,              302 } ,
	 {-21,              300 } ,
	 {-20,              299 } ,
	 {-19,              297 } ,
	 {-18,              296 } ,
	 {-17,              294 } ,
	 {-16,              292 } ,
	 {-15,              290 } ,
	 {-14,              288 } ,
	 {-13,              286 } ,
	 {-12,              284 } ,
	 {-11,              282 } ,
	 {-10,              279 } ,
	 { -9,              277 } ,
	 { -8,              274 } ,
	 { -7,              272 } ,
	 { -6,              269 } ,
	 { -5,              267 } ,
	 { -4,              264 } ,
	 { -3,              261 } ,
	 { -2,              258 } ,
	 { -1,              255 } ,
 	 {  0,              252 } ,
 	 {  1,              249 } ,
 	 {  2,              246 } ,
 	 {  3,              243 } ,
 	 {  4,              240 } ,
 	 {  5,              236 } ,
 	 {  6,              233 } ,
 	 {  7,              230 } ,
 	 {  8,              226 } ,
 	 {  9,              223 } ,
	 { 10,              219 } ,
	 { 11,              216 } ,
	 { 12,              212 } ,
	 { 13,              208 } ,
	 { 14,              205 } ,
	 { 15,              201 } ,
	 { 16,              197 } ,
	 { 17,              194 } ,
	 { 18,              190 } ,
	 { 19,              186 } ,
	 { 20,              183 } ,
	 { 21,              179 } ,
	 { 22,              175 } ,
	 { 23,              172 } ,
	 { 24,              168 } ,
	 { 25,              165 } ,
	 { 26,              161 } ,
	 { 27,              157 } ,
	 { 28,              154 } ,
	 { 29,              150 } ,
	 { 30,              147 } ,
	 { 31,              143 } ,
	 { 32,              140 } ,
	 { 33,              136 } ,
	 { 34,              133 } ,
	 { 35,              130 } ,
	 { 36,              127 } ,
	 { 37,              123 } ,
	 { 38,              120 } ,
	 { 39,              117 } ,
	 { 40,              114 } ,
	 { 41,              111 } ,
	 { 42,              108 } ,
	 { 43,              105 } ,
	 { 44,              102 } ,
	 { 45,              100 } ,
	 { 46,               97 } ,
	 { 47,               94 } ,
	 { 48,               92 } ,
	 { 49,               89 } ,
	 { 50,               87 } ,
	 { 51,               84 } ,
	 { 52,               82 } ,
	 { 53,               79 } ,
	 { 54,               77 } ,
	 { 55,               75 } ,
	 { 56,               73 } ,
	 { 57,               71 } ,
	 { 58,               69 } ,
	 { 59,               67 } ,
	 { 60,               65 } ,
	 { 61,               63 } ,
	 { 62,               61 } ,
	 { 63,               59 } ,
	 { 64,               58 } ,
	 { 65,               56 } ,
	 { 66,               54 } ,
	 { 67,               53 } ,
	 { 68,               51 } ,
	 { 69,               50 } ,
	 { 70,               48 } ,
	 { 71,               47 } ,
	 { 72,               45 } ,
	 { 73,               44 } ,
	 { 74,               43 } ,
	 { 75,               41 } ,
	 { 76,               40 } ,
	 { 77,               39 } ,
	 { 78,               38 } ,
	 { 79,               37 } ,
	 { 80,               36 } ,
	 { 81,               35 } ,
	 { 82,               34 } ,
	 { 83,               33 } ,
	 { 84,               32 } ,
	 { 85,               31 } ,
	 { 86,               30 } ,
	 { 87,               29 } ,
	 { 88,               28 } ,
	 { 89,               27 } ,
	 { 90,               27 } ,
	 { 91,               26 } ,
	 { 92,               25 } ,
	 { 93,               24 } ,
	 { 94,               24 } ,
	 { 95,               23 } ,
	 { 96,               22 } ,
	 { 97,               22 } ,
	 { 98,               21 } ,
	 { 99,               21 } ,
	 {100,               20 } ,
	 {101,               19 } ,
	 {102,               19 } ,
	 {103,               18 } ,
	 {104,               18 } ,
	 {105,               17 } ,
	 {106,               17 } ,
	 {107,               16 } ,
	 {108,               16 } ,
	 {109,               15 } ,
	 {110,               15 } ,
	 {111,               15 } ,
	 {112,               14 } ,
	 {113,               14 } ,
	 {114,               14 } ,
	 {115,               13 } ,
	 {116,               13 } ,
	 {117,               12 } ,
	 {118,               12 } ,
	 {119,               12 } ,
	 {120,               11 } ,
	 {121,               11 } ,
	 {122,               11 } ,
	 {123,               11 } ,
	 {124,               10 } ,
	 {125,               10 } ,



};



typedef struct{
	uint32_t minVol;
	uint32_t maxVol;
	uint32_t Current;
}TableCurrent_t, *lpTableCurrent_t;

#if 0
const TableCurrent_t CurrentTbl[] = {
	{.minVol =   0, .maxVol =   6, .Current =  10 },
	{.minVol =   6, .maxVol =   7, .Current =  12 },
	{.minVol =   7, .maxVol =   8, .Current =  15 },
	{.minVol =   8, .maxVol =   9, .Current =  20 },
	{.minVol =   9, .maxVol =  11, .Current =  25 },
	{.minVol =  11, .maxVol =  13, .Current =  30 },
	{.minVol =  13, .maxVol =  15, .Current =  35 },
	{.minVol =  15, .maxVol =  17, .Current =  40 },
	{.minVol =  17, .maxVol =  20, .Current =  42 },
	{.minVol =  20, .maxVol =  23, .Current =  45 },
	{.minVol =  23, .maxVol =  26, .Current =  50 },
	{.minVol =  26, .maxVol =  30, .Current =  60 },
	{.minVol =  30, .maxVol =  34, .Current =  70 },
	{.minVol =  34, .maxVol =  39, .Current =  80 },
	{.minVol =  39, .maxVol =  45, .Current =  90 },
	{.minVol =  45, .maxVol =  51, .Current = 100 },
	{.minVol =  51, .maxVol =  58, .Current = 110 },
	{.minVol =  58, .maxVol =  66, .Current = 130 },
	{.minVol =  66, .maxVol =  75, .Current = 150 },
	{.minVol =  75, .maxVol =  85, .Current = 170 },
	{.minVol =  85, .maxVol =  97, .Current = 180 },
	{.minVol =  97, .maxVol = 109, .Current = 210 },
	{.minVol = 109, .maxVol = 123, .Current = 230 },
	{.minVol = 123, .maxVol = 139, .Current = 260 },
	{.minVol = 139, .maxVol = 158, .Current = 290 },
	{.minVol = 158, .maxVol = 178, .Current = 330 },
	{.minVol = 178, .maxVol = 201, .Current = 370 },
	{.minVol = 201, .maxVol = 225, .Current = 410 },
	{.minVol = 225, .maxVol = 254, .Current = 470 },
	{.minVol = 254, .maxVol = 286, .Current = 530 },
	{.minVol = 286, .maxVol = 321, .Current = 590 },
	{.minVol = 321, .maxVol = 330, .Current = 700 },
};

#else

const TableCurrent_t CurrentTbl[] = {
	{.minVol =   0, .maxVol =   7, .Current =  0 },
	{.minVol =   7, .maxVol =  27, .Current =  10 },
	{.minVol =  27, .maxVol =  53, .Current =  20 },
	{.minVol =  53, .maxVol =  72, .Current =  30 },
	{.minVol =  72, .maxVol =  95, .Current =  40 },
	{.minVol =  95, .maxVol = 123, .Current =  60 },
	{.minVol = 123, .maxVol = 152, .Current =  70 },
	{.minVol = 125, .maxVol = 182, .Current =  80 },
	{.minVol = 182, .maxVol = 189, .Current =  90 },
	{.minVol = 189, .maxVol = 235, .Current =  110 },
	{.minVol = 235, .maxVol = 330, .Current =  150 },

};



#endif



uint16_t CurrentADC_MAX = 0;
uint16_t CurrentADC_MIN = 0;


uint16_t TempADC_MAX = 0;
uint16_t TempADC_MIN = 0;

uint32_t Warning_Timer = 3000;
uint32_t Warning_BlinkTimer = 500;

uint8_t Warning_flag = 0;


uint32_t PON_timer = 3000;
uint8_t  PON_flag = 0;

uint32_t SOUT_timer = 0;
uint32_t SIN2_Changetimer = 0;


uint32_t CurrentDisplayOnTimer = 3000;

void tasktimer1msec(void)
{

	
	extern void SegLEDTimer(void);
	extern void ADC_Timer(void);
	if( TaskTimer100msec  ){
		TaskTimer100msec--;

	}
	else{

		
		TaskTimer100msec = 99;
		
	}
	


	if( TaskTimer10msec ){
		TaskTimer10msec--;	
	}
	else{
		TaskTimer10msec = 9;		
	}


	if( TaskTimer1sec ){
		TaskTimer1sec--;
	}
	else{
		TaskTimer1sec = 999;
		task1secflag = 1;
	}

	if( TaskTimer_PeakCap ) TaskTimer_PeakCap--;

	if( Warning_Timer ) Warning_Timer--;
	if( Warning_BlinkTimer ) Warning_BlinkTimer--;
	if( PON_timer ) PON_timer--;
	
	SegLEDTimer();
	SegLED_Proc();
	ADC_Timer();

	if( SOUT_timer ) SOUT_timer--;
	if( SIN2_Changetimer ) SIN2_Changetimer--;
	if( CurrentDisplayOnTimer ) CurrentDisplayOnTimer--;
}


void Set_PON( uint8_t mode ){
	
	if( mode ) {
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}



void Set_SOUT( uint8_t mode ){
	
	if( mode ) {
		GPIO_SetBits(GPIOC, GPIO_Pin_15);
	}
	else{
		GPIO_ResetBits(GPIOC, GPIO_Pin_15);
	}
}

uint8_t SOUT_ChangeStep = 0;

void task_SINOUT(void )
{

	uint8_t S_IN2=0; 
	uint8_t S_IN1=0; 
	static uint8_t last_S_IN2 = 0;

	
	if( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_14 ) == Bit_SET ){
		S_IN2 = 1;
	}

	
	if( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_13 ) == Bit_SET  ){
		S_IN1 = 1;
	}


	if( last_S_IN2 != S_IN2 ){
		last_S_IN2 = S_IN2;
		
		SOUT_timer 			= 300;

		if( SOUT_ChangeStep == 0 ){
			SOUT_ChangeStep = 1;
			SIN2_Changetimer = 300;
		}
		else
		if( SOUT_ChangeStep == 1 ){
			if( SIN2_Changetimer == 0 ){
				Set_SOUT(1);
			}
		}
		
		
	}
	else{

		if( SOUT_timer == 0 ){
			Set_SOUT(0);
			SOUT_ChangeStep = 0;
		}
	}
	

	

}


void task_PON( void )
{
	uint32_t PA6Vol=0;
	
	if( PON_flag == 0 ){
		ADC_InternalVoltageSensor_Sample();

		PA6Vol = (PA6_ADC*500)/(4096-1);
		if( PA6Vol <= 45 ){      // 0.45
			PON_timer = 3000;
		}
		else
		if( PA6Vol <= 160){     // 1.60
			PON_timer = 2500;
		}
		else
		if( PA6Vol <= 250 ){	// 2.50
			PON_timer = 2000;
		}
		else
		if( PA6Vol <= 300 ){	// 3.00	
			PON_timer = 1500;
		}
		else
		if( PA6Vol <= 340 ){	// 3.40
			PON_timer = 1000;
		}
		else{
			PON_timer = 500;
		}
		
		PON_flag = 1;

		PON_timer -= 350;
		printf("\r\n PA6_ADC(%d)  PA6Vol(%d) PON_timer(%d)",PA6_ADC, PA6Vol, PON_timer );
	}
	else{
		
		if( PON_timer == 0 ){
			Set_PON(1);
		}
	}
	
}

void task(void)
{
	extern uint16_t Digit1_Value;
	extern uint16_t Digit2_Value;

	uint16_t TempVol = 0;
	uint32_t CurrVol = 0;
	uint32_t RMSCurrVol = 0;
	uint32_t CalVol = 0;
	uint32_t CurrentmA = 0;
	uint16_t i = 0;
	uint16_t tbl_size = 0;
	int temp_buf=0;

	//int int_buf1,int_buf2;
     //double f_buf1,f_buf2,f_buf3;

	uint32_t MaxVol = 0;
	uint32_t MinVol = 0;
	uint16_t v_value = 0;
	uint32_t VolBuf = 0;
	CurrentADC_MAX = 0;
	CurrentADC_MIN = 4096;

	#define WOUT_LOW 0

	
	do{
		ADC_InternalVoltageSensor_Sample();

		if( CurrentADC_MAX < CurrentADC ){
			CurrentADC_MAX  = CurrentADC;
		}

		if( CurrentADC_MIN > CurrentADC ){
			CurrentADC_MIN  = CurrentADC;
		}

		// Blink Check start =========================================
		if( Warning_Timer == 0 ){
		
			if( Warning_BlinkTimer == 0 ){
				Warning_BlinkTimer = 500;
				if( Warning_flag ){	
					Warning_flag = 0;
					GPIO_ResetBits(GPIOA, GPIO_Pin_0);
					
				}
				else{
					Warning_flag = 1;

					#if WOUT_LOW
					GPIO_ResetBits(GPIOA, GPIO_Pin_0);
					
					#else
					GPIO_SetBits(GPIOA, GPIO_Pin_0);
					#endif
				}	

				printf("\r\nWarning_Blink(%d)  ",Warning_flag );
			}	
			
		}
		else{
			// 경고해제 상태 
			Warning_flag = 0;
			Warning_BlinkTimer = 0;
			#if WOUT_LOW
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			#else
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			#endif
		}

		// Blink Check End =========================================


		task_SINOUT();
		
		
		if( TaskTimer_PeakCap == 0 ) break;
	}while(1);



	//TaskTimer_PeakCap = 1000;
	TaskTimer_PeakCap = 200;

	////////////////////////////// Current Mesument
	CurrVol = (CurrentADC_MAX*500)/(4096-1);
	RMSCurrVol = ((CurrVol/2)*707)/1000;
	MaxVol  = (CurrentADC_MAX*500)/(4096-1);
	MinVol  = (CurrentADC_MIN*500)/(4096-1);

	//CalVol = CurrVol;
	CalVol = MaxVol -MinVol ;
	if( CalVol > 500 ) CalVol = 500;
	
	printf("\r\n CurrentADC_MAX(%d), CurrentADC_MIN(%d) CurrentADC(%d)", CurrentADC_MAX, CurrentADC_MIN, CurrentADC );
	printf("\r\n MaxVol(%d), MinVol(%d) CalVol(%d)", MaxVol, MinVol, CalVol );
	//printf("\r\n CurrVol(%d), RMSCurrVol(%d) CalVol(%d)", CurrVol, RMSCurrVol, CalVol );

#if 0

	if( CalVol <= 25 ) {
		CurrentmA = 0;
	}
	else
	if( CalVol <= 49 ) {
		CurrentmA = 1;
	}	
	else
	if( CalVol <= 74 ) {
		CurrentmA = 2;
	}
	else
	if( CalVol <= 99 ) {
		CurrentmA = 3;
	}
	else
	if( CalVol <= 124 ) {
		CurrentmA = 4;
	}
	else
	if( CalVol <= 149 ) {
		CurrentmA = 5;
	}
	else
	if( CalVol <= 174 ) {
		CurrentmA = 6;
	}
	else
	if( CalVol <= 199 ) {
		CurrentmA = 7;
	}
	else
	if( CalVol <= 224 ) {
		CurrentmA = 8;
	}
	else
	if( CalVol <= 249 ) {
		CurrentmA = 9;
	}
	else{
		CurrentmA = 10;
	}



	printf("\r\n CurrentmA(%d), Current_CalVol(%d)", CurrentmA, CalVol );
	Digit1_Value = CurrentmA;
#else
	#define AVE_COUNTER	10
	if( CurrentUpdateConter < AVE_COUNTER ){
		CurrentBuf[CurrentUpdateConter] = CalVol;
		CurrentUpdateConter++;
	}
	else{
		for( i=0; i<AVE_COUNTER; i++ ){
			VolBuf += CurrentBuf[i];

			printf("\r\n i(%d) VolBuf(%d)   buf(%d)", i, VolBuf, CurrentBuf[i] );
			
		}
		VolBuf /= AVE_COUNTER;
		printf("\r\n AVE VolBuf(%d)",VolBuf );


#if 0
		if( VolBuf <= 50 ) {
			CurrentmA = 0;
		}
		else
		if( VolBuf <= 74 ) {
			CurrentmA = 1;
		}	
		else
		if( VolBuf <= 99 ) {
			CurrentmA = 2;
		}
		else
		if( VolBuf <= 124 ) {
			CurrentmA = 3;
		}
		else
		if( VolBuf <= 149 ) {
			CurrentmA = 4;
		}
		else
		if( VolBuf <= 174 ) {
			CurrentmA = 5;
		}
		else
		if( VolBuf <= 199 ) {
			CurrentmA = 6;
		}
		else
		if( VolBuf <= 224 ) {
			CurrentmA = 7;
		}
		else
		if( VolBuf <= 249 ) {
			CurrentmA = 8;
		}
		else
		if( VolBuf <= 275 ) {
			CurrentmA = 9;
		}
		else
		if( VolBuf <= 299 ) {
			CurrentmA = 10;
		}
		else
		if( VolBuf <= 324 ) {
			CurrentmA = 11;
		}
		else
		if( VolBuf <= 349 ) {
			CurrentmA = 12;
		}
		else
		if( VolBuf <= 374 ) {
			CurrentmA = 13;
		}
		else
		if( VolBuf <= 399 ) {
			CurrentmA = 14;
		}
		else
		if( VolBuf <= 424 ) {
			CurrentmA = 15;
		}
		else
		if( VolBuf <= 449 ) {
			CurrentmA = 16;
		}
		else
		if( VolBuf <= 474 ) {
			CurrentmA = 17;
		}
		else{
			CurrentmA = 18;
		}

#else
		// 2024.03.01
		if( VolBuf <= 142 ){
			CurrentmA = (VolBuf-2)/2;
		}
		else
		if( VolBuf <= 342 ){
		
			VolBuf *= 10;
			CurrentmA = (VolBuf)/19;
			
		}
		else
		if( VolBuf <= 360 ){
		
			VolBuf *= 10;
			CurrentmA = (VolBuf)/18;
			
		}
		else{
			VolBuf *= 10;
			CurrentmA = (VolBuf)/18;
		}
			
#endif
		

		#if 1			// 25.04.15 rollback

		#if 1	// 

		if( CurrentDisplayOnTimer == 0 ){		
			Digit1_Value = CurrentmA;
		}
		else{
			Digit1_Value = 0;
		}
		
		
		#else
		
		if(CurrentmA < 14 ){
			Digit1_Value = 0;
		}
		else{
			Digit1_Value = CurrentmA - 14;
		}
		#endif
		
		#else
		
		// 240313 Test
		if( CurrentmA <= 40 ){
			Digit1_Value = 0;
		}
		else
		if( CurrentmA <= 140 ){
			Digit1_Value = 1;
		}
		else{
			Digit1_Value = 2;
		}
		#endif

		printf("\r\n CurrentmA(%d), VolBuf(%d) FND(%d)", CurrentmA, VolBuf, Digit1_Value );
		
		CurrentUpdateConter = 0;
	}

#endif




	////////////////////////////// Temp Mesument
	temp_buf = 126;
	TempVol  = (TempADC*500)/(4096-1);
	
	tbl_size = sizeof( temp_tbl	)/ sizeof( temp_t	);
	for( i=0; i<tbl_size; i++ ){

		v_value = ((temp_tbl[i].v_value)*500)/330;	
		
		if( v_value < TempVol ){
			temp_buf = temp_tbl[i].temp;
			break;
		}	
	}

	printf("\r\n ");
	printf("\r\n TempVol(%d) temp_buf(%d) (%d)", TempVol, temp_buf, i );

	if( temp_buf < 0 ){
		Digit2_Value = abs(temp_buf)%1000;
		Digit2_Value |= 0x8000;
	}
	else{
		Digit2_Value = temp_buf%1000;
	}


	
	printf("\r\n temp_buf(%d) CurrentmA(%d) (%d)", temp_buf , CurrentmA);

	
	//if( (temp_buf >= 125) || ( CurrentmA >= 6) ){
	//if( (temp_buf >= 42) || ( VolBuf >= 400 ) ){   // V OUT 제어 사양 24.05.09
	if( (temp_buf >= 100) || ( Digit1_Value >= 6 ) ){	// 25.4.15
		// toggle 
		//Warning_flag = 0;
		//Warning_BlinkTimer = 0;
		#if 0
		if( CurrentmA >= 6 ) Digit1_Value = 0xFFFF;	// OL
		#endif
	}
	else{
		Warning_Timer = 3000;		
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	}


	task_PON();
	

	if( task1secflag ){
		task1secflag = 0;

		printf("\r\n GPIOC->IDR[%x]", GPIOC->IDR );

		printf("\r\n PA6Vol(%d) PA6_ADC(%d)  ", (PA6_ADC*500)/(4096-1) , PA6_ADC );

	
	}
	



}



