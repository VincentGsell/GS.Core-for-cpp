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
	uint32_t position = 0;
	uint32_t internalSize = 0;
	uint32_t currentSize = 0;

	void controlBuffer(uint32_t item);

public:
  GSMemoryStream();
  ~GSMemoryStream();
  void writeByte(uint8_t dataByte);
  void writeUint32(uint32_t dataUint32);
  void writeString(string dataString);

  uint8_t readByte();
  int32_t readInt32();
  uint32_t readUint32();
  uint64_t readUint64();
  string readString();
  GSMemoryStream* readMemoryStream(); //readbuffer, but directy in a GSMemoryStream. Use GSMemoryStream.data() to get your lovely char*.

  uint8_t* data();
  uint32_t size();
  void clear(const bool reallocation = false);

  void loadFromStream(GSMemoryStream* Source, bool clear = true); 
  void loadFromBuffer(char* sourceData, uint32_t len);

  void seekStart();
};


#endif
