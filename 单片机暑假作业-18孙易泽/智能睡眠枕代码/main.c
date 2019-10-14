/*******************˵��:**************************
����˯������� 18���ݿ�ѧ������
**************************************************/

#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int
uchar x,y,z,n,m,a=0;
//ѹ������������
uint num=0x1F;
uint c=0x1F;
uint b,d;
//˯�߼�¼����
uint e,f,g,h;
uint deep=0;
uchar condition[60]={0};
//����ʱ���������
int time_hour_1=0;
int time_hour_2=7;
int time_min_1=5;
int time_min_2=0;
//��������ʱ����
uint time;
//LCD1602���Ŷ���
sbit RS=P3^5;
sbit RW=P3^6;
sbit CS=P3^7;
#define LCDDATA P0
//DS1302���Ŷ���
sbit RST=P1^5;
sbit IO=P1^6;
sbit SCK=P1^7;
//4���������������Ŷ���
sbit key1=P2^7;
sbit key2=P2^6;
sbit key3=P2^5;
sbit key4=P2^4;
//���������Ŷ���
sbit buzz=P2^0;
//ѹ����������fsr�����Ŷ���
sbit f1=P1^0;
sbit f2=P1^1;
sbit f3=P1^2;
sbit f4=P1^3;
sbit f5=P1^4;
//DS1302��ַ����
#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe
//ʱ�����鶨��
uchar time_buf[8] = {0};
uchar dis_time_buf[16]={0};

/*****************
��ʱ����
*****************/

//1ms��ʱ
void Delay_xms(uint x)
{
  uint i,j;
  for(i=0;i<x;i++)
    for(j=0;j<112;j++);
}
//12us��ʱ
void Delay_xus(uint t)	  		 		
{ 
  for(;t>0;t--)
   {
	 _nop_();
   }
}

/*****************
LCD����
*****************/

//����LCDдʱ��
void LCD_en_write(void)       
{
   CS=1;    
    Delay_xus(20);
   CS=0;   
	Delay_xus(20);
} 
//дָ���
void Write_Instruction(uchar command)
{
  RS=0;
  RW=0;
  CS=1;
  LCDDATA=command;
  LCD_en_write();//д��ָ������
}
//д���ݺ���
void Write_Data(uchar Wdata)
{
  RS=1;
  RW=0;
  CS=1;
  LCDDATA=Wdata;
  LCD_en_write();//д������
}
//�ַ���ʾ��ʼ��ַ����
void LCD_SET_XY(uchar X,uchar Y)
{
  uchar address;
  if(Y==0)
    address=0x80+X;//Y=0,��ʾ�ڵ�һ����ʾ����ַ����Ϊ0x80
  else 
    address=0xc0+X;//Y��0ʱ����ʱ�ڵڶ�����ʾ����ַ����Ϊ0xC0
  Write_Instruction(address);//дָ�������ʾ��ʼ��ַ
}
//�ڵ�X��Y�п�ʼ��ʾWdata����Ӧ�ĵ����ַ�
void LCD_write_char(uchar X,uchar Y,uchar Wdata)
{
  LCD_SET_XY(X,Y);//д��ַ
  Write_Data(Wdata);//д�뵱ǰ�ַ�����ʾ
}
//LCD��������
void LCD_clear(void)
{
  Write_Instruction(0x01);
  Delay_xms(5);
}
//��ʾ����ʼ������
void LCD_init(void) 
{	
	Write_Instruction(0x38);
	Delay_xms(5);
	Write_Instruction(0x38);	
	Delay_xms(5);
	Write_Instruction(0x38);	

	Write_Instruction(0x08);	//����ʾ�����Թ�꣬��겻��˸
	Write_Instruction(0x01);	//����
	Delay_xms(5);
	
	Write_Instruction(0x04);	//дһ�ַ���������ʾ���ƶ�
	Delay_xms(5);
	Write_Instruction(0x0C);	//����ʾ����ꡢ��˸���ر�
}
//�������ý���ʱ��ʮλ
void LCD_hour_write_1(void)
{
	if(time_hour_1<0)
	{
		time_hour_1+=3;
	}
	if(0<=time_hour_1<3)
	{
		LCD_write_char(11,1,time_hour_1+'0');
	}
	if(time_hour_1>=3)
	{
		time_hour_1-=3;
		LCD_write_char(11,1,'0');
	}
}
//�������ý���ʱ�ĸ�λ
void LCD_hour_write_2(void)
{
	if(time_hour_2<0)
	{
		if(time_hour_1==0)
		{
			time_hour_1=2;
			time_hour_2=3;
		}
		else
		{	
			time_hour_2+=10;
			time_hour_1--;
		}
	}
	if(0<=time_hour_2<10)
	{
		if(time_hour_1==2)
		{
			if(time_hour_2==4)
			{
				time_hour_1=0;
				time_hour_2=0;
				LCD_write_char(12,1,time_hour_2+'0');
			}
			else
			{
				LCD_write_char(12,1,time_hour_2+'0');
			}
		}
		else
		{
			LCD_write_char(12,1,time_hour_2+'0');
		}
	}
	if(time_hour_2>=10)
	{
		time_hour_2-=10;
		time_hour_1++;
		LCD_write_char(12,1,'0');
	}
}

