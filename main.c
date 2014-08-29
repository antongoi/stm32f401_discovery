#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
//#include "misc.h"

#define MAXHORIZONTAL	640	/*maximum line length*/
#define MAXVERTICAL		480	/*the maximum length of the column*/
#define STARTPOINT		0	/*start point in new line, page*/
#define BLACKCOLOR		0	/*find color is black*/
#define WHITECOLOR		1	/*find color is white*/

#define LD6_ON			GPIO_SetBits(GPIOD, GPIO_Pin_15)	/*[PD15] pin 15: led blue.*/
#define LD6_OFF			GPIO_ResetBits(GPIOD, GPIO_Pin_15)	/*[PD15]*/
#define LD5_ON			GPIO_SetBits(GPIOD, GPIO_Pin_14)	/*[PD14] pin 14: led red.*/
#define LD5_OFF			GPIO_ResetBits(GPIOD, GPIO_Pin_14)	/*[PD14]*/
#define LD4_ON			GPIO_SetBits(GPIOD, GPIO_Pin_13)	/*[PD13] pin 13: led orange.*/
#define LD4_OFF			GPIO_ResetBits(GPIOD, GPIO_Pin_13)	/*[PD13]*/
#define LD3_ON			GPIO_SetBits(GPIOD, GPIO_Pin_12)	/*[PD12] pin 12: led green.*/
#define LD3_OFF			GPIO_ResetBits(GPIOD, GPIO_Pin_12)	/*[PD12]*/

/*create type bool*/
typedef int bool;
#define true 1
#define false 0

struct Two_Pixels {
	short Y0;
	short Y1;
	short Cr;
	short Cb;
};
typedef struct Two_Pixels TwoPixels;

struct RGBColorScheme {
	short R;
	short G;
	short B;
};
typedef struct RGBColorScheme RGB;

RGB YCCtoRGB(TwoPixels tp, short pixelNumber) {
	RGB result = {0, 0, 0};
	if (pixelNumber == 0) {
		result.R = (tp.Y0) + 1.402 * ((tp.Cr)-128);
		result.G = (tp.Y0) - 0.34414 * ((tp.Cb)-128) - 0.71414 * ((tp.Cr)-128);
		result.B = (tp.Y0) + 1.772 * ((tp.Cb)-128);
	} else {
		result.R = (tp.Y1) + 1.402 * ((tp.Cr)-128);
		result.G = (tp.Y1) - 0.34414 * ((tp.Cb)-128) - 0.71414 * ((tp.Cr)-128);
		result.B = (tp.Y1) + 1.772 * ((tp.Cb)-128);
	}
	return result;
}

short colorBits[8/*16*/];
short btn=0;

RGB testGlobalPixel = {0, 0, 0};

/*what the this??*/
void Delfy(uint32_t nCount)
{
	while(nCount--)
	{
	}
}

/*initialization connected to the pinout*/
void GPIOinit()
{
	GPIO_InitTypeDef gpioConf;

	//		--->	INPUT SIGNAL	[PA0]
	//Initialization input connected to the user button. signal(3v)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_0;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOA, &gpioConf);
	/*	*************************************************************************************************************************************************************************************	*/
	//		--->	INPUT SIGNAL 1	[PC1]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_1;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOC, &gpioConf);
	//		--->	INPUT SIGNAL 2	[PC3]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_3;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOC, &gpioConf);
	//		--->	INPUT SIGNAL 3	[PA1]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_1;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOA, &gpioConf);
	//		--->	INPUT SIGNAL 4	[PA3]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_3;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOA, &gpioConf);
	//		--->	INPUT SIGNAL 5	[PA4]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_4;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOA, &gpioConf);
	//		--->	INPUT SIGNAL 6	[PC2]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_2;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOC, &gpioConf);
	//		--->	INPUT SIGNAL 7	[PA6]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_6;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOA, &gpioConf);
	//		--->	INPUT SIGNAL 8	[PC4]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_4;		gpioConf.GPIO_Mode = GPIO_Mode_IN;	GPIO_Init(GPIOB, &gpioConf);
	//		---> 	INPUT SIGNAL 	[PE15]	PCLK
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);   gpioConf.GPIO_Pin = GPIO_Pin_15; 	gpioConf.GPIO_Mode = GPIO_Mode_IN;  GPIO_Init(GPIOE, &gpioConf);
	/*	*************************************************************************************************************************************************************************************	*/
	//		--->	INPUT SIGNAL	[PC15]	X 5
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_15;	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	GPIO_Init(GPIOC, &gpioConf);


	//		--->	OUTPUT SIGNAL												[PD12]						[PD13]						[PD14]						[PD15]
	//Initialization input connected to the LED.			pin 12: led green. 	[LD4]	pin 13: led orange. [LD3]	pin 14: led red. 	[LD5]	pin 15: led blue. 	[LD6]
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_100MHz; 	gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOD, &gpioConf);
	/*	*************************************************************************************************************************************************************************************	*/
	//		---> 	OUTOUT SIGNAL	[PA2]	XCLK
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_2;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_25MHz;		gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOA, &gpioConf);
	/*	*************************************************************************************************************************************************************************************	*/
	//		--->	OUTPUT SIGNAL	[PC14]	X 1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_14;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;	gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOC, &gpioConf);
	//		--->	OUTPUT SIGNAL	[PE6]	X 2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_6;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;	gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOE, &gpioConf);
	//		--->	OUTPUT SIGNAL	[PE4]	X 3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_4;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;	gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOE, &gpioConf);
	//		--->	OUTPUT SIGNAL	[PE2]	X 4
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	gpioConf.GPIO_Pin = GPIO_Pin_2;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;	gpioConf.GPIO_OType = GPIO_OType_PP;	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;	GPIO_Init(GPIOE, &gpioConf);
	}

