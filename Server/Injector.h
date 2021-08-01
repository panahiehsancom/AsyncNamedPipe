#pragma once

#include "PeripheralCommunicationFactory.h"

class Injector
{

public:
	std::shared_ptr<IAsyncServer> create_named_pipe_server(std::wstring name, size_t buffer_size);
};

