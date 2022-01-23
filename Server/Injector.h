#pragma once

#include "PeripheralCommunicationFactory.h"
#include "AsyncClientFactory.h"

class Injector
{

public:
	std::shared_ptr<IAsyncServer> create_named_pipe_server(std::wstring name, size_t buffer_size);
	std::shared_ptr<IAsyncServer> create_read_named_pipe_server(std::wstring name, size_t buffer_size); 
	std::shared_ptr<IAsyncServer> create_write_named_pipe_server(std::wstring name, size_t buffer_size);

	std::shared_ptr<IAsyncClientFactory> create_client_factory();
};

