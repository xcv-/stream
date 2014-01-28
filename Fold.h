#ifndef STREAM_FOLDL_H
#define STREAM_FOLDL_H

#include <iterator>
#include <type_traits>

namespace stream {

template <typename A, typename F>
struct FoldParams {
	A initial;
	F function;
};


template <typename Input, typename A, typename F>
struct FoldL {

	using ValueType = typename std::result_of<F(A, typename Input::ValueType)>::type;

	FoldL(Input input, FoldParams<A,F> params)
		: m_input(std::move(input)), m_params(std::move(params))
	{}

	operator ValueType() const {
		auto input = m_input;

		ValueType acc = std::move(m_params.initial);

		while (input.hasNext()) {
			input.next();
			acc = m_params.function(std::move(acc), input.get());
		}

		return acc;
	}

	private:
		Input m_input;
		FoldParams<A,F> m_params;
};


template <typename Input, typename A, typename F>
struct Output<Input, FoldParams<A,F>> {
	using Type = FoldL<Input, A, F>;
};

template <typename A, typename F>
struct IsOutputParams<FoldParams<A,F>> {
	static constexpr bool value = true;
};



template <typename A, typename F>
FoldParams<A,F> foldl(A initial, F function) {
	return FoldParams<A,F>{std::forward<A>(initial), function};
}

}

#endif
