#pragma once
#include <stack>
#include <cassert>
#include <iostream>

template <class T, typename State>
class StateMachine
{
public:
	StateMachine() = delete;
	StateMachine(State defaultState)
	{
		((T*)this)->m_states.push(defaultState);
	}
	virtual ~StateMachine() = default;

	template<typename ...Args>
	void Update(Args & ... args)
	{
		(((T*)this)->*(((T*)this)->m_states.top()))(args...);
	}

	State GetCurrentState()
	{
		return ((T*)this)->m_states.top();
	}

protected:
	void ReplaceState(State nextState)
	{
		((T*)this)->m_states.pop();
		((T*)this)->m_states.push(nextState);
	}

	void EnterTempState(State tempState)
	{
		((T*)this)->m_states.push(tempState);
	}

	void ExitTempState()
	{
		((T*)this)->m_states.pop();
#ifdef _DEBUG
		if (((T*)this)->m_states.empty())
		{
			std::cout << "\n\tERROR :\nStateMachine.h --> remove last state\n Next call to Update(Args... args) will crash because the previous state was remove instead of replace" << std::endl;
			assert(false);
		}
#endif // _DEBUG
	}

private:
	std::stack<State> m_states;
};
