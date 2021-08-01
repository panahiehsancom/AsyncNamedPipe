#include "AsyncClientFactory.h"

AsyncClientFactory::AsyncClientFactory(std::wstring pipe_address):pipe_address_(pipe_address)
{
}

std::shared_ptr<IClientEntity> AsyncClientFactory::build(const ClientType type, void * connect_handler,const std::string id, const bool connected)
{
	if (type == ClientType::VirtualNamedPipe)
	{
		HANDLE* handle = reinterpret_cast<HANDLE*>(connect_handler);
		HANDLE converted_handler = *handle;
		return std::make_shared<VirtualClientNamedPipe>(converted_handler, id, connected);
	}
	return nullptr;
}
