
#include "stdafx.h"
#include <stdio.h>
#include "com.h"

extern "C" __declspec(dllexport) Com* __stdcall newPortInstance(int port) {
	Com* p = new Com();
	if (!p->open(port)) {
		delete p;
		return 0;
	}
	return p;
}

extern "C" __declspec(dllexport) int __stdcall readPortValue(Com* com, int address) {
	if (!com) return 100000000;

	com->lock();

	unsigned char hi = (address >> 8) & 0xFF;
	unsigned char lo = (address & 0xFF);

	uchar CMD_READ[] = { 0x01, 0x03, 0, 0, 0, 1, 0, 0 };
	CMD_READ[2] = hi;
	CMD_READ[3] = lo;

	CRC16_package(CMD_READ, sizeof(CMD_READ));
	if (!com->write(CMD_READ, sizeof(CMD_READ))) {
		com->unlock();
		return 100000000;
	}

	unsigned char recv[7] = { 0 };
	int relen = 0;
	if ((relen = com->read(recv, sizeof(recv))) == sizeof(recv)) {
		com->unlock();
		if (checkCRC16(recv, sizeof(recv))) {
			unsigned short hi = recv[3];
			unsigned short lo = recv[4];
			short value = (hi << 8) | lo;
			return value;
		}
		else {
			return 100000001;
		}
	}
	else {
		com->unlock();
		return 100000002;
	}
}

extern "C" __declspec(dllexport) void __stdcall releasePortInstance(Com* com) {
	if (com) {
		delete com;
	}
}