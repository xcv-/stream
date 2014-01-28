#ifndef STREAM_H
#define STREAM_H

#include <type_traits>

namespace stream {

// Traits

template <typename T>
struct IsInputIterator {
	static constexpr bool value = false;
};

template <typename T>
struct IsOutputParams {
	static constexpr bool value = false;
};

template <typename T>
struct IsOutputAdapter {
	static constexpr bool value = false;
};

template <typename Input, typename Params>
struct Output;


// Exposed API

template <typename Input, typename OutputParams>
typename std::enable_if<
	IsInputIterator<Input>::value && IsOutputParams<OutputParams>::value,
	typename Output<Input, OutputParams>::Type
>::type operator >= (Input input, OutputParams outputParams) {

	return typename Output<Input, OutputParams>::Type(input, outputParams);
}

template <typename Input, typename OutputAdapter>
typename std::enable_if<
	IsInputIterator<Input>::value && IsOutputAdapter<OutputAdapter>::value,
	typename OutputAdapter::template ResultType<Input>
>::type operator >= (Input input, OutputAdapter adapter) {

	return adapter.result(input);
}

}

#include "Filter.h"
#include "Fold.h"
#include "ForEach.h"
#include "Map.h"
#include "Range.h"
#include "Singleton.h"
#include "Zip.h"

#include "STLInput.h"
#include "STLOutput.h"


#endif // STREAM_H
