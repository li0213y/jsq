#include "reg52.h"
#include "string.h"
#include "stdio.h"
#include "lcd1602.h"
#include "keyboard.h"

#define keydeng		14
#define keyzuoyi    12
long memory = 0; // ���ڴ洢�ڴ��е�ֵ
sbit k1=P3^0;	 //?����?P31?����?k1
sbit k2=P3^1;	 //?����?P31?����?k1
sbit k3=P3^2;	 //?����?P31?����?k1
code unsigned char KeyDisPlaybuf[16] = 
{
    '1', '2', '3', '+',\
    '4', '5', '6', '-',\ 
    '7', '8', '9', '*',\
    '<', '0', '=', '/'
};

long Count(char *ch, char len)//�򵥼����� ��֧�ּӼ��˳�+-*/
{
    unsigned char i = 0;
    char Operation;
    long Num1=0,Num2=0;
    while(i<len && ch[i]!='+' && ch[i]!='-' && ch[i]!='*' && ch[i]!='/')	 //��������� ��ֵ1
    {
        Num1*=10;
        Num1+=ch[i]-'0';
        i++;
    }
    Operation=ch[i++];
    while(i<len && ch[i]!='=')	   //��������� ��ֵ2
    {
        Num2*=10;
        Num2+=ch[i]-'0';
        i++;
    }
    switch(Operation)	   //����
    {
        case'+':return Num1+Num2;//�ӷ�
        case'-':return Num1-Num2;//����
        case'*':return Num1*Num2;//�˷�
        case'/':return Num1/Num2;//����
    }
	return 0;
}

void EasyCounter(void)  
{  
    unsigned char buff[30];  
    unsigned char text[30] = {0}; // ��ʼ��text����Ϊȫ0  
    unsigned char key;  
    unsigned char i = 0;  
    long tempResult; // ������ʱ�洢������  
  
    do  
    {  
        key = keycan(); // ��ȡ��ֵ  
  
        if (key >= 0 && key < K1) // ��������ּ����������  
        {  
            text[i++] = KeyDisPlaybuf[key]; // ��¼��ֵ��Ӧ���ַ�  
            text[i] = '\0'; // ȷ���ַ�����null��β  
            LCD_ShowStr(0, 0, "                "); // �����һ����ʾ  
            LCD_ShowStr(k1 - strlen(text), 0, text); // ��һ����ʾ�ַ�  
        }  
        else if (key == keyzuoyi && i > 0) // ����"<"ʱɾ��һλ  
        {  
            i--; // ע������ֻ������������ɾ��ʵ�ʵ��ַ�����Ϊ�ַ�����null��β��  
            text[i] = '\0'; // ȷ���ַ�����null��β  
            LCD_ShowStr(0, 0, "                "); // �����һ����ʾ  
            LCD_ShowStr(k1 - strlen(text), 0, text); // ������ʾ�ַ�  
        }  
        else if (key == k1) // k1: M+  
        {  
            memory += Count(text, strlen(text)); // ����ǰ���ʽ��ֵ�ӵ��ڴ���  
            LCD_ShowStr(0, 1, "                "); // ����ڶ�����ʾ  
            sprintf(buff, "M: %ld", memory); // ��ʽ���ڴ��е�ֵ��buff  
            LCD_ShowStr(0, 1, buff); // �ڶ�����ʾ�ڴ��е�ֵ  
            // ����text��׼����һ������  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
        else if (key == k2) // k2: M-  
        {  
            tempResult = Count(text, strlen(text)); // ���㵱ǰ���ʽ��ֵ  
            if (tempResult < abs(memory)) // ��ֹ�ڴ�ֵ��Ϊ�������򵥴���  
            {  
                memory -= tempResult; // ���ڴ��м�ȥ��ǰ���ʽ��ֵ  
            }  
            else  
            {  
                memory = 0; // �����ǰֵ�����ڴ�ֵ���������ڴ�Ϊ0  
            }  
            LCD_ShowStr(0, 1, "                "); // ����ڶ�����ʾ  
            sprintf(buff, "M: %ld", memory); // ��ʽ���ڴ��е�ֵ��buff  
            LCD_ShowStr(0, 1, buff); // �ڶ�����ʾ�ڴ��е�ֵ  
            // ����text��׼����һ������  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
        else if (key == k3) // k3: RM  
        {  
            memory = 0; // ����ڴ��е�ֵ  
            LCD_ShowStr(0, 1, "                "); // ����ڶ�����ʾ  
            LCD_ShowStr(0, 1, "M: 0"); // ��ʾ�ڴ������  
            // ����text��׼����һ������  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
    } while (key != keydeng); // ����'='ʱ�������벢���м���  
  
    // ����������ʾ  
    tempResult = Count(text, strlen(text));  
    sprintf(buff, "%ld", tempResult);  
    LCD_ShowStr(0, 1, "                "); // ����ڶ�����ʾ  
    LCD_ShowStr(k1 - strlen(buff), 1, buff); // �ڶ�����ʾ������  
}
void main(void)	//������
{
  LCD_Init();
  while(1)
  {
    EasyCounter();	//����ɨ��+����+��ʾ
  }
}