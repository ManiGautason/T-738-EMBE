#include <util/delay.h>
#include <unity.h>

#include "controller.h"
#include "PI_controller.h"

void test_controller_max_limit(void) {
    controller ctrl(1.0, 1000, 255);
    int output = ctrl.update(2000, 250);  // This should saturate the output
    TEST_ASSERT_EQUAL(254, output);
}

void test_controller_min_limit(void) {
    controller ctrl(1.0, 1000, 255);
    int output = ctrl.update(0, 250);  // This should minimize the output
    TEST_ASSERT_EQUAL(1, output);  // Assuming 1 is the minimum valid output
}

void test_PI_controller_max_limit(void) {
    PI_controller pi_ctrl(1.0, 1.0, 1000, 255, 0.01);
    int output = pi_ctrl.update(2000, 250);  // This should saturate the output
    TEST_ASSERT_EQUAL(254, output);
}

void test_PI_controller_min_limit(void) {
    PI_controller pi_ctrl(1.0, 1.0, 1000, 255, 0.01);
    int output = pi_ctrl.update(0, 250);  // This should minimize the output
    TEST_ASSERT_EQUAL(1, output);  // Assuming 1 is the minimum valid output
}

int main() {
    _delay_ms(2000);

    UNITY_BEGIN();

    RUN_TEST(test_controller_max_limit);
    RUN_TEST(test_controller_min_limit);

    RUN_TEST(test_PI_controller_max_limit);
    RUN_TEST(test_PI_controller_min_limit);

    UNITY_END();
}
