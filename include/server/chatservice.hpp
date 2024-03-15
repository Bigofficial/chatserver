#ifndef CHATSERVICE_H
#define CHATSERVICE_H
#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "json.hpp"
#include "redis.hpp"

#include <mutex>

using json = nlohmann::json;
using namespace std;
using namespace muduo::net;
using namespace muduo;
using namespace placeholders;
// 处理消息事件回掉方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 单例模式设计这个
// 聊天服务器业务类
class ChatService
{

public:
    // 获取单例对象的接口函数
    static ChatService *instance();
    // 处理登入业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 获取消息对应处理器
    MsgHandler getHandler(int msgid);

    //处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    //一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    //服务器异常退出，业务重置方法
    void reset();

    //添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

        // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
    
private:
    ChatService();

    // 存储消息id和其对应事件业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    //数据操作类对象
    UserModel _userModel;

    //存储在线用户通信连接 这个可能多线程访问，这个是个单例，注意线程安全 onMessage本身就是在不同线程中回调
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    //定义互斥锁，保护userconnmap线程安全
    mutex _connMutex;

    //离线消息操作对象
    OfflineMsgModel _offlineMsgModel;

    //操作好友列表对象
    FriendModel _friendModel;

    //操作群组对象
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;

};

#endif