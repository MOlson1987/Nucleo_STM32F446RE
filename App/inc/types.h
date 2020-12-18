#ifndef TYPES_H_
#define TYPES_H_

// Utility MACROS
#define BIT(X) ((uint32_t)(1 << (X)))
#define NULL   ((void*)0)

typedef enum bool
{
    false,
    true = !false,
} bool;


#endif
