/*	libscratch - Multipurpose objective C++ library.

		Copyright (c) 2013 - 2016 Angelo Geels <spansjh@gmail.com>

		Permission is hereby granted, free of charge, to any person
		obtaining a copy of this software and associated documentation
		files (the "Software"), to deal in the Software without
		restriction, including without limitation the rights to use,
		copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the
		Software is furnished to do so, subject to the following
		conditions:

		The above copyright notice and this permission notice shall be
		included in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
		OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
		NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
		HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
		WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
		FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "Common.h"
#include "Stream.hpp"

#ifdef SCRATCH_IMPL
#if WINDOWS
#pragma comment(lib, "wsock32.lib")
#endif

static bool _bWinsockInitialized = false;
#endif

#if !WINDOWS
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netdb.h>
#endif

SCRATCH_NAMESPACE_BEGIN;

class NetworkStream : public Stream
{
public:
#if WINDOWS
	WSADATA* ns_pWSAData;
	SOCKET ns_socket;
#else
	int32_t ns_socket;
#endif
	sockaddr_in* ns_psin;

	bool ns_bEOF;

public:
	NetworkStream();
	~NetworkStream();

	size_t Size();
	size_t Location();
	void Seek(int32_t iPos, int32_t iOrigin);
	bool AtEOF();

	bool Connect(const char* szAddress, uint16_t iPort);
	void Close();
	void Write(const void* p, uint32_t iLen);
	size_t Read(void* pDest, size_t iLen);

	bool IsConnected();

	static void Cleanup();
};

#ifdef SCRATCH_IMPL

NetworkStream::NetworkStream()
{
#if WINDOWS
	ns_pWSAData = new WSADATA;
#endif
	ns_socket = 0;
	ns_psin = new sockaddr_in;

	ns_bEOF = false;

#if WINDOWS
	if (!_bWinsockInitialized) {
		if (WSAStartup(MAKEWORD(2, 2), ns_pWSAData) != 0) {
#ifdef SCRATCH_NO_EXCEPTIONS
			return;
#else
			throw "Couldn't initialize winsock";
#endif
		}
		_bWinsockInitialized = TRUE;
	}
#endif
}

NetworkStream::~NetworkStream()
{
	if (ns_socket != 0) {
#if WINDOWS
		closesocket(ns_socket);
#else
		close(ns_socket);
#endif
	}

#if WINDOWS
	delete ns_pWSAData;
#endif
	delete ns_psin;
}

size_t NetworkStream::Size()
{
#ifdef SCRATCH_NO_EXCEPTIONS
	return 0;
#else
	throw "Function not supported in Network Stream";
#endif
}

size_t NetworkStream::Location()
{
#ifdef SCRATCH_NO_EXCEPTIONS
	return 0;
#else
	throw "Function not supported in Network Stream";
#endif
}

void NetworkStream::Seek(int32_t iPos, int32_t iOrigin)
{
#ifndef SCRATCH_NO_EXCEPTIONS
	throw "Function not supported in Network Stream";
#endif
}

bool NetworkStream::AtEOF()
{
	return ns_bEOF;
}

bool NetworkStream::Connect(const char* szAddress, uint16_t iPort)
{
	hostent* phe = gethostbyname(szAddress);
	if (phe == nullptr) {
		return false;
	}

	ns_psin->sin_family = AF_INET;
	ns_psin->sin_addr.s_addr = *(uint32_t*)phe->h_addr_list[0];
	ns_psin->sin_port = htons(iPort);

	ns_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(ns_socket, (sockaddr*)ns_psin, sizeof(sockaddr_in)) == 0) {
		return true;
	}

	return false;
}

void NetworkStream::Close()
{
#if WINDOWS
	closesocket(ns_socket);
#else
	close(ns_socket);
#endif
	ns_socket = 0;
}

void NetworkStream::Write(const void* p, uint32_t iLen)
{
	send(ns_socket, (const char*)p, iLen, 0);
}

size_t NetworkStream::Read(void* pDest, size_t iLen)
{
	int iRet = recv(ns_socket, (char*)pDest, iLen, 0);
	ns_bEOF = (iRet <= 0) || ((uint32_t)iRet < iLen);
	return iRet;
}

bool NetworkStream::IsConnected()
{
#if WINDOWS
	fd_set fds;
	fds.fd_array[0] = ns_socket;
	fds.fd_count = 1;
	return select(0, &fds, &fds, &fds, nullptr) == 1;
#else
	return true; //TODO: FIXME
#endif
}

void NetworkStream::Cleanup()
{
#if WINDOWS
	if (_bWinsockInitialized) {
		WSACleanup();
		_bWinsockInitialized = false;
	}
#endif
}

#endif

SCRATCH_NAMESPACE_END;
