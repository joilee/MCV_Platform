#include "stringUtil.h"

string Trim(string &str)   //提取不包含空格、制表符、回车、换行符的字符串
{
	str.erase(0, str.find_first_not_of(" \t\n\r"));

	str.erase(str.find_last_not_of(" \t\n\r") + 1);

	return str;
}