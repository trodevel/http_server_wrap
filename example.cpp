/*

Example.

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

// $Revision: 12038 $ $Date:: 2019-09-25 #$ $Author: serge $

#include <thread>                   // std::thread
#include <functional>               // std::bind
#include <iostream>                 // std::cout

#include "utils/mutex_helper.h"     // THIS_THREAD_SLEEP_MS
#include "utils/dummy_logger.h"     // dummy_log_set_log_level
#include "utils/set_this_thread_name.h"     // set_this_thread_name()

#include "server.h"                         // Server
#include "restful_interface/i_handler.h"    // IHandler
#include "restful_interface/str_helper.h"   // StrHelper

class Handler: public virtual restful_interface::IHandler
{
    const std::string handle( restful_interface::method_type_e type, const std::string & path, const std::string & body, const std::string & origin )
    {
        return "request: type " + restful_interface::StrHelper::to_string( type ) + ", path '" + path + "', body '" + body + "', origin " + origin;
    }
};

void dummy_thread( http_server_wrap::Server * server )
{
    utils::set_this_thread_name( "dummy" );

    std::cout << "dummy_thread(): started" << std::endl;

    while( true )
    {

        std::cout << "type 'exit' to exit" << std::endl;

        std::string input;

        std::getline( std::cin, input );

        if( input == "exit" || input == "quit" )
            break;

        THIS_THREAD_SLEEP_MS( 3 );
    }

    server->shutdown();

    std::cout << "dummy_thread(): exited" << std::endl;
}

int main()
{
    std::cout << "Hello, world" << std::endl;

    try
    {
        http_server_wrap::Config cfg = { "server.crt", "server.key", 8080, 5, 2 };

        http_server_wrap::Server s;

        Handler h;

        auto log_id   = dummy_logger::register_module( "http_server_wrap::Server" );

        dummy_logger::set_log_level( log_levels_log4j::Debug );

        s.init( cfg, log_id, & h );

        s.start();

        auto handler_thread = std::thread( std::bind( &dummy_thread, &s ) );

        handler_thread.join();

        std::cout << "exit" << std::endl;
    }
    catch( std::exception& e )
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return 0;
}
