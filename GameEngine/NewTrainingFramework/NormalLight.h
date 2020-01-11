#pragma once
#include "Convertable.h"
#include "Light.h"
#include <string>

enum class NormalLight::Type;

class NormalLight : public Light, public Convertable<NormalLight::Type, std::string>
{
public:

	enum class Type
	{
		POINT,
		DIRECTIONAL,
		SPOTLIGHT
	};

protected:
	Type type;

	NormalLight(Type type);

public:
	virtual void print(std::ostream& os) const override;

	virtual ~NormalLight() = 0;

	// Getters and setters
	NormalLight::Type getLightType() const;
	void setLightType(Type type);

	// Inherited via Convertable
	virtual void fts(const NormalLight::Type& first, std::string& second) const override;
	
	// virtual void stf(const SecondType& first, FirstType& second) const override;
};

