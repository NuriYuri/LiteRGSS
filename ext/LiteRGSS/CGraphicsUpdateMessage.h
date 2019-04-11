#ifndef CGraphicsUpdateMessage_H
#define CGraphicsUpdateMessage_H
#include <string>
#include "utils/ruby_common.h"
struct GraphicsUpdateMessage {
    VALUE errorObject;
    std::string message;
};
#endif
