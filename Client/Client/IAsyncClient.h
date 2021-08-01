#pragma once

#include <boost/functional.hpp>
#include <boost/signals2.hpp>

/**
 * IAsyncClient is an interface which can handle any async peripheral communication(Network Communication, NamedPipe, etc.)
 */
class IAsyncClient
{
public:
	
	/**
	 * initailize itself and all required objects.
	 * 
	 * \return true if success, otherwise return false 
	 */
	virtual bool init() = 0;
	
	/**
	 * connect to server or target device
	 * 
	 * \return true if success, otherwise return false 
	 */
	virtual bool connect() = 0;
	
	/**
	 * return the state of connection establishment
	 * 
	 * \return true if connected, otherwise return false 
	 */
	virtual bool is_connected() = 0;
	
	/**
	 * disconnect from server
	 * 
	 * \return true if success, otherwise return false 
	 */
	virtual bool disconnect() = 0;
	/**
	 *  anytime client receive anydata this function pointer will be called
	 * 
	 * \param func is function pointer
	 * \return connection of function pointer, you can disconnect receiving notifications by calling disconnect function of boost::signals2::connection object
	 */
	virtual boost::signals2::connection connect_on_data_received(std::function<void(const char* data, unsigned int size)> func) = 0;

	/**
	 * try to send data to server
	 * 
	 * \param data is something you want to send
	 * \param size is size of data
	 * \return true if success, otherwise return false
	 */
	virtual bool send(const char* data, size_t size) = 0;
};
