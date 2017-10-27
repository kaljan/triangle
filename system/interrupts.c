#include <stm32f10x.h>
#include "interrupts.h"

/*!
 ********************************************************************
 * @brief  This function handles Hard Fault exception.
 *
 ********************************************************************
 */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1) {}
}

/*!
 ********************************************************************
 * @brief  This function handles Memory Manage exception.
 *
 ********************************************************************
 */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1) {}
}

/*!
 ********************************************************************
 * @brief  This function handles Bus Fault exception.
 *
 ********************************************************************
 */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1) {}
}

/*!
 ********************************************************************
 * @brief  This function handles Usage Fault exception.
 *
 ********************************************************************
 */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1) {}
}

