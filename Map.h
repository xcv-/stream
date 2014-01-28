#ifndef STREAM_MAP_H
#define STREAM_MAP_H

#include <iterator>
#include <memory>
#include <type_traits>

namespace stream {

template <typename F>
struct MapParams {
	F function;
};


template <typename Input, typename F>
struct MapIterator {

	using ValueType = typename std::result_of<F (typename Input::ValueType)>::type;


	MapIterator(const Input& input, const MapParams<F>& params)
		: m_input(input), m_params(params)
	{}

	MapIterator(const MapIterator&) = default;

	MapIterator& operator = (const MapIterator& other) {
		m_input = other.m_input;
		m_value = other.m_value;
		return *this;
	}


	bool operator == (const MapIterator& other) const {
		return m_input == other.m_input;
	}


	bool hasNext() const {
		return m_input.hasNext();
	}

	void next() {
		m_input.next();
		m_value = std::make_shared<ValueType>(m_params.function(m_input.get()));
	}

	const ValueType& get() const {
		return *m_value;
	}


	Input m_input;
	MapParams<F> m_params;

	std::shared_ptr<ValueType> m_value;
};



template <typename Input, typename F>
struct Output<Input, MapParams<F>> {
	using Type = MapIterator<Input, F>;
};

template <typename F>
struct IsOutputParams<MapParams<F>> {
	static constexpr bool value = true;
};

template <typename Input, typename F>
struct IsInputIterator<MapIterator<Input, F>> {
	static constexpr bool value = true;
};



template <typename F>
MapParams<F> map(F function) {
	return MapParams<F>{function};
}

}


namespace std {

template <typename Input, typename F>
struct iterator_traits<stream::MapIterator<Input, F>> {
	// difference_type
	// pointer
	using value_type = typename stream::MapIterator<Input, F>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
