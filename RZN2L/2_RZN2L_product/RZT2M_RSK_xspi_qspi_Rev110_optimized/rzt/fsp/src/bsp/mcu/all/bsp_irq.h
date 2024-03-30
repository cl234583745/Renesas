/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU) */

#ifndef BSP_IRQ_H
#define BSP_IRQ_H

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_ICU_VECTOR_MAX_ENTRIES    (BSP_VECTOR_TABLE_MAX_ENTRIES)

#define BSP_PRV_GICD_ADDRESS(cpu_num)                   (cpu_num == 0 ? (GICD0) : (GICD1))
#define BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(cpu_num)    (cpu_num == 0 ? (GICR0_TARGET0_INTREG) : (GICR1_TARGET0_INTREG))
#define BSP_PRV_GICR_TARGET0_IFREG_ADDRESS(cpu_num)     (cpu_num == 0 ? (GICR0_TARGET0_IFREG) : (GICR1_TARGET0_IFREG))

#define BSP_PRV_IRQ_CONFIG_MASK               (0x000000FFU)
#define BSP_PRV_GICD_ICFGR_INT_CONFIG_MASK    (1UL << 1UL)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern void * gp_renesas_isr_context[BSP_ICU_VECTOR_MAX_ENTRIES + BSP_CORTEX_VECTOR_TABLE_ENTRIES];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Sets the ISR context associated with the requested IRQ.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @param[in]  p_context      ISR context for IRQ.
 **********************************************************************************************************************/
__STATIC_INLINE void R_FSP_IsrContextSet (IRQn_Type const irq, void * p_context)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    if (irq >= 0)
    {
        gp_renesas_isr_context[irq] = p_context;
    }
    else
    {
        gp_renesas_isr_context[(uint32_t) irq + BSP_VECTOR_NUM_OFFSET + BSP_ICU_VECTOR_MAX_ENTRIES] = p_context;
    }
}

/*******************************************************************************************************************//**
 * Clear the GIC pending interrupt.
 *
 * @param[in] irq            Interrupt for which to clear the Pending bit. Note that the enums listed for IRQn_Type are
 *                           only those for the Cortex Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqClearPending (IRQn_Type irq)
{
    GICD_Type         * GICD;
    GICR_SGI_PPI_Type * GICR_TARGET0_INTREG;

    GICD                = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICR_TARGET0_INTREG = BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(BSP_CFG_CPU);

    if (irq >= 0)
    {
        uint32_t _irq = (uint32_t) irq;
        GICD->GICD_ICPENDR[(((uint32_t) irq) /
                            BSP_VECTOR_NUM_OFFSET)] = (uint32_t) (0x00000001UL << (_irq % BSP_VECTOR_NUM_OFFSET));
    }
    else
    {
        uint32_t _irq = ((uint32_t) irq + BSP_VECTOR_NUM_OFFSET);
        GICR_TARGET0_INTREG->GICR_ICPENDR0 = (uint32_t) (0x00000001UL << _irq);
    }
}

/*******************************************************************************************************************//**
 * Get the GIC pending interrupt.
 *
 * @param[in] irq            Interrupt that gets a pending bit.. Note that the enums listed for IRQn_Type are
 *                           only those for the Cortex Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 *
 * @return  Value indicating the status of the level interrupt.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_BSP_IrqPendingGet (IRQn_Type irq)
{
    GICD_Type         * GICD;
    GICR_SGI_PPI_Type * GICR_TARGET0_INTREG;
    uint32_t            value = 0;

    GICD                = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICR_TARGET0_INTREG = BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(BSP_CFG_CPU);

    if (irq >= 0)
    {
        uint32_t _irq  = (uint32_t) irq;
        uint32_t shift = (_irq % BSP_VECTOR_NUM_OFFSET);
        value = (GICD->GICD_ISPENDR[(((uint32_t) irq) / BSP_VECTOR_NUM_OFFSET)] & (uint32_t) (0x00000001UL << shift)) >>
                shift;
    }
    else
    {
        uint32_t _irq  = ((uint32_t) irq + BSP_VECTOR_NUM_OFFSET);
        uint32_t shift = _irq;
        value = (GICR_TARGET0_INTREG->GICR_ISPENDR0 & (uint32_t) (0x00000001UL << shift)) >> shift;
    }

    return value;
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context.
 *
 * @param[in] irq            The IRQ number to configure.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqCfg (IRQn_Type const irq, uint32_t priority, void * p_context)
{
#if (52U == __CORTEX_R)
    GICD_Type         * GICD;
    GICR_SGI_PPI_Type * GICR_TARGET0_INTREG;

    GICD                = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICR_TARGET0_INTREG = BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(BSP_CFG_CPU);

    if (irq >= 0)
    {
        uint32_t _irq = (uint32_t) irq;

        /* Set the interrupt group to 1 (IRQ) */
        GICD->GICD_IGROUPR[(((uint32_t) irq) /
                            BSP_VECTOR_NUM_OFFSET)] |= (uint32_t) (0x00000001UL << (_irq % BSP_VECTOR_NUM_OFFSET));

        /* Set the interrupt priority */
        GICD->GICD_IPRIORITYR[((uint32_t) irq) /
                              4UL] &= (uint32_t) (~(BSP_PRV_IRQ_CONFIG_MASK << (8UL * (_irq % 4UL))));
        GICD->GICD_IPRIORITYR[((uint32_t) irq) /
                              4UL] |= (priority << (BSP_FEATURE_BSP_IRQ_PRIORITY_POS_BIT + (8UL * (_irq % 4UL))));

        /* Store the context. The context is recovered in the ISR. */
        R_FSP_IsrContextSet(irq, p_context);
    }
    else
    {
        uint32_t _irq = ((uint32_t) irq + BSP_VECTOR_NUM_OFFSET);

        /* Set the interrupt group to 1 (IRQ) */
        GICR_TARGET0_INTREG->GICR_IGROUPR0 |= (uint32_t) (0x00000001UL << _irq);

        /* Set the interrupt priority */
        GICR_TARGET0_INTREG->GICR_IPRIORITYR[(((uint32_t) irq + BSP_VECTOR_NUM_OFFSET)) /
                                             4UL] &= (uint32_t) (~(BSP_PRV_IRQ_CONFIG_MASK << (8UL * (_irq % 4UL))));
        GICR_TARGET0_INTREG->GICR_IPRIORITYR[(((uint32_t) irq + BSP_VECTOR_NUM_OFFSET)) /
                                             4UL] |=
            (priority << (BSP_FEATURE_BSP_IRQ_PRIORITY_POS_BIT + (8UL * (_irq % 4UL))));

        /* Store the context. The context is recovered in the ISR. */
        R_FSP_IsrContextSet(irq, p_context);
    }
