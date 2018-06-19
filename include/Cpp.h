/*
 * Cpp.h
 *
 *  Created on: 2016年8月24日
 *      Author: zzh
 */

#ifndef INCLUDE_CPP_H_
#define INCLUDE_CPP_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include "misc.h"
#include <stdlib.h>

using namespace std;

//__inline std::string Int2String(int iNum)
//{
//	std::stringstream ss;
//	std::string str;
//
//	ss << iNum;
//	ss >> str;
//
//	return str;
//}

//该函数耗时是stringstream方式的1/6
__inline std::string Int2String(int iNum)
{
	char temp[16];
	string str;

	sprintf(temp, "%d", iNum);
	string s(temp);
    return s;
}

__inline std::string Long2String(long lNum)
{
	std::stringstream ss;
	std::string str;

	ss << lNum;
	ss >> str;

	return str;
}

__inline std::string Ull2String(unsigned long long iNum)
{
	std::stringstream ss;
	std::string str;

	ss << iNum;
	ss >> str;

	return str;
}

__inline unsigned long long String2Ull(std::string sStr)
{
	unsigned long long result;
	std::istringstream ss(sStr);
    ss >> result;
    return result;
}

//数据库语句时，字符串你需要转换为带单引号
__inline std::string AddApostrophe(const std::string &sStr)
{
	return ("'" + sStr + "'");
}

//去掉字符串的单引号
__inline std::string RemoveApostrophe(std::string sStr)
{
	return (sStr.substr(1, sStr.size() - 2));
}

//s--源字符串;v--分割获得的字符串容器;c--以字符串c作为分割依据
__inline void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
#if(1 == 1)
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
	{
		v.push_back(s.substr(pos1));
	}
#else
	stringstream ss(s);
	string sub_str;
	while(getline(ss, sub_str,*(c.c_str()))) //以c为间隔分割s的内容
	{
		v.push_back(sub_str);
	}
#endif
}

//字符串拼接：通过给定的字符传进行分隔并拼接成一个长字符串
__inline string StrCat(std::vector<std::string>& v, const std::string& c)
{
    string sStr;

    for (std::vector<std::string>::iterator Itr = v.begin(); Itr != v.end(); Itr++)
    {
        if (Itr == v.begin())
        {
            sStr += *Itr;
        }
        else
        {
            sStr += c + (*Itr);
        }
    }
    return sStr;
}

//将时间转换为字符串:"2017-01-04 10:29:30"
__inline string GetLocalTimeToStr()
{
	time_t Time;
	struct tm * pTm;
	string sTime;
	char cYear[10] = {0};
	char cMon[10] = {0};
	char cDay[10] = {0};
	char cHour[10] = {0};
	char cMin[10] = {0};
	char cSec[10] = {0};

	time(&Time);
//	pTm = gmtime(&Time);    //格林尼治时区的时间
	pTm = localtime(&Time);    //当前时区的时间

	sprintf(cYear, "%04d", 1900 + pTm->tm_year);
	sprintf(cMon, "%02d", 1 + pTm->tm_mon);
	sprintf(cDay, "%02d", pTm->tm_mday);
	sprintf(cHour, "%02d", pTm->tm_hour);
	sprintf(cMin, "%02d", pTm->tm_min);
	sprintf(cSec, "%02d", pTm->tm_sec);

//	sTime = Int2String(1900 + pTm->tm_year) + "-" + Int2String(1 + pTm->tm_mon) + "-" + Int2String(pTm->tm_mday) + " "
//			     + Int2String(pTm->tm_hour) + ":" + Int2String(pTm->tm_min) + ":" + Int2String(pTm->tm_sec);
	sTime = string(cYear) + "-" + string(cMon) + "-" + string(cDay) + " "
			     + string(cHour) + ":" + string(cMin) + ":" + string(cSec);
	return sTime;
}

__inline string GetSysTimeUs()
{
//	string sTime;
	char cTime[32];

//	GetSysTimeUs((char *)sTime.c_str());
	GetSysTimeUs(cTime);
//	return sTime;
	return string(cTime);
}

__inline string HexToString(const unsigned char *pSrc, int iSrcLen)
{
	string sStr;
	if (NULL == pSrc)
	{
		return "";
	}

	int iLen = 2 * iSrcLen + 1;
	uchar pBuf[iLen];
	HexToStr(pSrc, pBuf, iSrcLen);
	sStr = string((char*)pBuf);

	return sStr;
}

__inline string HexToStringBigEndian(const unsigned char *pSrc, int iSrcLen)
{
	string sStr;
	if (NULL == pSrc)
	{
		return "";
	}

	int iLen = 2 * iSrcLen + 1;
	unsigned char *pBuf = new unsigned char[iLen];
	HexToStrBigEndian(pSrc, pBuf, iSrcLen);
	pBuf[iLen - 1] = '\0';
	sStr = string((char*)pBuf);
	delete[] pBuf;

	return sStr;
}

//去掉字符串首尾空格
__inline std::string& TrimBeforeAfter(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

//去掉字符串前面的空格
__inline std::string& TrimBefore(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    return s;
}

//去掉字符串前面的空格
__inline std::string& TrimAfter(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

//去掉字符串所有空格
__inline std::string& TrimAll(std::string &s)
{
	unsigned int index = 0;
	if( !s.empty())
	{
		while( (index = s.find(' ',index)) != string::npos)
		{
			s.erase(index,1);
		}
	}
	return s;
}


#endif /* INCLUDE_CPP_H_ */