//�������ý�����ӵ�ʮλ
void LCD_min_write_1(void)
{
	if(time_min_1<0)
	{
		time_min_1+=6;
	}
	if(0<=time_min_1<6)
	{
		LCD_write_char(14,1,time_min_1+'0');
	}
	if(time_min_1>=6)
	{
		time_min_1-=6;
		LCD_write_char(14,1,'0');
	}
}
//�������ý�����ӵĸ�λ
void LCD_min_write_2(void)
{
	if(time_min_2<0)
	{
		time_min_2+=10;
	}
	if(0<=time_min_2<10)
	{
		LCD_write_char(15,1,time_min_2+'0');
	}
	if(time_min_2>=10)
	{
		time_min_2-=10;
		LCD_write_char(15,1,'0');
	}
}

/************************
DS1302ʱ��ģ�����ô���
************************/

//DS1302��ʼ������
void ds1302_init(void) 
{
	RST=0;			//RST���õ�
	SCK=0;			//SCK���õ�
}
//��DS1302����һ�ֽ�����
uchar ds1302_read_byte(uchar addr) {

	uchar i,temp;	
	RST=1;					//����DS1302����
	//д��Ŀ���ַ��addr
	addr = addr | 0x01;    //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			IO=1;
			}
		else {
			IO=0;
			}
		SCK=1;
		SCK=0;
		addr = addr >> 1;
		}	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (IO) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		SCK=1;
		SCK=0;
		}	
	RST=0;					//ֹͣDS1302����
	return temp;
}

//��DS302����ʱ������
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//�� 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//�� 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//�� 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//ʱ 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//�� 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//�룬������ĵ�7λ�����ⳬ��59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//�� 	
}

/************************
������������
************************/
//����������ʼ��
void scanstart(void)
{
	key1=1;
	key2=1;
	key3=1;
	key4=1;
}
//����ɨ��
void scan(void)
{
	if(key1==0||key2==0||key3==0||key4==0)
	{
		Delay_xms(10);
		if(key1==0||key2==0||key3==0||key4==0)
		{
			if(key1==0)
			{
				x=1;
			}
			if(key2==0)
			{
				y=1;
			}
			if(key3==0)
			{
				z=1;
			}
			if(key4==0)
			{
				n=1;
			}
		}
	}
}

/*********************
fsrѹ��������
*********************/

//ѹ�����������ɨ��
void fsr()
{
	b=P1&num;
	if(b!=c)
	{
		c=b;
		d++;
	}
}

