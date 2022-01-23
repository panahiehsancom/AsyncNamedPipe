#include "ClientWritePipe.h"
 

ClientWritePipe::ClientWritePipe(std::string address, const unsigned int buffer_size) : address_(address), buffer_size_(buffer_size)
{
	is_connected_ = false;
	 
}

bool ClientWritePipe::init()
{
	is_connected_ = false;
	return true;
}

bool ClientWritePipe::connect()
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
	return is_connected_;
}

bool ClientWritePipe::is_connected()
{
	return is_connected_;
}

bool ClientWritePipe::disconnect()
{
	CloseHandle(pipe_);
	is_connected_ = false; 
	return is_connected_;
}

boost::signals2::connection ClientWritePipe::connect_on_data_received(std::function<void(const char* data, unsigned int size)> func)
{
	return on_data_received_connection_.connect(func);
}

bool ClientWritePipe::send(const char* data, size_t size)
{
	DWORD dwWritten; 
	bool state = WriteFile(pipe_, data, size, &dwWritten, NULL);
	return state;
}
 