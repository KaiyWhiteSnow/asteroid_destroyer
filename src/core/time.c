#include <SFML/System.h>

static sfClock* clock = NULL;

// Create clock
sfClock* getClock() {
    if (!clock) {
        clock = sfClock_create();
    }
    return clock;
}

// Returns deltatime
float restartDeltaTime(void) {
    sfTime deltaTime = sfClock_restart(getClock());
    return sfTime_asSeconds(deltaTime);
}
