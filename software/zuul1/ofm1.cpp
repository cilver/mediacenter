#include <system.h>
#include <stdlib.h>
#include "misc.h"

class Box
{
private:
    VGA *vga;
public:
    Box(VGA *vga) { this->vga = vga; }
    void draw();
};

class Left
{
public:
    Left() { }
    char *toArray();
};

class OrthodoxFileManager1
{
public:
    OrthodoxFileManager1() { }
    void init();
    int run();
private:
    VGA *vga;
    Box *box;
    SDCard *sdCard;
    Uart *uart;
    QuadroSegment *quadroSegment;
};

void OrthodoxFileManager1::init()
{
    uart = Uart::getInstance();
    uart->init((volatile uint32_t *)UART_BASE);
    uart->puts("StartUp OFM1...\r\n");
    quadroSegment = new QuadroSegment((volatile uint32_t *)MYSEGDISP2_0_BASE);
    sdCard = new SDCard();
    sdCard->init(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME);
    vga = new VGA("/dev/video_character_buffer_with_dma_0");
    box = new Box(vga);
    vga->clear();
    
}

void Box::draw()
{
    vga->draw("+--------------------------------------++--------------------------------------+", 0, 0);
    vga->draw("|                                      ||                                      |", 0, 1);
    vga->draw("|                                      ||                                      |", 0, 2);
    vga->draw("|                                      ||                                      |", 0, 3);
    vga->draw("|                                      ||                                      |", 0, 4);
}



int OrthodoxFileManager1::run()
{
    box->draw();

    if (sdCard->isPresent())
    {
        if (sdCard->isFAT16())
        {
            char fn[13];
            t_file_record fr;
            int foo = sdCard->alt_up_sd_card_find_first("/.", fn);

            while ((foo = sdCard->findNext(fn, &fr)) >= 0)
            {
                //uart->puts(fn);
                char dinges[255];
                ::sprintf(dinges, "%s %u\r\n", (const char *)fr.name, fr.file_size_in_bytes);
                //uart->puts((const char *)fr.name);
                uart->puts(dinges);
                uart->puts("\r\n");
            }

/*
            while ((foo = sdCard->findNext(fn)) >= 0)
            {
                //uart->puts((const char *)fr.name);
                uart->puts(fn);
                uart->puts("\r\n");
            }
*/
            //MyFile *file = sdCard->openFile(fn);
/*
            int fd = sdCard->fopen(fn);
            quadroSegment->setHex(fd);
            int bar = ::alt_up_sd_card_read(fd);
            quadroSegment->setHex(bar);
            uart->puts("Test\r\n");


            short int bijt;
            //bijt = file->read();

            uart->puts("Onzin\r\n");
*/

        }
        else
        {
            uart->puts("Geen FAT16\r\n");
        }
    }

    return 0;
}

int main()
{
    OrthodoxFileManager1 ofm1;
    ofm1.init();
    return ofm1.run();
}


