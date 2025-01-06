
#define F_CPU 16000000 // Clock Speed
#define BAUD 9600
#define UBRR F_CPU / 16 / BAUD - 1

#define Key_Prt PORTB
#define Key_Ddr DDRB
#define Key_Pin PINB

#define KEY_PRT PORTB
#define KEY_DDR DDRB
#define KEY_PIN PINB


#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

char GetKey();
char get_key();

int buid_num();
void textout ( const char *str);
void numout(int num);
void test_selector();
int switch_num(char key, int num);
int result();
int reset();

int decflag,Awhole,Adec,Bwhole,Bdec,Rwhole, Rdec, length, remain;
char key, ab, oper, last_key;



void USART_Init(unsigned int ubrr) {
//Set baud rate /
UBRR1H = (unsigned char) (ubrr >> 8);
UBRR1L = (unsigned char) ubrr;
UCSR1B = (1 << RXEN) | (1 << TXEN);
// Set frame format: 8data */
UCSR1C = (3 << UCSZ0);
}

const unsigned char keypad[4][4] ={{ '1','2','3','+'},
                                   { '4','5','6','-'},
                                   {'7','8','9','*'},
                                   {'.','0','=','/'}};

char get_key()
{
    unsigned char colloc, rowloc, KeyPress;
    
    while(1)
    {
        do
        {
            Key_Prt &= 0x0F;
            colloc = (Key_Pin & 0X0F);
        } while(colloc != 0x0F);
        do
        {
            do
             {
                 _delay_ms(100);
                 colloc = (Key_Pin & 0x0F);            
             } while(colloc == 0x0F);
             
        _delay_ms(20);
        colloc = (Key_Pin & 0x0F);
        } while ( colloc == 0x0F);
        
        while(1) 
        {
            Key_Prt = 0xEF;
            asm("nop\n\t"
                "nop\n\t");
            colloc = ( Key_Pin & 0x0F);
            if(colloc != 0x0F)
            {
                rowloc = 0;
                break;
            }
            Key_Prt = 0xDF;
            asm("nop\n\t"
                "nop\n\t");
            colloc = ( Key_Pin & 0x0F);
            if (colloc != 0x0F)
            {
                rowloc = 1;
                break;
            }
            Key_Prt = 0xBF;
            asm("nop\n\t"
                "nop\n\t");
            colloc = (Key_Pin & 0x0F);
            if (colloc !=0x0F)
            {
                rowloc = 2;
                break;
            }
            Key_Prt = 0x7F;
            //_delay_ms(20);
            asm("nop\n\t"
                "nop\n\t");
            colloc = (Key_Pin & 0x0F);
            rowloc = 3;
            break;
        }
        if(colloc == 0x0E)
        {KeyPress = (keypad[ rowloc][0]);}
        else if(colloc == 0x0D)
        {KeyPress = (keypad[ rowloc][1]);}
        else if(colloc == 0x0B)
        {KeyPress = (keypad[ rowloc][2]);}
        else
        {KeyPress = (keypad[ rowloc][3]);}
        return KeyPress;
    }
    
}

void USART_send (unsigned char ch)
{
    while (! (UCSR1A & (1<<UDRE)));
    UDR1 = ch;
}

