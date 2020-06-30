#ifndef ENCRYPT_H_INCLUDED
#define ENCRYPT_H_INCLUDED

//simple encryption
//key1..4: keys
//returns the encrypted data
//http://www.amobbs.com/thread-5466728-1-1.html
//��������ǰ�ù���һ�ֲ������㷨������ô�򵥣������Դ��ĳ��ڲʺ���ܹ��ļ����㷨��ͨ����λ��ȡģ���㣬��Щԭ���������Ѿ���Ŀȫ�ǲ�����ȫ�������ˡ�
unsigned long misc_encrypt(unsigned long input_data, unsigned long key1, unsigned long key2, unsigned long key3, unsigned long key4);

/******************************************************************************************************************
  TEA���ܽ����㷨
  TEA(Tiny Encryption Algorithm)��һ�ּ򵥸�Ч�ļ����㷨���Լ��ܽ����ٶȿ죬ʵ�ּ����ơ�
  �㷨�ܼ򵥣�TEA�㷨ÿһ�ο��Բ���64-bit(8-byte)������128-bit(16-byte)��Ϊkey���㷨���õ�������ʽ��
  �Ƽ��ĵ���������64�֣�����32�֡�
******************************************************************************************************************/
//#define MX 			(z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[p&3^e]^z)
//#define DELTA 		0x9e3779b9ul
//#define S_LOOPTIME  5
//#define BLOCK_SIZE  32   //PAGE_SIZE,��������Ҫ���ܵ����ݰ������޸Ĵ˲���(��λ:�ֽ�)

void tea_encrypt( unsigned char* buf, unsigned char* key );

void tea_decrypt( unsigned char* buf, unsigned char* key );

#if 0
void tea_test(void)
{
	unsigned char dat[16]="0123456789ABCDEF";
	unsigned char TEA_key[16];
	memcpy(TEA_key,"0123456789abcdef",16);//������֮ǰ��Ҫ���ú���Կ������ֻ��������Կ��DEMO��

	printf("tea_test\n");
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);
	tea_encrypt(dat,TEA_key);      //TEA���ܣ�����dat����������ݾ��Ǽ��ܺ�����ݡ�
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);
	tea_decrypt(dat,TEA_key);      //�������ݴ����dat���棬��TEA���ܾ��ܵõ�֮ǰ�����ġ�
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);

}
#endif


//typedef unsigned long uint32_t;
//from tea wiki
//v data input;
//k[0..3] 4*32=128bit key
void encrypt (unsigned long * v, unsigned long * k);

void decrypt (unsigned long * v, unsigned long * k);

#if 0
void my_tea_test(void) {
	unsigned char dat[16]="0123456789ABCDEF";
	unsigned char TEA_key[16];
	memcpy(TEA_key,"0123456789abcdef",16);//������֮ǰ��Ҫ���ú���Կ������ֻ��������Կ��DEMO��

	printf("my_tea_test\n");
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);
	encrypt(dat,TEA_key);      //TEA���ܣ�����dat����������ݾ��Ǽ��ܺ�����ݡ�
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);
	decrypt(dat,TEA_key);      //�������ݴ����dat���棬��TEA���ܾ��ܵõ�֮ǰ�����ġ�
	printf("dat = %s, TEA_key = %s\n", dat, TEA_key);


}
#endif

/* rc6 (TM)
 * Unoptimized sample implementation of Ron Rivest's submission to the
 * AES bakeoff.
 *
 * Salvo Salasio, 19 June 1998
 *
 * Intellectual property notes:  The name of the algorithm (RC6) is
 * trademarked; any property rights to the algorithm or the trademark
 * should be discussed with discussed with the authors of the defining
 * paper "The RC6(TM) Block Cipher": Ronald L. Rivest (MIT),
 * M.J.B. Robshaw (RSA Labs), R. Sidney (RSA Labs), and Y.L. Yin (RSA Labs),
 * distributed 18 June 1998 and available from the lead author's web site.
 *
 * This sample implementation is placed in the public domain by the author,
 * Salvo Salasio.  The ROTL and ROTR definitions were cribbed from RSA Labs'
 * RC5 reference implementation.
 */

/* RC6 is parameterized for w-bit words, b bytes of key, and
 * r rounds.  The AES version of RC6 specifies b=16, 24, or 32;
 * w=32; and r=20.
 */

#if 0
unsigned long RC6_S[R24];        /* Key schedule */

unsigned char RC6_Key[32]=
{
//	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
//	0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
//	0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0,
//	0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

void rc6_key_setup(void);

void rc6_block_encrypt(unsigned long *pt, unsigned long *ct);

void rc6_block_decrypt(unsigned long *ct, unsigned long *pt);

#if 0
void RC6_Test(void)
{
    unsigned long mingwen[4]={0x55555555, 0xaaaaaaaa, 0x5a5a5a5a, 0xa5a5a5a5};
    unsigned long miwen[4]={0x00000000, 0x00000000, 0x00000000, 0x00000000};
    unsigned long jiemi[4]={0x00000000, 0x00000000, 0x00000000, 0x00000000};
	unsigned char i;

    for(i=0;i<32;i++) RC6_Key[i]=i;//������֮ǰ��Ҫ���ú���Կ������ֻ��������Կ��DEMO��

	rc6_key_setup();
    rc6_block_encrypt(mingwen,miwen);//RC6���ܣ����ı�����
    rc6_block_decrypt(miwen,jiemi);  //RC6���ܣ����ı������
}

int main(void)
{
	unsigned char i=0;
	unsigned long output_data, input_data = 0xf0f04422ul;
	unsigned long key1 = 0xf0f0f0f0ul, key2 = 0x0f0f0f0ful, key3 = 0x55555555ul, key4 = 0x22222222ul;
    printf("Hello world!\n");
    for (i=0; i<10; i++) {
    	output_data=misc_encrypt(input_data, key1, key2, key3, key4);
    	printf("input data = %08x, encrypted data = %08x\n", input_data, output_data);
    	input_data=output_data;
    }

	//tea_test();
	//my_tea_test();
    return 0;
}
#endif

#endif // ENCRYPT_H_INCLUDED
