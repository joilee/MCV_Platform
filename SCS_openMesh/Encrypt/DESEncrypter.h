// DESEncrypter.h: interface for the DESEncrypter class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESENCRYPTER_H__0C2B68A3_2A49_4432_BBEC_1A99464DC751__INCLUDED_)
#define AFX_DESENCRYPTER_H__0C2B68A3_2A49_4432_BBEC_1A99464DC751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDESEncrypter
{
public:
	enum { ENCRYPT, DECRYPT };
	typedef bool(*PSubKey)[16][48];

public:
	CDESEncrypter();
	virtual ~CDESEncrypter();

	// Type��ENCRYPT:����,DECRYPT:����
	// ���������(Out)�ĳ��� >= ((datalen+7)/8)*8,����datalen�������8�ı�������С������
	// In ����= Out,��ʱ��/���ܺ󽫸������뻺����(In)������
	// ��keylen>8ʱϵͳ�Զ�ʹ��3��DES��/����,����ʹ�ñ�׼DES��/����.����16�ֽں�ֻȡǰ16�ֽ�
	bool Des_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type = ENCRYPT);

protected:
	void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);	//��׼DES��/����
	void SetKey(const char* Key, int len);									// ������Կ
	void SetSubKey(PSubKey pSubKey, const char Key[8]);						// ��������Կ
	void F_func(bool In[32], const bool Ki[48]);							// f ����
	void S_func(bool Out[32], const bool In[48]);							// S �д���
	void Transform(bool *Out, bool *In, const char *Table, int len);		// �任
	void Xor(bool *InA, const bool *InB, int len);							// ���
	void RotateL(bool *In, int len, int loop);								// ѭ������
	void ByteToBit(bool *Out, const char *In, int bits);					// �ֽ���ת����λ��
	void BitToByte(char *Out, const bool *In, int bits);					// λ��ת�����ֽ���

	// initial permutation IP
	const static char IP_Table[64];
	// final permutation IP^-1 
	const static char IPR_Table[64];
	// expansion operation matrix
	const static char E_Table[48];
	// 32-bit permutation function P used on the output of the S-boxes 
	const static char P_Table[32];
	// permuted choice table (key) 
	const static char PC1_Table[56];
	// permuted choice key (table) 
	const static char PC2_Table[48];
	// number left rotations of pc1 
	const static char LOOP_Table[16];
	// The (in)famous S-boxes 
	const static char S_Box[8][4][16];

	bool SubKey[2][16][48];		// 16Ȧ����Կ
	bool Is3DES;				// 3��DES��־
	char Tmp[256], deskey[16];
};

//////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DESENCRYPTER_H__0C2B68A3_2A49_4432_BBEC_1A99464DC751__INCLUDED_)
