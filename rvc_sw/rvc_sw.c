#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "rvc_sw.h"

#define TICK 100000000 

pthread_t inputThread, controllerThread;
struct timespec req;
MotorCommand motor_state;
CleanCommand cleaner_state;
ObstacleLocation obstacle_location;
bool dust_existence;
SensorState sensor_state;
int tick;

void init() {
    motor_interface(MOVE_FORWARD);
    cleaner_interface(ON);

    req.tv_sec = 0;
    req.tv_nsec = TICK;
    tick = 0;
}

void sensors_input() {
    printf("Waiting for Sensor Input...\n");
    // Determine obstacle location & dust existence
    if (scanf("%d %d %d %d %d",
        (int*)&obstacle_location.front_obstacle,
        (int*)&obstacle_location.left_obstacle,
        (int*)&obstacle_location.right_obstacle,
        (int*)&obstacle_location.back_obstacle,
        (int*)&dust_existence) != 5) {
        printf("Invalid input! Please provide 5 integer values.\n");
        return;
    }
    // Update sensor state
    sensor_state.front = obstacle_location.front_obstacle;
    sensor_state.left = obstacle_location.left_obstacle;
    sensor_state.right = obstacle_location.right_obstacle;
    sensor_state.back = obstacle_location.back_obstacle;
    sensor_state.dust = dust_existence;
}

void controller() {
    tick++;
    switch (motor_state) {
    case MOVE_FORWARD:
        if (sensor_state.front) {
            if (!sensor_state.left) {
                reset_tick();
                move_forward_to_turn_left();
            }
            else if (sensor_state.left && !sensor_state.right) {
                reset_tick();
                move_forward_to_turn_right();
            }
            else {
                move_forward_to_move_backward();
            }
        }
        else if (sensor_state.dust) {
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
        if (!sensor_state.left) {
            move_backward_to_turn_left();
        }
        else if (sensor_state.left && !sensor_state.right) {
            move_backward_to_turn_right();
        }
        else if (sensor_state.back && sensor_state.left && sensor_state.right) {
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

void cleaner_interface(CleanCommand next_cleaner_state) {
    cleaner_state = next_cleaner_state;
}

void motor_interface(MotorCommand next_motor_state) {
    motor_state = next_motor_state;
}

void move_forward_to_power_up() {
    printf("Motor State: MOVE_FORWARD -> POWER_UP\n");
    printf("Cleaner State: ON -> UP\n");
    motor_interface(POWER_UP);
    cleaner_interface(UP);
}

void move_forward_to_turn_left() {
    printf("Motor State: MOVE_FORWARD -> TURN_LEFT\n");
    printf("Cleaner State: ON -> OFF\n");
    motor_interface(TURN_LEFT);
    cleaner_interface(OFF);
}

void move_forward_to_move_backward() {
    printf("Motor State: MOVE_FORWARD -> MOVE_BACKWARD\n");
    printf("Cleaner State: ON -> OFF\n");
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);
}

void move_forward_to_turn_right() {
    printf("Motor State: MOVE_FORWARD -> TURN_RIGHT\n");
    printf("Cleaner State: ON -> OFF\n");
    motor_interface(TURN_RIGHT);
    cleaner_interface(OFF);
}

void turn_left_to_move_forward() {
    if (tick >= 15) {
        printf("Motor State: TURN_LEFT -> MOVE_FORWARD\n");
        printf("Cleaner State: OFF -> ON\n");
        motor_interface(MOVE_FORWARD);
        cleaner_interface(ON);
        sensor_state.front = false;
        reset_tick();
    }
}

void turn_right_to_move_forward() {
    if (tick >= 15) {
        printf("Motor State: TURN_RIGHT -> MOVE_FORWARD\n");
        printf("Cleaner State: OFF -> ON\n");
        motor_interface(MOVE_FORWARD);
        cleaner_interface(ON);
        sensor_state.front = false;
        reset_tick();
    }
}

void move_backward_to_turn_left() {
    printf("Motor State: MOVE_BACKWARD -> TURN_LEFT\n");
    printf("Cleaner State: OFF\n");
    motor_interface(TURN_LEFT);
}

void move_backward_to_turn_right() {
    printf("Motor State: MOVE_BACKWARD -> TURN_RIGHT\n");
    printf("Cleaner State: OFF\n");
    motor_interface(TURN_RIGHT);
}


void move_backward_to_stop() {
    printf("Motor State: MOVE_BACKWARD -> STOP\n");
    printf("Cleaner State: OFF\n");
    motor_interface(STOP);
    stop_threads();
}

void power_up_to_move_forward() {
    if (sensor_state.front || tick >= 10) {
        printf("Motor State: POWER_UP -> MOVE_FORWARD\n");
        printf("Cleaner State: OFF -> ON\n");
        motor_interface(MOVE_FORWARD);
        cleaner_interface(ON);
        reset_tick();
    }
}

void reset_tick() {
    tick = 0;
}

void thread_cleanup_handler(void* arg) {
    pthread_t* thread_id = (pthread_t*)arg;
    printf("Thread %lu is being cancelled, clean up initiated.\n", *thread_id);
}

void stop_threads() {
    printf("Sending cancel request to threads...\n");
    pthread_cancel(inputThread);
    pthread_cancel(controllerThread);
}