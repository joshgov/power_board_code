#include "sci.h"

void init_SCI(void){
    
    SCIBDH = SCIBDH_INIT;
    SCIBDL = SCIBDL_INIT;
    SCIC1 = SCIC1_INIT;
    SCIC2 = SCIC2_INIT;
}

void SCI_send_byte(uint8_t c)                
{
	while(!SCIS1_TDRE);
	SCID = c;
}

void SCI_send_byte_signed(int8_t c)
{
    while(!SCIS1_TDRE);
    SCID = c;
}

char nibble2asc(uint8_t n)
    {
    return (char)((n < 0xA)? (n + '0') : (n + ('A' - 0xA)));
    }

// char print_signed(int16_t){
//     if(n < 0xA){
//         n + '0'; // print numbers
//     }

//     if(n > 0xA){
//         (n + ('A' - 0xA));
//     }
// }

void printhexbyte( uint8_t n )
	{
    SCI_send_byte( nibble2asc( n >> 4  ) );  
    SCI_send_byte( nibble2asc( n & 0xf ) );    
	}

void printhexbyte_signed(int8_t n){
    SCI_send_byte(n >> 4);
    SCI_send_byte( n & 0xf);
}

void printhexword_negative(int16_t n){
    SCI_send_byte(0x2D);
    n = -n;
    printhexword(n);
}

void printhexword_signed(int16_t n){
    printhexbyte_signed( ((int8_t*)&n)[0] );
    printhexbyte_signed( ((int8_t*)&n)[1] );
}

void printhexword( uint16_t n )
	{
   	printhexbyte( ((byte*)&n)[0] );
    printhexbyte( ((byte*)&n)[1] );
	}

void printhex2word( uint32_t n)
{
    printhexbyte( ((uint8_t*)&n)[0] );
    printhexbyte( ((uint8_t*)&n)[1] );
    printhexbyte( ((uint8_t*)&n)[2] );
    printhexbyte( ((uint8_t*)&n)[3] );
}
void prints(char const * s)                         
    {
    for(;*s;s++)
        SCI_send_byte(*s);
    }