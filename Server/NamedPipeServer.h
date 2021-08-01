#pragma once

#include <Windows.h>
#include <thread>
#include <iostream>

#include "IAsyncServer.h"

class NamedPipeServer : public IAsyncServer
{
public:
	NamedPipeServer(std::wstring namepipe_address, size_t buffer);
	// Inherited via IAsyncServer
	bool init() override;
	bool start() override;
	bool stop() override;
	bool is_running() const override;
	boost::signals2::connection connect_on_client_add(std::function<void(int client_id)> func) override;
	boost::signals2::connection connect_on_client_disconnected(std::function<void(std::shared_ptr<IClientEntity>)> func) override;
	bool send(const char* data, size_t size, int client_id) override;
	bool send(const char* buffer, size_t size, std::shared_ptr<IClientEntity> clientEntity) override;
	boost::signals2::connection connect_on_data_received(std::function<void(std::shared_ptr<IClientEntity>client, const char* data, unsigned int size)> func) override;

private:
	void worker_thread();
	std::wstring namepipe_address_;
	std::shared_ptr<std::thread> receiving_thread_;
	HANDLE pipe_;
	bool is_running_;
	std::string get_last_error();
	boost::signals2::signal<void(int client_id)> client_add_connection_;
	boost::signals2::signal<void(std::shared_ptr<IClientEntity>)> client_disconnected_connection_;
	boost::signals2::signal<void(std::shared_ptr<IClientEntity>client, const char* data, unsigned int size)> on_data_received_connection_;
	size_t buffer_;
};

