#include "stringUtil.h"

string Trim(string &str)   //��ȡ�������ո��Ʊ�����س������з����ַ���
{
	str.erase(0, str.find_first_not_of(" \t\n\r"));

	str.erase(str.find_last_not_of(" \t\n\r") + 1);

	return str;
}