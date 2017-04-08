/*
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "UDPSocket.h"
#include "EMW10xxInterface.h"

const SocketAddress udp_server( "172.16.0.181", 10000 );

const int ECHO_SERVER_PORT = 7;

const int BUFFER_SIZE = 4096;
char buffer[BUFFER_SIZE] = {0};

void tcp_send( TCPSocket *tcp, const void *data, nsapi_size_t size )
{
    nsapi_size_t remain = size;
    uint8_t *data_tmp = (uint8_t *)data;

    while( remain )
    {
        remain -= tcp->send( data_tmp, remain );
        data_tmp = (uint8_t *)data + size - remain;
    }
}

void app_mbed_tcp_udp( )
{
    UDPSocket udpsocket;
    TCPSocket tcpsocket;
    EMW10xxInterface wifi_iface;
    nsapi_error_t ns_ret;

    int ret = wifi_iface.connect( "William Xu", "mx099555", NSAPI_SECURITY_WPA_WPA2, 0 );
    if ( ret != 0 ) {
        printf( "\r\nConnection error\r\n" );
        return;
    }
    printf( "\r\nConnection success\r\n" );

    udpsocket.open( &wifi_iface );
    tcpsocket.open( &wifi_iface );

    ns_ret = tcpsocket.connect( SocketAddress("10.0.3.10", 20000) );
    if ( ns_ret != NSAPI_ERROR_OK ) {
        printf( "\r\n TCP Connection error\r\n" );
        return;
    }

//    while(1){
//        //udpsocket.sendto( SocketAddress("10.0.3.10", 10000), buffer, BUFFER_SIZE );
//    }

    while(1)
    {
        tcp_send( &tcpsocket, buffer, BUFFER_SIZE );
    }
}




