//bibliotecas
#include <p18F4550.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "config_PIC18F4550.h"
//definção de entradas
//mux
#define A PORTAbits.RA0
#define B PORTAbits.RA1
#define C PORTAbits.RA2

//ferramentas
#define FerramentaA PORTBbits.RB0
#define FerramentaB PORTBbits.RB1
#define FerramentaC PORTBbits.RB2
#define FerramentaD PORTBbits.RB3
#define FerramentaE PORTBbits.RB4
#define FerramentaF PORTBbits.RB5
#define FerramentaG PORTBbits.RB6
#define FerramentaH PORTBbits.RB7

	//botoes
#define Lock PORTEbits.RE1
#define Imperial PORTAbits.RA3
//definição de saidas
	//LEDs
#define Led PORTEbits.RE2
	//BUZZER
//#define PWM_OUT PORTCbits.RC0
#define PWM_OUT PORTEbits.RE0
	//DISPLAY1
#define DATAa PORTDbits.RD0
#define DATAb PORTDbits.RD1
#define DATAc PORTDbits.RD2
#define DATAd PORTDbits.RD3
	//DISPLAY2
#define DATAe PORTDbits.RD4
#define DATAf PORTDbits.RD5
#define DATAg PORTDbits.RD6
#define DATAh PORTDbits.RD7


//VARIAVEIS
int n_ferramentas=64;
unsigned CanGetFerramentas=1;
unsigned alarme=0;

#pragma udata large_udata
unsigned char ferramentas_disp[65];
#pragma udata

int temp_buff=0;
int temp_lock=0;

	//delay
int CRYSTAL = 20000000;

void Delay_ms(float ms){
	int CICLO=0;
	ms=ms*1000;
	if(ms>=0.01){
		CICLO=(ms*CRYSTAL)/40000;
		Delay10KTCYx(CICLO);
	}else if(ms<=0.01 && ms>0.001){
		CICLO=(ms*CRYSTAL)/4000;
	 	Delay1KTCYx(CICLO);
	}else if(ms<=0.001 && ms<0.0001){
		CICLO=(ms*CRYSTAL)/400;
		Delay100TCYx(CICLO);
	}else if(ms<0.00001){
		CICLO=(ms*CRYSTAL)/40;
		Delay10TCYx(CICLO);
	}
}

void Delay(float s){
	int CICLO=0;
	if(s>=0.01){
		CICLO=(s*CRYSTAL)/40000;
		Delay10KTCYx(CICLO);
	}else if(s<=0.01 && s>0.001){
		CICLO=(s*CRYSTAL)/4000;
	 	Delay1KTCYx(CICLO);
	}else if(s<=0.001 && s<0.0001){
		CICLO=(s*CRYSTAL)/400;
		Delay100TCYx(CICLO);
	}else if(s<0.00001){
		CICLO=(s*CRYSTAL)/40;
		Delay10TCYx(CICLO);
	}
}


//configurar entradas e saidas

void check_ferramentas(void){

				int i;
                A=0;B=0;C=0;
            ferramentas_disp[0]=FerramentaA;
            ferramentas_disp[1]=FerramentaB;
            ferramentas_disp[2]=FerramentaC;
            ferramentas_disp[3]=FerramentaD;
            ferramentas_disp[4]=FerramentaE;
            ferramentas_disp[5]=FerramentaF;
            ferramentas_disp[6]=FerramentaG;
            ferramentas_disp[7]=FerramentaH;

                A=1;B=0;C=0;
            ferramentas_disp[8]=FerramentaA;
            ferramentas_disp[9]=FerramentaB;
            ferramentas_disp[10]=FerramentaC;
            ferramentas_disp[11]=FerramentaD;
            ferramentas_disp[12]=FerramentaE;
            ferramentas_disp[13]=FerramentaF;
            ferramentas_disp[14]=FerramentaG;
            ferramentas_disp[15]=FerramentaH;

                A=0;B=1;C=0;
            ferramentas_disp[16]=FerramentaA;
            ferramentas_disp[17]=FerramentaB;
            ferramentas_disp[18]=FerramentaC;
            ferramentas_disp[19]=FerramentaD;
            ferramentas_disp[20]=FerramentaE;
            ferramentas_disp[21]=FerramentaF;
            ferramentas_disp[22]=FerramentaG;
            ferramentas_disp[23]=FerramentaH;

                A=1;B=1;C=0;
            ferramentas_disp[24]=FerramentaA;
            ferramentas_disp[25]=FerramentaB;
            ferramentas_disp[26]=FerramentaC;
            ferramentas_disp[27]=FerramentaD;
            ferramentas_disp[28]=FerramentaE;
            ferramentas_disp[29]=FerramentaF;
            ferramentas_disp[30]=FerramentaG;
            ferramentas_disp[31]=FerramentaH;

                A=0;B=0;C=1;
            ferramentas_disp[32]=FerramentaA;
            ferramentas_disp[33]=FerramentaB;
            ferramentas_disp[34]=FerramentaC;
            ferramentas_disp[35]=FerramentaD;
            ferramentas_disp[36]=FerramentaE;
            ferramentas_disp[37]=FerramentaF;
            ferramentas_disp[38]=FerramentaG;
            ferramentas_disp[39]=FerramentaH;

                A=1;B=0;C=1;
            ferramentas_disp[40]=FerramentaA;
            ferramentas_disp[41]=FerramentaB;
            ferramentas_disp[42]=FerramentaC;
            ferramentas_disp[43]=FerramentaD;
            ferramentas_disp[44]=FerramentaE;
            ferramentas_disp[45]=FerramentaF;
            ferramentas_disp[46]=FerramentaG;
            ferramentas_disp[47]=FerramentaH;

                A=0;B=1;C=1;
            ferramentas_disp[48]=FerramentaA;
            ferramentas_disp[49]=FerramentaB;
            ferramentas_disp[50]=FerramentaC;
            ferramentas_disp[51]=FerramentaD;
            ferramentas_disp[52]=FerramentaE;
            ferramentas_disp[53]=FerramentaF;
            ferramentas_disp[54]=FerramentaG;
            ferramentas_disp[55]=FerramentaH;

                A=1;B=1;C=1;

            ferramentas_disp[56]=FerramentaA;
            ferramentas_disp[57]=FerramentaB;
            ferramentas_disp[58]=FerramentaC;
            ferramentas_disp[59]=FerramentaD;
            ferramentas_disp[60]=FerramentaE;
            ferramentas_disp[61]=FerramentaF;
            ferramentas_disp[62]=FerramentaG;
            ferramentas_disp[63]=FerramentaH;
}

