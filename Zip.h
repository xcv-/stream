#ifndef STREAM_ZIP_H
#define STREAM_ZIP_H

#include <iterator>
#include <type_traits>

#include "Curry.h"


namespace stream {

template <typename F, typename... Inputs>
struct ZipIterator {

	using ValueType = typename std::result_of<F (typename Inputs::ValueType...)>::type;


	ZipIterator(F zipperFunction, std::tuple<Inputs...> inputs)
		: m_zipperFunction(zipperFunction)
		, m_inputs(std::move(inputs))
	{}//{ PrintTypes<ValueType, typename Inputs::ValueType...>(); }

	ZipIterator(const ZipIterator& other) = default;
	ZipIterator(ZipIterator&& other) = default;

	ZipIterator& operator = (ZipIterator&& other) {
		m_inputs = std::move(other.m_input);
		m_value  = std::move(other.m_value);
		return *this;
	}

	ZipIterator& operator = (const ZipIterator& other) {
		m_inputs = other.m_inputs;
		m_value  = other.m_value;
		return *this;
	}


	bool operator == (const ZipIterator& other) const {
		return m_inputs == other.m_inputs;
	}

	template <std::size_t I = 0>
	typename std::enable_if<
		I < sizeof...(Inputs),
		bool
	>::type hasNext() const {
		return std::get<I>(m_inputs).hasNext() && hasNext<I+1>();
	}

	template <std::size_t I = 0>
	typename std::enable_if<
		I >= sizeof...(Inputs),
		bool
	>::type hasNext() const {
		return true;
	}


	void next() {
		using C = Curry<ValueType (typename Inputs::ValueType...)>;

		next<0>(C::curry(m_zipperFunction));
	}

	template <std::size_t I, typename F2>
	typename std::enable_if<I < sizeof...(Inputs)-1, void>::type
	next(F2 curriedZipper) {
		auto& input = std::get<I>(m_inputs);
		input.next();
		next<I+1>(curriedZipper(input.get()));
	}

	template <std::size_t I, typename F2>
	typename std::enable_if<I == sizeof...(Inputs)-1, void>::type
	next(F2 curriedZipper) {
		auto& input = std::get<I>(m_inputs);
		input.next();
		m_value = curriedZipper(input.get());
	}


	ValueType get() const {
		return m_value;
	}


	F m_zipperFunction;
	std::tuple<Inputs...> m_inputs;

	ValueType m_value;
};


template <typename... Inputs>
struct Zipper {

	template <typename F>
	using Iterator   = ZipIterator<F, Inputs...>;
	using InputTuple = std::tuple<Inputs...>;
	using ValueTuple = std::tuple<typename Inputs::ValueType...>;

	InputTuple m_inputs;


	explicit Zipper(std::tuple<Inputs...>&& inputs)
		: m_inputs{std::move(inputs)}
	{}

	explicit Zipper(const Inputs&... inputs)
		: m_inputs{inputs...}
	{}

	static ValueTuple tupleZipper(const typename Inputs::ValueType&... args) {
		return ValueTuple{args...};
	}


	template <typename F>
	Iterator<F> zip_with(F zipperFunction) && {
		return Iterator<F>{zipperFunction, std::move(m_inputs)};
	}

	Iterator<decltype(&tupleZipper)> zip() && {
		return Iterator<decltype(&tupleZipper)>{tupleZipper, std::move(m_inputs)};
	}
};



template <typename F, typename... Inputs>
struct IsInputIterator<ZipIterator<F, Inputs...>> {
	static constexpr bool value = true;
};


template <typename Input2, typename... Inputs1>
typename std::enable_if<
	IsInputIterator<Input2>::value,
	Zipper<Inputs1..., Input2>
>::type operator | (Zipper<Inputs1...>&& zipper1, Input2 input2) {
	return Zipper<Inputs1..., Input2>(
		std::tuple_cat(
			std::move(zipper1.m_inputs),
			std::make_tuple(std::move(input2))));
}

template <typename Input1, typename Input2>
typename std::enable_if<
	IsInputIterator<Input1>::value && IsInputIterator<Input2>::value,
	Zipper<Input1, Input2>
>::type operator | (Input1 input1, Input2 input2) {
	return Zipper<Input1, Input2>{std::move(input1), std::move(input2)};
}

}


namespace std {

template <typename F, typename... Inputs>
struct iterator_traits<stream::ZipIterator<F, Inputs...>> {
	// difference_type
	// pointer
	using value_type = typename stream::ZipIterator<F, Inputs...>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
