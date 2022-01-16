#pragma once

#include <string>

#include "AsyncClientFactory.h"
#include "ICommunicationFactory.h" 
#include "NamedPipeServer.h"

class PeripheralCommunicationFactory : public ICommunicationFactory
{
public:
	PeripheralCommunicationFactory(std::wstring named_pipe_address, unsigned int buffer_size);
	std::shared_ptr<IAsyncServer> build(CommunicationType communication) override;
private:
	std::wstring named_pipe_address_;
	unsigned int buffer_size_;
};

