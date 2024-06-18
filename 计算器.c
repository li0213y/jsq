#include "reg52.h"
#include "string.h"
#include "stdio.h"
#include "lcd1602.h"
#include "keyboard.h"

#define keydeng		14
#define keyzuoyi    12
long memory = 0; // 用于存储内存中的值
sbit k1=P3^0;	 //?¨ò?P31?úê?k1
sbit k2=P3^1;	 //?¨ò?P31?úê?k1
sbit k3=P3^2;	 //?¨ò?P31?úê?k1
code unsigned char KeyDisPlaybuf[16] = 
{
    '1', '2', '3', '+',\
    '4', '5', '6', '-',\ 
    '7', '8', '9', '*',\
    '<', '0', '=', '/'
};

long Count(char *ch, char len)//简单计算器 仅支持加减乘除+-*/
{
    unsigned char i = 0;
    char Operation;
    long Num1=0,Num2=0;
    while(i<len && ch[i]!='+' && ch[i]!='-' && ch[i]!='*' && ch[i]!='/')	 //输入的数据 数值1
    {
        Num1*=10;
        Num1+=ch[i]-'0';
        i++;
    }
    Operation=ch[i++];
    while(i<len && ch[i]!='=')	   //输入的数据 数值2
    {
        Num2*=10;
        Num2+=ch[i]-'0';
        i++;
    }
    switch(Operation)	   //计算
    {
        case'+':return Num1+Num2;//加法
        case'-':return Num1-Num2;//减法
        case'*':return Num1*Num2;//乘法
        case'/':return Num1/Num2;//除法
    }
	return 0;
}

void EasyCounter(void)  
{  
    unsigned char buff[30];  
    unsigned char text[30] = {0}; // 初始化text数组为全0  
    unsigned char key;  
    unsigned char i = 0;  
    long tempResult; // 用于临时存储计算结果  
  
    do  
    {  
        key = keycan(); // 读取键值  
  
        if (key >= 0 && key < K1) // 如果是数字键或运算符键  
        {  
            text[i++] = KeyDisPlaybuf[key]; // 记录键值对应的字符  
            text[i] = '\0'; // 确保字符串以null结尾  
            LCD_ShowStr(0, 0, "                "); // 清除第一行显示  
            LCD_ShowStr(k1 - strlen(text), 0, text); // 第一行显示字符  
        }  
        else if (key == keyzuoyi && i > 0) // 按下"<"时删除一位  
        {  
            i--; // 注意这里只减少索引，不删除实际的字符（因为字符串以null结尾）  
            text[i] = '\0'; // 确保字符串以null结尾  
            LCD_ShowStr(0, 0, "                "); // 清除第一行显示  
            LCD_ShowStr(k1 - strlen(text), 0, text); // 重新显示字符  
        }  
        else if (key == k1) // k1: M+  
        {  
            memory += Count(text, strlen(text)); // 将当前表达式的值加到内存中  
            LCD_ShowStr(0, 1, "                "); // 清除第二行显示  
            sprintf(buff, "M: %ld", memory); // 格式化内存中的值到buff  
            LCD_ShowStr(0, 1, buff); // 第二行显示内存中的值  
            // 重置text以准备下一次输入  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
        else if (key == k2) // k2: M-  
        {  
            tempResult = Count(text, strlen(text)); // 计算当前表达式的值  
            if (tempResult < abs(memory)) // 防止内存值变为负数（简单处理）  
            {  
                memory -= tempResult; // 从内存中减去当前表达式的值  
            }  
            else  
            {  
                memory = 0; // 如果当前值大于内存值，则设置内存为0  
            }  
            LCD_ShowStr(0, 1, "                "); // 清除第二行显示  
            sprintf(buff, "M: %ld", memory); // 格式化内存中的值到buff  
            LCD_ShowStr(0, 1, buff); // 第二行显示内存中的值  
            // 重置text以准备下一次输入  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
        else if (key == k3) // k3: RM  
        {  
            memory = 0; // 清除内存中的值  
            LCD_ShowStr(0, 1, "                "); // 清除第二行显示  
            LCD_ShowStr(0, 1, "M: 0"); // 显示内存已清除  
            // 重置text以准备下一次输入  
            memset(text, 0, sizeof(text));  
            i = 0;  
        }  
    } while (key != keydeng); // 按下'='时结束输入并进行计算  
  
    // 计算结果并显示  
    tempResult = Count(text, strlen(text));  
    sprintf(buff, "%ld", tempResult);  
    LCD_ShowStr(0, 1, "                "); // 清除第二行显示  
    LCD_ShowStr(k1 - strlen(buff), 1, buff); // 第二行显示计算结果  
}
void main(void)	//主函数
{
  LCD_Init();
  while(1)
  {
    EasyCounter();	//按键扫描+计算+显示
  }
}