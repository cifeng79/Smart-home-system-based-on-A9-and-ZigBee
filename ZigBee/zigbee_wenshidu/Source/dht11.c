#include "dht11.h"
#include "ioCC2530.h"
#include "OnBoard.h"

#define DATA_PIN P1_2

//温湿度定义
uchar ucharFLAG,uchartemp;
uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge=4;
uchar ucharT_data_H,ucharT_data_L,ucharRH_data_H,ucharRH_data_L,ucharcheckdata;
uchar ucharT_data_H_temp,ucharT_data_L_temp,ucharRH_data_H_temp,ucharRH_data_L_temp,ucharcheckdata_temp;
uchar ucharcomdata;
//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8RH_data_H-----
//----湿度低8位== U8RH_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- Delay();, Delay_10us();,COM(); 
//--------------------------------

//延时函数
void Delay_us() //1 us延时
{
  MicroWait(1);
}


void Delay_10us() //10 us延时
{
  MicroWait(10); 
}

void Delay_ms(uint Time)//n ms延时
{
    unsigned char i;
    while(Time--)
    {
        for(i=0;i<100;i++)
            Delay_10us();
    }
}


//温湿度传感
void COM(void)    // 温湿写入
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
        if(DATA_PIN)uchartemp=1;//如果超过30us还没置0，则数据为1，置0，则数据为0
        ucharFLAG=2;
        while((DATA_PIN)&&ucharFLAG++); //等待置0  
        if(ucharFLAG==1)break;    
        ucharcomdata<<=1;//ucharcomdata左移一位赋值给ucharcomdata
        ucharcomdata|=uchartemp;//两句合起来就是给最高位赋值
    }    
}


void DHT11(void)   //温湿传感启动
{
    P1DIR = 0xFF;  //设置为输出模式
    DATA_PIN=0;//置低电平
    Delay_ms(19);  //>18MS
    DATA_PIN=1; //置高电平，开始信号发送结束
    P1DIR = 0x00;  //设置为输入模式
    Delay_10us();
    Delay_10us();                        
    Delay_10us();
    Delay_10us();  //高电平，延时40us等待
    if(!DATA_PIN) //如果电平变低，则证明检测到开始信号
    {
        ucharFLAG=2; 
        while((!DATA_PIN)&&ucharFLAG++);//低电平，等待响应结束
        ucharFLAG=2;
        while((DATA_PIN)&&ucharFLAG++); //高电平，准备输出
        COM();
        ucharRH_data_H_temp=ucharcomdata;//湿度高八位
        COM();
        ucharRH_data_L_temp=ucharcomdata;//湿度低八位
        COM();
        ucharT_data_H_temp=ucharcomdata;//温度高八位
        COM();
        ucharT_data_L_temp=ucharcomdata;//温度低八位
        COM();
        ucharcheckdata_temp=ucharcomdata;//校验八位
        DATA_PIN=1; //置高，空闲状态
        uchartemp=(ucharT_data_H_temp+ucharT_data_L_temp+ucharRH_data_H_temp+ucharRH_data_L_temp);
        if(uchartemp==ucharcheckdata_temp)//校验
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
    else //没用成功读取，返回0
    {
        wendu_shi=0; 
        wendu_ge=0;
        
        shidu_shi=0; 
        shidu_ge=0;  
    } 
    
    P1DIR = 0xFF;  //设置为输出模式
}