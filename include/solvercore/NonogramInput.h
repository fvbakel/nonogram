#pragma once

#include <vector>
#include <string>

class NonogramInput {
    protected:
        std::vector<std::vector<int>>     m_x_clues;
        std::vector<std::vector<int>>     m_y_clues;
    public:
        NonogramInput();
        ~NonogramInput();

        void add_x_clue(std::vector<int>  &x_clue);
        std::vector<int>  *get_x_clue(int index);
        int get_nr_of_x_clues();

        void add_y_clue(std::vector<int>  &y_clue);
        std::vector<int>  *get_y_clue(int index);
        int get_nr_of_y_clues();
};