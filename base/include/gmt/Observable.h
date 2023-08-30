//
//  Observable.h
//
//  Created by Grigory Shabaganov on 20.02.2018.
//

#pragma once

#include <list>
#include <vector>

#include "gmt/Weak.h"

namespace gmt
{

template <typename T>
class Observable
{
public:
    void addObserver(EnableWeakFromThis<T> *observer) const;
    void removeObserver(EnableWeakFromThis<T> *observer) const;

protected:
    template <typename F>
    void notify(const F& callback) const;

private:
    mutable std::list<Weak<T>> observers_;
};

// Implementation

template <typename T>
void Observable<T>::addObserver(EnableWeakFromThis<T> *observer) const
{
    observers_.push_back(observer->weak());
}

template <typename T>
void Observable<T>::removeObserver(EnableWeakFromThis<T> *observer) const
{
    observers_.remove(observer->weak());
}

template <typename T>
template <typename F>
void Observable<T>::notify(const F& callback) const
{
    for (auto it = observers_.begin(); it != observers_.end();) {
        if (auto target = it->get()) {
            callback(target);
            ++it;
        } else {
            it = observers_.erase(it);
        }
    }
}

}
