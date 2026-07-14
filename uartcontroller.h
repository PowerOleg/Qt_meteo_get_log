#ifndef UART_H
#define UART_H

#include <QObject>
#include <QByteArray>
#include <QTimer>

class QSerialPort;

class UartController : public QObject
{
    Q_OBJECT
public:
    explicit UartController(QObject *parent = nullptr);
    ~UartController();
    void SetPortName(const QString &name);
private:
    bool PortInit();
    void SendData(QByteArray &byteArray, int length);

    int receiveMessageSize = 2;
    QSerialPort* port;
    QString m_portName;
    QByteArray m_buffer;
    QTimer *m_idleTimer;
signals:
   void SendDataToScreen(const int x, const int y);
public slots:
    void SlotRead();
    void SlotInit();
    void SlotClosePort();
private slots:
    void FlushBuffer();
};

#endif // UART_H
