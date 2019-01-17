#pragma once
#include "defines.h"
#include "ezio/tcp_connection.h"

void lua_push_tcp_connection(lua_State*L,const ezio::TCPConnectionPtr& conn);
void lua_push_ezio_buffer(lua_State*L, ezio::Buffer& buf);



void luaopen_netlib(lua_State* L);


