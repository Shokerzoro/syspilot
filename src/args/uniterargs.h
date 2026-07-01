//
// Created by ACA on 01.07.2026.
//

#ifndef UNITER_ELEVATOR_UNITERARGS_H
#define UNITER_ELEVATOR_UNITERARGS_H

namespace syspilot {

    enum class UniterCommands { // Verbosed version 2. Need update(yes/no): no
        run
    };


    enum class UniterArgs { // Verbosed version 2. Need update(yes/no): no
        servertype,
        serverip,
        updated,
        updatefailed
    };

}

#endif //UNITER_ELEVATOR_UNITERARGS_H
