#ifndef L_RBADAPTER_HEADER
#define L_RBADAPTER_HEADER
#include <string>
#include <stdexcept>
#include <ruby.h>
#include "metadata.h"

extern VALUE rb_eRGSSError;

namespace rb {
    template <class T>
    void Protect(VALUE self) {
        if(RDATA(self)->data == nullptr) { 
            auto errorMessage = std::string { "Disposed "}; 
            errorMessage += meta::Log<T>::classname;
            errorMessage += ".";
            rb_raise(rb_eRGSSError, "%s", errorMessage.c_str());
            throw std::runtime_error(errorMessage);
        }
    }

    template <class T>
    auto* GetPtr(VALUE self) {
        return reinterpret_cast<T*>(rb_data_object_get(self));
    }

    template <class T>
    auto& Get(VALUE self) {
        Protect<T>(self);
        return *GetPtr<T>(self);
    }

    template <class T>
    auto& GetSafe(VALUE self, VALUE expectedType) {
        if (rb_obj_is_kind_of(self, expectedType) != Qtrue) {
            auto errorMessage = std::string { "Expected " };
            errorMessage += meta::Log<T>::classname;
            errorMessage += " got ";
            errorMessage += RSTRING_PTR(rb_class_name(CLASS_OF(self)));
            errorMessage += ".";
            rb_raise(rb_eTypeError, "%s", errorMessage.c_str());
            throw std::runtime_error(errorMessage);
        }

        return Get<T>(self);
    }

    template <class T>
    void Free(void* data) {
        delete reinterpret_cast<T*>(data);
    }

    template <class T>
    void Mark(T* data) {}

    template <class T>
    VALUE AllocDrawable(VALUE klass) {
        auto value = new T();
        auto self = Data_Wrap_Struct(klass, Mark<T>, Free<T>, value);
        value->setSelf(self);
        return self;
    }

    template <class T>
    VALUE Alloc(VALUE klass) {
        return Data_Wrap_Struct(klass, Mark<T>, Free<T>, new T());
    }

    template <class T>
    VALUE Dispose(VALUE self) {
        if (RDATA(self)->data == nullptr) {
            return Qnil;
        }
        delete GetPtr<T>(self);
        RDATA(self)->data = nullptr;
        return Qnil;
    }
}
#endif