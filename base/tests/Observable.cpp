#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gmt/Observable.h>

namespace gmt
{

namespace tests
{

namespace observable
{

using ::testing::Sequence;

namespace
{

class Listener : public EnableWeakFromThis<Listener>
{
public:
    Listener() : EnableWeakFromThis<Listener>(this) {}

    MOCK_METHOD(void, foo, ());
    MOCK_METHOD(void, bar, ());
};

class A : public gmt::Observable<Listener>
{
public:
    void foo()
    {
        notify([](auto x){ x->foo(); });
    }

    void bar()
    {
        notify([](auto x){ x->bar(); });
    }
};

}

TEST(Observable, CallOnce)
{
    Listener listener;
    EXPECT_CALL(listener, foo()).Times(1);

    A a;
    a.addObserver(&listener);
    a.foo();
}

TEST(Observable, CallMultipleTimes)
{
    const int n = 7;

    Listener listener;
    EXPECT_CALL(listener, foo()).Times(n);

    A a;
    a.addObserver(&listener);
    for (int i = 0; i < n; i++) {
        a.foo();
    }
}

TEST(Observable, CallDifferentMethods)
{
    Listener listener;

    Sequence s;

    EXPECT_CALL(listener, foo()).Times(1)
        .InSequence(s);
    EXPECT_CALL(listener, bar()).Times(1)
        .InSequence(s);

    A a;
    a.addObserver(&listener);
    a.foo();
    a.bar();
}

TEST(Observable, DestroyedListener)
{
    A a;
    {
        Listener listener;
        EXPECT_CALL(listener, foo()).Times(0);
        a.addObserver(&listener);
    }
    a.foo();
}

TEST(Observable, MovedListener)
{
    std::unique_ptr<Listener> actualListener;
    A a;
    {
        auto listener = std::make_unique<Listener>();
        a.addObserver(listener.get());
        actualListener = std::move(listener);
    }

    EXPECT_CALL(*actualListener, foo()).Times(1);

    a.foo();
}

TEST(Observable, RemoveObserver)
{
    A a;
    Listener listener;
    a.addObserver(&listener);
    
    EXPECT_CALL(listener, foo()).Times(1);

    a.foo();
    a.removeObserver(&listener);

    EXPECT_CALL(listener, foo()).Times(0);

    a.foo();
}

}

}

}
