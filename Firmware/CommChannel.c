#include <stm32l4xx.h>
#include "CommChannel.h"
#include "Basile.h"
#include <stm32l4xx_hal.h>

#define U USART1
#define RX_BUF_LEN   128
#define TX_BUF_LEN   128

static uint8_t rxbuf[RX_BUF_LEN];
static uint8_t txbuf[TX_BUF_LEN];

static volatile uint32_t txh, txt, rxh, rxt;
static volatile bool txrunning;

#define TX_FREE      (txt + TX_BUF_LEN - txh - 1)
#define TX_FILL      (txh - txt)
#define TX_PUSH(b)   txbuf[txh++ % TX_BUF_LEN] = b
#define TX_POP()     txbuf[txt++ % TX_BUF_LEN]

#define RX_FREE      (rxt + RX_BUF_LEN - rxh - 1)
#define RX_FILL      (rxh - rxt)
#define RX_PUSH(b)   rxbuf[rxh++ % RX_BUF_LEN] = b
#define RX_POP()     rxbuf[rxt++ % RX_BUF_LEN]

void CommChannel_Initialize(void)
{
   U->CR1 |= USART_CR1_RXNEIE | USART_CR1_TCIE;
   U->CR3 |= USART_CR3_OVRDIS;

   NVIC_EnableIRQ(USART1_IRQn);
}

extern UART_HandleTypeDef huart1;

void CommChannel_SetBaudRate(uint32_t rate)
{
   while (txrunning) { }
   U->CR1 &= ~USART_CR1_TCIE;
   huart1.Init.BaudRate = rate;
   if (HAL_UART_Init(&huart1) != HAL_OK)
      _Error_Handler(__FILE__, __LINE__);
   U->CR1 |= USART_CR1_TCIE;
}

uint32_t CommChannel_Write(const void* data, uint32_t size)
{
   const uint32_t avail = TX_FREE;
   const uint32_t chunk = min(size, avail);
   for (uint32_t i = 0; i < chunk; ++i)
      TX_PUSH(((uint8_t*)data)[i]);

   if (chunk > 0 && (U->ISR & USART_ISR_TXE))
   {
      txrunning = true;
      U->TDR = TX_POP();
   }

   return chunk;
}

uint32_t CommChannel_Read(void* data, uint32_t size)
{
   const uint32_t avail = RX_FILL;
   const uint32_t chunk = min(size, avail);
   for (uint32_t i = 0; i < chunk; ++i)
      ((uint8_t*)data)[i] = RX_POP();

   return chunk;
}

void USART1_IRQHandler(void)
{
   if (U->ISR & USART_ISR_TC)
   {
      if (TX_FILL > 0)
         U->TDR = TX_POP();
      else
      {
         U->ICR |= USART_ICR_TCCF;
         txrunning = false;
      }
   }

   if (U->ISR & USART_ISR_RXNE)
   {
      const uint8_t data = U->RDR;
      if (RX_FREE > 0)
         RX_PUSH(data);
   }

   U->ICR = U->ISR;
}
