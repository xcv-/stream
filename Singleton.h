#ifndef STREAM_SINGLETON_H
#define STREAM_SINGLETON_H

#include <iterator>
#include <memory>
#include <type_traits>


#include "Fold.h"


namespace stream {

template <typename T, typename Store>
struct SingletonIterator {

	using ValueType = T;


	explicit SingletonIterator(Store&& value)
		: m_value(std::move(value))
	{}

	explicit SingletonIterator(const Store& value)
		: m_value(value)
	{}

	SingletonIterator(const SingletonIterator&) = default;
	SingletonIterator(SingletonIterator&&) = default;
	SingletonIterator& operator = (const SingletonIterator& other) = default;
	SingletonIterator& operator = (SingletonIterator&& other) = default;


	bool operator == (const SingletonIterator& other) const {
		return m_value == other.m_value;
	}


	bool hasNext() const {
		return !m_done;
	}

	void next() {
		m_done = true;
	}

	const ValueType& get() const {
		if (!m_result)
		   m_result = std::make_shared<ValueType>(m_value);

		return *m_result;
	}


	Store m_value;
	mutable std::shared_ptr<ValueType> m_result;

	bool m_done = false;
};


template <typename T, typename Store>
struct IsInputIterator<SingletonIterator<T, Store>> {
	static constexpr bool value = true;
};



struct singleton {
};


template <typename Input, typename A, typename F>
SingletonIterator<typename FoldL<Input, A, F>::ValueType, FoldL<Input, A, F>>
operator >= (FoldL<Input, A, F> fold, singleton) {

	using R = SingletonIterator<
		typename FoldL<Input, A, F>::ValueType,
		FoldL<Input, A, F>>;

	return R{std::move(fold)};
}

template <typename T>
SingletonIterator<T,T> operator >= (T value, singleton) {
	return SingletonIterator<T, T>(std::move(value));
}

}


namespace std {

template <typename T, typename Store>
struct iterator_traits<stream::SingletonIterator<T, Store>> {
	// difference_type
	// pointer
	using value_type = typename stream::SingletonIterator<T, Store>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
