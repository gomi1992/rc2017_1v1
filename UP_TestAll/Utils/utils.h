//
// Created by admin on 2017/6/1.
//

#ifndef RC2017_1V1_UTILS_H
#define RC2017_1V1_UTILS_H

/*
 * find_nearest_in_array_descend
 * find nearest value in array
 * value : target value
 * array : target array
 * start : index of start
 * end : index of end
 */
int find_nearest_in_array_descend(int value, const int *array, int start, int end);//在数组中找到最近值（四灰度调用）

int find_in_array_descend_upper(int value, const int *array, int start, int end);

int find_in_array_descend_lower(int value, const int *array, int start, int end);

void debug_bluetooth_puts(char *s);

#endif //RC2017_1V1_UTILS_H