void waitPCLK()
{
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15) != 0) { break;}	// PCLK byte rate
	}
}

/*input signal from camera*/
void inputsignal()
{

	//while(1) if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != 0) break;

	int Y0 = 0, Y1 = 0, Cb = 0, Cr = 0;

	waitPCLK();

	//if we get 3v - do
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)	colorBits[0]=1; /*R=R*2 + 1;*/	else	colorBits[0]=0; /*R=R*2;*/	//[PC1]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) != 0)	colorBits[2]=1; /*R=R*2 + 1;*/	else	colorBits[2]=0; /*R=R*2;*/ 	//[PC3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != 0)	colorBits[4]=1; /*R=R*2 + 1;*/	else	colorBits[4]=0; /*R=R*2;*/	//[PA1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) != 0)	colorBits[6]=1; /*R=R*2 + 1;*/	else	colorBits[6]=0; /*R=R*2;*/	//[PA3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) != 0)	colorBits[3]=1; /*R=R*2 + 1;*/	else	colorBits[3]=0; /*R=R*2;*/	//[PA4]

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) != 0)	colorBits[1]=1; /*G=G*2 + 1;*/	else	colorBits[1]=0; /*G=G*2;*/	//[PC2]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)	colorBits[5]=1; /*G=G*2 + 1;*/	else	colorBits[5]=0; /*G=G*2;*/	//[PA6]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0)	colorBits[7]=1; /*G=G*2 + 1;*/	else	colorBits[7]=0; /*G=G*2;*/	//[PC4]

	int i = 7;
	for (; i >= 0; i--) Cb = Cb*2 + colorBits[i];

	waitPCLK();

	//if we get 3v - do
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)	colorBits[0]=1; /*R=R*2 + 1;*/	else	colorBits[0]=0; /*R=R*2;*/	//[PC1]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) != 0)	colorBits[2]=1; /*R=R*2 + 1;*/	else	colorBits[2]=0; /*R=R*2;*/ 	//[PC3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != 0)	colorBits[4]=1; /*R=R*2 + 1;*/	else	colorBits[4]=0; /*R=R*2;*/	//[PA1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) != 0)	colorBits[6]=1; /*R=R*2 + 1;*/	else	colorBits[6]=0; /*R=R*2;*/	//[PA3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) != 0)	colorBits[3]=1; /*R=R*2 + 1;*/	else	colorBits[3]=0; /*R=R*2;*/	//[PA4]

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) != 0)	colorBits[1]=1; /*G=G*2 + 1;*/	else	colorBits[1]=0; /*G=G*2;*/	//[PC2]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)	colorBits[5]=1; /*G=G*2 + 1;*/	else	colorBits[5]=0; /*G=G*2;*/	//[PA6]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0)	colorBits[7]=1; /*G=G*2 + 1;*/	else	colorBits[7]=0; /*G=G*2;*/	//[PC4]

	i = 7;
	for (; i >= 0; i--) Y0 = Y0*2 + colorBits[i];

	waitPCLK();

	//if we get 3v - do
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)	colorBits[0]=1; /*R=R*2 + 1;*/	else	colorBits[0]=0; /*R=R*2;*/	//[PC1]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) != 0)	colorBits[2]=1; /*R=R*2 + 1;*/	else	colorBits[2]=0; /*R=R*2;*/ 	//[PC3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != 0)	colorBits[4]=1; /*R=R*2 + 1;*/	else	colorBits[4]=0; /*R=R*2;*/	//[PA1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) != 0)	colorBits[6]=1; /*R=R*2 + 1;*/	else	colorBits[6]=0; /*R=R*2;*/	//[PA3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) != 0)	colorBits[3]=1; /*R=R*2 + 1;*/	else	colorBits[3]=0; /*R=R*2;*/	//[PA4]

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) != 0)	colorBits[1]=1; /*G=G*2 + 1;*/	else	colorBits[1]=0; /*G=G*2;*/	//[PC2]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)	colorBits[5]=1; /*G=G*2 + 1;*/	else	colorBits[5]=0; /*G=G*2;*/	//[PA6]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0)	colorBits[7]=1; /*G=G*2 + 1;*/	else	colorBits[7]=0; /*G=G*2;*/	//[PC4]

	i = 7;
	for (; i >= 0; i--) Cr = Cr*2 + colorBits[i];

	waitPCLK();

	//if we get 3v - do
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)	colorBits[0]=1; /*R=R*2 + 1;*/	else	colorBits[0]=0; /*R=R*2;*/	//[PC1]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) != 0)	colorBits[2]=1; /*R=R*2 + 1;*/	else	colorBits[2]=0; /*R=R*2;*/ 	//[PC3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != 0)	colorBits[4]=1; /*R=R*2 + 1;*/	else	colorBits[4]=0; /*R=R*2;*/	//[PA1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) != 0)	colorBits[6]=1; /*R=R*2 + 1;*/	else	colorBits[6]=0; /*R=R*2;*/	//[PA3]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) != 0)	colorBits[3]=1; /*R=R*2 + 1;*/	else	colorBits[3]=0; /*R=R*2;*/	//[PA4]

	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) != 0)	colorBits[1]=1; /*G=G*2 + 1;*/	else	colorBits[1]=0; /*G=G*2;*/	//[PC2]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)	colorBits[5]=1; /*G=G*2 + 1;*/	else	colorBits[5]=0; /*G=G*2;*/	//[PA6]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0)	colorBits[7]=1; /*G=G*2 + 1;*/	else	colorBits[7]=0; /*G=G*2;*/	//[PC4]

	i = 7;
	for (; i >= 0; i--) Y1 = Y1*2 + colorBits[i];

	TwoPixels tp = {Y0, Y1, Cr, Cb};
	RGB pix0 = YCCtoRGB(tp, 0);
