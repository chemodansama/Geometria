//
//  utils.cpp
//
//  Created by Grigory Shabaganov on 10.08.2013.
//

#include "gmt/utils.h"

#include <chrono>
#include <sstream>

namespace
{

class RandomEngine
{
public:
#ifdef __clang__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#endif
	operator std::default_random_engine &() { return engine; }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

private:
	std::random_device rd;
	std::default_random_engine engine{ rd() };
};

}

namespace details
{

std::default_random_engine &randomEngine()
{
	static RandomEngine instance;
	return instance;
}

}

namespace gmt
{

long getTime()
{
	const auto time =
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return static_cast<long>(time);
}

}
