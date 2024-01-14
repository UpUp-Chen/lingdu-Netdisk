#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <QByteArray>

typedef unsigned int uint;

// message status
#define REGISTER_SUCCESS "Register success"
#define REGISTER_FAIL "Register fail"
#define LOGIN_SUCCESS "Login success"
#define LOGIN_FAIL1 "Login fail1"
#define LOGIN_FAIL2 "Login fail2"
#define LOGOUT_SUCCESS "Logout success"
#define LOGOUT_FAIL1 "Logout fail1"
#define LOGOUT_FAIL2 "Logout fail2"
#define ADD_FOLDER_SUCCESS "AddFolder success"
#define ADD_FOLDER_FAIL "AddFolder fail"
#define ADD_FILE_SUCCESS "AddFile success"
#define ADD_FILE_FAIL1 "AddFile fail1"
#define ADD_FILE_FAIL2 "AddFile fail2"
#define DELETE_FOLDER_SUCCESS "DeleteFolder success"
#define DELETE_FOLDER_FAIL "DeleteFolder fail"
#define UP_FOLDER_SUCCESS "UPFolder success"
#define UP_FOLDER_FAIL "UPFolder fail"


// enumeration class specify the message's type
enum MSGTYPE
{
    ENUM_MSG_TYPE_REGIST_REQUEST,           // 注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,           // 注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,            // 登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,            // 登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,       // 在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,       // 在线用户回复
    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,       // 搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,       // 搜索用户回复
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,       // 添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,       // 添加好友回复
    ENUM_MSG_TYPE_ADD_FRIEND_AGGREE,        // 同意添加好友
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,        // 拒绝添加好友
    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,     // 刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,     // 刷新好友回复
    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,    // 删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,    // 删除好友回复
    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,     // 私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,     // 私聊回复
    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,       // 群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,       // 群聊回复
    REGISTER,
    LOGIN,
    LOGOUT,
    REFRESH_FOLDER,
    ADD_FOLDER,
    DELETE_FOLDER,
    UP_FOLDER,
    UPLOAD,
};
// our protocal data unit through QTcpSocket
// To unify,for all transmission data, we use this data form
struct PDU
{
    uint len;         // total len // this is the first unit, because we need to read it first
    uint type;        // message type
    char meta[96];    // meta data: file name / usename&password
    uint dataLen;     // data len
    char data[];      // transmission data
};

PDU *mkPDU(uint dataLen);

#endif
