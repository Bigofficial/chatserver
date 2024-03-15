
/*
    muduo网络库提供两个类
    TcpServer: 服务器
    TcpClient: 客户端

  其实就是epoll+线程池封装
  关注用户连接和断开， 用户可读可写事件，具体由框架实现

*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders; //占位符
/*
1.//组合TCPSERVER对象
2.//创建EL事件循环指针
3.明确TCPSERVER构造需要什么参数
4.在当前类构造函数中，注册处理连接回掉函数，和处理读写事件的回掉函数
5.设置合适的服务端线程数量，muduo会自己划分io线程和worker线程
*/

class ChatServer{
public:
    ChatServer(EventLoop* loop, //事件循环 reactor反应队列
                const InetAddress &listenAddr, //ip+port
                const string &nameArg) //服务器名字
                : _server(loop, listenAddr, nameArg), _loop(loop)
    {
            //给服务器注册用户连接断开回掉 （不知道什么时候发生，用回掉
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1)); //用了成员方法，要绑定器绑定,又因为这个oc方法还有个this参数所以要加this,又因为有一个参数，加上_1
           
            //给服务器注册用户读写事件回掉
            _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

            //设置服务器端线程数量 1个IO，3个worker线程
            _server.setThreadNum(4); //2的话就一个监听连接，一个干活
    }

        //开启事件循环
    void start(){
        _server.start();
    }
private:
    //处理用户连接创建和断开 epoll listenfd accpet
    void onConnection(const TcpConnectionPtr& conn){
        
                if(conn->connected()){
                    cout << conn->peerAddress().toIpPort() << " -> " <<
                        conn->localAddress().toIpPort() << " state:online" <<endl;
                }
                else{
                    cout << conn->peerAddress().toIpPort() << " ->" <<
                        conn->localAddress().toIpPort() << "state:offline" <<endl;
                        conn->shutdown(); //close(fd)
                        // _loop->quit();
                }
    }
    //处理用户读写事件
    void onMessage(const TcpConnectionPtr &conn,//连接
                    Buffer *buffer,//缓冲区
                    Timestamp time){//接收数据时间信息
        string buf = buffer->retrieveAllAsString();
        cout<<"recv data:" << buf << " time:" << time.toString() << endl;
        conn->send(buf);
    }


    TcpServer _server;
    EventLoop *_loop;


};


int main(){
    EventLoop loop; //epoll
    InetAddress addr("127.0.0.1", 6000);

    ChatServer server(&loop, addr, "ChatServer");
    server.start();//listenfd epoll_ctl=>epoll
    loop.loop(); //epoll wait 以阻塞方式，等待用户连接，已连接用户读写事件等

    return 0;
}