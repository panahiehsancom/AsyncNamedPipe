#pragma once

#include <boost/functional.hpp>
#include <boost/signals2.hpp>


#include "IClientEntity.h"
/**
 * this interface can handle most of async communication. I use the boost lib because signaling(calling connected functions) is too simple with it. 
 * implementing this interface can has a map of clients and their id`s. this id`s is continouse and unique. 
 */
class IAsyncServer
{

public:
	/**
	 * initailize itself and all required objects.
	 * 
	 * \return true if success, otherwise return false
	 */
	virtual bool init() = 0;
	/**
	 * start to receiving and sending data from\to clients
	 * 
	 * \return true if success, otherwise return false
	 */
	virtual bool start() = 0;
	/**
	 * stop receiving and sending data by clients
	 * 
	 * \return true if success, otherwise return false
	 */
	virtual bool stop() = 0;
	/**
	 * return the state of server
	 * 
	 * \return true if works, otherwise false
	 */
	virtual bool is_running() const = 0;
	/**
	 * if any clients connected we should give him an ID. this ID is usefull for identify him in continouse process. and call anyobject needed to know
	 * 
	 * \param func function pointer
	 * \return connection of function pointer, you can disconnect receiving notifications by calling disconnect function of boost::signals2::connection object
	 */
	virtual boost::signals2::connection connect_on_client_add(std::function<void(int client_id)> func) = 0;
	/**
	 * send a notification if clients disconnected or one object call disconnect function of IClientEntity
	 * 
	 * \param func function pointer
	 * \return connection of function pointer, you can disconnect receiving notifications by calling disconnect function of boost::signals2::connection object
	 */
	virtual boost::signals2::connection connect_on_client_disconnected(std::function<void(std::shared_ptr<IClientEntity>)> func) = 0;
	/**
	 * send specific data to client by id
	 * 
	 * \param buffer data
	 * \param size of data
	 * \param client_id is id of client
	 * \return true if sending data successfully completed, otherwise return false
	 */
	virtual bool send(const char* data, size_t size, int client_id) = 0;
	/**
	 * send specific data to client by ClientEntity
	 * 
	 * \param buffer data
	 * \param size of data
	 * \param clientEntity is client
	 * \return true if sending data successfully completed, otherwise return false
	 */
	virtual bool send(const char* buffer, size_t size, std::shared_ptr<IClientEntity> clientEntity) = 0;
	/**
	 * whenever server receive data this function will by called and specify which client is sitted behind the line.
	 * 
	 * \param func is function pointer
	 * \return connection of function pointer, you can disconnect receiving notifications by calling disconnect function of boost::signals2::connection object
	 */
	virtual boost::signals2::connection connect_on_data_received(std::function<void(std::shared_ptr<IClientEntity> client, const char * data, unsigned int size)> func) = 0;
};
