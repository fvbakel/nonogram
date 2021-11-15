#pragma once

class Observable;

class Observer 
{ 
    public:
        virtual ~Observer() = default;
        virtual void Update(Observable&) = 0;
};