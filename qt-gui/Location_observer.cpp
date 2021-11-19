//#include "Location_observer.h"
#include "NonogramQt.h"
#include <chrono>
#include <thread>

Location_observer::Location_observer(NonogramQt *gui) {
    m_gui = gui;
}


void Location_observer::update(Observable *updated_observable) {
    Location *location = dynamic_cast<Location*>(updated_observable);
    if (m_gui != nullptr) {
        m_gui->draw_location(location);
        std::this_thread::sleep_for(std::chrono::milliseconds(m_delay));
    }
}