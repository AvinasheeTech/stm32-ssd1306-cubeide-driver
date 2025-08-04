#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void spi_init(void);
void spi_transfer(uint8_t *c,size_t len);
void spi_reset(bool level);
void spi_set_cs(bool level);
void spi_set_dc(bool level);

#endif