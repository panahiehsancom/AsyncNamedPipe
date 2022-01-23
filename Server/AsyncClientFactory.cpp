 #include "AsyncClientFactory.h"

AsyncClientFactory::AsyncClientFactory()
{
}
 
std::shared_ptr<IClientEntity> AsyncClientFactory::build(const ClientType type, void * connect_handler,const std::string id, const bool connected)
{
	if (type == ClientType::VirtualNamedPipe)
	{ 
		return std::make_shared<VirtualClientNamedPipe>(connect_handler, id, connected);
	}
	return nullptr;
}