/*********************
LCD��ʾ����
*********************/
//Ĭ����ʾ����
void Display1(void)
{
   
   LCD_write_char(1,0,dis_time_buf[4]+'0');
   LCD_write_char(2,0,dis_time_buf[5]+'0');
   LCD_write_char(3,0,'/');
   LCD_write_char(4,0,dis_time_buf[6]+'0');
   LCD_write_char(5,0,dis_time_buf[7]+'0');
   LCD_write_char(7,0,dis_time_buf[8]+'0');
   LCD_write_char(8,0,dis_time_buf[9]+'0');
	 LCD_write_char(9,0,':');
   LCD_write_char(10,0,dis_time_buf[10]+'0');
   LCD_write_char(11,0,dis_time_buf[11]+'0');
	 LCD_write_char(12,0,':');
	 LCD_write_char(13,0,dis_time_buf[12]+'0');
   LCD_write_char(14,0,dis_time_buf[13]+'0');
   //��2����ʾ  
   LCD_write_char(0,1,'a');
	 LCD_write_char(1,1,'l');
	 LCD_write_char(2,1,'a');
	 LCD_write_char(3,1,'r');
	 LCD_write_char(4,1,'m');
	 LCD_write_char(5,1,'|');
	 LCD_write_char(10,1,'|');
	 LCD_write_char(11,1,'s');
	 LCD_write_char(12,1,'t');
	 LCD_write_char(13,1,'a');
	 LCD_write_char(14,1,'r');
	 LCD_write_char(15,1,'t');
}
//�������ý���
void Display2(void)
{
	LCD_write_char(0,0,'s');
	LCD_write_char(1,0,'e');
	LCD_write_char(2,0,'t');
	LCD_write_char(3,0,'t');
	LCD_write_char(4,0,'i');
	LCD_write_char(5,0,'n');
	LCD_write_char(6,0,'g');
	LCD_write_char(8,0,'a');
	LCD_write_char(9,0,'l');
	LCD_write_char(10,0,'a');
	LCD_write_char(11,0,'r');
	LCD_write_char(12,0,'m');
	LCD_write_char(13,0,':');
	LCD_write_char(13,1,':');

	LCD_hour_write_2();
	LCD_hour_write_1(); //Сʱ����ʾ
	LCD_min_write_1(); //�ֵ�ʮλ��ʾ
	LCD_min_write_2(); //�ֵĸ�λ��ʾ
	
}
//˯�߽���
void Display3(void)
{
	LCD_write_char(3,0,'g');
	LCD_write_char(4,0,'o');
	LCD_write_char(5,0,'o');
	LCD_write_char(6,0,'d');
	LCD_write_char(8,0,'n');
	LCD_write_char(9,0,'i');
	LCD_write_char(10,0,'g');
	LCD_write_char(11,0,'h');
	LCD_write_char(12,0,'t');
	LCD_write_char(13,0,'!');
	
	LCD_write_char(11,1,'|');
	LCD_write_char(12,1,'s');
	LCD_write_char(13,1,'t');
	LCD_write_char(14,1,'o');
	LCD_write_char(15,1,'p');

}
//���ӽ��ѽ���
void Display4(void)
{
	LCD_write_char(2,0,'g');
	LCD_write_char(3,0,'o');
	LCD_write_char(4,0,'o');
	LCD_write_char(5,0,'d');
	LCD_write_char(7,0,'m');
	LCD_write_char(8,0,'o');
	LCD_write_char(9,0,'r');
	LCD_write_char(10,0,'n');
	LCD_write_char(11,0,'i');
	LCD_write_char(12,0,'n');
	LCD_write_char(13,0,'g');
	LCD_write_char(14,0,'!');
	
	LCD_write_char(11,1,'|');
	LCD_write_char(12,1,'s');
	LCD_write_char(13,1,'t');
	LCD_write_char(14,1,'o');
	LCD_write_char(15,1,'p');
}
//˯��״���鿴����
void Display5(void)
{
	LCD_write_char(0,0,'s');
	LCD_write_char(1,0,'l');
	LCD_write_char(2,0,'e');
	LCD_write_char(3,0,'e');
	LCD_write_char(4,0,'p');
	LCD_write_char(6,0,'h');
	LCD_write_char(7,0,'o');
	LCD_write_char(8,0,'u');
	LCD_write_char(9,0,'r');
	LCD_write_char(10,0,':');
	LCD_write_char(11,0,f/60+'0');
	LCD_write_char(12,0,f/6+'0');
	LCD_write_char(13,0,':');
	LCD_write_char(14,0,f%6+'0');
	LCD_write_char(15,0,'0');
	
	LCD_write_char(0,1,'d');
	LCD_write_char(1,1,'e');
	LCD_write_char(2,1,'e');
	LCD_write_char(3,1,'p');
	LCD_write_char(4,1,':');
	LCD_write_char(5,1,deep/60+'0');
	LCD_write_char(6,1,deep/6+'0');
	LCD_write_char(7,1,':');
	LCD_write_char(8,1,deep%6+'0');
	LCD_write_char(9,1,'0');
}
//�������ͽ���
void Display6(void)
{
	LCD_write_char(2,0,'s');
	LCD_write_char(3,0,'e');
	LCD_write_char(4,0,'n');
	LCD_write_char(5,0,'d');
	LCD_write_char(7,0,'m');
	LCD_write_char(8,0,'e');
	LCD_write_char(9,0,'s');
	LCD_write_char(10,0,'s');
	LCD_write_char(11,0,'a');
	LCD_write_char(12,0,'g');
	LCD_write_char(13,0,'e');
}
//�������ͳɹ�����
void Display7(void)
{	
	LCD_write_char(9,1,'s');
	LCD_write_char(10,1,'u');
	LCD_write_char(11,1,'c');
	LCD_write_char(12,1,'c');
	LCD_write_char(13,1,'e');
	LCD_write_char(14,1,'s');
	LCD_write_char(15,1,'s');
}


