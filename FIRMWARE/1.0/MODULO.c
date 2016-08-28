//bibliotecas
#include <p18F2550.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include "config_PIC18F2550.h"
#include "2550_lcd16.h"


//definção de entradas
	//botoes
#define PageUp PORTBbits.RB2
#define PageDown PORTBbits.RB3
#define ExibirIndisp PORTBbits.RB1
#define ExibirDisp PORTBbits.RB0
#define VerficarFerramenta PORTCbits.RC4
#define DesligarAlarme PORTCbits.RC5
	//RF
#define DATA0 PORTAbits.RA0
#define DATA1 PORTAbits.RA1
#define DATA2 PORTAbits.RA2
#define DATA3 PORTAbits.RA3
#define CLK PORTAbits.RA4

//definição de saidas
	//LEDs
#define LedDisp PORTCbits.RC2
#define LedIndisp PORTCbits.RC1
#define Infravermelho PORTCbits.RC6
	//BUZZER
//#define Buzzer PORTCbits.RC0
#define PWM_OUT PORTCbits.RC0
	//DISPLAY

//VARIAVEIS
int n_ferramentas=8;
int linhas_do_display=2;
int total_paginas=0;
char ferramentas[][16];

unsigned exibir_disp=1;
unsigned alarme=0;
unsigned pagina=1;
unsigned buffer[8]={0,0,0,0,0,0,0,0};
unsigned ferramentas_disp[];//={0,0,0,0,
						//0,0,0,0};
//CONFIG VAR
	//delay
int CRYSTAL = 20000000;
	//pwm
unsigned PWM_ON=0;
unsigned PWM_UPDATE=0;
unsigned PWM_CONFIG=0;
float FREQUENCY=0;
float T=0;
float TON=0;
float TOFF=0;
int PWM_PORCENTAGEM=0;
	//RF
float PeriodoDeTransmissao=0.01;




void Delay_ms(float ms){
	int CICLO=0;
	ms=ms*1000;
	if(ms>=0,01){
		CICLO=(ms*CRYSTAL)/40;
		Delay10KTCYx(CICLO);
	}else if(ms<=0,01 && ms>0,001){
		CICLO=(ms*CRYSTAL)/400;
	 	Delay1KTCYx(CICLO);
	}else if(ms<=0,001 && ms<0,0001){
		CICLO=(ms*CRYSTAL)/4000;
		Delay100TCYx(CICLO);
	}else if(ms>0,00001){
		CICLO=(ms*CRYSTAL)/40000;
		Delay10TCYx(CICLO);
	}
}

void Delay(float s){
	int CICLO=0;
	if(s>=0,01){
		CICLO=(s*CRYSTAL)/40;
		Delay10KTCYx(CICLO);
	}else if(s<=0,01 && s>0,001){
		CICLO=(s*CRYSTAL)/400;
	 	Delay1KTCYx(CICLO);
	}else if(s<=0,001 && s<0,0001){
		CICLO=(s*CRYSTAL)/4000;
		Delay100TCYx(CICLO);
	}else if(s>0,00001){
		CICLO=(s*CRYSTAL)/40000;
		Delay10TCYx(CICLO);
	}
}

void PWM (void){
	PWM_ON=1;
    if (PWM_UPDATE){
        PWM_UPDATE=0;
        T=1000/FREQUENCY;
        PWM_PORCENTAGEM=PWM_PORCENTAGEM/100;
        TON=T*PWM_PORCENTAGEM;
        TOFF=T-TON;
    }
    PWM_OUT=1;
    Delay_ms(TON);
    PWM_OUT=1;
    Delay_ms(TOFF);

}

//configurar entradas e saidas
void config_io(void){
PORTA = 0X00;                       //Clear PORTA
PORTB = 0X00;                       //Clear PORTB
PORTC = 0X00;                       //Clear PORTC


LATA = 0X00;                       //Clear PORTA
LATB = 0X00;                       //Clear PORTB
LATC = 0X00;                       //Clear PORTC

TRISA=0b00011111;
TRISB=0b00001111;
TRISC=0b00110000;

Delay(3);
}

int traduzirP(void){
	int output = 0;
	buffer[0]=0;
	buffer[1]=0;
	buffer[2]=0;
	buffer[3]=0;
	buffer[4]=0;
	buffer[5]=0;
	buffer[6]=0;
	buffer[7]=0;

	if(DATA0){
		output+=1;
		buffer[0]=1;
	}
	if(DATA1){
		output+=2;
		buffer[1]=1;
	}
	if(DATA2){
		output+=4;
		buffer[2]=1;
	}
	if(DATA3){
		output+=8;
		buffer[3]=1;
	}
	return(output);
}

int traduzirS(int output){
	if(DATA0){
		output+=16;
		buffer[4]=1;
	}
	if(DATA1){
		output+=32;
		buffer[5]=1;
	}
	if(DATA2){
		output+=64;
		buffer[6]=1;
	}
	buffer[7]=DATA3;
	//if(DATA3){
	//	output+=128;
	//	buffer[7]=1;
	//}
	return(output);
}

