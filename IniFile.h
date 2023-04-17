#pragma once
#include "essentials.h"

class IniFile
{
	bool m_read;
	string m_data;

public:
	IniFile(bool read = true, string str = "");
	~IniFile();

	void SetData(string str);

	string ReadKey(string key);
	int ReadKeyInt(string key);
	bool WriteKey(string key, string value);
	bool WriteKeyInt(string key, int value);

	const string operator[](string key);

	const char* Data();
};