#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "rvc_sw.h"

#define TICK 1000000

pthread_t inputThread, controllerThread;
MotorCommand motor_state;
CleanCommand cleaner_state;
Sensors sensors;
int tick;

void* input_thread(void* arg) {
    printf("Sensor Input (Front Left Right Back Dust) [Example: 1 0 1 0 1]\n");
    while (1) {
        sensors_input();
        usleep(TICK);
    }
    return NULL;
}

void* controller_thread(void* arg) {
    while (1) {
        controller();
        usleep(TICK);
    }
    return NULL;
}


int main() {
    init();

    // Create Threads
    if (pthread_create(&inputThread, NULL, input_thread, NULL) != 0 ||
        pthread_create(&controllerThread, NULL, controller_thread, NULL) != 0) {
        perror("Error creating thread");
        return 1;
    }

    // Wait for Threads
    pthread_join(inputThread, NULL);
    pthread_join(controllerThread, NULL);

    return 0;
}

void init() {
    update_motor_state(MOVE_FORWARD);
    update_cleaner_state(ON);
    sensors = (Sensors){ false, false, false, false, false };
    tick = 0;
}

void sensors_input() {
    printf("Waiting for Sensor Input...\n");
    scanf("%d %d %d %d %d",
        (int*)&sensors.front,
        (int*)&sensors.left,
        (int*)&sensors.right,
        (int*)&sensors.back,
        (int*)&sensors.dust);
}

void controller() {
    tick++;
    switch (motor_state) {
    case MOVE_FORWARD:
        if (sensors.front) {
            if (!sensors.left) {
                reset_tick();
                move_forward_to_turn_left();
            }
            else if (sensors.left && !sensors.right) {
                reset_tick();
                move_forward_to_turn_right();
            }
            else {
                move_forward_to_move_backward();
            }
        }
        else if (sensors.dust) {
            reset_tick();
            move_forward_to_power_up();
        }
        break;
    case TURN_LEFT:
        turn_left_to_move_forward();
        break;
    case TURN_RIGHT:
        turn_right_to_move_forward();
        break;
    case MOVE_BACKWARD:
        if (!sensors.left && !sensors.right) {
            move_backward_to_turn_left();
        }
        else if (sensors.left && !sensors.right) {
            move_backward_to_turn_right();
        }
        else if (sensors.front && sensors.left && sensors.right) {
            move_backward_to_stop();
        }
        break;
    case POWER_UP:
        power_up_to_move_forward();
        break;
    default:
        printf("Unhandled State!\n");
        break;
    }
}

void update_cleaner_state(CleanCommand new_state) {
    cleaner_state = new_state;
}

void update_motor_state(MotorCommand new_state) {
    motor_state = new_state;
}

void move_forward_to_power_up() {
    printf("State: MOVE_FORWARD -> POWER_UP\n");
    update_motor_state(POWER_UP);
    update_cleaner_state(UP);
}

void move_forward_to_turn_left() {
    printf("State: MOVE_FORWARD -> TURN_LEFT\n");
    update_motor_state(TURN_LEFT);
    update_cleaner_state(OFF);
}

void move_forward_to_move_backward() {
    printf("State: MOVE_FORWARD -> MOVE_BACKWARD\n");
    update_motor_state(MOVE_BACKWARD);
    update_cleaner_state(OFF);
}

void move_forward_to_turn_right() {
    printf("State: MOVE_FORWARD -> TURN_RIGHT\n");
    update_motor_state(TURN_RIGHT);
    update_cleaner_state(OFF);
}

void turn_left_to_move_forward() {
    if (tick >= 15) {
        printf("State: TURN_LEFT -> MOVE_FORWARD\n");
        update_motor_state(MOVE_FORWARD);
        update_cleaner_state(ON);
        sensors.front = false;
        reset_tick();
    }
}

void turn_right_to_move_forward() {
    if (tick >= 15) {
        printf("State: TURN_RIGHT -> MOVE_FORWARD\n");
        update_motor_state(MOVE_FORWARD);
        update_cleaner_state(ON);
        sensors.front = false;
        reset_tick();
    }
}

void move_backward_to_turn_left() {
    printf("State: MOVE_BACKWARD -> TURN_LEFT\n");
    update_motor_state(TURN_LEFT);
    update_cleaner_state(OFF);
}

void move_backward_to_stop() {
    printf("State: MOVE_BACKWARD -> STOP\n");
    update_motor_state(STOP);
    update_cleaner_state(OFF);
}

void move_backward_to_turn_right() {
    printf("State: MOVE_BACKWARD -> TURN_RIGHT\n");
    update_motor_state(TURN_RIGHT);
    update_cleaner_state(OFF);
}

void power_up_to_move_forward() {
    if (sensors.front || tick >= 15) {
        printf("State: POWER_UP -> MOVE_FORWARD\n");
        update_motor_state(MOVE_FORWARD);
        update_cleaner_state(ON);
        reset_tick();
    }
}

void reset_tick() {
    tick = 0;
}