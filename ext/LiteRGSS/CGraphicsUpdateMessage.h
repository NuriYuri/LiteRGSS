#ifndef CGraphicsUpdateMessage_H
#define CGraphicsUpdateMessage_H
#include <string>
#include "ruby_common.h"
struct GraphicsUpdateMessage {
    VALUE errorObject;
    std::string message;
};
#endif
