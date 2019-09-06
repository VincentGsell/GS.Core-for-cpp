#pragma once

#ifndef GSTRANSPORTH
#define GSTRANSPORTH
//---------------------------------------------------------------------------
#include <string>
using namespace std;

class GSTransport
{
private:
protected:
public:
	GSTransport()
	{};
	virtual ~GSTransport()
	{};

	string transportName()
	{
		return u8"unknown";
	}
};



#endif