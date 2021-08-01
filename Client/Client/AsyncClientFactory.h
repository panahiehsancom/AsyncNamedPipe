#pragma once

#include "IAsyncClientFactory.h"

class AsyncClientFactory : public IAsyncClientFactory
{
public:
	AsyncClientFactory(std::wstring pipe_address);
	// Inherited via IAsyncClientFactory
	std::shared_ptr<IAsyncClient> build(const ClientType type) override;
private:

	std::wstring pipe_address_;
};

