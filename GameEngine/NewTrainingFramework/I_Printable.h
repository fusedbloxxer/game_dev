#pragma once
#include <ostream>

class I_Printable
{
public:
	friend std::ostream& operator<<(std::ostream& os, const I_Printable& object);
	virtual void print(std::ostream& os) const = 0;
	virtual ~I_Printable() = default;
};