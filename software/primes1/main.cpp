#include <stdio.h>
#include <system.h>
#include <stdbool.h>
#include <sys/alt_irq.h>
 
class MyTimerTest
{
public:
    void init();
    int run();
    static void isr(void *context);
};

void MyTimerTest::init()
{
    ::alt_ic_isr_register(MYTIMER1_0_IRQ_INTERRUPT_CONTROLLER_ID, MYTIMER1_0_IRQ, isr, 0, 0);
}

int MyTimerTest::run()
{
    while (true)
    {
    }

    return 0;
}

void MyTimerTest::isr(void *context)
{
    ::printf("Interrupt\r\n");
}

int main()
{
    MyTimerTest mtt;
    
    return mtt.run();
}



