#pragma once
#ifndef _STATES_H_
#define _STATES_H_

// Abstract base class for all States.
class State
{
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual  ~State() = default;
protected:
	State() {}
	
};


#endif
