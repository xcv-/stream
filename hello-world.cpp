#include <iostream>
#include <initializer_list>
using std::string;

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/casts.hpp>
#include <boost/lambda/construct.hpp>
using namespace boost::lambda;

#include "Stream.h"
using namespace stream;

template <typename T>
using il = std::initializer_list<T>;

int main() {
	il<il<char>> message = {
		{'H', 'e', 'l', 'l', 'o'},
		{',', ' '},
		{'W', 'o', 'r', 'l', 'd'},
		{'!'}
	};

	auto append = [](const string& s, char c) { return s + c; };

	message >= from_stl()

		>= map([&](const auto& chars) -> string {
			return chars >= from_stl() >= foldl(string(), append); })

		>= foldl(string(), _1 + _2)

		>= singleton()

		>= for_each(std::cout << _1 << "\n");
}
