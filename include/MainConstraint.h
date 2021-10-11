#pragma once

#include <vector>

#include <constants.h>
#include <Constraint.h>


class MainConstraint : public Constraint {
    
    public:
        MainConstraint(enum direction direction,std::vector<int> *blacks);
        ~MainConstraint();
};

typedef std::vector<MainConstraint*>    main_constraints;

