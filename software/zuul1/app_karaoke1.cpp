#include <system.h>
#include "misc.h"
#include "mymidi.h"
#include <altera_up_avalon_video_pixel_buffer_dma.h>

class Karaoke1
{
private:
    VGATerminal *vgaTerminal;
    Uart *uart;
    QuadroSegment *qs;
    SDCardEx *sdCard;
    MyFile *myFile;
    KarFile *karFile;
public:
    void init();
    int run();
};

void Karaoke1::init()
{
    qs = new QuadroSegment((volatile uint32_t *)MYSEGDISP2_0_BASE);
    qs->setInt(0);
    vgaTerminal = new VGATerminal("/dev/video_character_buffer_with_dma_0");
    vgaTerminal->clear();
    vgaTerminal->puts("Karaoke 1\r\n");
    uart = Uart::getInstance();
    uart->init((volatile uint32_t *)UART_BASE);
    uart->puts("Uart\r\n");
    sdCard = new SDCardEx();

    sdCard->init(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME,
            (volatile void *)ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_BASE);
    
    if (sdCard->isPresent() && sdCard->isFAT16())
    {
        myFile = sdCard->openFile("VOGUE.KAR");
        karFile = new KarFile(myFile);
        karFile->read();
        //qs->setHex(::Utility::be_32_toh(karFile->tracks[0]->chunkSizeBE));
        
        for (vector<KarTrack>::iterator it = karFile->tracks.begin();
                it != karFile->tracks.end(); ++it)
        {
            KarTrack track = *it;
            track.parse();

            for (MIDIEventVector::iterator it = track.events.begin();
                    it != track.events.end(); ++it)
            {
                vgaTerminal->puts((*it)->toString());
                vgaTerminal->puts("\r\n");
                //TextEvent *te = dynamic_cast<TextEvent *>(*it);
                
                //if (te)
                //    vgaTerminal->puts("TextEvent");
            }

            uint32_t chunkSize = ::Utility::be_32_toh(track.chunkSizeBE);
            qs->setHex(chunkSize);
            vgaTerminal->puts(it->toString());
            vgaTerminal->puts("\r\n");
            //break;
        }

        vgaTerminal->puts(karFile->toString());
        vgaTerminal->puts("\r\n");
        //uart->printf("He%ulo Wo%rld\r\n");
/*
        for (int i = 0; i < 100; i++)
        {
            uint16_t foo;
            foo = myFile->read();
            
        }*/
            
        //karFile = new KarFile(myFile);
        
    }
    else
    {
        vgaTerminal->puts("\r\nGeen SD Kaart aanwezig!");
    }
    
    mstd::vector<int> vector1(100);
    vector1.push_back(1);
    vector1.push_back(5);
    vector1.push_back(4);
    vector1.push_back(6);
    int dinges = 0;

    for (mstd::vector<int>::iterator it = vector1.begin(); it != vector1.end(); ++it)
        dinges += *it;

    uart->printf("Hello %s\r\n", "World");
    //vgaTerminal->clear();

    //alt_up_pixel_buffer_dma_dev *pbd;
    //pbd = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");
    
    //if (pbd)
    //    vgaTerminal->puts("OK\r\n");
 
    volatile uint32_t * const pixels = (volatile uint32_t * const)SRAM_BASE;

    for (int i = 0; i < 10000; i++)
        pixels[i] = 0;
    
}

int Karaoke1::run()
{
    
    return 0;
}

int main()
{
    Karaoke1 kar1;
    kar1.init();
    return kar1.run();
}

