#ifndef STREAM_CURRY_H
#define STREAM_CURRY_H

#include <functional>


namespace stream {

template <typename R, typename... As>
struct Curry;

template <typename R, typename... As>
using CurryType = typename Curry<R (As...)>::Type;


template <typename R, typename A>
struct Curry<R (A)> {

	using Type = std::function<R(A)>;

	template <typename F>
	static Type curry(F f) {
		return [=](A arg) {
			return f(std::move(arg));
		};
	}
};


template <typename R, typename A1, typename A2, typename... As>
struct Curry<R (A1, A2, As...)> {

	using Type = std::function<typename Curry<R (A2, As...)>::Type (A1)>;

	template <typename F>
	static Type curry(F f) {

		return [=](const A1& arg1) {
			auto curried = [=](A2 arg2, As... args) {
				return f(std::move(arg1), std::move(arg2), std::move(args)...);
			};

			return Curry<R (A2, As...)>::curry(curried);
		};
	}
};

template <typename R, typename... As>
struct Curry<R (As...) const> : public Curry<R (As...)> {
};


template <typename F>
struct MethodType;

template <typename T, typename F>
struct MethodType<F (T::*)> {
	using Type = F;
};

template <typename F>
using LambdaType = typename MethodType< decltype(&F::operator()) >::Type;


template <typename F>
auto curry(F f) -> typename Curry<LambdaType<decltype(f)>>::Type {
	return Curry<LambdaType<decltype(f)>>::curry(f);
}

}

#endif // STREAM_CURRY_H
