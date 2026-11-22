#ifndef SMART_PTR_HH
#define SMART_PTR_HH

#include "prelude.h"

// this is just your traditional std::auto_ptr
// (std::auto_ptr was removed from modern standards in favor of std::unique_ptr)

// TODO: make sure this is correct

template <typename T>
struct SmartPtr
{
    typedef T element_type;

    struct MoveReference
    {
        MoveReference(T * & source, T * value) : source(source), value(value) {}

        T * Take() const
        {
            T * ptr = value;
            source = nullptr;
            return ptr;
        }

        T * & source;
        T * value;
    };

    explicit SmartPtr(T * ptr = nullptr)
        : inner(ptr)
    {
    }

    SmartPtr(MoveReference const & reference) : inner(reference.Take()) {}

    operator MoveReference() { return MoveReference(inner, inner); }

    SmartPtr & operator=(T * ptr)
    {
        if (ptr != inner)
            delete inner;
        inner = ptr;
        return *this;
    }

    ~SmartPtr() { delete inner; }

    T & operator*() const { return *inner; }

    T * operator->() const { return inner; }

    T * Get() const { return inner; }

    // TODO
    T * Move()
    {
        T * tmp = inner;
        inner = nullptr;
        return tmp;
    }

private:
    // TODO
    SmartPtr(SmartPtr &) {}

private:
    /* +00 */ T * inner;
};

#endif // SMART_PTR_HH
