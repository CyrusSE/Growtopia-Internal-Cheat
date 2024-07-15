#pragma once
#include <vector>
#include <proton/MiscUtils.h>


#pragma pack(push, 1)
struct __declspec(align(1)) alignas(1) NetSocket {
   public:
    uint64_t vtable;

    int m_socket;

    std::vector<char> m_readBuffer;
    std::vector<char> m_writeBuffer;
    unsigned int m_idleTimer, m_idleReadTimer; //time of last communication
    bool m_bWasDisconnected;

};
#pragma pack(pop)


class NetHTTP {
   public:

    enum eState {
        STATE_IDLE,
        STATE_ACTIVE,
        STATE_FINISHED,
        STATE_FORWARD, //about to forward to another URL
        STATE_ABORT
    };

    enum eEndOfDataSignal {
        END_OF_DATA_SIGNAL_RTSOFT_MARKER = 0, //sort of legacy, but if the characters in C_END_DOWNLOAD_MARKER_std::string are detected,
                                              //the connection is ended. If content-length is sent by the server, that is used also.
        END_OF_DATA_SIGNAL_HTTP               //standard way, double linefeeds will end the connection. If content-length is sent by the server,
                                              //that is also used.
    };
    enum eError { ERROR_NONE, ERROR_CANT_RESOLVE_URL, ERROR_COMMUNICATION_TIMEOUT, ERROR_WRITING_FILE, ERROR_404_FILE_NOT_FOUND };

    uint64_t vtable;

    std::string m_serverName;  //eg, "www.rtsoft.com"
    std::string m_query;       //eg, "crap/index.html"
    std::string m_replyHeader; //store what the server sends us
    int m_port;
    eEndOfDataSignal m_endOfDataSignal;
    NetSocket* m_netSocket;
    eState m_state;
    eError m_error;
    vector<char> m_downloadData; //holds the actual file/stream of what we've got
    std::string m_downloadHeader;
    uint32_t m_expectedFileBytes; //0 if content-length is unknown
    std::string m_postData;          //optional, for if we want to send post data
    unsigned int m_timer;       //we scan for an end of mark, we don't want to do it too often
    int m_idleTimeOutMS;
    FILE* m_pFile; //if not null, we're putting out download into this
    char unknown[16];
    int m_bytesWrittenToFile;
    std::string m_fileName;
    std::string m_contentType;
};
