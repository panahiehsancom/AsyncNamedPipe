#pragma once

#include "IAsyncServer.h"

class ICommunicationFactory {

public:

	enum class CommunicationType
	{
		None = 0x00,
		NamedPipeServer = 0x01,
		TCPServer = 0x02
	};

	virtual std::shared_ptr<IAsyncServer> build(CommunicationType communication) = 0;

};
