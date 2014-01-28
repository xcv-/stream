#include <iostream>
#include <string>
#include <initializer_list>

#include <boost/phoenix.hpp>
using namespace boost::phoenix;
using namespace boost::phoenix::placeholders;

#include "Stream.h"
using namespace stream;

int main() {
	auto nums = range(1,101);

	( nums >= map([] (int x) { return std::to_string(x); })
	| nums >= map([] (int x) { return x % 3 ? "" : "Fizz"; })
	| nums >= map([] (int x) { return x % 5 ? "" : "Buzz"; })
	)
	.zip_with(_1 + " " + _2 + _3)
	>= for_each(std::cout << _1 << "\n");
}
