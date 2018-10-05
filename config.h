/*

Simple-Web-Server Wrapper. Config.

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

// $Revision: 9778 $ $Date:: 2018-10-05 #$ $Author: serge $

#ifndef HTTP_SERVER_WRAP__CONFIG_H
#define HTTP_SERVER_WRAP__CONFIG_H

#include <string>                   // std::string
#include <cstdint>                  // std::uint32_t

namespace http_server_wrap {

struct Config
{
    std::string     server_cert;
    std::string     server_key;
    uint16_t        port;
    uint32_t        max_threads;
    uint32_t        max_clients;
};

} // namespace http_server_wrap

#endif // HTTP_SERVER_WRAP__CONFIG_H
