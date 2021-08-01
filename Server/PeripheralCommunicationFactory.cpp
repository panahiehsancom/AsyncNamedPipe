#include "PeripheralCommunicationFactory.h"

PeripheralCommunicationFactory::PeripheralCommunicationFactory(std::wstring named_pipe_address, unsigned int buffer_size)
	: named_pipe_address_(named_pipe_address), buffer_size_(buffer_size)
{
}

std::shared_ptr<IAsyncServer> PeripheralCommunicationFactory::build(CommunicationType communication)
{
	if (communication == ICommunicationFactory::CommunicationType::NamedPipeServer)
		return std::make_shared<NamedPipeServer>(named_pipe_address_, buffer_size_);
	return nullptr;
}
