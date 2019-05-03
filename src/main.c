#include "inc/lm4f120h5qr.h"
#include <stdio.h>
#include <stdlib.h>
/*
 * Stellaris LaunchPad
 *
 * 16x2 Lcd
 *
 * Lokman GÖKDERE
 */

#ifndef LCD_H_
#define LCD_H_

#define LCDPORT                  GPIO_PORTB_BASE
#define LCDPORTENABLE    SYSCTL_PERIPH_GPIOB
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define D4                               GPIO_PIN_4
#define D5                               GPIO_PIN_5
#define D6                               GPIO_PIN_6
#define D7                               GPIO_PIN_7

void Lcd_Komut(unsigned char);  //Lcd ye komut göndermeye yarar
void Lcd_Temizle(void);                 //Lcd ekranını temizler
void Lcd_Puts(char*);                   //String ifade yazdırır
void Lcd_Goto(char,char);               //Kursorü istenilen yere gönderir
void Lcd_init(void);                    //Lcd başlangıç ayarları
void Lcd_Putch(unsigned char);  //Tek karekter yazdırır

#endif /* LCD_H_ */


#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


void Lcd_init() {

        SysCtlPeripheralEnable(LCDPORTENABLE);
        GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, RS,  0x00 );

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  0x20 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        Lcd_Komut(0x28);
        Lcd_Komut(0xC0);
        Lcd_Komut(0x06);
        Lcd_Komut(0x80);
        Lcd_Komut(0x28);
        Lcd_Komut(0x0f);
        Lcd_Temizle();

}
void Lcd_Komut(unsigned char c) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

}

void Lcd_Putch(unsigned char d) {

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0xf0) );
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (d & 0x0f) << 4 );
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

}
void Lcd_Goto(char x, char y){

        if(x==1)
                Lcd_Komut(0x80+((y-1)%16));
        else
                Lcd_Komut(0xC0+((y-1)%16));
}

void Lcd_Temizle(void){
        Lcd_Komut(0x01);
        SysCtlDelay(10);
}

void Lcd_Puts( char* s){

        while(*s)
                Lcd_Putch(*s++);
}


void port_E_init() {
   volatile unsigned long tmp;
   SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
   tmp = SYSCTL_RCGCGPIO_R;
   GPIO_PORTE_LOCK_R = 0x4C4F434B;
   GPIO_PORTE_CR_R = 0x3F;

   GPIO_PORTE_AMSEL_R = 0x00;
   GPIO_PORTE_PCTL_R = 0x00000000;
   GPIO_PORTE_DIR_R = 0x00;
   GPIO_PORTE_AFSEL_R = 0x00;
   GPIO_PORTE_PUR_R = 0x3F;
   GPIO_PORTE_DEN_R = 0x3F;
}



#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"


