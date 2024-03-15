#include "chatserver.hpp"
#include "chatservice.hpp"
#include <functional>
#include <string>
#include "json.hpp"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册连接回掉
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息回掉
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量

    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}

// 回掉上报连接相关信息
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{

    //客户端断开连接
    if(!conn->connected()){
            ChatService::instance()->clientCloseException(conn);
            conn->shutdown();
    }

    
}

// 上报读写信息的回掉函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, // 连接
                           Buffer *buffer,               // 缓冲区
                           Timestamp time)
{

    string buf = buffer->retrieveAllAsString();
    LOG_INFO << "buf: " << buf;
    //数据反序列化
    json js = json::parse(buf);

    //目的：解藕网络模块和业务模块代码
    //通过js["msgid"] 获取=》业务handler=》conn js time都传给你

    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());//js对象用get转成整形

    //回调消息绑定好的事件处理器，来执行相应业务处理
    msgHandler(conn, js, time);
    
}
