#pragma once

#include <solvercore/Nonogram.h>

class NonogramQt;

class Location_observer : public Observer  {

    public:
        Location_observer(NonogramQt *qui);

        void update(Observable *updated_observable) override;

    private:
        NonogramQt      *m_gui = nullptr;
        unsigned int     m_delay = 1;
};