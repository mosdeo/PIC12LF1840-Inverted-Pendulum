#include <xc.h> // include standard header file
 
void OSC_Initial(void)
{
	if(1)//16MHz
	{
		// set up oscillator control register
	    OSCCONbits.SPLLEN=0;    // PLL is disabled
	    OSCCONbits.IRCF=0x0F;   //set OSCCON IRCF bits to select OSC frequency=16Mhz
	    OSCCONbits.SCS=0x02;    //set the SCS bits to select internal oscillator block
	}
	if(0)//32MHz
	{
	    OSCCONbits.SPLLEN=1;    // PLL is Enabled
	    OSCCONbits.IRCF=0x0E;   //set OSCCON IRCF bits to select OSC frequency=8Mhz
	    OSCCONbits.SCS=0x00;    //set the SCS bits to select internal oscillator block
	}    
}

void IO_Initial(void)
{	
	//設定各pin腳為
	//類比\數位			I\O
	ANSELAbits.ANSA0=0,	TRISAbits.TRISA0=0;	//LED(Motor +)
	ANSELAbits.ANSA1=1,	TRISAbits.TRISA1=1;	//ADC in
	ANSELAbits.ANSA2=0,	TRISAbits.TRISA2=0;	//LED(Motor -)
  /*RA3 無類比功能*/	TRISAbits.TRISA3=0; //Output Only
	ANSELAbits.ANSA4=0,	TRISAbits.TRISA4=0;	//Tx
	/*RA5 無類比功能*/	TRISAbits.TRISA5=1;	//Rx
}

void UART_Initial(void)
{
	TXCKSEL=1;				//設定 TX pin from RA0 to RA4
	RXDTSEL=1;				//設定 RX pin from RA1 to RA5
	
	//設定Baud Rate
	//取((_XTAL_FREQ/(64*BAUDRATE))-1)的...
	SPBRGL=12;	//整數
  SPBRGH=0;	//小數
	
	TXSTAbits.BRGH=0;		//選擇低速BR
	TXSTAbits.TXEN = 1; //使傳送可以傳送
	TXSTAbits.SYNC = 0;	//設定為非同步
	
	PIR1bits.RCIF=0;		//確保旗標清除
	PIE1bits.RCIE=1;		//Rx中斷啟用
	RCSTAbits.SPEN=1;		//啟用UART
	RCSTAbits.CREN=1;		//Rx可接收
}

void PWM_Initial(void)
{
	//禁止PWM輸出
	TRISAbits.TRISA0=1;
	TRISAbits.TRISA2=1;
	
	//設定PWM週期,PWM Period = (PR2+1)*4*TOSC �h
	PR2=0x40;
	
	//設定CCP1CON,將CCP模組設為PWM之用
	CCP1CON=0b00001100;
	
	unsigned char DummyNumber=0x00;
	//設定Duty Cycle, 填寫CCPR1L(高8位)與CCP1CONbits.CC1P1(低2位)
	CCPR1L = DummyNumber>>2;
	CCP1CONbits.DC1B = DummyNumber&0x03;
	
	//設定Timer2
	//清除中斷旗標
	PIR1bits.TMR2IF=0;
	
	//輸入脈波預除頻,4^n,n={0,1,2,3}
	T2CONbits.T2CKPS=0x0;
	
	//啟動Timer2
	TMR2ON=1;
	
	//開放PWM輸出
	TRISAbits.TRISA0=0;
	TRISAbits.TRISA2=0;
}