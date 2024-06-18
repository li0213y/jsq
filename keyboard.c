#include "reg52.h"
#include "keyboard.h"

#define Key   P1	   

code unsigned char KeyValueL[]={0xf7,0xfb,0xfd,0xfe};
code unsigned char KeyValueH[]={0x70,0xb0,0xd0,0xe0};
//code unsigned char KeyValueH[]={0xf7,0xfb,0xfd,0xfe};
//code unsigned char KeyValueL[]={0x70,0xb0,0xd0,0xe0};

void KeyDelay(unsigned char xms)
{
  unsigned char i;
  while(xms--)
  {
    for(i=0; i<120; i++);
  }
}
unsigned char keycan(void)	 //°´¼üÉ¨Ãè
{
  unsigned char i,j;
  unsigned char tmp;
  Key=0xf0;
  if(Key!=0xf0)
  {
    KeyDelay(10);
    if(Key!=0xf0)
    {
      for(i=0; i<4; i++)
      {
        Key=KeyValueL[i];
        for(j=0; j<4; j++)
        {
          if((Key&0xf0)==KeyValueH[j])
          {
            tmp=i+j*4;
            while(Key!=KeyValueL[i]);
            return tmp;
          }
        }
      }
    }
  }
  return 0xff;
}
