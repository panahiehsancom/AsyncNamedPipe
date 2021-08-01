#pragma once

#include "IAsyncClient.h"

#include "NamedPipeClient.h"

class IAsyncClientFactory {

public:
	enum class ClientType {
		NamedPipe = 0x01,
	};

	virtual std::shared_ptr<IAsyncClient> build(const ClientType type) = 0;

	virtual ~IAsyncClientFactory() {} //! virtual interface destructor
};
