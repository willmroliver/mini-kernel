#ifndef INC_LIB_DEVICETREE_INIT_H
#define INC_LIB_DEVICETREE_INIT_H

#include <drivers/uart-pl011.h>
#include "dt.h"

uart_base_t *dt_init_uart_pl011(struct fdt_node *dt);

#endif
