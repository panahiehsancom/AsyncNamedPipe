#include "NamedPipeServer.h"
#define BUFFER_SIZE		4096 
NamedPipeServer::NamedPipeServer(std::wstring namepipe_address, size_t buffer) : namepipe_address_(namepipe_address), buffer_(buffer)
{
	is_running_ = false;
}

bool NamedPipeServer::init()
{
	SECURITY_ATTRIBUTES sa;
	sa.lpSecurityDescriptor = (PSECURITY_DESCRIPTOR)malloc(
		SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(sa.lpSecurityDescriptor,
		SECURITY_DESCRIPTOR_REVISION);
	// ACL is set as NULL in order to allow all access to the object.
	SetSecurityDescriptorDacl(sa.lpSecurityDescriptor, TRUE, NULL, FALSE);
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;

	pipe_ = CreateNamedPipe(
		namepipe_address_.c_str(), // name of the pipe
		PIPE_ACCESS_DUPLEX, // 1-way pipe -- send only
		PIPE_TYPE_MESSAGE |			// Message type pipe 
		PIPE_READMODE_MESSAGE | PIPE_WAIT,	// Message-read mode , // send data as a byte stream 
		PIPE_UNLIMITED_INSTANCES, // no outbound buffer
		BUFFER_SIZE,				// Output buffer size in bytes
		BUFFER_SIZE,				// Input buffer size in bytes

		NMPWAIT_USE_DEFAULT_WAIT,	// Time-out interval
		&sa							// Security attributes
	);
	if (pipe_ == NULL || pipe_ == INVALID_HANDLE_VALUE) {
		//Failed to create outbound pipe instance."
		//look up error code here using GetLastError()
		std::cout << get_last_error() << std::endl;
		return false;
	}
	return true;
}

bool NamedPipeServer::start()
{
	BOOL result = ConnectNamedPipe(pipe_, NULL);
	if (!result)
	{
		std::cout << get_last_error() << std::endl;
		//look up error code here using GetLastError()
		CloseHandle(pipe_);
		return false;
	}
	is_running_ = true;
	receiving_thread_ = std::make_shared<std::thread>(std::bind(&NamedPipeServer::worker_thread, this));
	return true;
}

bool NamedPipeServer::stop()
{
	is_running_ = false;
	if (receiving_thread_->joinable())
		receiving_thread_->join();
	return true;
}

bool NamedPipeServer::is_running() const
{
	return is_running_;
}

boost::signals2::connection NamedPipeServer::connect_on_client_add(std::function<void(int client_id)> func)
{
	return client_add_connection_.connect(func);
}

boost::signals2::connection NamedPipeServer::connect_on_client_disconnected(std::function<void(std::shared_ptr<IClientEntity>)> func)
{
	return client_disconnected_connection_.connect(func);
}

bool NamedPipeServer::send(const char* data, size_t size, int client_id)
{
	DWORD cbBytesWritten, cbReplyBytes;
	bool bResult = WriteFile(		// Write to the pipe.
		pipe_,					// Handle of the pipe
		data,				// Buffer to write to 
		size,			// Number of bytes to write 
		&cbBytesWritten,		// Number of bytes written 
		NULL);
	if (!bResult)
	{
		std::cout << get_last_error() << std::endl;
	}
	return bResult;
}

bool NamedPipeServer::send(const char* buffer, size_t size, std::shared_ptr<IClientEntity> clientEntity)
{
	int id = std::atoi(clientEntity->get_client_id().c_str());
	return send(buffer, size, id);
}

boost::signals2::connection NamedPipeServer::connect_on_data_received(std::function<void(std::shared_ptr<IClientEntity>client, const char* data, unsigned int size)> func)
{
	return on_data_received_connection_.connect(func);
}

void NamedPipeServer::worker_thread()
{
	char buffer[2048];
	DWORD cbBytesRead, cbRequestBytes;
	while (is_running_)
	{
		cbRequestBytes = buffer_;
		bool bResult = ReadFile(			// Read from the pipe.
			pipe_,					// Handle of the pipe
			buffer,				// Buffer to receive data
			buffer_,			// Size of buffer in bytes
			&cbBytesRead,			// Number of bytes read
			NULL);					// Not overlapped I/O

		if (!bResult/*Failed*/ || cbBytesRead == 0/*Finished*/)
		{
			std::cout << get_last_error() << std::endl;
			is_running_ = false;
		} 
		std::string data(buffer, cbBytesRead);
		on_data_received_connection_(nullptr, data.c_str(), data.size());
	}
}

std::string NamedPipeServer::get_last_error()
{
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "no text error can extract";

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)& messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);
	return message;


}
