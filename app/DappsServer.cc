#include "DappsServer.h"
#include "DappsSocket.h"
#include <cstdlib>
#include <iostream>

dapps::DappsServer::DappsServer(dapps::Dapps* _app)
{
	m_app = _app;
	m_loop = NULL;
	m_server = NULL;
	init();
	std::cout << "init complete" <<std::endl;
}

void dapps::DappsServer::init()
{
	if(m_server != NULL)
	{
		// Already initialized.
		return;
	}
	m_loop = uv_default_loop();
	m_server = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
	uv_tcp_init(m_loop, m_server);
	m_server->data = (void*) this;
	
	std::string ipAddress = m_app->config->getConfig()->get("server")->getString("host");
	int64_t port = m_app->config->getConfig()->get("server")->getInt("port");
	
	uv_ip4_addr(ipAddress.c_str(), port, &m_addr);
	uv_tcp_bind(m_server, (const sockaddr*)&m_addr, 0);
	int r = uv_listen((uv_stream_t*) m_server, 1, DappsServer::onNewConnection);
	
	if(r)
	{
		std::cout << "something went wrong" << std::endl;
	}
}

void dapps::DappsServer::onNewConnection(uv_stream_t* server, int status)
{
	if(status < 0)
	{
		std::cout << "New Connection error::" << uv_strerror(status) << std::endl;
		return;
	}
	DappsServer* _this = (DappsServer*) server->data;
	if(_this == NULL)
	{
		std::cout << "No context passed to new connection" << std::endl;
		DappsServer::rejectIncomingConnection(server);
		return;
	}
	DappsSocket* _socket = new DappsSocket(_this);
}

void dapps::DappsServer::rejectIncomingConnection(uv_stream_t* server)
{
	uv_tcp_t* _client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
	uv_tcp_init(server->loop, _client);
	if (uv_accept(server, (uv_stream_t*) _client) == 0)
	{
		uv_close((uv_handle_t*)_client, onRejectConnection);
	}
}

void dapps::DappsServer::onRejectConnection(uv_handle_t* _handle)
{
	std::cout << "Rejecting connection" <<std::endl;
	free(_handle);
}

dapps::Dapps* dapps::DappsServer::getApp() 
{
	return m_app;
}

uv_loop_t* dapps::DappsServer::getLoop() 
{
	return m_loop;
}

uv_tcp_t* dapps::DappsServer::getServer()
{
	return m_server;
}
