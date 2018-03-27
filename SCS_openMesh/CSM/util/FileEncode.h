/*
检测文件的类型，是否包含BOM
*/
#include "text_encoding_detect.h"
#include <stdio.h>
#include <tchar.h>
using namespace AutoIt::Common;
class FileEncode
{
public:
	FileEncode();
	~FileEncode();
	bool checkBOM(const char * filename);
private:

};

