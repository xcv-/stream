#include <iostream>
#include <initializer_list>

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

#include "Stream.h"

using namespace stream;



template <typename... Ts>
std::ostream& operator<< (std::ostream& o, const std::tuple<Ts...>& tup) {
	o << std::get<0>(tup) << std::endl;
}


using std::cout;
using std::get;
using il = std::initializer_list<int>;
using fl = std::initializer_list<float>;
using sl = std::initializer_list<std::string>;

int main() {

	( il{1,2,3,4}                         >= from_stl()
	| fl{1,2,3,4,5}                       >= from_stl() >= map(_1 / 2)
	| sl{"asdf", "asdf", "asdf", "sup"}   >= from_stl()
	| sl{"top", "kek", "IT WERKS", "/g/"} >= from_stl()
	).zip()
		>= for_each([](const auto& tup) {
			cout
				<< get<0>(tup) << " "
				<< get<1>(tup) << " "
				<< get<2>(tup) << " "
				<< get<3>(tup) << "\n";
		});

	return 0;
}
