//#define FOSC 20
#pragma config FOSC = HSPLL_HS     // Oscilador HS com PLL habilitado, HS usado pelo USB
#pragma config USBDIV = 2          // A fonte de clock para o USB vem do bloco 96MHz PLL dividido por 2
#pragma config PLLDIV = 5          // Oscilador de entrada equivalente a 20MHz dividido por 5
#pragma config CPUDIV = OSC1_PLL2  // 96MHz PLL dividido por 2
#pragma config PBADEN = OFF        // Desabilita conversor A/D no PORTB
#pragma config VREGEN = ON         // Habilita o regulador de tensão da USB
#pragma config BOR = OFF           // Brown-out Reset desabilitado, tanto no hardware como no software
#pragma config PWRT = ON           // Habilita Power-up Timer
#pragma config WDT = OFF           // Watch-dog Timer desabilitado
#pragma config LVP = OFF           // Desabilita programação por baixa tensão
#pragma config DEBUG = OFF         // Desabilita modo de DEGUB



/*
// Tciclo = 4/Fosc = 0,2us
//#pragma config FOSC = INTRC

#pragma config FOSC =HS 
#pragma config WDT = ON 	//Desabilita o Watchdog Timer (WDT).
#pragma config PWRT = ON	//Habilita o Power-up Timer (PWRT).
#pragma config BOR = OFF    //Brown-out Reset (BOR) habilitado somente no hardware.
//#pragma config BORV = 1 	//Voltagem do BOR é 4,33V.
#pragma config PBADEN = OFF	//RB0,1,2,3 e 4 configurado como I/O digital.
#pragma config LVP = OFF	//Desabilita o Low Voltage Program.


*/