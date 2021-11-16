#include <solvercore/Observable.h>


void Observable::attach(Observer *o) {
    m_observers.insert(o); 
}

void Observable::detach(Observer *o)
{
    m_observers.erase(o);
}

void Observable::notify()
{
    for (auto* o : m_observers) {
        o->update(this);
    }
}