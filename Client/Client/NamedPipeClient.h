#pragma once
#include <thread>
#include <windows.h>

#include "IAsyncClient.h"

class NamedPipeClient : public IAsyncClient
{

	// Inherited via IAsyncClient
public:
	NamedPipeClient(std::wstring address, unsigned int  buffer_size);	
	bool init() override;
	bool connect() override;
	bool is_connected() override;
	bool disconnect() override;
	boost::signals2::connection connect_on_data_received(std::function<void(const char* data, unsigned int size)> func) override;
	bool send(const char* data, size_t size) override;

private:
	void worker_thread();
	std::wstring address_;
	const unsigned int buffer_size_;
	bool is_connected_;
	bool write_command_;
	HANDLE pipe_;
	std::shared_ptr<std::thread> receiving_thread_;
	boost::signals2::signal<void(const char* data, unsigned int size)> on_data_received_connection_;
};

