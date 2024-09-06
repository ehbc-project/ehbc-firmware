#ifndef EHBCFW_TYPES_H__
#define EHBCFW_TYPES_H__

#include <stdint.h>
#include <stddef.h>

#include "config.h"

typedef long maddr_t;

typedef volatile uint8_t  hwreg8_t;
typedef volatile uint16_t hwreg16_t;
typedef volatile uint32_t hwreg32_t;
typedef volatile uint64_t hwreg64_t;

#endif
