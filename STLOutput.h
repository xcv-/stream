#ifndef STREAM_STLOUTPUT_H
#define STREAM_STLOUTPUT_H

#include <vector>
#include <algorithm>


namespace stream {

template <typename Input>
struct STLInputIteratorAdaptor {
	STLInputIteratorAdaptor(const Input& input, bool isEnd)
		: m_input(input), m_isEnd(isEnd)
	{
		if (!isEnd && m_input.hasNext())
			m_input.next();
	}


	bool operator == (const STLInputIteratorAdaptor& other) const {
		if (m_isEnd && other.m_isEnd)
			return true;
		else if (m_isEnd)
			return other == *this;

		if (other.m_isEnd)
			return m_last;
		else
			return m_input == other.m_input;
	}

	bool operator != (const STLInputIteratorAdaptor& other) const {
		return !(*this == other);
	}


	typename Input::ValueType operator*() const {
		return m_input.get();
	}

	STLInputIteratorAdaptor& operator++() {
		if (m_input.hasNext())
			m_input.next();
		else
			m_last = true;

		return *this;
	}

	STLInputIteratorAdaptor operator++(int) {
		auto this2 = *this;
		m_input.next();
		return this2;
	}


private:
	Input m_input;
	bool m_isEnd;
	bool m_last = false;
};


struct STLVectorOutputAdapter {
	template <typename Input>
	using ResultType = typename std::vector<typename Input::ValueType>;

	template <typename Input>
	ResultType<Input> result(const Input& input) {
		ResultType<Input> result;

		/*auto it = input.iterator();
		while (it.hasNext()) {
			it.next();
			result.push_back(it.get());
		}*/
		std::copy(begin(input), end(input), std::back_inserter(result));

		return result;
	}
};

template <>
struct IsOutputAdapter<STLVectorOutputAdapter> {
	static constexpr bool value = true;
};


inline STLVectorOutputAdapter to_vector() {
	return STLVectorOutputAdapter();
}

}


namespace std {

template <typename Input>
typename enable_if<
	stream::IsInputIterator<Input>::value,
	stream::STLInputIteratorAdaptor<Input>
>::type begin(const Input& input) {
	return stream::STLInputIteratorAdaptor<Input>(input, false);
}

template <typename Input>
typename enable_if<
	stream::IsInputIterator<Input>::value,
	stream::STLInputIteratorAdaptor<Input>
>::type end(const Input& input) {
	return stream::STLInputIteratorAdaptor<Input>(input, true);
}

template <typename Input>
struct iterator_traits<stream::STLInputIteratorAdaptor<Input>> {
	// difference_type
	// pointer
	using value_type = typename Input::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
