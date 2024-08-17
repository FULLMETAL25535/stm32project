#include "stm32f10x.h"                  // Device header
#include "Delay.h"
uint8_t KeyNum = 0;
/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init_A(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA1和PA2引脚初始化为推挽输出
	
	
}
void Key_Init_B(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	 
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PB1和PB11引脚初始化为上拉输入
}

/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键2按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												//置键码为2
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键3按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 3;												//置键码为3
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键4按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 4;												//置键码为4
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键5按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 5;												//置键码为5
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键6按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 6;												//置键码为6
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键7按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 7;												//置键码为7
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键8按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 8;												//置键码为8
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键9按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 9;												//置键码为9
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键10按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 10;												//置键码为10
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键11按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 11;												//置键码为11
	}
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)			//读PB11输入寄存器的状态，如果为0，则代表按键12按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 12;												//置键码为12
	}
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
