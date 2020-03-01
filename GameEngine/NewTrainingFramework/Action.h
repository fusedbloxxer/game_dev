#pragma once

class Action
{
public:
	virtual bool isExecuting() const = 0;
	virtual void execute() = 0;
	virtual ~Action() = 0;
};