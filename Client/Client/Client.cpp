
#include <ctime>
#include <iostream>
#include <thread>


#include "AsyncClientFactory.h"

std::string convert_standard_time(int year, int month, int day, int hour, int minute, int second);
std::string get_now();
void data_received(const char* data, unsigned int size)
{
	std::string str_rec_data(data, size);
	std::cout << "data received :" << str_rec_data << std::endl;
}

int main()
{

	std::shared_ptr<AsyncClientFactory> factory = std::make_shared<AsyncClientFactory>(L"\\\\.\\pipe\\pipe");
	std::shared_ptr<IAsyncClient> client = factory->build(IAsyncClientFactory::ClientType::NamedPipe);
	client->connect_on_data_received(std::bind(data_received, std::placeholders::_1, std::placeholders::_2));
	if (client == nullptr)
		return -1;
	bool init_state = client->init();
	if (!init_state)
	{
		std::cout << "initialization of client failed" << std::endl;
		return -1;
	}
	bool connect_state = client->connect();
	if (!connect_state)
	{
		std::cout << "cannot connect to named pip server" << std::endl;
		return -1;
	}
	while (client->is_connected())
	{
		std::string time = get_now();
		client->send(time.c_str(), time.size());
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
    return 0;
}

std::string convert_standard_time(int year, int month, int day, int hour, int minute, int second)
{
	std::string stryear = std::to_string(year);
	std::string strmonth = std::to_string(month);
	std::string strday = std::to_string(day);
	std::string strhour = std::to_string(hour);
	std::string strminute = std::to_string(minute);
	std::string strsecond = std::to_string(second);

	if (strmonth.size() == 1)
		strmonth = "0" + strmonth;

	if (strday.size() == 1)
		strday = "0" + strday;

	if (strhour.size() == 1)
		strhour = "0" + strhour;

	if (strminute.size() == 1)
		strminute = "0" + strminute;

	if (strsecond.size() == 1)
		strsecond = "0" + strsecond;


	std::string time = stryear + "-" + strmonth + "-" + strday + " " + strhour + ":" + strminute + ":" + strsecond;
	return time;

}
std::string get_now()
{
	auto start = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(start);
	struct tm* tmp = localtime(&now_time);

	return convert_standard_time(tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
}
