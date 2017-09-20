#ifndef REUART_H
#define REUART_H

#include <Arduino.h>

// AVR
#ifdef ARDUINO_ARCH_AVR
class HwSerial : public HardwareSerial
{
public:
    HwSerial(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
             volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
             volatile uint8_t *ucsrc, volatile uint8_t *udr) :
        HardwareSerial(ubrrh, ubrrl, ucsra, ucsrb, ucsrc, udr) {}

    inline void _rx_complete_irq(void);
    void _tx_udr_empty_irq(void);
    virtual size_t write(uint8_t c);
    using HardwareSerial::write;
};

extern HwSerial CCSerial;
#endif

// SAM
#ifdef ARDUINO_ARCH_SAM
class HwSerial : public UARTClass
{
public:
    HwSerial(Uart* pUart, IRQn_Type dwIrq, uint32_t dwId,
             RingBuffer* pRx_buffer, RingBuffer* pTx_buffer) :
        UARTClass(pUart, dwIrq, dwId, pRx_buffer, pTx_buffer) {}

    void IrqHandler(void);
    virtual size_t write(const uint8_t c);
    using UARTClass::write;
};

extern HwSerial CCSerial;
#endif

// SAMD
#ifdef ARDUINO_ARCH_SAMD
// TODO
#endif

#endif