void check_button(void){
	if (Lock==0){
		//Delay10KTCYx(50);
		Delay_ms(10);
		if (Lock==0){
			CanGetFerramentas=~CanGetFerramentas;
			 musc_step=~musc_step;
		}
	}
	if (Imperial==0){
		//Delay10KTCYx(50);
		Delay_ms(10);
		if (Imperial==0){
			alarme=0;
			temp_lock=temp_buff;
			if(musc_step){musica=~musica;}
		}
	}
}

void print(int j){
	int num1=j%10;
	int num2=j/10;
	switch(num1){
		case 0:
			DATAa=0;
			DATAb=0;
			DATAc=0;
			DATAd=0;
		break;
		case 1:
			DATAa=1;
			DATAb=0;
			DATAc=0;
			DATAd=0;
		break;
		case 2:
			DATAa=0;
			DATAb=1;
			DATAc=0;
			DATAd=0;
		break;
		case 3:
			DATAa=1;
			DATAb=1;
			DATAc=0;
			DATAd=0;
		break;
		case 4:
			DATAa=0;
			DATAb=0;
			DATAc=1;
			DATAd=0;
		break;
		case 5:
			DATAa=1;
			DATAb=0;
			DATAc=1;
			DATAd=0;
		break;
		case 6:
			DATAa=0;
			DATAb=1;
			DATAc=1;
			DATAd=0;
		break;
		case 7:
			DATAa=1;
			DATAb=1;
			DATAc=1;
			DATAd=0;
		break;
		case 8:
			DATAa=0;
			DATAb=0;
			DATAc=0;
			DATAd=1;
		break;
		case 9:
			DATAa=1;
			DATAb=0;
			DATAc=0;
			DATAd=1;
		break;
	}
	switch(num2){
		case 0:
			DATAe=0;
			DATAf=0;
			DATAg=0;
			DATAh=0;
		break;
		case 1:
			DATAe=1;
			DATAf=0;
			DATAg=0;
			DATAh=0;
		break;
		case 2:
			DATAe=0;
			DATAf=1;
			DATAg=0;
			DATAh=0;
		break;
		case 3:
			DATAe=1;
			DATAf=1;
			DATAg=0;
			DATAh=0;
		break;
		case 4:
			DATAe=0;
			DATAf=0;
			DATAg=1;
			DATAh=0;
		break;
		case 5:
			DATAe=1;
			DATAf=0;
			DATAg=1;
			DATAh=0;
		break;
		case 6:
			DATAe=0;
			DATAf=1;
			DATAg=1;
			DATAh=0;
		break;
		case 7:
			DATAe=1;
			DATAf=1;
			DATAg=1;
			DATAh=0;
		break;
		case 8:
			DATAe=0;
			DATAf=0;
			DATAg=0;
			DATAh=1;
		break;
		case 9:
			DATAe=1;
			DATAf=0;
			DATAg=0;
			DATAh=1;
		break;
	}
}

void check_alarme(void){
	if (temp_buff!=temp_lock){
			alarme = 1;
		}
	if (alarme){
		PWM_OUT=1;
	}
	if (alarme==0){
		PWM_OUT=0;
	}

}

void set_led(void){
	Led=CanGetFerramentas;
}

int verificar_ferramentas(void){

	int i;
	int j=0;
	for (i=0; i<n_ferramentas;i++){
		if(ferramentas_disp[i]==1){
			j++;
		}
	}
	temp_buff=j;
	if(CanGetFerramentas==1){
		temp_lock=j;
	}
	return (j);
}

void main(void){

PORTA = 0X00;                       //Clear PORTA
PORTB = 0X00;                       //Clear PORTB
PORTC = 0X00;                       //Clear PORTC
PORTD = 0X00;                       //Clear PORTD
PORTE = 0x00;                       //Clear PORTE

LATA = 0X00;                       //Clear PORTA
LATB = 0X00;                       //Clear PORTB
LATC = 0X00;                       //Clear PORTC
LATD = 0X00;                       //Clear PORTD
LATE = 0x00;                       //Clear PORTE

TRISA=0b00001000;
TRISB=0b11111111;
TRISC=0b00000000;
TRISD=0b00000000;
TRISE=0b010;

ADCON1=15; //jesus falou que é 15 e pronto. para de testar

	while(1){
		check_button();
		check_ferramentas();
		print(verificar_ferramentas());
		set_led();

	check_alarme();
	}
}
