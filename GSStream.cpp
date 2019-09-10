//---------------------------------------------------------------------------

using namespace std;

#include <iostream>
#include <string>
#include <vector>
#include "GSStream.h"


uint32_t CST_BUFSIZE = 32*1024;
bool GSStreamH_TRACE = false;

GSMemoryStream::GSMemoryStream()
{
  position = 0;
  internalSize=0;
  currentSize = CST_BUFSIZE;
  buffer = (uint8_t*) malloc(currentSize);
  gsbzero(buffer, CST_BUFSIZE);
  if (GSStreamH_TRACE)
	cout << "GSMemoryStream Builded" << endl;
};

GSMemoryStream::~GSMemoryStream()
{
  if (GSStreamH_TRACE)
	cout << "GSMemoryStream freed" << endl;
};

void GSMemoryStream::controlBuffer(uint32_t item)
{
	if ((internalSize + item) > currentSize)	{
	  if ((uint8_t*)realloc(buffer, currentSize + currentSize) == NULL)
		throw "Failled to reallocate memory";
	  currentSize = currentSize + currentSize;
	}
};

void GSMemoryStream::loadFromStream(GSMemoryStream* Source, bool clear)
{
	if (clear)
	{
		GSMemoryStream::clear();
	};
	Source->seekStart();
	controlBuffer(Source->size());
	memcpy(&buffer[position], Source->data(), Source->size());
	position = position + Source->size();
	internalSize = internalSize + Source->size();
}

void GSMemoryStream::loadFromBuffer(char* sourceData, uint32_t len)
{
	controlBuffer(len);
	memcpy(&buffer[position], sourceData, len);
	position = position + len;
	internalSize = internalSize + len;
}

void GSMemoryStream::clear(const bool reallocation)
{
	if (reallocation)
	{
		if ((uint8_t*)realloc(buffer, CST_BUFSIZE) == NULL)
			throw "realloc failure";
		currentSize = CST_BUFSIZE;
	}
	position = 0;
	internalSize = 0;
	gsbzero(buffer, CST_BUFSIZE);
}

void GSMemoryStream::writeByte(uint8_t dataByte)
{
	uint8_t ll = sizeof(uint8_t);
	controlBuffer(ll);
	memcpy(&buffer[position], &dataByte, ll);
	position = position + ll;
	internalSize = internalSize + ll;
};

uint8_t GSMemoryStream::readByte()
{
	uint8_t ll = 255;
	memcpy(&ll, &buffer[position], sizeof(uint8_t));
	position = position + sizeof(uint8_t);
	return ll;
}

string GSMemoryStream::readString()
{
	uint32_t statusStringLength = readUint32();
	string b = ""; 
	b.assign((char*)&buffer[position], statusStringLength);
	position += statusStringLength;
	return b;
}

string GSMemoryStream::readRawString(bool bytesLenPrefixed)
{
	if (bytesLenPrefixed)
	{
		return readString();
	}
	else
	{
 		string b = "";
		uint32_t p = internalSize - position;
		b.assign((char*)&buffer[position],p);
		position += p;
		return b;
	}
}

void GSMemoryStream::writeRawString(string txt, bool bytesLenPrefixed)
{
	if (bytesLenPrefixed)
	{
		writeString(txt);
	}
	else
	{
		uint32_t ll = txt.size();
		//writeUint32(ll);
		if (ll > 0)
		{
			controlBuffer(ll);
			memcpy(&buffer[position], txt.c_str(), ll);
			position += ll;
			internalSize += ll;
		}
	}
}

GSMemoryStream* GSMemoryStream::readMemoryStream()
{
	uint64_t memLength = readUint64();
	GSMemoryStream* b = new(GSMemoryStream);
	b->loadFromBuffer((char*)&buffer[position], uint32_t(memLength));
	position += memLength;
	return b;
}

double GSMemoryStream::readDouble()
{
	uint8_t s = sizeof(double);
	double ll = -1;
	memcpy(&ll, &buffer[position], s);
	position = position + s;
	return ll;
}


void GSMemoryStream::seekStart()
{
	position = 0;
}

void GSMemoryStream::writeUint32(uint32_t dataUint32)
{
	uint32_t ll = sizeof(uint32_t);
	controlBuffer(ll);
	memcpy(&buffer[position], &dataUint32, ll);
	position = position + ll;
	internalSize = internalSize + ll;
};

int32_t GSMemoryStream::readInt32()
{
	int32_t ll = 0;
	memcpy(&ll, &buffer[position], sizeof(int32_t));
	position = position + sizeof(int32_t);
	return ll;
};

uint32_t GSMemoryStream::readUint32()
{
	uint32_t ll = 0;
	memcpy(&ll, &buffer[position], sizeof(uint32_t));
	position = position + sizeof(uint32_t);
	return ll;
};

uint64_t GSMemoryStream::readUint64()
{
	uint64_t ll = 0;
	memcpy(&ll, &buffer[position], sizeof(uint64_t));
	position = position + sizeof(uint64_t);
	return ll;
}

void GSMemoryStream::writeUint64(uint64_t dataUint64)
{
	uint64_t ll = sizeof(uint64_t);
	controlBuffer(ll);
	memcpy(&buffer[position], &dataUint64, ll);
	position = position + ll;
	internalSize = internalSize + ll;
};



void GSMemoryStream::writeString(string dataString)
{
	uint32_t ll = dataString.size();
	writeUint32(ll);
	if (ll > 0)
	{
		controlBuffer(ll);
		memcpy(&buffer[position], dataString.c_str(), ll);
		position += ll;
		internalSize += ll;
	}
};

uint8_t* GSMemoryStream::data()
{
  return buffer;
};

uint32_t GSMemoryStream::size()
{
  return internalSize;
};




