// AUTOGENERATED FILE, DO NOT EDIT BY HAND
#ifndef PB_MERCURY_H
#define PB_MERCURY_H

#include <memory>
#include <vector>
#include <PbCommon.h>
#include <PbWriter.h>
#include <PbReader.h>

class Header : public BaseProtobufMessage {
private:
public:
    Header() {};
    std::string uri;
    std::string method;
    
    bool decodeField(std::shared_ptr<PbReader> reader)	{
        switch (reader->currentTag)
        {
        case 1:
            reader->decodeString(uri);
            break;
        case 3:
            reader->decodeString(method);
            break;
        default:
            return false;
        }
        return true;
    }

    void encodeWithWriter(std::shared_ptr<PbWriter> writer) {
        writer->addString(1, uri);
        writer->addString(3, method);
    }
};

#endif
