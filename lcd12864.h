//lcd12864.h 
#ifndef   _LCD12864_H_
#define   _LCD12864_H_
/**************************************************************
               iO�ں궨����
***************************************************************/
sbit CS =P0^0;//��Ӧʵ������IO1 RS
sbit SID=P0^1;//��Ӧʵ������IO2 RW
sbit SCK=P0^2;//��Ӧʵ������IO3 E
 
void write_command( unsigned char Cbyte );   //д��ָ���
void write_data( unsigned char Dbyte );  //д��ָ������
void lcd_init( void );   //��ʾ����ʼ��
void lcd_clear_txt( void );  //��ʾ������
void location_xy_12864(unsigned char x,unsigned char y);
void put_str(unsigned char row,unsigned char col,unsigned char *puts);
void put_char(unsigned char row,unsigned char col,unsigned char put);
void lcd_display_picture(unsigned char p[][16]);
void clear_graph();
void lcd_bar();
#endif
