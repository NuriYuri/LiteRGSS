#ifndef CRubyGlobalBitmaps_H
#define CRubyGlobalBitmaps_H

#include "ruby.h"

class CRubyGlobalBitmaps {
public:
    CRubyGlobalBitmaps() = default;
    virtual ~CRubyGlobalBitmaps();
    void clear();
};

#endif