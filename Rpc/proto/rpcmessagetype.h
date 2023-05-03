#ifndef RPCMESSAGETYPE_H
#define RPCMESSAGETYPE_H

namespace g14rpc
{

enum class MessageType
{
    UNKNOWN,
    BASE,
    GET_CPU_STATE,
    SET_CPU_PROFILE
};

}


#endif // RPCMESSAGETYPE_H
