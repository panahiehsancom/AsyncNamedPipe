#pragma once

#include "IAsyncClientFactory.h"
#include "VirtualClientNamedPipe.h"

class AsyncClientFactory : public IAsyncClientFactory
{
public:
	AsyncClientFactory();
	// Inherited via IAsyncClientFactory
	std::shared_ptr<IClientEntity> build(const ClientType type, void * connect_handler, const std::string id, const bool connected) override;
private:

};