int main(void)
{   
    USART_Init(UBRR);
    ab=0;
    decflag=0;
    int sent;
    sent=0;
    oper='+';
    while(1){
        KEY_DDR = 0xF0; //R4321 = Out   &   C4321 = In
        KEY_PRT = 0xFF; 
        if(sent==1)
        {
                textout("Press any number button to reset");
                USART_send(0xD);       
                USART_send(0xA);
                textout("Press any operator to use the result as the 1st operator");
                USART_send(0xD);       
                USART_send(0xA);
                textout("Or press the = to iterate");
                USART_send(0xD);       
                USART_send(0xA);
                sent=0;
                key=get_key();
                
                
                switch(key)
                {
                    case('='):
                    {
                        Awhole=Rwhole;
                        Adec=Rdec;
                        
                        if(Awhole != 0)
                        {
                            length=floor(log10(abs(Awhole))) + 1;
                        }
                        if(length>3)
                        {
                            textout("Number too large to iterate, resetting now");
                            USART_send(0xD);       
                            USART_send(0xA);
                            reset();
                        }
                        break;
                    }
                    case('+'):
                    {
                        Awhole=Rwhole;
                        Adec=Rdec;
                        
                        decflag=0;
                        ab=1;
                        oper='+';
                        USART_send(0xD);       
                        USART_send(0xA);
                        textout("Function Selected: ");
                        USART_send(oper);
                        
                        break;
                    }
                    case('-'):
                    {
                        Awhole=Rwhole;
                        Adec=Rdec;
                        
                        decflag=0;
                        ab=1;
                        oper='-';
                        USART_send(0xD);       
                        USART_send(0xA);
                        textout("Function Selected: ");
                        USART_send(oper);
                        
                        break;
                    }
                    case('*'):
                    {
                        Awhole=Rwhole;
                        Adec=Rdec;
                        
                        decflag=0;
                        ab=1;
                        oper='*';
                        USART_send(0xD);       
                        USART_send(0xA);
                        textout("Function Selected: ");
                        USART_send(oper);
                        
                        break;
                    }
                    case('/'):
                    {
                        Awhole=Rwhole;
                        Adec=Rdec;
                        
                        decflag=0;
                        ab=1;
                        oper='/';
                        USART_send(0xD);       
                        USART_send(0xA);
                        textout("Function Selected: ");
                        USART_send(oper);
                        
                        break;
                    }
                    case('1'):
                    {
                        
                        reset();
                        USART_send('p');
                        
                        break;
                    }
                    case('2'):
                    {
                        
                        reset();
                        
                        break;
                    }
                    case('3'):
                    {
                        reset();
                        
                        break;
                    }
                    case('4'):
                    { 
                        reset();
                       
                        break;
                    }
                    case('5'):
                    {
                        reset();
                        
                        break;
                    }
                    case('6'):
                    {
                        reset();
                        
                        break;
                    }
                    case('7'):
                    {
                        reset();
                        
                        break;
                    }
                    case('8'):
                    {
                        reset();
                        
                        break;
                    }
                    case('9'):
                    {
                        reset();
                        
                        break;
                    }
                    case('0'):
                    {
                        reset();
                        
                        break;
                    }
                    
                    
                }
        }
                        
        
        switch(ab)
        {
            case(0):
            {
                if(key=='=')
                {
                    break;
                }
                if(decflag==0)
                {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Enter integer portion of the first operand(3 digits)");
                    USART_send(0xD);       
                    USART_send(0xA);
                    Awhole=build_num();
                    switch(Awhole)
                    {
                        case(66):
                        {
                            USART_send(0xD);       
                            USART_send(0xA);
                            textout("Execute order 66");
                            USART_send(0xd);
                            USART_send(0xA);
                            break;
                        }
                        case(69):
                        {
                            USART_send(0xd);
                            USART_send(0xA);
                            textout("Nice.");
                            USART_send(0xd);
                            USART_send(0xA);
                            break;
                        }
                        case(420):
                        {
                            USART_send(0xd);
                            USART_send(0xA);
                            textout("420 Blaze it");
                            break;
                        }
                        case(117):
                        {
                            USART_send(0xd);
                            USART_send(0xA);
                            textout("Is that you chief?");
                            USART_send(0xd);
                            USART_send(0xA);
                            break;
                        }
                        case(151):
                        {
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Bulbasaur");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Ivysaur");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Venasaur");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Charmander");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Charmeleon");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Charizard");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Squirtle");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Wartortel");
                            USART_send(0Xd);
                            USART_send(0xa);
                            textout("Blastoise");
                            break;
                        }
                    }
                }
                if(decflag==1)
                {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Enter decimal portion of the first operand(2 digits)");
                    USART_send(0xD);       
                    USART_send(0xA);
                    Adec=build_num();
                    
        
                }
                break;
            }
            case(1):
            {
                if(key=='=')
                {
                    break;
                }
                if(decflag==0)
                {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Enter integer portion of the second operand(3 digits)");
                    USART_send(0xD);       
                    USART_send(0xA);
                    Bwhole=build_num();
                }
                if(decflag==1)
                {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Enter decimal portion of the second operand(2 digits)");
                    USART_send(0xD);       
                    USART_send(0xA);
                    Bdec=build_num();
        
                }
                break;
            }
        }
        switch(key)
        {
            case('+'):
            {
                ab=1;
                oper='+';
                USART_send(0xD);       
                USART_send(0xA);
                textout("Function Selected: ");
                USART_send(oper);
                break;
            }
            case('-'):
            {
                ab=1;
                oper='-';
                USART_send(0xD);       
                USART_send(0xA);
                textout("Function Selected: ");
                USART_send(oper);
                break;
            }
            case('*'):
            {
                ab=1;
                oper='*';
                USART_send(0xD);       
                USART_send(0xA);
                textout("Function Selected: ");
                USART_send(oper);
                break;
            }
            case('/'):
            {
                ab=1;
                oper='/';
                USART_send(0xD);       
                USART_send(0xA);
                textout("Function Selected: ");
                USART_send(oper);
                break;
            }
            case('='):
            {
                USART_send(0xD);       
                USART_send(0xA);
                result();
                textout("The result is: ");                
                USART_send(0xD);       
                USART_send(0xA);
                numout(Awhole);
                USART_send('.');
                if(Adec<10)
                {
                    USART_send('0');
                }
                numout(Adec);
                USART_send(oper);
                numout(Bwhole);
                USART_send('.');
                if(Bdec<10)
                {
                    USART_send('0');
                }
                numout(Bdec);
                USART_send('=');
                numout(Rwhole);
                USART_send('.');
                if(Rdec<10)
                {
                    USART_send('0');
                }
                numout(Rdec);
                if (oper=='/')
                {
                    textout("  With a remainder of: ");
                    numout(remain);
                }
                USART_send(0xD);       
                USART_send(0xA);
                USART_send(0xD);       
                USART_send(0xA);
                sent=1;
                break;
            }
        }
        
        
        _delay_ms(20);
    }
        return 0;
}

