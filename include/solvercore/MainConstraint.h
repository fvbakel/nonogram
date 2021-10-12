#pragma once

#include <vector>

#include <solvercore/constants.h>
#include <solvercore/Constraint.h>


class MainConstraint : public Constraint {
    
    public:
        MainConstraint(enum direction direction,std::vector<int> *blacks);
        ~MainConstraint();
};

typedef std::vector<MainConstraint*>    main_constraints;

