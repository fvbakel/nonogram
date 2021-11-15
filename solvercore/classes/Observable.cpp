#include <solvercore/Observable.h>


void Observable::Attach(Observer& o) {
    m_observers.insert(&o); 
}

void Observable::Detach(Observer& o)
{
    m_observers.erase(&o);
}

void Observable::Notify()
{
    for (auto* o : m_observers) {
        o->Update(*this);
    }
}