#pragma once

class Observable;

class Observer 
{ 
    public:
        virtual ~Observer() = default;
        virtual void update(Observable*) = 0;
};