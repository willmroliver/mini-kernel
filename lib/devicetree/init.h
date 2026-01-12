#ifndef INC_LIB_DEVICETREE_INIT_H
#define INC_LIB_DEVICETREE_INIT_H

#include <drivers/arm/uart-pl011.h>
#include "dt.h"

struct uart *dt_init_uart_pl011(struct fdt_node *dt);

#endif
