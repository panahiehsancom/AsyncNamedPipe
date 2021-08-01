// Server.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Injector.h"

std::shared_ptr<IAsyncServer> server_;
int counter_ = 0;
void data_received(std::shared_ptr<IClientEntity> client, const char* data, size_t size)
{
	std::cout << "data received :" << data << std::endl;
	std::string temp = std::to_string(counter_);
	server_->send(temp.data(), temp.size(), client);
	counter_++;
}

int main()
{
	std::shared_ptr<Injector> injector = std::make_shared<Injector>();
	server_ = injector->create_named_pipe_server(L"\\\\.\\pipe\\HelloWorld", 2048);
	server_->connect_on_data_received(std::bind(data_received, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	bool init_state = server_->init();
	if (!init_state)
	{
		std::cout << "cannot initialize named pip server" << std::endl;
		return -1;
	}
	std::cout << "server initialized" << std::endl;
	bool start_state = server_->start();
	if (!start_state)
	{
		std::cout << "cannot start named pip server" << std::endl;
		return -1;
	}
	std::cout << "server started" << std::endl;
	while (server_->is_running())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


}

