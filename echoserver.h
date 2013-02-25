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

class EchoServer : public Listenner<SocketEvents, int> {
public:
	EchoServer() {}
	virtual ~EchoServer() {}

	virtual void register_event(const SocketEvents& event, const int& socket);
};

#endif /* ECHOSERVER_H_ */
