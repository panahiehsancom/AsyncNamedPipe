#include "Injector.h"

std::shared_ptr<IAsyncServer> Injector::create_named_pipe_server(std::wstring name, size_t buffer_size)
{
	std::shared_ptr<PeripheralCommunicationFactory> factory = std::make_shared<PeripheralCommunicationFactory>(name, buffer_size);
	return factory->build(ICommunicationFactory::CommunicationType::NamedPipeServer);
}
