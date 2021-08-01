#pragma once

#include "IAsyncClientFactory.h"

class AsyncClientFactory : public IAsyncClientFactory
{
public:
	AsyncClientFactory(std::wstring pipe_address);
	// Inherited via IAsyncClientFactory
	std::shared_ptr<IClientEntity> build(const ClientType type, void * connect_handler, const std::string id, const bool connected) override;
private:

	std::wstring pipe_address_;
};

