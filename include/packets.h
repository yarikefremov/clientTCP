#ifndef PACKETS_H
#define PACKETS_H

#define NAME_LEN     96 //UTF-8
#define PASSWORD_LEN 32 //ASCII only

enum class PacketCode{
    BroadcastServerUDPPacketCode                  = -0x001,
    BroadcastClientUDPPacketCode                  = -0x002,
    BroadcastServerUDPConfirmationPacketCode      = -0x003,
    RegistrationClientTCPPacketCode               = -0x004,
    RegistrationServerTCPFailurePacketCode        = -0x005,
    RegistrationServerTCPSuccessPacketCode        = -0x006,
    AuthorizationClientTCPPacketCode              = -0x007,
    AuthorizationServerTCPFailurePacketCode       = -0x008,
    AuthorizationServerTCPSuccessPacketCode       = -0x009,
    SmallSizeMessageClientTCPPacketCode           = -0x010,
    MediumSizeMessageClientTCPPacketCode          = -0x011,
    LargeSizeMessageClientTCPPacketCode           = -0x012,
    AnySizeMessageServerTCPConfirmationPacketCode = -0x013,
    SmallSizeMessageServerTCPBroadcastPacketCode  = -0x014,
    MediumSizeMessageServerTCPBroadcastPacketCode = -0x015,
    LargeSizeMessageServerTCPBroadcastPacketCode  = -0x016,
    AnySizeMessageClientTCPConfirmationPacketCode = -0x017
}; //-0x001... -0x01F (31)



enum class RegistrationErrorCode{
    NameAlreadyExist = -0x020,
}; //-0x020... -0x024 (5)



enum class AuthorizationErrorCode{
    NameDoesntExistError = -0x025,
    WrongPasswordError   = -0x026,
}; //-0x025... -0x02F (11)




//For UTF-8, the actual message size is equal to the (size of the array)/2
//Small 2-48 (1-24 symbol)
//Medium 50-120 (25-60 symbol)
//Large 122-512 (61-256)
enum class MessageSize{
    SmallSizeMessage  =  48,
    MediumSizeMessage = 120,
    LargeSizeMessage  = 512
};



typedef struct BroadcastServerUDPPacket{
    char code = char(PacketCode::BroadcastServerUDPPacketCode);
}BroadcastServerUDPPacket;



typedef struct BroadcastClientUDPPacket{
    char code = char(PacketCode::BroadcastClientUDPPacketCode);
}BroadcastClientUDPPacket;



typedef struct BroadcastServerUDPConfirmationPacket{
    char code = char(PacketCode::BroadcastServerUDPConfirmationPacketCode);
}BroadcastServerUDPConfirmationPacket;



typedef struct RegistrationClientTCPPacket{
    char code     = char(PacketCode::RegistrationClientTCPPacketCode);
    char name     [NAME_LEN];
    char password [PASSWORD_LEN];
}RegistrationClientTCPPacket;



typedef struct RegistrationServerTCPFailurePacket{
    char code     = char(PacketCode::RegistrationServerTCPFailurePacketCode);
    char errorCode;
}RegistrationServerTCPFailurePacket;



typedef struct RegistrationServerTCPSuccessPacket{
    char     code = char(PacketCode::RegistrationServerTCPSuccessPacketCode);
    unsigned id;
}RegistrationServerTCPSuccessPacket;



typedef struct AuthorizationClientTCPPacket{
    char code     = char(PacketCode::AuthorizationClientTCPPacketCode);
    char name     [NAME_LEN];
    char password [PASSWORD_LEN];
}AuthorizationClientTCPPacket;



typedef struct AuthorizationServerTCPFailurePacket{
    char code     = char(PacketCode::AuthorizationServerTCPFailurePacketCode);
    char errorCode;
}AuthorizationServerTCPFailurePacket;



typedef struct AuthorizationServerTCPSuccessPacket{
    char     code = char(PacketCode::AuthorizationServerTCPSuccessPacketCode);
    unsigned id;
}AuthorizationServerTCPSuccessPacket;



typedef struct SmallSizeMessageClientTCPPacket{
    char     code      = char(PacketCode::SmallSizeMessageClientTCPPacketCode);
    unsigned senderId;
    unsigned receiverId;
    unsigned messageLen;
    char     message   [unsigned(MessageSize::SmallSizeMessage)];
}SmallSizeMessageClientTCPPacket;



typedef struct MediumSizeMessageClientTCPPacket{
    char     code      = char(PacketCode::MediumSizeMessageClientTCPPacketCode);
    unsigned id;
    unsigned receiverId;
    unsigned messageLen;
    char     message   [unsigned(MessageSize::MediumSizeMessage)];
}MediumSizeMessageClientTCPPacket;



typedef struct LargeSizeMessageClientTCPPacket{
    char     code      = char(PacketCode::LargeSizeMessageClientTCPPacketCode);
    unsigned senderId;
    unsigned receiverId;
    unsigned messageLen;
    char     message   [unsigned(MessageSize::LargeSizeMessage)];
}LargeSizeMessageClientTCPPacket;



typedef struct AnySizeMessageServerTCPConfirmationPacket{
    char code = char(PacketCode::AnySizeMessageServerTCPConfirmationPacketCode);
}AnySizeMessageServerTCPConfirmationPacket;



typedef struct SmallSizeMessageServerTCPBroadcastPacket{
    char     code = char(PacketCode::SmallSizeMessageServerTCPBroadcastPacketCode);
    unsigned senderId;
    unsigned receiverId;
    unsigned messageLen;
    char     message[unsigned(MessageSize::SmallSizeMessage)];
}SmallSizeMessageServerTCPBroadcastPacket;



typedef struct MediumSizeMessageServerTCPBroadcastPacket{
    char     code      = char(PacketCode::MediumSizeMessageServerTCPBroadcastPacketCode);
    unsigned senderId;
    unsigned receiverId;
    unsigned messageLen;
    char     message   [unsigned(MessageSize::MediumSizeMessage)];
}MediumSizeMessageServerTCPBroadcastPacket;



typedef struct LargeSizeMessageServerTCPBroadcastPacket{
    char     code      = char(PacketCode::LargeSizeMessageServerTCPBroadcastPacketCode);
    unsigned senderId;
    unsigned receiverId;
    unsigned messageLen;
    char     message   [unsigned(MessageSize::LargeSizeMessage)];
}LargeSizeMessageServerTCPBroadcastPacket;



typedef struct AnySizeMessageClientTCPConfirmationPacket{
    char code = char(PacketCode::AnySizeMessageClientTCPConfirmationPacketCode);
}AnySizeMessageClientTCPConfirmationPacket;



#endif // PACKETS_H
