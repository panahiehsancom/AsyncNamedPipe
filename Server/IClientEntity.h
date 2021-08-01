#pragma once

#include <boost/signals2.hpp>

/**
 * I use this interface for receiving and sending any data to client. it can be used with most of server and client communications, 
 * like Network Communications, Serial Ports, NamedPipes, etc.
 * 
 */
class IClientEntity
{
public:
	/**
	 * show the conectivity status of client
	 * 
	 * \return true if connected otherwise return false
	 */
	virtual bool is_connected() const = 0;
	/**
	 * send all char * data in one or multi interupt to end-user
	 * 
	 * \param data is all data you need to be send
	 * \param size of data 
	 * \return if send success return true otherwise return false.
	 */
	virtual bool send(const char* data, const unsigned int size) = 0;
	/**
	 * disconnect the connection
	 * 
	 * \return if disconnecting finished it return true otherwise return false/
	 */
	virtual bool disconnect() = 0;
	/**
	 * return client id which it should be unique between all clients
	 * 
	 * \return string of id it can be (GUID, incremental int,...)
	 */
	virtual std::string get_client_id() const = 0;

	virtual ~IClientEntity() {}//! virtual interface destructor
};