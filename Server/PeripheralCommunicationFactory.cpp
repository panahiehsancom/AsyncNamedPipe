#include "PeripheralCommunicationFactory.h"

PeripheralCommunicationFactory::PeripheralCommunicationFactory(std::wstring named_pipe_address, unsigned int buffer_size)
	: named_pipe_address_(named_pipe_address), buffer_size_(buffer_size)
{
}

std::shared_ptr<IAsyncServer> PeripheralCommunicationFactory::build(CommunicationType communication)
{
	if (communication == ICommunicationFactory::CommunicationType::NamedPipeServer)
	{
		std::shared_ptr<IAsyncClientFactory> client_factory = std::make_shared<AsyncClientFactory>();
		return std::make_shared<NamedPipeServer>(named_pipe_address_, buffer_size_, client_factory);
	}
	return nullptr;
}
