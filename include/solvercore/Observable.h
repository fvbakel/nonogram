#pragma once
#include <unordered_set>
#include <solvercore/Observer.h>

class Observable 
{ 
    private:
        std::unordered_set<Observer*> m_observers; 

    public: 
        virtual ~Observable() = default;
        void attach(Observer *o);
        void detach(Observer *o);
        void notify();

};