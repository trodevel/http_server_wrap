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

// $Revision: 9794 $ $Date:: 2018-10-08 #$ $Author: serge $

#ifndef HTTP_SERVER_WRAP__SERVER_H
#define HTTP_SERVER_WRAP__SERVER_H

#include <thread>                           // std::thread

#include "config.h"                         // Config

#include "threcon/i_controllable.h"         // IControllable
#include "restful_interface/i_handler.h"    // restful_interface::IHandler

namespace http_server_wrap {

class Server: public restful_interface::IHandler, public virtual threcon::IControllable
{
public:
    struct HttpsServerWrap;

public:

    Server();
    ~Server();

    bool init(
            const Config                & cfg,
            uint32_t                    log_id,
            restful_interface::IHandler * handler );

    void start();

    // interface threcon::IControllable
    bool shutdown() override;

    // interface restful_interface::IHandler
    const std::string handle( restful_interface::method_type_e type, const std::string & path, const std::string & body, const std::string & origin ) override;

private:

    void thread_func();
    void init_handlers();

private:

    uint32_t            log_id_;
    IHandler            * handler_;

    HttpsServerWrap     * server_;  // need to use raw-pointer instead of std::unique_ptr because the latter one needs to know the definition of the object, 18a08

    std::thread         worker_;
};

} // namespace http_server_wrap

#endif // HTTP_SERVER_WRAP__SERVER_H
