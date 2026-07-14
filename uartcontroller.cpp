#include <QDebug>
#include <QtSerialPort>
#include "uartcontroller.h"

UartController::UartController(QObject *parent) : QObject{parent}, m_portName("COM4")
{
    port = new QSerialPort();
    m_idleTimer = new QTimer(this);
    m_idleTimer->setSingleShot(true);          // однократный таймер
    m_idleTimer->setInterval(1000);

    connect(port, &QSerialPort::readyRead, this, &UartController::SlotRead);
    connect(m_idleTimer, &QTimer::timeout, this, &UartController::FlushBuffer);
    connect(port, &QSerialPort::bytesWritten, [](qint64 bytes)
    {
        qDebug() << "Отправлено байт:" << bytes;
    });
    connect(port, &QSerialPort::errorOccurred, [this](QSerialPort::SerialPortError error)
    {
        if (error == QSerialPort::ResourceError)
        {
            qDebug() << "Ошибка порта:" << port->errorString();
            port->close();
        }
    });
}

UartController::~UartController()
{
    port->close();
    delete port;
}

void UartController::SlotRead()
{
    // Читаем все доступные байты и добавляем в буфер
        m_buffer.append(port->readAll());

        // Каждый пришедший байт перезапускает таймер простоя
        m_idleTimer->start();
    /*
    QByteArray received = port->read(receiveMessageSize);
    if (received.size() == 2)
    {
        int x = static_cast<int>(static_cast<unsigned char>(received[0]));
        int y = static_cast<int>(static_cast<unsigned char>(received[1]));
        emit SendDataToScreen(x, y);
    }
    received.clear();*/
}


void UartController::FlushBuffer()
{
    if (m_buffer.isEmpty())
        return;

    // Формируем имя файла: meteo_log_ДДММГГ_ЧЧММ.txt
    QString timestamp = QDateTime::currentDateTime().toString("yyMMdd_hhmm");
    QString fileName = QString("meteo_log_%1.txt").arg(timestamp);

    // Сохраняем в текущей рабочей папке (для QtCreator — это папка проекта)
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(m_buffer);
        file.close();
        qDebug() << "Сохранено" << m_buffer.size() << "байт в" << fileName;
    } else {
        qDebug() << "Ошибка сохранения файла:" << file.errorString();
    }

    m_buffer.clear();
}

void UartController::SlotInit()
{
    if (PortInit())//"/dev/ttyUSB0" в Linux
    {
        qDebug() << "Порт инициализирован";
    }
    else
    {
        qDebug() << "Ошибка инициализации порта";
    }
}


void UartController::SlotClosePort()
{
    port->close();
}

bool UartController::PortInit()
{
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setBaudRate(QSerialPort::Baud9600);
    port->setPortName(m_portName);//"/dev/ttyUSB0" в Linux //"/dev/ttyACM0"
    port->close();//Закрываем порт
    return port->open(QSerialPort::ReadWrite);
}

void UartController::SetPortName(const QString &name)
{
    m_portName = name;
}


/*void UartController::SendData(QByteArray &byteArray, int length)
{
    if (!port->isOpen())
    {
        qDebug() << "Порт не открыт!";
        return;
    }
    if (byteArray.length() == length)
    {
        for (int i = 0; i<length; i++)
        {
            QByteArray temp;
            temp.resize(0);
            temp.append(byteArray[i]);
            port->write(temp);
            port->flush();
            //port->waitForBytesWritten();//waitForBytesWritten блокирует поток и readyRead всё крашит
            temp.clear();
        }
        byteArray.clear();
    }
}*/
