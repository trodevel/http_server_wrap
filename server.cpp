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

// $Revision: 9788 $ $Date:: 2018-10-05 #$ $Author: serge $

#include "server.h"             // self

#include "utils/dummy_logger.h" // dummy_log
#include "utils/set_this_thread_name.h"      // set_this_thread_name()

namespace http_server_wrap {

Server::Server():
    log_id_( 0 ),
    handler_( nullptr )
{
}

bool Server::init(
        const Config                & cfg,
        uint32_t                    log_id,
        restful_interface::IHandler * handler )
{
    assert( handler );

    log_id_                 = log_id;
    handler_                = handler;

    server_.reset( new HttpsServer( cfg.server_cert, cfg.server_key ) );

    server_->config.port                = cfg.port;
    server_->config.thread_pool_size    = cfg.max_threads;

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

    server_->stop();

    worker_.join();

    return true;
}

void Server::thread_func()
{
    utils::set_this_thread_name( "http_serv" );

    server_->start();
}

const std::string Server::handle( restful_interface::method_type_e type, const std::string & path, const std::string & body, const std::string & origin )
{
    return handler_->handle( type, path, body, origin );
}

} // namespace http_server_wrap
