//bibliotecas
#include <p18F4550.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>
#include <timers.h>
#include "config_PIC18F4550.h"
#include "4550_lcd16.h"

//definção de entradas
	//sensores
#define Infravermelho PORTCbits.RC1
//#define Ferramentas PORTBbits
#define FerramentaA PORTBbits.RB0
#define FerramentaB PORTBbits.RB1
#define FerramentaC PORTBbits.RB2
#define FerramentaD PORTBbits.RB3
#define FerramentaE PORTBbits.RB4
#define FerramentaF PORTBbits.RB5
#define FerramentaG PORTBbits.RB6

#define FerramentaH PORTBbits.RC0
#define FerramentaI PORTBbits.RC1
#define FerramentaJ PORTBbits.RC2
#define FerramentaK PORTBbits.RC3
#define FerramentaL PORTBbits.RC4

//#define FerramentaH PORTBbits.RB7
	//botoes
#define PageUp PORTCbits.RC5
#define PageDown PORTCbits.RC6
#define ExibirIndisp PORTCbits.RC4
#define ExibirDisp PORTCbits.RC2

//definição de saidas
	//LEDs
#define LedDisp PORTEbits.RE0
#define LedIndisp PORTEbits.RE1
	//BUZZER
//#define Buzzer PORTCbits.RC0
#define PWM_OUT PORTCbits.RC0
	//RF
#define DATA0 PORTAbits.RA0
#define DATA1 PORTAbits.RA1
#define DATA2 PORTAbits.RA2
#define DATA3 PORTAbits.RA3
#define CLK PORTAbits.RA4
	//DISPLAY

//VARIAVEIS
int n_ferramentas=12;
int linhas_do_display=4;
int total_paginas=0;


//ar ferramentas[8][16]={("1234567890123456")
char ferramentas[12][16]={("Alicate de Bico"),
                         ("Alicate de Bico"),
                         ("Alicate de Bico"),
                         ("Alicate de Bico"),
                         ("Alicate Corte"),
                         ("Alicate Corte"),
                         ("Alicate Corte"),
                         ("Alicate Corte"),
                         ("Alicate Uni."),
                         ("Chave de Fenda"),
                         ("Chave Phillips"),
                         ("Chave Phillips")};
/*
char ferramnetas[8][16]={('A','	l','i','c','a','t','e',' ','c','o','r','t','e','\0','',''),
                         ('c','h','a','v','e',' ','d','e',' ','f','e','n','d','a','\0',''),
                         ('A','	l','i','c','a','t','e',' ','b','i','c','0','\0','','',''),
                         ('','','','','','','','','','','','','','','',''),
                         ('','','','','','','','','','','','','','','',''),
                         ('','','','','','','','','','','','','','','',''),
                         ('','','','','','','','','','','','','','','',''),
                         ('','','','','','','','','','','','','','','','')};
*/

unsigned exibir_disp=1;
unsigned alarme=0;
unsigned pagina=1;
unsigned ferramentas_disp[12]={0,0,0,0,
						0,0,0,0,0,0,0,0};
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
PORTD = 0X00;                       //Clear PORTD
PORTE = 0x00;                       //Clear PORTE

LATA = 0X00;                       //Clear PORTA
LATB = 0X00;                       //Clear PORTB
LATC = 0X00;                       //Clear PORTC
LATD = 0X00;                       //Clear PORTD
LATE = 0x00;                       //Clear PORTE

TRISA=0b00000000;
TRISB=0b11111111;
TRISC=0b0111110;
TRISD=0b11110000;
TRISE=0b000;

Delay(3);
}

