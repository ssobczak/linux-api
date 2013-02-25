/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#ifndef ECHOSERVER_H_
#define ECHOSERVER_H_

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "listenner.h"
#include "socketserver.h"

class EchoServer : public SocketsServer {
public:
	EchoServer() {}
	virtual ~EchoServer() {}

protected:
	virtual void on_new_client(int socket);
	virtual void on_sock_ready(int socket);
};

#endif /* ECHOSERVER_H_ */
