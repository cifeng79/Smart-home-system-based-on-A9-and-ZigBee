#include "dht11.h"
#include "ioCC2530.h"
#include "OnBoard.h"

#define DATA_PIN P1_2

//��ʪ�ȶ���
uchar ucharFLAG,uchartemp;
uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge=4;
uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
uchar ucharT_data_H_temp,ucharT_data_L_temp,ucharRH_data_H_temp,ucharRH_data_L_temp,ucharcheckdata_temp;
uchar ucharcomdata;
//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

//��ʱ����
void Delay_us() //1 us��ʱ
{
  MicroWait(1);
}


void Delay_10us() //10 us��ʱ
{
  MicroWait(10); 
}

void Delay_ms(uint Time)//n ms��ʱ
{
    unsigned char i;
    while(Time--)
    {
        for(i=0;i<100;i++)
            Delay_10us();
    }
}


//��ʪ�ȴ���
void COM(void)    // ��ʪд��
{     
    uchar i;         
    for(i=0;i<8;i++)    
    {
        ucharFLAG=2; 
        while((!DATA_PIN)&&ucharFLAG++);
        Delay_10us();
        Delay_10us();
        Delay_10us();
        uchartemp=0;
        if(DATA_PIN)uchartemp=1;//�������30us��û��0��������Ϊ1����0��������Ϊ0
        ucharFLAG=2;
        while((DATA_PIN)&&ucharFLAG++); //�ȴ���0  
        if(ucharFLAG==1)break;    
        ucharcomdata<<=1;//ucharcomdata����һλ��ֵ��ucharcomdata
        ucharcomdata|=uchartemp;//������������Ǹ����λ��ֵ
    }    
}


void DHT11(void)   //��ʪ��������
{
    P1DIR = 0xFF;  //����Ϊ���ģʽ
    DATA_PIN=0;//�õ͵�ƽ
    Delay_ms(19);  //>18MS
    DATA_PIN=1; //�øߵ�ƽ����ʼ�źŷ��ͽ���
    P1DIR = 0x00;  //����Ϊ����ģʽ
    Delay_10us();
    Delay_10us();                        
    Delay_10us();
    Delay_10us();  //�ߵ�ƽ����ʱ40us�ȴ�
    if(!DATA_PIN) //�����ƽ��ͣ���֤����⵽��ʼ�ź�
    {
        ucharFLAG=2; 
        while((!DATA_PIN)&&ucharFLAG++);//�͵�ƽ���ȴ���Ӧ����
        ucharFLAG=2;
        while((DATA_PIN)&&ucharFLAG++); //�ߵ�ƽ��׼�����
        COM();
        ucharRH_data_H_temp=ucharcomdata;//ʪ�ȸ߰�λ
        COM();
        ucharRH_data_L_temp=ucharcomdata;//ʪ�ȵͰ�λ
        COM();
        ucharT_data_H_temp=ucharcomdata;//�¶ȸ߰�λ
        COM();
        ucharT_data_L_temp=ucharcomdata;//�¶ȵͰ�λ
        COM();
        ucharcheckdata_temp=ucharcomdata;//У���λ
        DATA_PIN=1; //�øߣ�����״̬
        uchartemp=(ucharT_data_H_temp+ucharT_data_L_temp+ucharRH_data_H_temp+ucharRH_data_L_temp);
        if(uchartemp==ucharcheckdata_temp)//У��
        {
            ucharRH_data_H=ucharRH_data_H_temp;
            ucharRH_data_L=ucharRH_data_L_temp;
            ucharT_data_H=ucharT_data_H_temp;
            ucharT_data_L=ucharT_data_L_temp;
            ucharcheckdata=ucharcheckdata_temp;
        }
        wendu_shi=ucharT_data_H/10; 
        wendu_ge=ucharT_data_H%10;
        
        shidu_shi=ucharRH_data_H/10; 
        shidu_ge=ucharRH_data_H%10;        
    } 
    else //û�óɹ���ȡ������0
    {
        wendu_shi=0; 
        wendu_ge=0;
        
        shidu_shi=0; 
        shidu_ge=0;  
    } 
    
    P1DIR = 0xFF;  //����Ϊ���ģʽ
}