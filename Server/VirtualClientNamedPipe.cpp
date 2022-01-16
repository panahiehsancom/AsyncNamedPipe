#include "VirtualClientNamedPipe.h"

VirtualClientNamedPipe::VirtualClientNamedPipe(HANDLE pipe, std::string id, bool isconnected):pipe_(pipe), id_(id), is_connected_(isconnected)
{
}

bool VirtualClientNamedPipe::is_connected() const
{
	return is_connected_;
}

bool VirtualClientNamedPipe::send(const char* data, const unsigned int size)
{
	if (is_connected_)
	{
		DWORD dwWritten;
		bool bSuccess = WriteFile(pipe_, data, size, &dwWritten, NULL); 
		return bSuccess;
	}
	else
		return false;
}

bool VirtualClientNamedPipe::disconnect()
{
	is_connected_ = false;
	return is_connected_;
}

std::string VirtualClientNamedPipe::get_client_id() const
{
	return id_;
}
