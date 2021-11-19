#pragma once

class Observable;

class Observer 
{ 
    public:
        virtual void update(Observable*) = 0;
};