int build_num()
{
    int num, n, deccount;
    num=0;
    n=0;
    deccount=0;
    
    if(decflag==0)
    {
        while(n<3)
        {
         last_key=key;
         key=get_key();
            
         switch (key)
            {
             case('0'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('1'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('2'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('3'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('4'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('5'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('6'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('7'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('8'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('9'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('+'):
             {  decflag=0;    
                return num;
                break;
             }
             case('-'):
             {
                  decflag=0;      
                return num;
                break;
             }
             case('*'):
             {
                        decflag=0;
                return num;
                break;
             }
             case('/'):
             {
                       decflag=0;
                return num;
                break;
             }
             case('.'):
             {      
                decflag=1;
                return num;
                deccount++;
                if (deccount==2)
                {
                    reset();
                    return 0;
                }
                break;
             }
             case('='):
             {      decflag=0;
                return num;
                
                break;
             }
             
            
            }
        }
    }
    else if (decflag==1)
    {
        while(n<2)
        {
            last_key=key;
            key=get_key();
            
            switch (key)
            {
             case('0'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('1'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('2'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('3'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('4'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('5'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('6'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('7'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('8'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('9'):
             {
                 num=switch_num(key,num);
                 USART_send(key);
                 n=n+1;
                 break;
             }
             case('+'):
             {     decflag=0;
                 if(n<2)
                 {
                    
                     num=num*10;
                 }
                return num;
                break;
             }
             case('-'):
             { 
                     decflag=0;
                 if(n<2)
                 {
                     
                     num=num*10;
                 }
                return num;
                break;
             }
             case('*'):
             {  decflag=0;
                 if(n<2)
                 {
                     num=num*10;
                 }
                return num;
                break;
             }
             case('/'):
             {      decflag=0;
                 if(n<2)
                 {
                     num=num*10;
                 }
                return num;
                break;
             }
             case('='):
             {      decflag=0;
                 if(n<2)
                 {
                     num=num*10;
                 }
                return num;
                break;
             }
             case('.'):
                {
                    deccount++;
                    if(deccount==2)
                    {
                        reset();
                        return 0;
                    }
                    break;
                }
            }
        }
    }
    while (n>=2)
    {
        last_key=key;
        key=get_key();
            
            switch (key)
            {
             case('0'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
             }
             case('1'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('2'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('3'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('4'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('5'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('6'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('7'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('8'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);
                 break;
                 break;
             }
             case('9'):
             {
                 USART_send(0xD);       
                 USART_send(0xA);
                 textout("Digits at maximum");
                 USART_send(0xD);       
                 USART_send(0xA);
                 numout(num);                 
                 break;
             }
             case('+'):
             {      
                
                switch(ab)
                {
                    case(0):
                    {
                    decflag=0;
                    return num;
                    break;
                    }
                    case(1):
                    {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Function already chosen, to change please reset");
                    USART_send(0xD);       
                    USART_send(0xA);
                    numout(num);
                    break;
                    }
                }
                break;
             }
             case('-'):
             { 
               switch(ab)
                {
                    case(0):
                    {
                    decflag=0;
                    return num;
                    break;
                    }
                    case(1):
                    {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Function already chosen, to change please reset");
                    USART_send(0xD);       
                    USART_send(0xA);
                    numout(num);
                    break;
                    }
                }
                break;
             }
             case('*'):
             {  
                switch(ab)
                {
                    case(0):
                    {
                    decflag=0;
                    return num;
                    break;
                    }
                    case(1):
                    {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Function already chosen, to change please reset");
                    USART_send(0xD);       
                    USART_send(0xA);
                    numout(num);
                    break;
                    }
                }
                break;
             }
             case('/'):
             {      
                switch(ab)
                {
                    case(0):
                    {
                    decflag=0;
                    return num;
                    break;
                    }
                    case(1):
                    {
                    USART_send(0xD);       
                    USART_send(0xA);
                    textout("Function already chosen, to change please reset");
                    USART_send(0xD);       
                    USART_send(0xA);
                    numout(num);
                    break;
                    }
                }
                break;
             }
             case('.'):
             {      
                 if(decflag==0)
                { 
                  decflag=1;
                  return num;
                  deccount++;
                  break;
                 }
                 else if(decflag==1)
                 {
                     USART_send(0xD);       
                     USART_send(0xA);
                     textout("Already editing decimal");
                     USART_send(0xD);       
                     USART_send(0xA);
                     numout(num);
                     deccount++;
                     break;
                 }
                if (deccount==2)
                {
                    reset();
                    return 0;
                }
             }
             case('='):
             {      
                return num;
                decflag=0;
                break;
             }
            }
    }
    return 0;
}

int result()
{
    int carry, tempa, tempb, tempr, x,y,z;
    carry=0;
    x=0;
    y=0;
    z=0;
    tempa=0;
    tempb=0;
    tempr=0;
    float temprf;
    
    switch(oper)
    {
        case'+':
        {
            Rdec=Adec+Bdec;
            while(Rdec>100)
            {
                Rdec=Rdec-100;
                carry=carry+1;
            }
            Rwhole=Awhole+Bwhole+carry;
            USART_send(0xD);       
            USART_send(0xA);
            USART_send(0xD);       
            USART_send(0xA);
            return 0;
        }
        case'-':
        {
            Rdec=Adec-Bdec;
            while(Rdec<0)
            {
                Rdec=Rdec+100;
                carry=carry+1;
            }
            Rwhole=Awhole-Bwhole-carry;
            USART_send(0xD);       
            USART_send(0xA);
            USART_send(0xD);       
            USART_send(0xA);
            return 0;
        }
        case'*':
        {
            Rdec=(Awhole*Adec+Awhole*Bdec+Adec*Bdec);
            if(Awhole==0 && Bwhole==0)
            {
                
                temprf=Rdec/100;
                Rdec=round(temprf);
            }
            while(Rdec>100)
            {
                Rdec=Rdec-100;
                carry=carry+1;
            }
            Rwhole=Awhole*Bwhole+carry;
            USART_send(0xD);       
            USART_send(0xA);
            USART_send(0xD);       
            USART_send(0xA);
            return 0;
        }
        case'/':
        {
            tempa=Awhole*100+Adec;//combines both parts of a
            tempb=Bwhole*100+Bdec;//combines both parts of b
            remain=tempa%tempb;//Gives Remainder
            remain=remain/100;
            temprf=tempa/tempb;//divides a by b
            temprf=temprf*100;
            
            tempr=round(temprf);//rounds to the nearest integer, also makes the value an integer again

            if(tempr/100!=0)
            {
            while(x<2)
            {
                z=y;            
                y=tempr%10;//splits last digit from integer
                tempr=tempr-y;
                tempr=tempr/10;
                x++;
                }
            }
            Rdec=y*10+z;//recombines decimal parts of the result
            Rwhole=tempr;//should be the whole number part of the result
            USART_send(0xD);       
            USART_send(0xA);
            USART_send(0xD);       
            USART_send(0xA);
            textout("For a premium version of division as well as other add-on features, a $10 monthly subscription can be purchased at web.mst.edu/rdua/");
            USART_send(0xD);       
            USART_send(0xA);
            
            return 0;
            
        }
    }
    
    
    return 0;
}

void textout ( const char *str)
{
    while(*str) USART_send(*str++);
}

void numout(int num)
{
    char NumStr[42];
    itoa(num,NumStr, 10);
    textout(NumStr);
}

int switch_num(char key, int num)
{
    num=num*10+(key-'0');          
    return num;
}

int reset()
{
    decflag=0;
    Awhole=0;
    Adec=0;
    Bwhole=0;
    Bdec=0;
    Rwhole=0;
    Rdec=0; 
    ab=0;
    key='a';
    oper='+';
    last_key='r';
    USART_send(0xD);       
    USART_send(0xA);    
    textout("System variables have been reset");
    USART_send(0xD);       
    USART_send(0xA);
    USART_send(0xD);       
    USART_send(0xA);
    
    return 0;
    
}
