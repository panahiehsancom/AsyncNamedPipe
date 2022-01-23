#pragma once

#include "IAsyncClient.h"
 

class IAsyncClientFactory {

public:
	enum class ClientType {
		FullDuplex = 0x01,
		Write = 0x02,
		Read = 0x03,
	};

	virtual std::shared_ptr<IAsyncClient> build(const ClientType type, std::string pipe_address) = 0;

	virtual ~IAsyncClientFactory() {} //! virtual interface destructor
};
