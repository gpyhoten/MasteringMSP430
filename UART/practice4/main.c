/**
 *@�ļ���UCS/practice3/main.c
 *@���ߣ��ڼ���
 *@�汾��V1.0.0
 *@����2017��6��20��
 *@��飺ʹ��GPIO�ڵ��������
 *       ��MCLK=8MHz��SMCLK=1MHz��ACLK=32.768KHz
 *       ���ô���UART1������Ϊ9600,8λ���ݣ���У�飬�޵�ַ�������ߣ�
 *       �㰴ťS1���͡�a��
 *       �㰴ťS2���͡�b��
 *       �㰴ťS3���͡�c��
 *       �㰴ťS4���͡�d��     
 *@���뻷����IAR for 430 7.10.3
 */
/**
 *����ͷ�ļ�
 */
#include "msp430f149.h"
/**
 *���Ӻ궨��
 */
int state=1;
/**
 *��������
 */

/**
 *����ȫ�ֱ���
 */

int main( void )
{
  //�����ʼ������
  // �رտ��Ź���ʱ������ֹ�������
  WDTCTL = WDTPW + WDTHOLD;
  //����ʱ�ӣ�ACLK=XT1��SMCLK=XT2/8��MCLK=XT2
  BCSCTL1=0x00;
  BCSCTL2=SELM_2+DIVM_0+SELS+DIVS_3;
  //�������������ж�
  IE1|=OFIE;
  IE1&=~(OFIE);
  //�������ж�
  __bis_SR_register(GIE);
  while(state);
  //����S1-S4��������ģʽ���������ж�
  P1DIR&=~(BIT0+BIT1+BIT2+BIT3);
  P1IES|=(BIT0+BIT1+BIT2+BIT3);
  P1IE|=(BIT0+BIT1+BIT2+BIT3);
  //��ʼ������I/O
  P3SEL|=BIT6+BIT7;
  P3DIR&=~BIT7;
  P3DIR|=BIT6;
  //��ʼ��UART0
  ME2|=UTXE1+URXE1;//����ʹ�ô���URAT0�������������
  U1CTL|=CHAR; //8λ����λ
  U1TCTL|=SSEL1;//SMCLK=1MHZ
  U1BR0=104;
  U1BR1=0;
  U1MCTL=0;
  U1CTL&=~SWRST;
  //��ʼ��D1-D8��
  P6DIR=0xff;
  while(1)
  {
  //�������������

  }
}       

/**
 *@��飺����NMU�ж�
 *@��������
 *@����ֵ����
 */
#pragma vector=NMI_VECTOR
__interrupt
void NMI_ISR(void)
{
  do
  {
    //����жϱ��λ
    IFG1&=~OFIFG;
  }
  //�ȴ�ʱ�����������λ������1
  while(IFG1&OFIFG);
  state=0;
}

/**
 *@��飺����PORT1�ж�
 *@��������
 *@����ֵ����
 */
#pragma vector=PORT1_VECTOR
__interrupt
void PORT1_ISR(void)
{
  while(!(IFG2&UTXIFG1));
  switch(P1IFG)
  {
  case 0x00:
    break;
  case 0x01:
    TXBUF1='a';
    break;
  case 0x02:
    TXBUF1='b';
    break;  
  case 0x04:
    TXBUF1='c';
    break;
  case 0x08:
    TXBUF1='d';
    break;
  default:
    break;
  }
  P1IFG=0x00;
}