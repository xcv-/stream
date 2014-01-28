#ifndef STREAM_FOREACH_H
#define STREAM_FOREACH_H

#include <iterator>
#include <type_traits>

namespace stream {

template <typename F>
struct ForEachParams {
	F function;
};


template <typename Input, typename F>
struct ForEach {

	ForEach(Input input, ForEachParams<F> params)
		: m_input(std::move(input)), m_params(std::move(params))
	{}

    ~ForEach() {
        run();
    }

    void run() {
		while (m_input.hasNext()) {
			m_input.next();
			m_params.function(m_input.get());
		}
	}

	private:
		Input m_input;
		ForEachParams<F> m_params;
};


template <typename Input, typename F>
struct Output<Input, ForEachParams<F>> {
	using Type = ForEach<Input, F>;
};

template <typename F>
struct IsOutputParams<ForEachParams<F>> {
	static constexpr bool value = true;
};



template <typename F>
ForEachParams<F> for_each(F function) {
	return ForEachParams<F>{function};
}

}

#endif
