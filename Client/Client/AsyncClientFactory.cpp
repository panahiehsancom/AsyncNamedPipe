#include "AsyncClientFactory.h"

AsyncClientFactory::AsyncClientFactory(std::wstring pipe_address):pipe_address_(pipe_address)
{
}

std::shared_ptr<IAsyncClient> AsyncClientFactory::build(const ClientType type)
{
	if (type == ClientType::NamedPipe)
		return std::make_shared<NamedPipeClient>(pipe_address_, 2048);
	return std::shared_ptr<IAsyncClient>();
}
