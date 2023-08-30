//
//  Weak.h
//
//  Created by Grigory Shabaganov on 20.02.2018.
//

#pragma once

#include <cstdio>
#include <cassert>
#include <type_traits>

namespace gmt
{

namespace details
{

template <typename T>
struct WeakControlBlock
{
    int count;
    T *target;
};

} // end of details namespace

template <typename T>
class Weak
{
public:
    Weak() = default;
    Weak(details::WeakControlBlock<T> *controlBlock);

    Weak(const Weak<T> &source);
    Weak(Weak<T>&& source) noexcept;

    Weak& operator=(const Weak<T> &source);
    Weak& operator=(Weak<T>&& source);

    bool operator==(const Weak<T> &other) const;

    ~Weak();

    /**
      Returns nullptr if the target was destroyed.
    */
    T* get() const;

    bool valid() const { return controlBlock_->target != nullptr; }
    operator bool() const { return valid(); }

    T& operator->() const { return *controlBlock_->target; }
    T& operator*() const { return *controlBlock_->target; }

private:
    details::WeakControlBlock<T> *controlBlock_{ nullptr };

    void releaseControlBlock();
};

template <typename T>
class EnableWeakFromThis
{
public:
    EnableWeakFromThis(T* self);

    EnableWeakFromThis(EnableWeakFromThis<T>&) = delete;
    EnableWeakFromThis& operator=(EnableWeakFromThis<T>&) = delete;

    EnableWeakFromThis(EnableWeakFromThis<T>&& source) = delete;
    EnableWeakFromThis& operator=(EnableWeakFromThis<T>&& source) = delete;

    virtual ~EnableWeakFromThis();

    Weak<T> weak();

private:
    mutable details::WeakControlBlock<T> *controlBlock_{ nullptr };

    T *self_;
    details::WeakControlBlock<T> *controlBlock();
};

// Templates Implementation ==================================================================================

template <typename T>
EnableWeakFromThis<T>::EnableWeakFromThis(T* self)
    : self_{ self }
{
}

template <typename T>
Weak<T>::Weak(details::WeakControlBlock<T> *controlBlock)
    : controlBlock_{ controlBlock }
{
    controlBlock_->count += 1;
}

template <typename T>
Weak<T>::Weak(const Weak<T> &source)
{
    controlBlock_ = source.controlBlock_;
    controlBlock_->count += 1;
}

template <typename T>
Weak<T>::Weak(Weak<T>&& source) noexcept
{
    controlBlock_ = source.controlBlock_;
    source.controlBlock_ = nullptr;
}

template <typename T>
Weak<T>& Weak<T>::operator=(const Weak<T> &source)
{
    if (this == &source) {
        return *this;
    }

    releaseControlBlock();

    controlBlock_ = source.controlBlock_;
    controlBlock_->count += 1;

    return *this;
}

template <typename T>
Weak<T>& Weak<T>::operator=(Weak<T>&& source)
{
    if (this == &source) {
        return *this;
    }

    releaseControlBlock();

    controlBlock_ = source.controlBlock_;
    source.controlBlock_ = nullptr;

    return *this;
}

template <typename T>
bool Weak<T>::operator==(const Weak<T> &other) const
{
    return controlBlock_ == other.controlBlock_;
}

template <typename T>
T* Weak<T>::get() const
{
    return controlBlock_ ? controlBlock_->target : nullptr;
}

template <typename T>
Weak<T>::~Weak()
{
    releaseControlBlock();
}
    
template <typename T>
void Weak<T>::releaseControlBlock()
{
    if (controlBlock_) {
        assert(controlBlock_->count > 0);
        controlBlock_->count -= 1;
        if (!controlBlock_->count) {
            delete controlBlock_;
        }
        controlBlock_ = nullptr;
    }
}

template <typename T>
details::WeakControlBlock<T> *EnableWeakFromThis<T>::controlBlock()
{
    if (!controlBlock_) {
        controlBlock_ = new details::WeakControlBlock<T>{ 1, self_ };
    }

    return controlBlock_;
}

template <typename T>
Weak<T> EnableWeakFromThis<T>::weak()
{
    return Weak<T>{ controlBlock() };
}

template <typename T>
EnableWeakFromThis<T>::~EnableWeakFromThis()
{
    if (controlBlock_) {
        assert(controlBlock_->count > 0);
        controlBlock_->count -= 1;
        controlBlock_->target = nullptr;
        if (!controlBlock_->count) {
            delete controlBlock_;
        }
    }
}

}
