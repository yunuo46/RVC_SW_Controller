#ifndef RVC_CONTROLLER_H
#define RVC_CONTROLLER_H

#include <stdbool.h>

// Enum Definitions
typedef enum {
    INIT,
    MOVE_FORWARD,
    TURN_LEFT,
    TURN_RIGHT,
    MOVE_BACKWARD,
    STOP,
    POWER_UP
} MotorCommand;

typedef enum {
    OFF,
    ON,
    UP
} CleanCommand;

// Sensors Struct
typedef struct {
    bool front;
    bool left;
    bool right;
    bool back;
    bool dust;
} Sensors;

// Global Variables
extern MotorCommand motor_state;
extern CleanCommand cleaner_state;
extern Sensors sensors;


// Function Declarations
void init();
void sensors_input();
void controller();
void update_cleaner_state(CleanCommand new_state);
void update_motor_state(MotorCommand new_state);
void move_forward_to_power_up();
void move_forward_to_turn_left();
void move_forward_to_move_backward();
void move_forward_to_turn_right();
void turn_left_to_move_forward();
void turn_right_to_move_forward();
void move_backward_to_turn_left();
void move_backward_to_stop();
void move_backward_to_turn_right();
void power_up_to_move_forward();
void reset_tick();

#endif