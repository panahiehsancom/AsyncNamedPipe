#include "ServerPipeRead.h" 


ServerPipeRead::ServerPipeRead(std::wstring namepipe_address, size_t buffer, std::shared_ptr<IAsyncClientFactory> client_factory) :
	namepipe_address_(namepipe_address), buffer_size_(buffer), client_factory_(client_factory)
{
	is_running_ = false;
}

bool ServerPipeRead::init()
{
	pipe_ = CreateNamedPipe(namepipe_address_.c_str(),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		buffer_size_,
		buffer_size_,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	if (pipe_ == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

bool ServerPipeRead::start()
{
	is_running_ = true;
	thread_group_.create_thread(std::bind(&ServerPipeRead::worker_thread, this));
	return is_running_;
}

bool ServerPipeRead::stop()
{
	is_running_ = false;
	thread_group_.join_all();
	return true;
}

bool ServerPipeRead::is_running() const
{
	return is_running_;
}

boost::signals2::connection ServerPipeRead::connect_on_client_add(std::function<void(int client_id)> func)
{
	return client_add_connection_.connect(func);
}

boost::signals2::connection ServerPipeRead::connect_on_client_disconnected(std::function<void(std::shared_ptr<IClientEntity>)> func)
{
	return client_disconnected_connection_.connect(func);
}

bool ServerPipeRead::send(const char* data, size_t size, int client_id)
{ 
	return false;
}

bool ServerPipeRead::send(const char* buffer, size_t size, std::shared_ptr<IClientEntity> clientEntity)
{
	int id = std::atoi(clientEntity->get_client_id().c_str());
	return send(buffer, size, id);
}

boost::signals2::connection ServerPipeRead::connect_on_data_received(std::function<void(std::shared_ptr<IClientEntity>client, const char* data, unsigned int size)> func)
{
	return on_data_received_connection_.connect(func);
}

void ServerPipeRead::worker_thread()
{
	DWORD dwRead;
	std::vector<char> buffer(buffer_size_);
	while (pipe_ != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(pipe_, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(pipe_, buffer.data(), sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				if (current_client_ == nullptr && client_factory_ != nullptr)
				{
					current_client_ = client_factory_->build(IAsyncClientFactory::ClientType::VirtualNamedPipe, pipe_, "unknown", true);
				}
				on_data_received_connection_(current_client_, buffer.data(), dwRead);
			}
		}
		DisconnectNamedPipe(pipe_);
	}
}

std::string ServerPipeRead::get_last_error()
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
