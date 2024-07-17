#ifndef __BSP_DMAC_H
#define __BSP_DMAC_H
#include "hal_data.h"


void DMAC_Init(void);

void set_transfer_length(transfer_cfg_t const * p_config, volatile uint16_t _length);
void set_transfer_dst_src_address(transfer_cfg_t const * const p_config,
        const volatile uint8_t * _p_src, const volatile uint8_t * _p_dest);

#endif /* __BSP_DMAC_H */