//	RGB pix1 = YCCtoRGB(tp, 1);

	testGlobalPixel = pix0;

/*	waitPCLK();

	//if we get 3v - do
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) != 0)	colorBits[8]=1; G=G*2 + 1;	else	colorBits[8]=0; G=G*2;	//[PC2]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) != 0)	colorBits[10]=1; G=G*2 + 1;	else	colorBits[10]=0; G=G*2; 	//[PA6]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) != 0)	colorBits[12]=1; G=G*2 + 1;	else	colorBits[12]=0; G=G*2;	//[PC4]

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) != 0)	colorBits[14]=1; B=B*2 + 1;	else	colorBits[14]=0; B=B*2;	//[PC1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) != 0)	colorBits[11]=1; B=B*2 + 1;	else	colorBits[11]=0; B=B*2; 	//[PC3]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) != 0)	colorBits[9]=1; B=B*2 + 1;	else	colorBits[9]=0; B=B*2;	//[PA1]
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)	colorBits[13]=1; B=B*2 + 1;	else	colorBits[13]=0; B=B*2;	//[PA3]
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) != 0)	colorBits[15]=1; B=B*2 + 1;	else	colorBits[15]=0; B=B*2;	//[PA4]

	R=R*8;
	G=G*4;
	B=B*8;*/

	//test block
		LD3_OFF;
		LD4_OFF;
		LD5_OFF;
		LD5_ON;
		LD6_OFF;
		while(1) {
			if (testGlobalPixel.R > 230 && testGlobalPixel.G > 230 && testGlobalPixel.B > 230) LD4_ON;
			if (testGlobalPixel.R < 30 && testGlobalPixel.G < 30 && testGlobalPixel.B < 30) LD6_ON;
			if (testGlobalPixel.R < 100 && testGlobalPixel.G > 200 && testGlobalPixel.B < 100) LD3_ON;
			//if (testGlobalPixel.R < 100 && testGlobalPixel.G < 100 && testGlobalPixel.B > 200) LD5_ON;

			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) != 0) break;
		}
	//end

//	btn = 0;
//	while(btn<4) button();

	/*short i = 0;
	if(R>220 && G>220 && B>220)		{LD6_ON; LD6_OFF;}
	else if (R<55 && G<55 && B<55) {LD5_ON; LD5_OFF;}
	else {LD3_ON; LD3_OFF;}
	*//*if(R>210 && G>210 && B>210) 	{ LD6_ON; i=0;	LD6_OFF; }
	else {LD4_ON; LD4_OFF;}
	if(R<50 && G<50 && B<50) 	{ LD5_ON; i=0;	LD5_OFF; }
	else {LD3_ON; LD3_OFF;}*/
}

void run()
{
	while(1)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2); 	//reference frequency. port XCLK

			inputsignal();						//get color signal and determine color
		}
}

void button()
{
	int isBreak = 0;
	while(1){
		if(isBreak) break;
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) != 0)
			while(1) if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0){ isBreak=1; break;}
	}
	//	{
		btn++;
		LD3_OFF;
		LD4_OFF;
		LD5_OFF;
		LD6_OFF;
		int i = btn*4-4;
		if(colorBits[i]==1) LD4_ON;
		if(colorBits[i+1]==1) LD5_ON;
		if(colorBits[i+2]==1) LD6_ON;
		if(colorBits[i+3]==1) LD3_ON;
//	}
}

int main(void)
{
	GPIOinit();
	run();
	return 0;
}
