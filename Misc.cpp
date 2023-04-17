#include "Misc.h"

vector<string> Split(const string& str, char splitter)
{
	vector<string> res;

	size_t pos = 0, spos = str.find(splitter);
	while (spos != string::npos)
	{
		res.push_back(str.substr(pos, spos - pos));
		pos = spos + 1;
		spos = str.find(splitter, pos);
	}
	
	res.push_back(str.substr(pos, str.length() - pos));
	return res;
}

string DateToString(SYSTEMTIME& date)
{
	return to_string(date.wYear)
		+ '-' + to_string(date.wMonth)
		+ '-' + to_string(date.wDayOfWeek)
		+ '-' + to_string(date.wDay)
		+ '-' + to_string(date.wHour)
		+ '-' + to_string(date.wMinute)
		+ '-' + to_string(date.wSecond)
		+ '-' + to_string(date.wMilliseconds);
}

SYSTEMTIME StringToDate(string str)
{
	SYSTEMTIME time{};
	vector<string> splitter = Split(str, '-');

	if (splitter.size() == 8)
	{
		time.wYear = atoi(splitter[0].c_str());
		time.wMonth = atoi(splitter[1].c_str());
		time.wDayOfWeek = atoi(splitter[2].c_str());
		time.wDay = atoi(splitter[3].c_str());
		time.wHour = atoi(splitter[4].c_str());
		time.wMinute = atoi(splitter[5].c_str());
		time.wSecond = atoi(splitter[6].c_str());
		time.wMilliseconds = atoi(splitter[7].c_str());
	}

	return time;
}
