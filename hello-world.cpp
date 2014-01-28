#include <iostream>
#include <initializer_list>
using std::string;

#include <boost/phoenix.hpp>
using namespace boost::phoenix;
using namespace boost::phoenix::placeholders;

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

	message >= from_stl()

		>= map([&](const auto& chars) -> string {
			return chars >= from_stl() >= foldl(string(), _1 + _2); })

		>= foldl(string(), _1 + _2)

		>= singleton()

		>= for_each(std::cout << _1 << "\n");
}
