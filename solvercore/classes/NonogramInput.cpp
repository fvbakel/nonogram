#include <solvercore/NonogramInput.h> 
        
NonogramInput::NonogramInput() {
    return;
}

NonogramInput::~NonogramInput() {
   m_x_clues.clear();
   m_y_clues.clear();
}

void NonogramInput::add_x_clue(std::vector<int>  &x_clue) {
    m_x_clues.push_back(std::vector<int>(x_clue));
}

std::vector<int> *NonogramInput::get_x_clue(int index) {
    return &(m_x_clues.at(index));
}

int NonogramInput::get_nr_of_x_clues() {
    return m_x_clues.size();
}

void NonogramInput::add_y_clue(std::vector<int>  &y_clue) {
    m_y_clues.push_back(std::vector<int>(y_clue));
}

std::vector<int> *NonogramInput::get_y_clue(int index) {
    return &(m_y_clues.at(index));
}

int NonogramInput::get_nr_of_y_clues() {
    return m_y_clues.size();
}

