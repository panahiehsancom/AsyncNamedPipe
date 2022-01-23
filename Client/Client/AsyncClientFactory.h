#pragma once


#include "IAsyncClientFactory.h"
#include "ClientReadPipe.h"
#include "ClientWritePipe.h"
#include "NamedPipeClient.h"

class AsyncClientFactory : public IAsyncClientFactory
{
public:
	AsyncClientFactory();
	// Inherited via IAsyncClientFactory
	std::shared_ptr<IAsyncClient> build(const ClientType type, std::string pipe_address) override;
};