void check_ferramentas(void){
	ferramentas_disp[0]=FerramentaA;
	ferramentas_disp[1]=FerramentaB;
	ferramentas_disp[2]=FerramentaC;
	ferramentas_disp[3]=FerramentaD;
	ferramentas_disp[4]=FerramentaE;
	ferramentas_disp[5]=FerramentaF;
	ferramentas_disp[6]=FerramentaG;
	ferramentas_disp[7]=FerramentaH;
	ferramentas_disp[8]=FerramentaI;
	ferramentas_disp[9]=FerramentaJ;
	ferramentas_disp[10]=FerramentaK;
	ferramentas_disp[11]=FerramentaL;

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

void send(void){
	CLK=0;
	//Delay10KTCYx(10);
	Delay_ms(PeriodoDeTransmissao);
	CLK=1;
	DATA0=0;
	DATA1=0;
	DATA2=0;
	DATA3=0;
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

void dicionario (char A){
	switch(A){
		case 1://'?': //inicio de transmissoa
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case 0://'?': //fim de trasmissao
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case 3://'?': //inicio de lista
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case 4://'?': //fim de lista
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case 5://'?': //ferramenta
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case 6://'?': // variavel
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
		break;

		case '0':
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '1':
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '2':
			DATA3=0;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '3':
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '4':
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '5':
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '6':
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '7':
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '8':
			DATA3=1;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case '9':
			DATA3=1;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
		break;

		case 'A':
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'B':
			DATA3=0;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'C':
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'D':
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'E':
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'F':
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'G':
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'H':
			DATA3=1;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'I':
			DATA3=1;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'J':
			DATA3=1;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'K':
			DATA3=1;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'L':
			DATA3=1;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'M':
			DATA3=1;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'N':
			DATA3=1;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'O':
			DATA3=1;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
		break;

		case 'P':
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'Q':
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'R':
			DATA3=0;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'S':
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'T':
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'U':
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'V':
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'W':
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'X':
			DATA3=1;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'Y':
			DATA3=1;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'Z':
			DATA3=1;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
		break;

		case 'a':
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'b':
			DATA3=0;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'c':
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'd':
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'e':
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'f':
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'g':
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'h':
			DATA3=1;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'i':
			DATA3=1;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'j':
			DATA3=1;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'k':
			DATA3=1;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'l':
			DATA3=1;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'm':
			DATA3=1;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'n':
			DATA3=1;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'o':
			DATA3=1;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
		break;

		case 'p':
			DATA3=0;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'q':
			DATA3=0;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'r':
			DATA3=0;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 's':
			DATA3=0;DATA2=0;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 't':
			DATA3=0;DATA2=1;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'u':
			DATA3=0;DATA2=1;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'v':
			DATA3=0;DATA2=1;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'w':
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'x':
			DATA3=1;DATA2=0;DATA1=0;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'y':
			DATA3=1;DATA2=0;DATA1=0;DATA0=1;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

		case 'z':
			DATA3=1;DATA2=0;DATA1=1;DATA0=0;
			send();
			DATA3=0;DATA2=1;DATA1=1;DATA0=1;
			send();
		break;

	}
}

void enviarRF(void){
	int i,j;
	dicionario(1);	//inicio de transmissao
	for (j=0;j<n_ferramentas;j++){
		dicionario(3);	//inico de lista
		for(i=0;i<16;i++){
			dicionario(ferramentas[j][i]);
		}
	}
	dicionario(6);	//variavel
	DATA0=ferramentas_disp[0];
	DATA1=ferramentas_disp[1];
	DATA2=ferramentas_disp[2];
	DATA3=ferramentas_disp[3];
	send();
	DATA0=ferramentas_disp[4];
	DATA1=ferramentas_disp[5];
	DATA2=ferramentas_disp[6];
	DATA3=ferramentas_disp[7];
	send();
	DATA0=alarme;
	DATA1=alarme;
	DATA2=alarme;
	DATA3=alarme;
	send();
	dicionario(0);
}

void verificar_ferramentas(void){
	int i;
	int j=0;
	for (i=0; i<=n_ferramentas;i++){
		if(ferramentas_disp[i]){
			j++;
		}else{
			alarme=1;
			i=n_ferramentas+1;
		}
		if(j==n_ferramentas){   //VERIFICAR
			alarme=0;
		}
	}
}


void check_IR(void){
    int i=0;
    OpenTimer0(TIMER_INT_ON
              &T0_8BIT
              &T0_SOURCE_INT
              &T0_PS_1_256);
    WriteTimer0(0x00);
//	int TIMER = ReadTimer0;
    while(ReadTimer0()<=0x08){	//colocar timer 0b00001000
        if(Infravermelho==0){
            Delay_ms(0.006);
            if(Infravermelho==0){
                i++;
            }
        }
    }
    CloseTimer0();
    if(i==3){
        verificar_ferramentas();
    }else if(i==4){
        alarme=0;
    }
}

void start(void){
	total_paginas=n_ferramentas/linhas_do_display;
	CLK=1;
	lcd_inicia(0x28,0x0F,0x06);
	lcd_LD_cursor (0);
	lcd_cls();
	lcd_posicao(1,1);
	lcd_print("Projeto 36t");
	config_io();
}


void main(void){
	start();
	while(1){
		check_ferramentas();
		check_button();
		print();
		check_alarme();
		set_led();
		check_IR();
		enviarRF();
		PWM();
	}
}
