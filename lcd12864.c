//lcd12864.c
#include <reg51.h>
#include <intrins.h>
#include "lcd12864.h"
/**************************************************************
 
 
      //���з�ʽ����
/*******************************************************************
              ����������
********************************************************************/      
unsigned char code AC_TABLE[]={                   //�������
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
};
/****************************************************************                        
              ����һ���ֽ�
*****************************************************************/
void send_byte(unsigned char dbyte)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    SCK = 0;
    dbyte=dbyte<<1;
    SID = CY;
    SCK = 1;
    SCK = 0;
  }
}
/**********************************************************
              ����һ���ֽ�
***********************************************************/
 
unsigned char receive_byte(void)
{
  unsigned char i,temp1,temp2;
  temp1=temp2=0;
  for(i=0;i<8;i++)
  {
    temp1=temp1<<1;
    SCK = 0;
    SCK = 1;                
    SCK = 0;
    if(SID) temp1++;
  }
  for(i=0;i<8;i++)
  {
    temp2=temp2<<1;
    SCK = 0;
    SCK = 1;
    SCK = 0;
    if(SID) temp2++;
  }
  return ((0xf0&temp1)+(0x0f&temp2));
}
/****************************************************************
                      ���æ״̬
******************************************************************/
void check_busy( void )
{
  do send_byte(0xfc);     //11111,RW(1),RS(0),0
  while(0x80&receive_byte());
}
 
/******************************************************************
           дһ���ֽڵ�ָ��
*******************************************************************/
void write_command( unsigned char Cbyte )
{
  CS = 1;
  check_busy();
  send_byte(0xf8);          //11111,RW(0),RS(0),0
  send_byte(0xf0&Cbyte);
  send_byte(0xf0&Cbyte<<4);
  CS = 0;
}
/*************************************************************
                 дһ���ֽڵ�����
**************************************************************/
void write_data( unsigned char dbyte )
{
  CS = 1;
  check_busy();
  send_byte(0xfa);          //11111,RW(0),RS(1),0
  send_byte(0xf0&dbyte);
  send_byte(0xf0&dbyte<<4);
  CS = 0;
}
 
/******************************************************************
                         lcd��ʼ������
*******************************************************************/
void lcd_init( void )
{
     write_command(0x30);
     write_command(0x03);
     write_command(0x0c);
     write_command(0x01);
     write_command(0x06);
}
 
/*******************************************************************************************************
                                 �趨��꺯��
********************************************************************************************************/
void location_xy_12864(unsigned char x,unsigned char y)
{
  switch(x)
  {
    case 0:
      x=0x80;break;
    case 1:
      x=0x90;break;
    case 2:
      x=0x88;break;
    case 3:
      x=0x98;break;
    default:
      x=0x80;
  }
  y=y&0x07;
  write_command(0x30);
  write_command(y+x);
  write_command(y+x);
 
}
/***********************************************************************************
                  ����ı�
************************************************************************************/
void lcd_clear_txt( void )
{
  unsigned char i;
  write_command(0x30);
  write_command(0x80);
  for(i=0;i<64;i++)
  write_data(0x20);
  location_xy_12864(0,0);      
}
/****************************************************************************************
                      ��ʾ�ַ���
*****************************************************************************************/
void put_str(unsigned char row,unsigned char col,unsigned char *puts)
{    
  write_command(0x30);
  write_command(AC_TABLE[8*row+col]);
  while(*puts != '\0')
  {

    write_data(*puts);
    puts++;

}
 
/****************************************************************************************
                      ��ʾ�ַ�
*****************************************************************************************/
void put_char(unsigned char row,unsigned char col,unsigned char put)
{    
  write_command(0x30);
  write_command(AC_TABLE[8*row+col]);
  write_data(put);
}
 
/****************************************************************************************
                      ��ʾͼƬ
*****************************************************************************************/
void lcd_display_picture(unsigned char p[][16])
{
    unsigned char x,y,a,b,c;
    write_command(0x34);    
    x = 0x80;
    y = 0x80;
    for(c=0;c<2;c++)//�Ȼ��ϰ������ٻ��°���
    {
         for(a=0;a<32;a++)
        {
            write_command(y+a);
            write_command(x);
            for(b=0;b<16;b++)
                write_data(p[a+c*32][b]);
        }
        x=0x88;
    }

    write_command(0x36);    
    write_command(0x30);    
}

void lcd_display_rectangle(unsigned char p[][16])
{
    unsigned char x,y,a,b,c;
    write_command(0x34);    
    x = 0x80;
    y = 0x80;
    for(c=0;c<2;c++)//�Ȼ��ϰ������ٻ��°���
    {
         for(a=0;a<32;a++)
        {
            write_command(y+a);
            write_command(x);
            //for(b=0;b<16;b++)
            write_data(0x80);
        }
        x=0x88;
    }
     write_command(0x80 | 0x1f);
	write_command(0x80 | 8);
	for(c=0; c<8; ++c){
write_data(0xff); //����ȫ������
write_data(0xff); //д�������ֽڣ�X ��ַ�Զ���1
}
    write_command(0x80);
	write_command(0x80);
	for(c=0; c<8; ++c){
write_data(0xff); //����ȫ������
write_data(0xff); //д�������ֽڣ�X ��ַ�Զ���1
}
    write_command(0x36);    
    write_command(0x30);    

}

void clear_graph(){
    unsigned char x,y,a,b,c;
    write_command(0x34);    
    x = 0x80;
    y = 0x80;
         for(a=0;a<32;a++)
        {
            write_command(y+a);
            write_command(x);
	for(b=0;b<8;b++){
	write_data(0x20);
	write_data(0x20);
	}
         }
    x = 0x88;
    y = 0x80;
         for(a=0;a<32;a++)
        {
            write_command(y+a);
            write_command(x);
	for(b=0;b<8;b++){
	write_data(0x20);
	write_data(0x20);
	}
         }
    write_command(0x36);    
    write_command(0x30);    
}
void lcd_bar(){
	unsigned char x,y,a,b,c;
	c=current_speed_val/4;
    write_command(0x34);    
    x = 0x9d;
    y = 0x80;
    a=30;
    while(a>=0&&c>0){
    	write_command(y+a);
        write_command(x);
        write_command(0x80);
	write_command(0x80);

        a--;
        c--;
	}
	a=31;
	if(c){
		x = 0x8d;
		while(c>0){
			write_command(y+a);
        write_command(x);
        write_command(0x80);
	write_command(0x80);

	c--;
	a--;
		}
	}
    write_command(0x36);    
    write_command(0x30);   
	
}
