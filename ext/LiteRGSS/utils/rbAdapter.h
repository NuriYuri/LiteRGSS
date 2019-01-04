#ifndef L_RBADAPTER_HEADER
#define L_RBADAPTER_HEADER
#include <string>
#include "utils/metadata.h"
#include "ruby.h"

namespace rb {
    template <class T>
    auto Protect(VALUE self) {
        if(RDATA(self)->data == nullptr) { 
            auto disposedMessage = std::string {"Disposed "};
            disposedMessage += meta::Log<T>::classname;
            disposedMessage += ".";
            rb_raise(rb_eRGSSError, "%s", disposedMessage.c_str());
        }
    }

    template <class T>
    auto& Get(VALUE self) {
        Protect<T>(self);
        return *reinterpret_cast<T*>(rb_data_object_get(self));
    }

    template <class T>
    void Free(void* data) {
        delete reinterpret_cast<T*>(data);
    }

    template <class T>
    VALUE Alloc(VALUE klass) {
        return Data_Wrap_Struct(klass, NULL, Free<T>, new T());
    }

}
#endif