#ifndef STREAM_STLINPUT_H
#define STREAM_STLINPUT_H

#include <iterator>
#include <vector>
#include <initializer_list>


namespace stream {

template <typename C>
struct STLInput;

template <typename C>
struct STLInputIterator {
	private:
		typename C::const_iterator m_it;
		typename C::const_iterator m_end;
		bool m_first = true;

	public:
		using ValueType = typename C::value_type;

		STLInputIterator(const C& container)
			: m_it(container.begin())
			, m_end(container.end())
		{}

		STLInputIterator(const STLInputIterator& other) = default;
		STLInputIterator& operator = (const STLInputIterator& other) = default;


		bool operator == (const STLInputIterator& other) const {
			return m_it == other.m_it;
		}

		bool hasNext() const {
			if (m_first)
				return m_it != m_end;

			auto itCopy = m_it;
			++itCopy;
			return itCopy != m_end;
		}

		void next() {
			if (m_first)
				m_first = false;
			else
				++m_it;
		}

		ValueType get() const {
			return *m_it;
		}
};


template <typename C>
struct IsInputIterator<STLInputIterator<C>> {
	static constexpr bool value = true;
};



template <typename C>
struct from {};

struct from_stl {};


template <typename C>
STLInputIterator<C> operator >= (const C& container, from<C>) {
	return STLInputIterator<C>(container);
}

template <typename C>
auto operator >= (const C& container, from_stl)
		-> typename std::enable_if<
		       std::is_same<decltype(begin(container)), typename C::const_iterator>::value,
		       STLInputIterator<C>
		>::type {

	return STLInputIterator<C>(container);
}

}


namespace std {

template <typename C>
struct iterator_traits<stream::STLInputIterator<C>> {
	// difference_type
	// pointer
	using value_type = typename stream::STLInputIterator<C>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
