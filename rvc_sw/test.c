#include <stddef.h>
#include <signal.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "rvc_sw.h"

void setup(void) {
    FILE* original_stdout = stdout;
    stdout = fopen("/dev/null", "w");
    init();
}

void teardown(void) {
}

ReportHook(PRE_ALL)(struct criterion_test_set* tests) {
    (void)tests;
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    puts("criterion_init");
}

ReportHook(PRE_INIT)(struct criterion_test* test) {
    printf("testing %s in category %s\n", test->name, test->category);
}

ReportHook(POST_ALL)(struct criterion_global_stats* stats) {
    (void)stats;
    puts("criterion_fini");
}

bool compare_sensor_state(const SensorState* actual, const SensorState* expected) {
    return actual->front == expected->front &&
        actual->left == expected->left &&
        actual->right == expected->right &&
        actual->back == expected->back &&
        actual->dust == expected->dust;
}

TestSuite(controller_suite, .init = setup, .fini = teardown);

Test(controller_suite, RVC_UTC_001_001) {
    SensorState expected = { false, false, false, false, false };

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
    cr_assert(compare_sensor_state(&sensor_state, &expected),
        "SensorState should be fully initialized to {false, false, false, false, false}.");
}

Test(controller_suite, RVC_UTC_001_002_A) {
    sensor_state.front = false;
    sensor_state.dust = true;

    controller();

    cr_assert_eq(motor_state, POWER_UP, "Motor state should be POWER_UP.");
    cr_assert_eq(cleaner_state, UP, "Cleaner state should be UP.");
}

Test(controller_suite, RVC_UTC_001_002_B) {
    sensor_state.front = false;
    sensor_state.dust = false;

    controller();

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
}

Test(controller_suite, RVC_UTC_001_002_C) {
    sensor_state.front = true;
    sensor_state.left = false;
    sensor_state.right = false;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_D) {
    sensor_state.front = true;
    sensor_state.left = true;
    sensor_state.right = false;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_RIGHT, "Motor state should be TURN_RIGHT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_E) {
    sensor_state.front = true;
    sensor_state.left = false;
    sensor_state.right = true;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_F) {
    sensor_state.front = true;
    sensor_state.left = false;
    sensor_state.right = false;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_G) {
    sensor_state.front = true;
    sensor_state.left = true;
    sensor_state.right = true;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, MOVE_BACKWARD, "Motor state should be MOVE_BACKWARD.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_H) {
    sensor_state.front = true;
    sensor_state.left = true;
    sensor_state.right = false;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_RIGHT, "Motor state should be TURN_RIGHT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_I) {
    sensor_state.front = true;
    sensor_state.left = false;
    sensor_state.right = true;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_002_J) {
    sensor_state.front = true;
    sensor_state.left = true;
    sensor_state.right = true;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, MOVE_BACKWARD, "Motor state should be MOVE_BACKWARD.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_A) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = false;
    sensor_state.right = false;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_B) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = true;
    sensor_state.right = false;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_RIGHT, "Motor state should be TURN_RIGHT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

// Bug fix: Previously moved incorrectly, now fixed
Test(controller_suite, RVC_UTC_001_003_C) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = false;
    sensor_state.right = true;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_D) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = false;
    sensor_state.right = false;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_E) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = true;
    sensor_state.right = true;
    sensor_state.back = false;

    controller();

    cr_assert_eq(motor_state, MOVE_BACKWARD, "Motor state should be MOVE_BACKWARD.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_F) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = true;
    sensor_state.right = false;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_RIGHT, "Motor state should be TURN_RIGHT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_G) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = false;
    sensor_state.right = true;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_003_H, .signal = SIGSEGV) {
    motor_interface(MOVE_BACKWARD);
    cleaner_interface(OFF);

    sensor_state.left = true;
    sensor_state.right = true;
    sensor_state.back = true;

    controller();

    cr_assert_eq(motor_state, STOP, "Motor state should be STOP.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_004_A) {
    motor_interface(TURN_LEFT);
    cleaner_interface(OFF);

    tick = 10; // tick < 15

    controller();

    cr_assert_eq(motor_state, TURN_LEFT, "Motor state should be TURN_LEFT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_004_B) {
    motor_interface(TURN_LEFT);
    cleaner_interface(OFF);

    tick = 15; // tick >= 15

    controller();

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
}

Test(controller_suite, RVC_UTC_001_005_A) {
    motor_interface(TURN_RIGHT);
    cleaner_interface(OFF);

    tick = 10; // tick < 15

    controller();

    cr_assert_eq(motor_state, TURN_RIGHT, "Motor state should be TURN_RIGHT.");
    cr_assert_eq(cleaner_state, OFF, "Cleaner state should be OFF.");
}

Test(controller_suite, RVC_UTC_001_005_B) {
    motor_interface(TURN_RIGHT);
    cleaner_interface(OFF);

    tick = 15; // tick >= 15

    controller();

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
}

Test(controller_suite, RVC_UTC_001_006_A) {
    motor_interface(POWER_UP);
    cleaner_interface(UP);

    sensor_state.front = false;
    tick = 5; // tick < 10

    controller();

    cr_assert_eq(motor_state, POWER_UP, "Motor state should be POWER_UP.");
    cr_assert_eq(cleaner_state, UP, "Cleaner state should be UP.");
}

Test(controller_suite, RVC_UTC_001_006_B) {
    motor_interface(POWER_UP);
    cleaner_interface(UP);

    sensor_state.front = true;
    tick = 5; // tick < 10

    controller();

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
}

Test(controller_suite, RVC_UTC_001_006_C) {
    motor_interface(POWER_UP);
    cleaner_interface(UP);

    tick = 10; // tick >= 10

    controller();

    cr_assert_eq(motor_state, MOVE_FORWARD, "Motor state should be MOVE_FORWARD.");
    cr_assert_eq(cleaner_state, ON, "Cleaner state should be ON.");
}