/****************
����������
****************/
void buzzer()
{
		 for(time=0;time<10000;time++);
		 buzz=1;
		 for(time=0;time<10000;time++);
		 buzz=0;
		 for(time=0;time<10000;time++);
		 buzz=1;
		 for(time=0;time<10000;time++);
		 buzz=0;
		 for(time=0;time<10000;time++);
		 buzz=1;
		 for(time=0;time<10000;time++);
		 buzz=0;
		 for(time=0;time<10000;time++);
		 buzz=1;
		 for(time=0;time<10000;time++);
		 buzz=0;
		 for(time=0;time<10000;time++);
		 buzz=1;
		 for(time=0;time<10000;time++);
}	
/******************
��������
******************/
//�������ݷ��ͺ���
void sendDate(char date)
{
    SBUF=date;		  //���յ������ݷ��뷢�ͻ���������
    while(!TI);       //�ȴ������������
    TI=0;			  //���������ɱ�־λ
}

/******************
��ʱ�����ж�
******************/
//��ʱ��0��ʼ��
void Init_timer0()
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1������TR0��������
	TH0=0X00;	//����ʱ������ֵ����ʱ1ms
	TL0=0X00;	
	TR0=1;//�򿪶�ʱ��	
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
}
//��ʱ��1��ʼ��
void Init_timer1()  
{
    TMOD=0x20;      //���ü�����1�Ĺ�����ʽ2
    TH1=0xfd;	    //���ü�����1�ĳ�ֵ������������
    TL1=0xfd;		//���ü�����1�ĳ�ֵ������������
    PCON=0x00;      // �����ʱ���0x00���ӱ�	 0x80�ӱ�
    SCON=0x50;		//���ù�����ʽ1 ������������
    EA=1;		    //�������ж�
    ES=1;			//�������ڽ����ж�
    TR1=1;			//������1��ʼ����
}
//��ʱ��2��ʼ��
void Init_timer2(void)
{
 RCAP2H=0x3c;//��T2��ʼֵ0x3cb0�����20��Ϊ1��,ÿ�����ʱ��Ϊ50ms
 RCAP2L=0xb0;
 TR2=1;	     //������ʱ��2
 ET2=1;		 //�򿪶�ʱ��2�ж�
 EA=1;		 //�����ж�
}
//��ʱ��0�жϺ���
void Timer0() interrupt 1
{
	TH0=0X00;	//����ʱ������ֵ����ʱ1ms
	TL0=0X00;
	if(g==1)
	{
		e++;
		if(e==8435)
		{
			if(d<4)
			{
				deep++;
			}
			e=0;
			condition[f]=d;
			f++;
			d=0;
		}
	}

}	
//�����ж�
void Uart() interrupt 4
{
		uchar date;
    date=SBUF;        //ȡ�����ܵ�������
    RI=0;			  //��������жϱ�־λ
}

//��ʱ��2�жϺ���
void Timer2() interrupt 5	  //��ʱ��2��5���ж�
{
 static uchar t;
 TF2=0;
 t++;
 if(t==4)               //���200ms(50ms*4)��ȡһ��ʱ��
  {
   t=0;
   ds1302_read_time();  //��ȡʱ�� 
   dis_time_buf[0]=(time_buf[0]>>4); //��   
   dis_time_buf[1]=(time_buf[0]&0x0f);
   
   dis_time_buf[2]=(time_buf[1]>>4);   
   dis_time_buf[3]=(time_buf[1]&0x0f);
  
   dis_time_buf[4]=(time_buf[2]>>4); //��  
   dis_time_buf[5]=(time_buf[2]&0x0f);
   
   dis_time_buf[6]=(time_buf[3]>>4); //��   
   dis_time_buf[7]=(time_buf[3]&0x0f);
   
   dis_time_buf[14]=(time_buf[7]&0x07); //����
   
   //��2����ʾ  
   dis_time_buf[8]=(time_buf[4]>>4); //ʱ   
   dis_time_buf[9]=(time_buf[4]&0x0f);   

   dis_time_buf[10]=(time_buf[5]>>4); //��   
   dis_time_buf[11]=(time_buf[5]&0x0f);   

   dis_time_buf[12]=(time_buf[6]>>4); //��   
   dis_time_buf[13]=(time_buf[6]&0x0f);
  }
}

/***************************
����˯�����������
***************************/