#endif
}

/*******************************************************************************************************************//**
 * Enable the IRQ in the GIC (Without clearing the pending bit).
 *
 * @param[in] irq            The IRQ number to enable. Note that the enums listed for IRQn_Type are only those for the
 *                           Cortex Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqEnableNoClear (IRQn_Type const irq)
{
    GICD_Type         * GICD;
    GICR_SGI_PPI_Type * GICR_TARGET0_INTREG;

    GICD                = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICR_TARGET0_INTREG = BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(BSP_CFG_CPU);

    if (irq >= 0)
    {
        uint32_t _irq = (uint32_t) irq;
        GICD->GICD_ISENABLER[(((uint32_t) irq) /
                              BSP_VECTOR_NUM_OFFSET)] |= (uint32_t) (0x00000001UL << (_irq % BSP_VECTOR_NUM_OFFSET));
    }
    else
    {
        uint32_t _irq = ((uint32_t) irq + BSP_VECTOR_NUM_OFFSET);
        GICR_TARGET0_INTREG->GICR_ISENABLER0 |= (uint32_t) (0x00000001UL << _irq);
    }
}

/*******************************************************************************************************************//**
 * Enable the IRQ in the GIC (With clearing the pending bit).
 *
 * @param[in] irq            The IRQ number to enable. Note that the enums listed for IRQn_Type are only those for the
 *                           Cortex Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqEnable (IRQn_Type const irq)
{
    /* Clear pending interrupts in the GIC. */
    R_BSP_IrqClearPending(irq);

    /* Enable the interrupt in the GIC. */
    R_BSP_IrqEnableNoClear(irq);
}

