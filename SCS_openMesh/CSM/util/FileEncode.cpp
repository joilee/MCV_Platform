#include "FileEncode.h"
FileEncode::FileEncode()
{
}

FileEncode::~FileEncode()
{
}

bool FileEncode::checkBOM(const char * filename)
{
	//判断文件的格式，是否包含BOM
	wchar_t  file2D[200];
	swprintf(file2D, 100, L"%hs", filename);
	FILE *file= _wfopen(file2D, L"rb");

	if (file == NULL)
	{
		wprintf(L"\nCould not open file.\n");
		return 1;
	}

	// Get file size
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read it all in
	unsigned char *buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	// Detect the encoding
	TextEncodingDetect textDetect;
	TextEncodingDetect::Encoding encoding = textDetect.DetectEncoding(buffer, fsize);

	bool flag = false;
	wprintf(L"\nEncoding: ");
	if (encoding == TextEncodingDetect::None)
	{
		wprintf(L"Binary");		
	}
	else if (encoding == TextEncodingDetect::ASCII)
	{
		wprintf(L"ASCII (chars in the 0-127 range)");	
	}
	else if (encoding == TextEncodingDetect::ANSI)
	{
		wprintf(L"ANSI (chars in the range 0-255 range)");		
	}
	else if (encoding == TextEncodingDetect::UTF8_BOM)
	{
		wprintf(L"UTF-8 BOM");
		flag = true;
	}
	else if (encoding == TextEncodingDetect::UTF8_NOBOM)
	{
		wprintf(L"UTF-8");
	}
	else if (encoding == TextEncodingDetect::UTF16_LE_BOM)
	{
		wprintf(L"UTF-16 Little Endian");
		flag = true;
	}
	else if (encoding == TextEncodingDetect::UTF16_LE_NOBOM)
	{
		wprintf(L"UTF-16 Little Endian");
	} 
	else if (encoding == TextEncodingDetect::UTF16_BE_BOM)
	{
		wprintf(L"UTF-16 Big Endian");
		flag = true;
	}
	else if (encoding == TextEncodingDetect::UTF16_BE_NOBOM)
	{
		wprintf(L"UTF-16 Big Endian");
	} 
	// Free up
	delete[] buffer;
	return flag;
}
