#ifndef CHATSERVER_H

#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
using namespace muduo;
using namespace muduo::net;


//聊天服务器主类
class ChatServer
{

public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg);

    // 启动服务
    void start();

private:
    // 回掉上报连接相关信息
    void onConnection(const TcpConnectionPtr &conn);

    // 上报读写信息的回掉函数
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time);

    TcpServer _server;//组合muduo库，实现服务器功能的类对象
    EventLoop *_loop;//指向事件循环对象的指针
};
#endif