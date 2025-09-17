#include <SFML/System.h>

static sfClock* clock = NULL;

sfClock* getClock() {
    if (!clock) {
        clock = sfClock_create();
    }
    return clock;
}

float restartDeltaTime(void) {
    sfTime deltaTime = sfClock_restart(getClock());
    return sfTime_asSeconds(deltaTime);
}
