#pragma once

class Object
{
public:
    virtual const char *getType() const = 0;
};

#define DECLARE_OBJECT_TYPE(name)                                                                  \
    static const char *getTypeStatic()                                                             \
    {                                                                                              \
        return #name;                                                                              \
    }                                                                                              \
    const char *getType() const override                                                           \
    {                                                                                              \
        return getTypeStatic();                                                                    \
    }

template<class T>
inline T *object_cast(Object *object)
{
    if (object->getType() == T::getTypeStatic())
    {
        return static_cast<T *>(object);
    }
    return nullptr;
}
