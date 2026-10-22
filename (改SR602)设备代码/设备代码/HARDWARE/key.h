
#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

/************************************************
 * Key Definition
 * Key mapping:
 * KEY_LEFT  - Left Key  (0)
 * KEY_RIGHT - Right Key (1)
 * KEY_UP    - Up Key    (2)
 * KEY_DOWN  - Down Key  (3)
 ***********************************************/

/* Key enumeration */
typedef enum
{
    KEY_LEFT = 0,  /* Left Key */
    KEY_RIGHT = 1,  /* Right Key */
    KEY_UP = 2,  /* Up Key */
    KEY_DOWN = 3,  /* Down Key */
} key_value_t;

/* Function declarations */
void key_init(void);  /* Key initialization */
uint8_t key_scan(void);    /* Key scan, returns key value */
#endif
