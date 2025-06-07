#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

void app_main(void);
void floating_point_test(void);
void pi_calculation_wrapper(void);
void measure_execution_time(void (*p_func)(void), const char* p_func_name, ...);
double calculate_pi_gauss_legendre(int iterations);

#endif // APP_MAIN_H