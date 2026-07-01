//
// Created by ACA on 01.07.2026.
//

#ifndef UNITER_ELEVATOR_ELEVATORARGS_H
#define UNITER_ELEVATOR_ELEVATORARGS_H

namespace syspilot {

enum class ElevatorCommands { // Verbosed version 2. Need update(yes/no): no
    update,
    uninstall
};


enum class ElevatorArgs { // Verbosed version 2. Need update(yes/no): no
    approot,
    updates,
    newversion,
    prepared,
    apppid
};

}

#endif //UNITER_ELEVATOR_ELEVATORARGS_H
