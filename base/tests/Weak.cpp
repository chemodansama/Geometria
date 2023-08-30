#include <gtest/gtest.h>

#include <gmt/Weak.h>

namespace gmt
{

namespace tests
{

namespace weak
{

namespace
{

class A : public gmt::EnableWeakFromThis<A>
{
public:
	A() : gmt::EnableWeakFromThis<A>(this)
	{
	}
};

}
TEST(Weak, Basic)
{
	gmt::Weak<A> w;
	{
		A a;
		w = a.weak();
	}
	EXPECT_EQ(w.get(), nullptr);
}

TEST(Weak, NullAfterDelete)
{
	auto a = std::make_unique<A>();
	auto w = a->weak();
	a.reset();
	EXPECT_EQ(w.get(), nullptr);
}

TEST(Weak, Move)
{
	auto a = std::make_unique<A>();
	auto w = a->weak();
	auto b = std::move(a);

	EXPECT_EQ(a.get(), nullptr);
	EXPECT_EQ(w.get(), b.get());
}

TEST(Weak, Multiple)
{
	auto a = std::make_unique<A>();
	auto w1 = a->weak();
	auto w2 = a->weak();

	EXPECT_EQ(w1.get(), w2.get());
	EXPECT_EQ(w1.get(), a.get());
}

TEST(Weak, MoveWeak)
{
	auto a = std::make_unique<A>();
	auto w1 = a->weak();
	auto w2 = std::move(w1);

	EXPECT_EQ(w1.get(), nullptr);
	EXPECT_EQ(w2.get(), a.get());
}

TEST(Weak, MoveEmpty)
{
	auto a = std::make_unique<A>();
	auto w1 = a->weak();
	a.reset();
	auto w2 = std::move(w1);

	EXPECT_EQ(w1.get(), nullptr);
	EXPECT_EQ(w2.get(), nullptr);
}

TEST(Weak, CopyWeak)
{
	auto a = std::make_unique<A>();
	auto w1 = a->weak();
	auto w2 = w1;

	EXPECT_EQ(w1.get(), w2.get());
	EXPECT_EQ(w2.get(), a.get());
}

TEST(Weak, CopyWeakAndNullify)
{
	auto a = std::make_unique<A>();
	auto w1 = a->weak();
	auto w2 = w1;
	a.reset();

	EXPECT_EQ(w1.get(), w2.get());
	EXPECT_EQ(w2.get(), nullptr);
}

}

}

}
