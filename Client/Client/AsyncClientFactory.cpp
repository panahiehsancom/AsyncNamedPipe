#include "AsyncClientFactory.h"

AsyncClientFactory::AsyncClientFactory()
{
}

std::shared_ptr<IAsyncClient> AsyncClientFactory::build(const ClientType type, std::string pipe_address)
{
	if (type == ClientType::FullDuplex)
		return std::make_shared<NamedPipeClient>(pipe_address, 2048);
	if (type == ClientType::Read)
		return std::make_shared<ClientReadPipe>(pipe_address, 2048);
	if (type == ClientType::Write)
		return std::make_shared<ClientWritePipe>(pipe_address, 2048);
	return std::shared_ptr<IAsyncClient>();
}
