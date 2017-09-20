#include "ControlChain.h"
#include "ReUART.h"

inline void uart_byte_recv(unsigned char b)
{
    cc_data_t received = {&b, 1};
    cc_parse(&received);
}

////////////////////////////////////////////////////////////////////////////////
// AVR architecture

#ifdef ARDUINO_ARCH_AVR

#if defined(HAVE_HWSERIAL0)

#include <HardwareSerial_private.h>

#if defined(UBRRH) && defined(UBRRL)
HwSerial CCSerial(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR);
#else
HwSerial CCSerial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
#endif

// ISR
#if defined(USART_RX_vect)
ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
ISR(USART0_RX_vect)
#elif defined(USART_RXC_vect)
ISR(USART_RXC_vect) // ATmega8
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
    CCSerial._rx_complete_irq();
}

void HwSerial::_rx_complete_irq(void)
{
    // check parity error
    if (bit_is_clear(*_ucsra, UPE0)) {
        uart_byte_recv(*_udr);
    }
    else {
        *_udr;
    }
}

void HwSerial::_tx_udr_empty_irq(void)
{
    HardwareSerial::_tx_udr_empty_irq();

    if (_tx_buffer_head == _tx_buffer_tail) {
        // disable driver
        digitalWrite(TX_DRIVER_PIN, LOW);
    }
}

size_t HwSerial::write(uint8_t c)
{
    // enable driver
    digitalWrite(TX_DRIVER_PIN, HIGH);
    for (volatile int delay = 0; delay < 100; delay++);

    return HardwareSerial::write(c);
}

#endif // HAVE_HWSERIAL0

#endif // end of ARDUINO_ARCH_AVR

////////////////////////////////////////////////////////////////////////////////
// SAM architecture (ARM cortex m3)

#ifdef ARDUINO_ARCH_SAM

#include <UARTClass.h>
#include <RingBuffer.h>

static RingBuffer rx_buffer, tx_buffer;

HwSerial CCSerial(UART, UART_IRQn, ID_UART, &rx_buffer, &tx_buffer);

// waiting for 'https://github.com/arduino/ArduinoCore-sam/pull/1' be merged
void UART_Handler(void)
{
    CCSerial.IrqHandler();
}

void HwSerial::IrqHandler( void )
{
    uint32_t status = _pUart->UART_SR;

    // Did we receive data?
    if ((status & UART_SR_RXRDY) == UART_SR_RXRDY)
        uart_byte_recv(_pUart->UART_RHR);

    // Do we need to keep sending data?
    if ((status & UART_SR_TXRDY) == UART_SR_TXRDY) {
        if (_tx_buffer->_iTail != _tx_buffer->_iHead) {
            _pUart->UART_THR = _tx_buffer->_aucBuffer[_tx_buffer->_iTail];
            _tx_buffer->_iTail = (unsigned int)(_tx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE;
        }
        else {
            // Mask off transmit interrupt so we don't get it anymore
            _pUart->UART_IDR = UART_IDR_TXRDY;

            // disable driver
            digitalWrite(TX_DRIVER_PIN, HIGH);
        }
    }

    // Acknowledge errors
    if ((status & UART_SR_OVRE) == UART_SR_OVRE || (status & UART_SR_FRAME) == UART_SR_FRAME) {
        // TODO: error reporting outside ISR
        _pUart->UART_CR |= UART_CR_RSTSTA;
    }
}

// write one byte to register and wait until it to be send
size_t HwSerial::write(const uint8_t c)
{
    // enable driver
    digitalWrite(TX_DRIVER_PIN, HIGH);
    for (volatile int delay = 0; delay < 100; delay++);

    return UARTClass::write(c);
}

#endif // end of ARDUINO_ARCH_SAM

////////////////////////////////////////////////////////////////////////////////
// SAMD architecture (ARM cortex m0)

#ifdef ARDUINO_ARCH_SAMD
// TODO
#endif // end of ARDUINO_ARCH_SAMD
