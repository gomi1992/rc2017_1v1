//
// Created by admin on 2017/6/1.
//

#include "utils.h"
#include "../UpLib/UP_Bluetooth.h"

int find_nearest_in_array_descend(int value, const int *array, int start, int end) {
    int i;
    for (i = start; i < end; i++) {
        if (value > array[i])
            break;
    }
    if (i > start)
        return (value - array[i - 1]) < (array[i] - value) ? i - 1 : i;
    else
        return i;
}

int find_in_array_descend_upper(int value, const int *array, int start, int end) {
    int i;
    for (i = start; i < end; i++) {
        if (value > array[i])
            break;
    }
    return i;
}

int find_in_array_descend_lower(int value, const int *array, int start, int end) {
    int i;
    for (i = start; i < end; i++) {
        if (value > array[i])
            return (i > 0) ? i - 1 : i;
    }
    return i;
}

void debug_bluetooth_puts(char *s) {
#ifdef DEBUG_ON
    UP_Bluetooth_Puts(s);
#endif
}