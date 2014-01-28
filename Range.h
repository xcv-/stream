#ifndef STREAM_RANGE_H
#define STREAM_RANGE_H

#include <iterator>
#include <type_traits>

namespace stream {

template <typename Num>
struct RangeIterator {

	using ValueType = Num;


	RangeIterator(Num start, Num end, Num step)
		: m_growing(step > Num(0))
		, m_current(start - step)
		, m_end(std::move(end))
		, m_step(std::move(step))
	{}

	RangeIterator(const RangeIterator&) = default;
	RangeIterator(RangeIterator&&) = default;

	RangeIterator& operator = (const RangeIterator&) = default;
	RangeIterator& operator = (RangeIterator&&) = default;


	bool hasNext() const {
		if (m_growing)
			return m_current + m_step < m_end;
		else
			return m_current + m_step > m_end;
	}

	void next() {
		m_current += m_step;
	}

	ValueType get() const {
		return m_current;
	}


	bool m_growing;
	Num m_current, m_end, m_step;
};



template <typename Num>
struct IsInputIterator<RangeIterator<Num>> {
	static constexpr bool value = true;
};



template <typename Num>
RangeIterator<Num> range(Num start, Num end, Num step) {
	return RangeIterator<Num>(start, end, step);
}

template <typename Num>
RangeIterator<Num> range(Num start, Num end) {
	if (start <= end)
		return range(start, end, Num(1));
	else
		return range(start, end, Num(-1));
}

template <typename Num>
RangeIterator<Num> range(Num start) {
	return range(start, Num(start - Num(1)), Num(1));
}

}


namespace std {

template <typename Num>
struct iterator_traits<stream::RangeIterator<Num>> {
	// difference_type
	// pointer
	using value_type = typename stream::RangeIterator<Num>::ValueType;
	using iterator_category = forward_iterator_tag;
	using reference = const value_type&;
};

}

#endif
