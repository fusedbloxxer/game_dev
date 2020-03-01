#pragma once

template<typename InputType, typename OutputType>
class ObjectConverter {
public:
	virtual OutputType inputToOutput(const InputType& input) const = 0;
	virtual InputType outputToInput(const OutputType& output) const = 0;
	virtual ~ObjectConverter() = default;
};