#pragma once

template<typename FirstType, typename SecondType>
class Convertable
{
public:
	// Second to first conversion
	virtual void fts(const FirstType& first, SecondType& second) const = 0;
	virtual void stf(const SecondType& first, FirstType& second) const = 0;
	virtual ~Convertable() = default;
};
