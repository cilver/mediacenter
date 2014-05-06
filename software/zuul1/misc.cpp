#define SYSTEM_BUS_WIDTH 32

#include <stdint.h>
#include <stdio.h>
#include <altera_avalon_pio_regs.h>
#include <sys/alt_irq.h>
#include <io.h>
#include "misc.h"

void InfraRood::init(volatile uint32_t *base, int irq, int ctl)
{
    this->base = base;
    alt_ic_isr_register(ctl, irq, isrBridge, 0, 0);
    IOWR(base, 0, 0);
}

void InfraRood::isr(void *context)
{
    if (observer != 0)
        observer->update();

    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(base, 0);
    IOWR(base, 0, 0);
}

void InfraRood::setObserver(Observer *observer)
{
    this->observer = observer;
}

void InfraRood::isrBridge(void *context)
{
    InfraRood::getInstance()->isr(context);
}

InfraRood *InfraRood::getInstance()
{
    static InfraRood instance;
    return &instance;
}

Uart *Uart::getInstance()
{
    static Uart instance;
    return &instance;
}

JtagUart *JtagUart::getInstance()
{
    static JtagUart instance;
    return &instance;
}

void JtagUart::init(volatile uint32_t *base)
{
}

void JtagUart::puts(const char *s)
{
    ::puts(s);
}

void Uart::init(volatile uint32_t *base)
{
    this->base = base;
}

void Uart::putc(const char c)
{
    while ((base[2] & (1<<6)) == 0)
    {
    }

    base[1] = c;
}

void Uart::puts(const char *s)
{
    while (*s)
        putc(*s++);
}

void SDCard::init(const char *name)
{
    sd_card_dev = ::alt_up_sd_card_open_dev(name);
}

bool SDCard::isPresent()
{
    return alt_up_sd_card_is_Present();
}

bool SDCard::isFAT16()
{
    return alt_up_sd_card_is_FAT16();
}

int SDCard::fopen(char *fn)
{
    return alt_up_sd_card_fopen(fn, true);
}

bool SDCard::write(int sd_fileh, char c)
{
    return ::alt_up_sd_card_write(sd_fileh, c);
}

bool SDCard::fclose(int sd_fileh)
{
    return ::alt_up_sd_card_fclose(sd_fileh);
}



