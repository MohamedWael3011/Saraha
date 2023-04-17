#pragma once
#include "essentials.h"

vector<string> Split(const string& str, char splitter);
string DateToString(SYSTEMTIME& date);
SYSTEMTIME StringToDate(string str);