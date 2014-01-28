#ifndef STREAM_FILTER_H
#define STREAM_FILTER_H

#include <iterator>

namespace stream {

template <typename P>
struct FilterParams {
	P predicate;
};


template <typename Input, typename P>
struct FilterIterator {

	using ValueType = typename Input::ValueType;


	FilterIterator(const Input& input, const FilterParams<P>& params)
		: m_input(input), m_params(params), m_nextInput(input)
	{}

	FilterIterator(const FilterIterator& other) = default;

	FilterIterator& operator = (const FilterIterator& other) {
		m_input = other.m_input;
		m_value = other.m_value;

		m_nextPasses = other.m_nextPasses;
		m_nextInput = other.m_nextInput;
		m_nextValue = other.m_nextValue;

		return *this;
	}


	bool operator == (const FilterIterator& other) const {
		return m_input == other.m_input;
	}


	bool hasNext() const {
		if (m_nextPasses)
			return true;

		m_nextInput = m_input;

		while (m_nextInput.hasNext()) {
			m_nextInput.next();
			m_nextValue = m_nextInput.get();

			if (!m_params.predicate(m_nextValue)) {
				m_input = m_nextInput;
			}
			else {
				m_nextPasses = true;
				return true;
			}
		}

		return false;
	}

	void next() {
		if (m_nextPasses) {
			m_input = std::move(m_nextInput);
			m_value = std::move(m_nextValue);
			m_nextPasses = false;
			return;
		}

		do {
			m_input.next();
			m_value = m_input.get();
		} while (!m_params.predicate(m_value));
	}

	ValueType get() const {
		return m_value;
	}


	mutable Input m_input;
	FilterParams<P> m_params;

	ValueType m_value;

	mutable bool m_nextPasses = false;
	mutable Input m_nextInput;
	mutable ValueType m_nextValue;
};



template <typename Input, typename P>
struct Output<Input, FilterParams<P>> {
	using Type = FilterIterator<Input, P>;
};

template <typename P>
struct IsOutputParams<FilterParams<P>> {
	static constexpr bool value = true;
};

template <typename Input, typename P>
struct IsInputIterator<FilterIterator<Input, P>> {
	static constexpr bool value = true;
};



template <typename P>
FilterParams<P> filter(P predicate) {
	return FilterParams<P>{predicate};
}

}


namespace std {

template <typename Input, typename P>
struct iterator_traits<stream::FilterIterator<Input, P>> {
	// difference_type
	// pointer
	using value_type = typename stream::FilterIterator<Input, P>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
