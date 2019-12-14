#pragma once

template<typename T>
class IBuilder
{
public:
	virtual T* build() const = 0;
	virtual ~IBuilder() = default;
};