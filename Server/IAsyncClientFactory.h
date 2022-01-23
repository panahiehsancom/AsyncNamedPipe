#pragma once

#include "IClientEntity.h"

#include "VirtualClientNamedPipe.h"
 
class IAsyncClientFactory {

public:
	enum ClientType { 
		VirtualNamedPipe =0x01,
		//VirtualTCPClient =0x02,
		//VirtualUDPClient = 0x03,
		//VirtualSerialPortClient = 0x04
	};

	virtual std::shared_ptr<IClientEntity> build(const ClientType type, void* connect_handler, const std::string id, const bool connected) = 0;

	virtual ~IAsyncClientFactory() {} //! virtual interface destructor
};
