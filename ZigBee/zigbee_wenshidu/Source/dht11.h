#ifndef AD_H
#define AD_H

typedef unsigned char uchar;
typedef unsigned int  uint;

extern uchar shidu_shi,shidu_ge,wendu_shi,wendu_ge;

extern void Delay_us(void);
extern void Delay_10us(void);
extern void Delay_ms(uint Time);
extern void COM(void);
extern void DHT11(void);

#endif 