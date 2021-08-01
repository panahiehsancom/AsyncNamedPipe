#pragma once

#include <string>
#include <Windows.h>

#include "IClientEntity.h"

class VirtualClientNamedPipe : public IClientEntity
{
public:
	VirtualClientNamedPipe(HANDLE pipe, std::string id, bool isconnected);
	// Inherited via IClientEntity
	bool is_connected() const;
	bool send(const char* data, const unsigned int size) override;
	bool disconnect() override;
	std::string get_client_id() const override;
private:
	HANDLE pipe_;
	std::string id_; 
	bool is_connected_;

};

