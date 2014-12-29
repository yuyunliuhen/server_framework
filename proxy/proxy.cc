/****************************************************************************
 Copyright (c) 2013-2014 King Lee

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "proxy.h"
#include "reactor.h"
#include "proxy_packet_handle.h"

#ifdef __LINUX
#include "easy_dump.h"
#endif // __LINUX

Proxy::Proxy(const easy_char* __host /*= "0.0.0.0"*/,easy_uint32 __port /*= 9876*/ )
	: Server_Impl(Reactor::instance(),__host,__port),host_(__host),port_(__port)
{
#ifdef __LINUX
	signal(SIGSEGV,dump);
	//	when calls send() function twice if peer socket is closed, the SIG_PIPE signal will be trigger. and the SIG_PIPE 's default action is exit process.
	//	just ignore it! if use gdb debug,add 'handle SIGPIPE nostop print' or 'handle SIGPIPE nostop noprint' before run.
	signal(SIGPIPE,SIG_IGN);
#endif // __LINUX
	packet_handle_ = new Proxy_Packet_Handle(this);
}

Proxy::~Proxy()
{
	Reactor::destory();
}

int Proxy::event_loop()
{
	static const easy_int32 __max_time_out = 5000*1000;
	return Reactor::instance()->event_loop(__max_time_out);
}

easy_int32 Proxy::handle_packet( easy_int32 __fd,const std::string& __string_packet )
{
	packet_handle_->handle_packet(__fd,__string_packet);
	return 0;
}

void Proxy::connected( easy_int32 __fd )
{
	//	some schedule algorithm for allocation one level.algorithm may be polling,wight,ip hash,response time and so on.

}

void Proxy::dis_connected( easy_int32 __fd )
{

}

