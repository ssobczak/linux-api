/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#ifndef DISPATCHSERVER_H_
#define DISPATCHSERVER_H_

#include "socketserver.h"
#include "workerspool.h"

class DispatchServer: public SocketsServer {
public:
	DispatchServer(WorkersPool& wp);
	virtual ~DispatchServer();

protected:
	virtual void on_new_client(int socket);
	virtual void on_sock_ready(int socket);

private:
	WorkersPool& workers_pool_;

	bool handle_new_client(int socket);
	bool handle_sock_ready(int socket);
};

#endif /* DISPATCHSERVER_H_ */
