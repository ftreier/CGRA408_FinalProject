// trim from start (in place)
#pragma once
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>

using namespace std;

static inline void ltrim(string &s, char c)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](int ch) { return ch != c; }));
}

static inline void ltrim(string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !isspace(ch); }));
}

static inline void rtrim(string &s, char c)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [&](int ch) { return ch != c; }).base(), s.end());
}

// trim from end (in place)
static inline void rtrim(string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !isspace(ch); }).base(), s.end());
}

static inline void trim(string &s, char c)
{
	ltrim(s, c);
	rtrim(s, c);
}

// trim from both ends (in place)
static inline void trim(string &s)
{
	ltrim(s);
	rtrim(s);
}

vector<string> splitString(string input, string delimiter)
{
	vector<string> output;
	size_t start = 0;
	size_t end = 0;

	while (start != string::npos && end != string::npos)
	{
		start = input.find_first_not_of(delimiter, end);

		if (start != string::npos)
		{
			end = input.find_first_of(delimiter, start);

			if (end != string::npos)
			{
				output.push_back(input.substr(start, end - start));
			}
			else
			{
				output.push_back(input.substr(start));
			}
		}
	}

	return output;

}