/*******************************************************************************************************************//**
 * Disables interrupts in the GIC.
 *
 * @param[in] irq            The IRQ number to disable in the GIC. Note that the enums listed for IRQn_Type are
 *                           only those for the Cortex Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqDisable (IRQn_Type const irq)
{
    GICD_Type         * GICD;
    GICR_SGI_PPI_Type * GICR_TARGET0_INTREG;

    GICD                = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICR_TARGET0_INTREG = BSP_PRV_GICR_TARGET0_INTREG_ADDRESS(BSP_CFG_CPU);

    if (irq >= 0)
    {
        uint32_t _irq = (uint32_t) irq;
        GICD->GICD_ICENABLER[(((uint32_t) irq) /
                              BSP_VECTOR_NUM_OFFSET)] = (uint32_t) (0x00000001UL << (_irq % BSP_VECTOR_NUM_OFFSET));
    }
    else
    {
        uint32_t _irq = ((uint32_t) irq + BSP_VECTOR_NUM_OFFSET);
        GICR_TARGET0_INTREG->GICR_ICENABLER0 = (uint32_t) (0x00000001UL << _irq);
    }

    __DSB();
    __ISB();
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context, clears pending interrupts, then enables the interrupt.
 *
 * @param[in] irq            Interrupt number.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqCfgEnable (IRQn_Type const irq, uint32_t priority, void * p_context)
{
    R_BSP_IrqCfg(irq, priority, p_context);
    R_BSP_IrqEnable(irq);
}

/*******************************************************************************************************************//**
 * @brief      Finds the ISR context associated with the requested IRQ.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @return  ISR context for IRQ.
 **********************************************************************************************************************/
__STATIC_INLINE void * R_FSP_IsrContextGet (IRQn_Type const irq)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    return gp_renesas_isr_context[irq];
}

/*******************************************************************************************************************//**
 * Sets the interrupt detect type.
 *
 * @param[in] irq             The IRQ number to configure.
 * @param[in] detect_type     GIC detect type of the interrupt (0 : active-HIGH level, 1 : rising edge-triggerd).
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqDetectTypeSet (IRQn_Type const irq, uint32_t detect_type)
{
    uint32_t    _irq = (uint32_t) irq;
    GICD_Type * GICD;

    GICD = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);

    if (0 != detect_type)
    {
        GICD->GICD_ICFGR[(((uint32_t) irq) / 16UL)] |= BSP_PRV_GICD_ICFGR_INT_CONFIG_MASK << (2UL * (_irq % 16UL));
    }
    else
    {
        GICD->GICD_ICFGR[(((uint32_t) irq) / 16UL)] &= ~(BSP_PRV_GICD_ICFGR_INT_CONFIG_MASK << (2UL * (_irq % 16UL)));
    }
}

/*******************************************************************************************************************//**
 * Sets the interrupt Group.
 *
 * @param[in] irq               The IRQ number to configure.
 * @param[in] interrupt_group   GIC interrupt group number ( 0 : FIQ, 1 : IRQ ).
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqGroupSet (IRQn_Type const irq, uint32_t interrupt_group)
{
    uint32_t    _irq = (uint32_t) irq;
    GICD_Type * GICD;

    GICD = BSP_PRV_GICD_ADDRESS(BSP_CFG_CPU);
    GICD->GICD_IGROUPR[(((uint32_t) irq) /
                        BSP_VECTOR_NUM_OFFSET)] |= (interrupt_group << (_irq % BSP_VECTOR_NUM_OFFSET));
}

/*******************************************************************************************************************//**
 * Sets the interrupt mask level.
 *
 * @param[in] mask_level          The interrupt mask level
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqMaskLevelSet (uint32_t mask_level)
{
    FSP_CRITICAL_SECTION_SET_STATE(mask_level << BSP_FEATURE_BSP_IRQ_PRIORITY_POS_BIT);
}

/*******************************************************************************************************************//**
 * Gets the interrupt mask level.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 *
 * @return  Value indicating the interrupt mask level.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_BSP_IrqMaskLevelGet (void)
{
    return (uint32_t) ((FSP_CRITICAL_SECTION_GET_CURRENT_STATE() >> BSP_FEATURE_BSP_IRQ_PRIORITY_POS_BIT) &
                       0x0000001FUL);
}

/*******************************************************************************************************************//**
 * @internal
 * @addtogroup BSP_MCU_PRV Internal BSP Documentation
 * @ingroup RENESAS_INTERNAL
 * @{
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
void bsp_irq_cfg(void);                // Used internally by BSP
void bsp_common_interrupt_handler(uint32_t id);

/** @} (end addtogroup BSP_MCU_PRV) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
