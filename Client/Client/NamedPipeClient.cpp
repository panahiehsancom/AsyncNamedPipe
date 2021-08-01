#include "NamedPipeClient.h"

NamedPipeClient::NamedPipeClient(std::wstring address,const unsigned int buffer_size) : address_(address), buffer_size_(buffer_size)
{
	is_connected_ = false;
}

bool NamedPipeClient::init()
{
	is_connected_ = false;
	return true;
}

bool NamedPipeClient::connect()
{
	while (true)
	{
		pipe_ = CreateFile(
			address_.c_str(),			// Pipe name 
			GENERIC_READ |			// Read and write access 
			GENERIC_WRITE,
			0,						// No sharing 
			NULL,					// Default security attributes
			OPEN_EXISTING,			// Opens existing pipe 
			0,						// Default attributes 
			NULL);					// No template file 

		// Break if the pipe handle is valid. 
		if (pipe_ != INVALID_HANDLE_VALUE)
			break;

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			//Could not open pipe call GetLastError();
			return false;
		}

		// All pipe instances are busy, so wait for 20 seconds.  
		if (!WaitNamedPipe(address_.c_str(), 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return false;
		}

		
	}

	// Set data to be read from the pipe as a stream of messages
	DWORD dwMode = PIPE_TYPE_BYTE;
	BOOL bResult = SetNamedPipeHandleState(pipe_, &dwMode, NULL, NULL);
	if (!bResult)
	{
		// "SetNamedPipeHandleState failed GetLastError());
		return false;
	} 
	is_connected_ = true;
	receiving_thread_ = std::make_shared<std::thread>(std::bind(&NamedPipeClient::worker_thread, this));
	return is_connected_;
}

bool NamedPipeClient::is_connected()
{
	return is_connected_;
}

bool NamedPipeClient::disconnect()
{
	CloseHandle(pipe_);
	is_connected_ = false;
	return is_connected_;
}

boost::signals2::connection NamedPipeClient::connect_on_data_received(std::function<void(const char* data, unsigned int size)> func)
{
	return on_data_received_connection_.connect(func);
}

bool NamedPipeClient::send(const char* data, size_t size)
{ 
	DWORD cbBytesWritten = 0, cbRequestBytes = 0;
	DWORD cbBytesRead = 0, cbReplyBytes = 0;

	 
	bool bResult = WriteFile(			// Write to the pipe.
		pipe_,						// Handle of the pipe
		data,					// Message to be written
		size,				// Number of bytes to write
		&cbBytesWritten,			// Number of bytes written
		NULL);						// Not overlapped 

	if (!bResult/*Failed*/ || cbRequestBytes != cbBytesWritten/*Failed*/)
	{
		//_tprintf(_T("WriteFile failed w/err 0x%08lx\n"), GetLastError());
		return false;
	}

	//_tprintf(_T("Sends %ld bytes; Message: \"%s\"\n"), cbBytesWritten, chRequest);
	return true;
}

void NamedPipeClient::worker_thread()
{
	char chReply[2048];		// Server -> Client
	DWORD cbBytesRead = 2048, cbReplyBytes = 0;
	std::string client_id = "1";
	do
	{
		bool bResult = ReadFile(			// Read from the pipe.
			pipe_,					// Handle of the pipe
			chReply,				// Buffer to receive the reply
			cbReplyBytes,			// Size of buffer 
			&cbBytesRead,			// Number of bytes read 
			NULL);					// Not overlapped 

		if (!bResult && GetLastError() != ERROR_MORE_DATA)
		{
			//_tprintf(_T("ReadFile failed w/err 0x%08lx\n"), GetLastError());
			disconnect();
			break;
		}
		std::string input(chReply, cbReplyBytes);

		on_data_received_connection_(chReply, cbReplyBytes);
		//_tprintf(_T("Receives %ld bytes; Message: \"%s\"\n"),	cbBytesRead, chReply);

	} while (is_connected_);  // Repeat loop if ERROR_MORE_DATA 
}
