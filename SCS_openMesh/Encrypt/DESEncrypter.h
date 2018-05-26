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

	// Type—ENCRYPT:加密,DECRYPT:解密
	// 输出缓冲区(Out)的长度 >= ((datalen+7)/8)*8,即比datalen大的且是8的倍数的最小正整数
	// In 可以= Out,此时加/解密后将覆盖输入缓冲区(In)的内容
	// 当keylen>8时系统自动使用3次DES加/解密,否则使用标准DES加/解密.超过16字节后只取前16字节
	bool Des_Go(char *Out, char *In, long datalen, const char *Key, int keylen, bool Type = ENCRYPT);

protected:
	void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);	//标准DES加/解密
	void SetKey(const char* Key, int len);									// 设置密钥
	void SetSubKey(PSubKey pSubKey, const char Key[8]);						// 设置子密钥
	void F_func(bool In[32], const bool Ki[48]);							// f 函数
	void S_func(bool Out[32], const bool In[48]);							// S 盒代替
	void Transform(bool *Out, bool *In, const char *Table, int len);		// 变换
	void Xor(bool *InA, const bool *InB, int len);							// 异或
	void RotateL(bool *In, int len, int loop);								// 循环左移
	void ByteToBit(bool *Out, const char *In, int bits);					// 字节组转换成位组
	void BitToByte(char *Out, const bool *In, int bits);					// 位组转换成字节组

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

	bool SubKey[2][16][48];		// 16圈子密钥
	bool Is3DES;				// 3次DES标志
	char Tmp[256], deskey[16];
};

//////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DESENCRYPTER_H__0C2B68A3_2A49_4432_BBEC_1A99464DC751__INCLUDED_)
