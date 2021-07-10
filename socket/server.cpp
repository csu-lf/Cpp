/**
 * @file sever.cpp
 * @author csu-lf (csu.lifeng@qq.com)
 * @brief socket.hpp 测试文件，作为服务端，特别是多线程应用测试，查看CSocket对多线程编程兼容性如何
 * @version 0.1
 * @date 2021-07-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "socket.hpp"
#include <thread>
#include "fd.hpp"


int thread_main();


class CFd *fds = new (std::nothrow) CFd(5);
class CSocket *server_socket = new CSocket("tcp");

int main(int argc, char const *argv[])
{
    if(fds == NULL || fds->Len() == -1 )
    {
        printf("内存不足\n");
        server_socket->Close();
        return -1;
    }

    server_socket->Bind(1010);
    server_socket->Listen();

    std::thread *my_thread = new std::thread(thread_main);
    my_thread->detach();
    while(1);

    server_socket->Close();

    return 0;
}

int thread_main()
{
    int a[100];
    for(int i = 0; i < 100; i++)
    {
        a[i] = i;
    }
    printf("new thread\n");
    int _socket = server_socket->Accept();
    fds->Add(_socket);
    class CSocket *client_socket = new CSocket(_socket);
    printf("link\n");
    std::thread *my_thread_child = new std::thread(thread_main);
    my_thread_child->detach();
    if(client_socket->Socket() < 0)
    {
        printf("accept erro\n");
        printf("thread exit\n");
        fds->Delete(_socket);
        return -1;
    }

    while(client_socket->IsConnect())
    {
        if(fds->Poll(fds->Find(_socket), POLL_READ, 0))
        {
            client_socket->Recv();
            printf("recv: %s\n", client_socket->Buf());
        }
        else
            printf("unblock no recv\n");

        sleep(3);
    }
    client_socket->Close();
    printf("thread exit\n");
    fds->Delete(_socket);

    return 0;
}