//�������ú���
void setalarm() 
{
	Display2();
	while(1)
	{
		scan();
		//����ʱ�����ý���
		if(n==1)
		{			
			Delay_xms(200);
			charlie:
			n=0;
			LCD_clear();
			break;
		}
		if(y==1)
		{
			y=0;
			Delay_xms(200);
			time_hour_2+=1;//Сʱ��1
			LCD_clear();
			Display2();
		}
		if(z==1)
		{
			z=0;
			Delay_xms(200);
			time_hour_2-=1;//Сʱ��1
			LCD_clear();
			Display2();
		}
		if(x==1)
		{
			x=0;
			Delay_xms(200);
			//���ӷֵ�ʮλ���ý���
			while(1)
			{
				scan();
				if(y==1)
				{
					y=0;
					Delay_xms(200);
					time_min_1+=1;//�ֵ�ʮλ��1
					LCD_clear();
					Display2();
				}
				if(z==1)
				{
					z=0;
					Delay_xms(200);
					time_min_1-=1;//�ֵ�ʮλ��1
					LCD_clear();
					Display2();
				}
				if(n==1)
				{
					n=0;
					LCD_clear();
					goto charlie;
				}
				if(x==1)
				{
					x=0;
					Delay_xms(200);
					//���ӷֵĸ�λ���ý���
					while(1)
					{
						scan();
						if(y==1)
						{
							y=0;
							Delay_xms(200);
							time_min_2+=1;//�ֵĸ�λ��1
							LCD_clear();
							Display2();
						}
						if(z==1)
						{
							z=0;
							Delay_xms(200);
							time_min_2-=1;//�ֵĸ�λ��1
							LCD_clear();
							Display2();
						}
						if(n==1)
						{
							Delay_xms(200);
							n=0;
							LCD_clear();
							goto charlie;
						}
						if(x==1)
						{
							Delay_xms(200);
							x=0;
							m=1;
							LCD_clear();
							Display3();
							goto charlie;
						}
					}
				}
			}
		}
	}
}
//˯��ģʽ
void sleepmode()
{
	m=0;
	LCD_clear();
	Display3();
	Delay_xms(200);
	while(1)
	{
		g=1;
		scan();
		fsr();
		if(n==1)
		{
			begin:
			n=0;
			LCD_clear();
			buzz=1;
			Delay_xms(200);
			g=0;
			break;
		}
		//������������ʱ�䣬��ʼ����
		if(time_hour_1==dis_time_buf[8]&&time_hour_2==dis_time_buf[9]&&time_min_1==dis_time_buf[10]&&time_min_2==dis_time_buf[11])
		{
			while(1)
			{
				scan();
				fsr();
				//��⵽ѹ����������ѹ��
				if(c!=num)
				{
					LCD_clear();
					Display4();
					Delay_xms(200);
					buzzer();
				}
				//��⵽ѹ����������ѹ��
				if(c==num)
				{
					LCD_clear();
					g=0;
					goto begin;
				}
				if(n==1)
				{
					n=0;
					LCD_clear();
					buzz=1;
					Delay_xms(200);
					g=0;
					goto begin;
				}
			}
		}
	}
}
//�������ݷ���
void bluetooth()
{
	Display6();
	while(1)
	{
		scan();
		if(n==1)
		{
			n=0;
			LCD_clear();
			Delay_xms(200);
			break;
		}
		if(z==1)
		{
		  z=0;
		  for(h=0;h<60;h++)
		  {
				sendDate(condition[h]+'0');
			}
			Delay_xms(200);
			Display7();
		}
	}
}
//˯��״���鿴
void sleepcondition()
{
	Display5();
	while(1)
	{
		scan();
		if(n==1)
		{
		  n=0;
			LCD_clear();
			Delay_xms(200);
			break;
		}
	}
}

//������
void main(void)
{
	Delay_xms(50);//�ȴ�ϵͳ�ȶ�
	LCD_init();   //LCD��ʼ��
	LCD_clear();  //����   
	ds1302_init();  //DS1302��ʼ��
	Delay_xms(10); //��ʱ
	Init_timer0(); //��ʱ��0��ʼ��
	Init_timer1(); //��ʱ��1��ʼ��
	Init_timer2(); //��ʱ��2��ʼ�� 
	scanstart(); //������ʼ��
	while(1)
	{
		Display1(); //��ʼ����
		scan(); //����ɨ��
		if(m==1)
	  {
		 sleepmode(); //m=1ʱ����˯��ģʽ
		}
		if(x==1) //�����������ý���
		{
			x=0;
			Delay_xms(200);
			LCD_clear();
			setalarm();		 
		}
		if(y==1) //����˯�߼�¼����
		{
			y=0;
			Delay_xms(200);
			LCD_clear();
			sleepcondition();	 
		}
		if(z==1) //�����������ͽ���
		{
			z=0;
			Delay_xms(200);
			LCD_clear();
			bluetooth();		
		}
		if(n==1) //����˯��ģʽ
		{
			n=0;
			m=1; 
			Delay_xms(200);
			LCD_clear();
		}	
	}
}