//---------------------------------------------------------------------------
#pragma once


#ifndef GSStreamH
#define GSStreamH
//---------------------------------------------------------------------------

#include <string>
#include <vector>

using namespace std;

#define gsbzero(b, len) (memset((b), '\0', (len)), (void)0);

class GSMemoryStream
{
protected:
	uint8_t* buffer;
	uint64_t position = 0;
	size_t internalSize = 0;
	size_t currentSize = 0;

	void controlBuffer(uint64_t item);

public:
  GSMemoryStream();
  ~GSMemoryStream();
  void writeByte(uint8_t dataByte);
  void writeUint32(uint32_t dataUint32);
  void writeUint64(uint64_t dataUint64);
  void writeString(string dataString);

  uint8_t readByte();
  int32_t readInt32();
  uint32_t readUint32();
  uint64_t readUint64();
  string readString();
  double readDouble();
  string readRawString(bool bytesLenPrefixed = false);
  void writeRawString(string txt, bool bytesLenPrefixed = false);
  GSMemoryStream* readMemoryStream(bool prefixed = true, uint64_t lenIfNotPrefixed = 0); //readbuffer, but directy in a GSMemoryStream. Use GSMemoryStream.data() to get your lovely char*.

  uint8_t* data(); //Whole data.
  uint32_t size();
  void clear(const bool reallocation = false);

  void loadFromStream(GSMemoryStream* Source, bool clear = true); 
  void loadFromBuffer(char* sourceData, uint64_t len);

  void seekStart();
  uint64_t seekPos();
  void setPosition(uint64_t pos);
};


#endif
