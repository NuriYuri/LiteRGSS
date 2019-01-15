#ifndef SPRITE_DISPOSER_H
#define SPRITE_DISPOSER_H
#include "utils/rbAdapter.h"

void DisposeAllSprites(VALUE table);
template <class T>
void DisposeAs(VALUE self) {
    auto& cppValue = rb::Get<T>(self);
    cppValue.disposeFromViewport();
    rb::Dispose<T>(self);
}


#endif