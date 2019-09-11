
#ifndef GSTRANSPORTTCPH
#define GSTRANSPORTTCPH

#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include "GSTransport.h"
#include "GSStream.h"

class GSTransportTCP : public GSTransport
{
protected:
	WSADATA WSAData;
	SOCKET fsock;
	SOCKADDR_IN fsin;
	bool ftrace = false;
	char* internalBuffer;
	uint32_t BUFFER_MEMORY = 32 * 1024; //32K

public:

	GSTransportTCP()
	{
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		internalBuffer = (char*)malloc(BUFFER_MEMORY); 
	};

	~GSTransportTCP()
	{
		WSACleanup();
		free(internalBuffer);
	};


	bool disconnect()
	{
		closesocket(fsock);
		return true;
	};

	bool connect(string IP, int port, uint32_t* errorCode)
	{
		fsin.sin_addr.s_addr = inet_addr(IP.c_str());
		fsin.sin_family = AF_INET;
		fsin.sin_port = htons(60000);
		fsock = socket(AF_INET, SOCK_STREAM, 0);
		//bind(sock, (SOCKADDR *)&sin, sizeof(sin));
		uint32_t t = ::connect(fsock, (SOCKADDR *)&fsin, sizeof(fsin)); //no error, connect api return 0.
		memcpy((void*)errorCode, &t, sizeof(t));
		return (t == 0);
	};


	//one shot received, char* interface. 
	//buffer must be "mallocable".
	bool receive(GSMemoryStream& data, uint32_t timeOut = 0)
	{
		uint32_t r =0, l=0, ll=0, rr = 0;
		fd_set rfd;
		FD_ZERO(&rfd);
		FD_SET(fsock, &rfd);
		timeval _timeout = { timeOut };

		r = select(fsock + 1, &rfd, NULL, NULL, &_timeout);
		if (r == -1)
		{ 
			if (ftrace)
			{
				cout << "receive() Socket Select error" << std::endl;
			}
			return false;
		}

		if (r = 0)
			return true;

		r = recv(fsock, (char*)&ll, sizeof(ll), 0);
		ll = ntohl(ll);

		//--- REALLOCATION
		if (ll > BUFFER_MEMORY)
			realloc((char*)internalBuffer, ll);
		//--- REALLOCATION

		do
		{
			r = recv(fsock,(char*)internalBuffer, ll, 0);
			rr += r;
			if (ftrace)
			{
				if (r > 0)
					cout << "Bytes received: "<< r << " (" <<rr*100/ll << "%)" << std::endl;
				else if (r == 0)
					cout << "Connection closed" << std::endl;
				else
					cout << "recv failed: " << WSAGetLastError << std::endl;
			}
		} while (r > 0 && rr < ll);
	
		data.clear();
		data.loadFromBuffer(internalBuffer, ll);
		return (ll>0);
	};


	bool send(char* buffer, uint32_t len, bool sizedPrefix = true)
	{
		uint32_t sended = 0;
		if (sizedPrefix)
		{
			GSMemoryStream secondary;
			secondary.writeUint32(htonl(len));
			secondary.loadFromBuffer(buffer, len);
			sended = ::send(fsock, (char*)secondary.data(), secondary.size(), 0);
			if (ftrace)
				cout << "bytes sent (size prefixed) : " << sended << std::endl;
		}
		else
		{
			sended = ::send(fsock, buffer, len, 0);
			if (ftrace)
				cout << "bytes sent : " << sended << std::endl;
		}
		return (sended > 0);
	};

	string transportName()
	{
		return u8"TCP/IP transport layer";
	};

};

#endif