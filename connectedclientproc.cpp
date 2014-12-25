#include "connectedclientproc.h"
#include "removethreadfunc.h"
#include "connected_client_data_t.h"
#include "httpstatistics.h"

#include <iostream>
#include <sstream>

namespace HttpSniffer
{

ConnectedClientProc::ConnectedClientProc(void* data) :
    _id(static_cast<connected_client_data_t*>(data)->client_id),
    _remove_thread_func(static_cast<connected_client_data_t*>(data)->remove_thread_func),
    _http_statistics(static_cast<connected_client_data_t*>(data)->http_statistics),
    _client_socket(static_cast<connected_client_data_t*>(data)->client_socket)
{
    //std::cout << "client construcor\n" << std::endl;
}

ConnectedClientProc::~ConnectedClientProc()
{
    //std::cout << "client destrucor\n" << std::endl;
    _client_socket.close();
    _remove_thread_func->exec(_id);
}

void ConnectedClientProc::operator()()
{
    while (true)
    {
        std::string msg = _client_socket.recv();
        if (msg.find("Get event") != string::npos)
        {
            //std::cout << "thread: " << _id << std::endl;
            std::stringstream ss;
            _http_statistics->get(ss);
            std::string msg = ss.str();
            if (!msg.empty())
                _client_socket.send(msg+"\r\n");
            else
                _client_socket.send("No events\r\n");
        }
        else
        {
            //std::cout << "incoming: " << msg << "\n";
        }

        if (_client_socket.is_closed())
        {
            //std::cout << "thread closed\n";
            break;
        }
    }
}

}
