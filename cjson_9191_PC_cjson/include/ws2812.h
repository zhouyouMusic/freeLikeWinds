/*----------------------------------------------------------------------------   
 * ��Ŀ����:

     RGB LED��������

 * ��������:

     1����������������ݷ��͵õ�LED������ʾ
	 2����ָ�����ؿ�ʼ��������������ݷ��͵�LED��ʾ
	 3�����͸�λ�ź�

 * ��Ȩ��Ϣ:

     (c) �������, 2014.

 * ��ʷ�汾:
     2014-03-15:
       - ��ʼ�汾 V1.0.0;

 * ����˵��:
     MCU:             STC15W204S
     ����:      	  �ڲ�����33MHz
     ��չģ��:  	  -
     ���:            Keil.C51.V9.01

 * ��ע:

------------------------------------------------------------------------------*/

#ifndef _WS2812_H_
#define _WS2812_H_

#include "gpio.h"


#define SNUM 12
/*--------------------------------��������------------------------------------*/
void IO_Init(void);						//IO�˿ڳ�ʼ��
void SendOnePix(unsigned char *ptr);	//����һ��LED��24bit����
void ResetDataFlow(void);				//оƬ��λ���壬Ϊ������һ֡��׼��
void SendOneFrame(unsigned char *ptr);	//����һ֡����
void SendSameColor(unsigned char *ptr,unsigned char cnt);  //��ͬ��ɫ�������ɴ�
void SendOneFrameFrom(unsigned char i,unsigned char *ptr);//�ӵ�i�����ؿ�ʼ����
														  //һ֡����
void SendOneFrameSince(unsigned char i,unsigned char *ptr);//�ӵ�i�����ص������
void Switch12LED(int flag);

#endif

//-------------------------THE END------------------------*/

