/*

Simple-Web-Server Wrapper.

Copyright (C) 2018 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 9801 $ $Date:: 2018-10-08 #$ $Author: serge $

#include "server.h"             // self

#include "Simple-Web-Server/client_https.hpp"   // Simple-Web-Server
#include "Simple-Web-Server/server_https.hpp"   // Simple-Web-Server
#include "Simple-Web-Server/str_helper.h"       // to_endpoint_string()

#include "utils/dummy_logger.h" // dummy_log
#include "utils/set_this_thread_name.h"      // set_this_thread_name()

namespace http_server_wrap {

typedef SimpleWeb::Server<SimpleWeb::HTTPS> HttpsServer;
typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;

struct Server::HttpsServerWrap
{
    std::shared_ptr<HttpsServer>    w;
};

Server::Server():
    log_id_( 0 ),
    handler_( nullptr ),
    server_( nullptr )
{
}

Server::~Server()
{
    if( server_ )
    {
        delete server_;
        server_ = nullptr;
    }
}

bool Server::init(
        const Config                & cfg,
        uint32_t                    log_id,
        restful_interface::IHandler * handler )
{
    assert( handler );

    log_id_                 = log_id;
    handler_                = handler;

    server_ = new HttpsServerWrap;

    server_->w.reset( new HttpsServer( cfg.server_cert, cfg.server_key ) );

    server_->w->config.port                = cfg.port;
    server_->w->config.thread_pool_size    = cfg.max_threads;

    init_handlers();

    dummy_log_debug( log_id_, "init: ok" );

    return true;
}

void Server::start()
{
    worker_ = std::thread( std::bind( & Server::thread_func, this ) );
}

// interface threcon::IControllable
bool Server::shutdown()
{
    dummy_log_debug( log_id_, "shutdown()" );

    server_->w->stop();

    worker_.join();

    return true;
}

void Server::thread_func()
{
    utils::set_this_thread_name( "http_serv" );

    server_->w->start();
}

void Server::init_handlers()
{
    server_->w->default_resource["GET"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::GET, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["HEAD"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::HEAD, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["POST"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::POST, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["PUT"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::PUT, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["DELETE"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::DELETE, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["CONNECT"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::CONNECT, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["OPTIONS"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::OPTIONS, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["TRACE"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::TRACE, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };

    server_->w->default_resource["PATCH"] = [this]( std::shared_ptr<HttpsServer::Response> response, std::shared_ptr<HttpsServer::Request> request )
    {
        auto content = request->content.string();
        auto res = handler_->handle( restful_interface::method_type_e::PATCH, request->path, content, SimpleWeb::to_endpoint_string( * request ) );
        response->write( res );
    };
}

const std::string Server::handle( restful_interface::method_type_e type, const std::string & path, const std::string & body, const std::string & origin )
{
    return handler_->handle( type, path, body, origin );
}

} // namespace http_server_wrap
