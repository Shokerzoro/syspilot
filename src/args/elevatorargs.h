//
// Created by ACA on 01.07.2026.
//

#ifndef UNITER_ELEVATOR_ELEVATORARGS_H
#define UNITER_ELEVATOR_ELEVATORARGS_H

namespace syspilot {

enum class ElevatorCommands { // Verbosed version 3. Need update(yes/no): no
    update,
    restartapp,
    uninstall
};


enum class ElevatorArgs { // Verbosed version 3. Need update(yes/no): no
    approot,
    updates,
    newversion,
    apppid
};


enum class ElevatorFlags { // Verbosed version 1. Need update(yes/no): no
    prepared
};

}

#endif //UNITER_ELEVATOR_ELEVATORARGS_H
