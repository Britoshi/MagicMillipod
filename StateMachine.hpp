#include "State.hpp"

class StateMachine
{

public: 
    State *currentState;
    StateMachine();
    ~StateMachine();

    void Update();
}