void check_button(void){
	if (ExibirDisp==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (ExibirDisp==0){
			exibir_disp=1;
		}
	}
	if (ExibirIndisp==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (ExibirIndisp==0){
			exibir_disp=0;
		}
	}
	if (PageUp==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (PageUp==0){
			pagina++;
			if(pagina>total_paginas){
				pagina=1;
			}
		}
	}
	if (PageDown==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (PageDown==0){
			pagina--;
			if(pagina<1){
				pagina=total_paginas;
			}
		}
	}
	if (VerficarFerramenta==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (VerficarFerramenta==0){
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
		}
	}
	if (DesligarAlarme==0){
		//Delay10KTCYx(50);
		Delay_ms(500);
		if (DesligarAlarme==0){
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
			Infravermelho=1;
			Delay_ms(0.01);
			Infravermelho=0;
			Delay_ms(0.01);
		}
	}
}

void print(void){
	int i,t, j=0;
	lcd_cls();
	if (exibir_disp){
		for (i=0; i++; i<n_ferramentas){
			if (ferramentas_disp[i+(pagina-1)*linhas_do_display] && j<linhas_do_display){
				j++;
				if(j==0){
					lcd_posicao(1,1);
					for(t=0;t++;t<16){
						lcd_printchar(ferramentas[i+(pagina-1)*linhas_do_display][t]);
					}
				}else if (j==1){
					lcd_posicao(2,1);
					for(t=0;t++;t<16){
						lcd_printchar(ferramentas[i+(pagina-1)*linhas_do_display][t]);
					}
				}
			}
		}
	}else{
		for (i=0; i++; i<n_ferramentas){
			if (ferramentas_disp[i+(pagina-1)*linhas_do_display]==0 && j<linhas_do_display){
				j++;
				if(j==0){
					lcd_posicao(1,1);
					for(t=0;t++;t<16){
						lcd_printchar(ferramentas[i+(pagina-1)*linhas_do_display][t]);
					}
				}else if (j==1){
					lcd_posicao(2,1);
					for(t=0;t++;t<16){
						lcd_printchar(ferramentas[i+(pagina-1)*linhas_do_display][t]);
					}
				}
			}
		}
	}
}



void check_alarme(void){
	if (alarme){
		if(PWM_CONFIG==0){
			PWM_UPDATE=1;
			FREQUENCY=500;
			PWM_CONFIG=1;
		}
		PWM_ON=1;
	}else{
		PWM_ON=0;
	}
}

void set_led(void){
	LedDisp=exibir_disp;
	LedIndisp=~LedDisp;
}





void start(void){
	CLK=1;
	lcd_inicia(0x28,0x0F,0x06);
	lcd_LD_cursor (0);
	lcd_cls();
	lcd_posicao(1,1);
	lcd_print("Projeto 36t");
	config_io();
}

void LerRF(void){
	unsigned P_Byte = 0;	//primeira parte da transmissao
	unsigned S_Byte = 0;	//segunda parte da transmissao
	int RF_IN;
	unsigned A_Byte = 0;	//listas
	int NA_Byte = -1;	//numero de listas
	int NCA_Byte = 0;	//numero de caracteres
	unsigned B_Byte = 0;    //variavel
	int NB_Byte = 0;	//numero de variaveis



	if (CLK == 1 && P_Byte == 0){
		P_Byte = 1;
		RF_IN = traduzirP();
	}

	if (CLK && P_Byte){
		P_Byte = 0;
		S_Byte = 1;
		RF_IN = traduzirS(RF_IN);
	}

	if(S_Byte){
		S_Byte = 0;
		if(RF_IN!=0){
			if (A_Byte == 0 && B_Byte == 1){
				if(RF_IN==0){
					alarme=0;
				}else{
					alarme=1;
				}
			}
			if (A_Byte == 1 && B_Byte == 1){
				total_paginas=NA_Byte/linhas_do_display;
				if(NB_Byte>=NA_Byte){
					A_Byte=0;
				}else{
					int i;
					for(i=0;i<8;i++){
						NB_Byte++;
						ferramentas_disp[NB_Byte]=buffer[i];
					}
				}
			}
			if (A_Byte == 1 && B_Byte == 0){
				if(RF_IN==3){
					NCA_Byte=0;
					NA_Byte++;
				}else if(RF_IN==6){
					B_Byte=1;
				}else{
					ferramentas[NA_Byte][NCA_Byte]=RF_IN;
				}
			}
			if (A_Byte == 0 && B_Byte == 0){
				if(RF_IN == 1){
					A_Byte = 1;
				}else{
					P_Byte = 0;
					S_Byte = 0;
					A_Byte = 0;
					NA_Byte = -1;
					B_Byte = 0;
					NB_Byte = 0;
				}
			}
		}else{
			P_Byte = 0;
			S_Byte = 0;
			A_Byte = 0;
			NA_Byte = -1;
			B_Byte = 0;
			NB_Byte = 0;
		}
	}
}
void main(void){
	start();
	while(1){
		LerRF();
		check_button();
		print();
		check_alarme();
		set_led();
        PWM();
	}
}






