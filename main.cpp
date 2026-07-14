#include <QCoreApplication>
#include <QDebug>
#include "uartcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UartController uart;
    uart.SetPortName("COM4");
    uart.SlotInit();
    return a.exec();
}
