// Server.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "Injector.h"

#define Async 

std::shared_ptr<IAsyncServer> read_server_; 
std::shared_ptr<IAsyncServer> server_;
 
int counter_ = 0;

void data_received(std::shared_ptr<IClientEntity> client, const char* data, size_t size)
{
	std::cout << data;
	std::string temp = std::to_string(counter_) +"\r\n";
	server_->send(temp.data(), temp.size(), 0);
	counter_++;
}

int main()
{
#ifndef Async
	std::shared_ptr<Injector> injector = std::make_shared<Injector>(); 
	server_ = injector->create_named_pipe_server(L"\\\\.\\pipe\\pipe", 2048);
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
#endif
#ifdef Async
	std::shared_ptr<Injector> injector = std::make_shared<Injector>();
	read_server_ = injector->create_read_named_pipe_server(L"\\\\.\\pipe\\rpipe", 2048);
	server_ = injector->create_write_named_pipe_server(L"\\\\.\\pipe\\wpipe", 2048);
	read_server_->connect_on_data_received(std::bind(data_received, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	bool init_state = read_server_->init();
	if (!init_state)
	{
		std::cout << "cannot initialize read pip server" << std::endl;
		return -1;
	}
	init_state = server_->init();
	if (!init_state)
	{
		std::cout << "cannot initialize write pip server" << std::endl;
		return -1;
	}
	std::cout << "both read&write server initialized" << std::endl;
	bool start_state = read_server_->start();
	if (!start_state)
	{
		std::cout << "cannot start read named pip server" << std::endl;
		return -1;
	}
	start_state = server_->start();
	if (!start_state)
	{
		std::cout << "cannot start write named pip server" << std::endl;
		return -1;
	}
	std::cout << "server started" << std::endl;
	while (server_->is_running() || read_server_->is_running())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#endif

}

