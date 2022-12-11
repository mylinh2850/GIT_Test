#include "main.h"
// Noi khai bao hang so
#define     LED     PORTD
#define     ON      1
#define     OFF     0

// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};

// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);

void BaiTap_UART();
void SendKeyToUART();
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0;
	init_system();
    lcd_clear();
    LcdClearS();
    delay_ms(1000);

	while (1)
	{
        while (!flag_timer3);
        flag_timer3 = 0;
        scan_key_matrix();
        BaiTap_UART();
        //SendKeyToUART();
        DisplayLcdScreen();
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_system(void)
{
    TRISB = 0x00;		//setup PORTB is output
    TRISA = 0x00;
    init_lcd();
    LED = 0x00;
    init_interrupt();
    delay_ms(1000);
    init_timer0(4695);//dinh thoi 1ms sai so 1%
    init_timer1(9390);//dinh thoi 2ms
    init_timer3(46950);//dinh thoi 10ms
    SetTimer0_ms(2);
    SetTimer1_ms(10);
    SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
    init_key_matrix_with_uart();
    init_uart();
    init_adc();
}

void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED | arrayMapOfOutput[index];
	}

}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		LED = LED & ~arrayMapOfOutput[index];
	}
}

void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<=7 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

unsigned char isButtonCall()
{
    if(key_code[0] == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char isButtonMessage()
{
    if(key_code[1] == 1)
        return 1;
    else
        return 0;
}

void BaiTap_UART()
{
    int i_uart;
    unsigned long pH_value,adc_value;
    adc_value = get_adc_value();
    pH_value = (adc_value * 140)/1023;
    if (pH_value<100)
    {
        LcdPrintNumS(0,10,pH_value/10);
        LcdPrintStringS(0,11,".");
        LcdPrintNumS(0,12,pH_value%10);
    }
    else
    {
        LcdPrintNumS(0,10,pH_value/10);
        LcdPrintStringS(0,12,".");
        LcdPrintNumS(0,13,pH_value%10);
    }
    
    if (flagUartComplete)
    {
        flagUartComplete = 0;
        uart_putchar(0x04);
        uart_putchar(0x03);
        uart_putchar(0x05);
        uart_putchar(pH_value%256);
    }
}

void SendKeyToUART()
{
    int i;
    for (i=0; i<16; i++)
        if (key_code[i] == 1)
            uart_putchar(i);
}