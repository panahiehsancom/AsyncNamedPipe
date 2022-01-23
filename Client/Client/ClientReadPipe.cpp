#include "ClientReadPipe.h"

ClientReadPipe::ClientReadPipe(std::string address, const unsigned int buffer_size) : address_(address), buffer_size_(buffer_size)
{
	is_connected_ = false; 
}

bool ClientReadPipe::init()
{
	is_connected_ = false;
	return true;
}

bool ClientReadPipe::connect()
{
	std::wstring waddress(address_.begin(), address_.end());
	pipe_ = CreateFile(waddress.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	// Break if the pipe handle is valid. 
	if (pipe_ == INVALID_HANDLE_VALUE)
		return false;

	is_connected_ = true;
	receiving_thread_ = std::make_shared<std::thread>(std::bind(&ClientReadPipe::worker_thread, this));
	return is_connected_;
}

bool ClientReadPipe::is_connected()
{
	return is_connected_;
}

bool ClientReadPipe::disconnect()
{
	CloseHandle(pipe_);
	is_connected_ = false;
	receiving_thread_->join();
	return is_connected_;
}

boost::signals2::connection ClientReadPipe::connect_on_data_received(std::function<void(const char* data, unsigned int size)> func)
{
	return on_data_received_connection_.connect(func);
}

bool ClientReadPipe::send(const char* data, size_t size)
{
	return false;
}

void ClientReadPipe::worker_thread()
{
	char chReply[2048];		// Server -> Client
	DWORD cbBytesRead = 2048, cbReplyBytes = 0;
	DWORD dwRead;
	do
	{ 
		if (ReadFile(pipe_, chReply, cbBytesRead, &dwRead, NULL) != false)
		{
			if (dwRead > 0)
			{
				on_data_received_connection_(chReply, dwRead);
			}
		}
	} while (is_connected_);  // Repeat loop if ERROR_MORE_DATA 
}