int main(void) {

        SysCtlClockSet(
                        SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
                        | SYSCTL_OSC_MAIN);

        Lcd_init();
        port_E_init();

        int b1,b2,b3,b4,b5,b6,b7;
        int onlar=0,birler=0, v_onlar=0,v_yuzler=0;
        int yirmi,on,bes,bir,yarimlik,ceyreklik,metelik,delik,kurusluk;
        int yedek;
        char sayi[2];

        volatile unsigned delay,delay2,delay3;

        Lcd_Goto(1,4);
        Lcd_Puts("PARA BOZMA");
        Lcd_Goto(2,5);
        Lcd_Puts("MAKINESI");

        for(delay=0;delay<3000000;delay++);

        Lcd_Temizle();




        delay=0;
        delay2=0;

        while (1) {

        	b1 = GPIO_PORTE_DATA_R & 0x02;
        	b2 = GPIO_PORTE_DATA_R & 0x04;
        	b3 = GPIO_PORTE_DATA_R & 0x08;
        	b4 = GPIO_PORTE_DATA_R & 0x10;
        	b5 = GPIO_PORTE_DATA_R & 0x20;

        	if(b1 != 0x02){

        		onlar++;
        		onlar = onlar % 10;
        		itoa(onlar,sayi,10);
        		Lcd_Goto(1,12);
        		Lcd_Puts(sayi);
        		for(delay3=0;delay3<900000;delay3++);
        		delay=0;
        		delay2=0;
        	}
        	else if(b2 != 0x04){
        		birler++;
        	    birler = birler % 10;
        	    itoa(birler,sayi,10);
        	    Lcd_Goto(1,13);
        	    Lcd_Puts(sayi);
        	    for(delay3=0;delay3<900000;delay3++);
        	    delay=0;
        	    delay2=0;
        	}
        	else if(b3 != 0x08){
        		delay2++;
        		Lcd_Goto(1,14);
        	    Lcd_Puts(".");
        	    delay=0;
        	}
        	else if(b4 != 0x10){
        	    v_onlar++;
        	    v_onlar = v_onlar % 10;
        	    itoa(v_onlar,sayi,10);
        	    Lcd_Goto(1,15);
        	    Lcd_Puts(sayi);
        	    for(delay3=0;delay3<900000;delay3++);
        	    delay=0;
        	    delay2=0;
        	}
        	else if(b5 !=  0x20){
        	    v_yuzler++;
        	    v_yuzler = v_yuzler % 10;
        	    itoa(v_yuzler,sayi,10);
        	    Lcd_Goto(1,16);
        	    Lcd_Puts(sayi);
        	    for(delay3=0;delay3<900000;delay3++);
        	    delay=0;
        	    delay2=0;
        	}

        	if(delay2 >= 200){
        		Lcd_Temizle();
        		onlar=0;
        		birler=0;
        		v_onlar=0;
        		v_yuzler=0;
        		delay2=0;
        	}

        	if(delay==2000000){

        		Lcd_Temizle();

        		onlar = onlar*10;

        		yirmi = onlar/20;
        		onlar = onlar - yirmi*20;
        		yedek = onlar/10;

        		itoa(yedek,sayi,10);
        		Lcd_Goto(1,12);
        		Lcd_Puts(sayi);

        		itoa(birler,sayi,10);
        		Lcd_Goto(1,13);
        		Lcd_Puts(sayi);

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(yirmi,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("YIRMILIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		on = onlar/10;

        		itoa(birler,sayi,10);
        		Lcd_Goto(1,13);
        		Lcd_Puts(sayi);

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(on,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("ONLUK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		bes = birler/5;
        		birler = birler - bes*5;

        		itoa(birler,sayi,10);
        		Lcd_Goto(1,13);
        		Lcd_Puts(sayi);

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(bes,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("BESLIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		bir = birler;

        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(bir,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("BIRLIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		v_onlar = v_onlar*10;

        		yarimlik = v_onlar/50;
        		v_onlar = v_onlar - yarimlik*50;



        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(yarimlik,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("YARIMLIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		yedek = v_onlar + v_yuzler;

        		ceyreklik = yedek/25;
        		yedek = yedek - ceyreklik*25;

        		v_onlar = yedek/10;
        		v_yuzler = yedek%10;

        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		itoa(v_onlar,sayi,10);
        		Lcd_Goto(1,15);
        		Lcd_Puts(sayi);

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(ceyreklik,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("CEYREKLIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();

        		metelik = yedek/10;
        		yedek = yedek - metelik*10;
        		v_yuzler = yedek%10;

        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		Lcd_Goto(1,15);
        		Lcd_Puts("0");

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(metelik,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("METELIK");

        		for(delay3=0;delay3<6000000;delay3++);


        		Lcd_Temizle();

        		delik = yedek/5;
        		yedek = yedek -delik*5;
        		v_yuzler = yedek %10;

        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		Lcd_Goto(1,15);
        		Lcd_Puts("0");

        		itoa(v_yuzler,sayi,10);
        		Lcd_Goto(1,16);
        		Lcd_Puts(sayi);


        		itoa(delik,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("DELIK");

        		for(delay3=0;delay3<6000000;delay3++);

        		kurusluk = yedek;

        		Lcd_Goto(1,13);
        		Lcd_Puts("0");

        		Lcd_Goto(1,14);
        		Lcd_Puts(".");

        		Lcd_Goto(1,15);
        		Lcd_Puts("0");

        		Lcd_Goto(1,16);
        		Lcd_Puts("0");

        		itoa(kurusluk,sayi,10);
        		Lcd_Goto(2,1);
        		Lcd_Puts(sayi);
        		Lcd_Goto(2,3);
        		Lcd_Puts("KURUSLUK");

        		for(delay3=0;delay3<6000000;delay3++);

        		Lcd_Temizle();
        		Lcd_Goto(1,1);
        		Lcd_Puts("Lutfen Resetleme");
        		Lcd_Goto(2,5);
        		Lcd_Puts("Yapiniz!");



        		onlar=0;
        		birler=0;
        		v_onlar=0;
        		v_yuzler=0;

        		delay=0;

        	}

        	delay++;

        }
}
