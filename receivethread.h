#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

/*
 * This is a thread that deals with incomming UDP output packlets from the iPuP-PRO
 * this inclused the feedb back position packet if available.
 */

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QUdpSocket>
#include <QElapsedTimer>

#define bzero(ptr, size) memset(ptr, 0, size)

#define UDP_OUTPUT_CHANNELS 500

//	Packet word offsets
#define	XP2_UDP_OUTPACKET_HEADER_OFFSET     0	// uint16
#define	XP2_UDP_OUTPACKET_CODE_OFFSET		1	// uint32
#define	XP2_UDP_OUTPACKET_CHANNELS_OFFSET	3	// uint16
#define XP2_UDP_OUTPACKET_STATUS_OFFSET     4   // uint16
#define	XP2_UDP_OUTPACKET_DATA_OFFSET		5	// uint16 ...
#define XP2_UDP_OUTPACKET_CRC_OFFSET		5	// uint16 + data channels


//	(header + time stamp + channel count) + max data + crc
//#define	XP2_UDP_INPUT_BUFFER_SIZE	(((XP2_UDP_INPACKET_DATA_OFFSET)*2) + (UDP_INPUT_CHANNELS *2) + 2)
#define	XP2_UDP_OUTPUT_BUFFER_SIZE	(((XP2_UDP_OUTPACKET_DATA_OFFSET)*2) + (UDP_OUTPUT_CHANNELS *2) + 2)

//  data coming into app, eg output data from Xpress
#define UDP_INPUT_HEADER            0x5502
#define UDP_INPUT_FEEDBACK_HEADER   0x5503

const	quint16 cCrc16Table[16] = {
                0x0000, 0xcc01, 0xd801, 0x1400,
                0xf001, 0x3c00, 0x2800, 0xe401,
                0xa001, 0x6c00, 0x7800, 0xb401,
                0x5000, 0x9c01, 0x8801, 0x4400
                };


typedef struct
{
    unsigned frames : 8;
    unsigned seconds : 6;
    unsigned minutes : 6;
    unsigned hours : 10;
}s_TimeCode;


#define	XP2_TIME_CODE_BUFFER	(5+1+2+1+2+1+3+1)	// >HHHH:MM:SS:FFF/0


class receiveThread : public QThread
{
    Q_OBJECT
public:
    receiveThread(QObject *parent = 0);
    ~receiveThread();

    int  startThread(QString bindAddress, quint16 port);
    void stopThread(void);
    void run();

    //quint64 getReceivedStamp( void ) { return nanoReceived; }
    quint64 getMeasuredStamp( void ) { return nanoStamp; }
    //quint64 getFBReceivedStamp( void ) { return nanofbReceived; }
    quint64 getFBMeasuredStamp( void ) { return nanofbStamp; }
    quint16 getOutputData( char * ptr ) { if(ptr==NULL || !outputChannelCount ) return 0; memcpy(ptr,outputBuffer,outputChannelCount*sizeof(quint16)); return outputChannelCount; }
    quint16 getFeedbackData( char * ptr ) { if(ptr==NULL || !feedbackChannelCount ) return 0; memcpy(ptr,feedbackBuffer,feedbackChannelCount*sizeof(quint16)); return feedbackChannelCount; }
    quint64 getTimeStamp( void ) { return timer.nsecsElapsed(); }
    int     getTimeCode( char *b ) { if(b == NULL) return 0; sprintf(b,"TC= %3.3d:%2.2d:%2.2d:%3.3d",oTimeCode.hours,oTimeCode.minutes,oTimeCode.seconds,oTimeCode.frames); return -1; }
    int     getfbTimeCode( char *b ) { if(b == NULL) return 0; sprintf(b,"TC= %3.3d:%2.2d:%2.2d:%3.3d",fbTimeCode.hours,fbTimeCode.minutes,fbTimeCode.seconds,fbTimeCode.frames); return -1; }
    QHostAddress getSocketSenderAddress( void ) {return inAddress;}
    int getOutputChannelCount() { return outputChannelCount; }
    int getFeedbackChannelCount() { return feedbackChannelCount; }

signals:

    void datagramReceived(void);
    void fbDatagramReceived(void);

private slots:

    int processPendingDatagrams();

public slots:

private:

    QMutex  mutex;
    QString sendAddress;
    QString bindAddress;
    quint16 port;

    QUdpSocket *isocket;
    QElapsedTimer timer;

    int quit;

    int checkUdpOutputPacket( quint16 bytesIn, char *pData );
    int processUdpOutputPacket( quint16 bytesIn, char *pData );
    quint16 crcGenerate(char *dPtr, unsigned pLength);

    quint16 outputChannelCount;
    quint16 outputStatus;
    quint16 feedbackChannelCount;
    quint16 feedbackStatus;
    char outputBuffer[XP2_UDP_OUTPUT_BUFFER_SIZE];
    char feedbackBuffer[XP2_UDP_OUTPUT_BUFFER_SIZE];
    quint64 nanoReceived;
    quint64 nanoStamp;
    quint64 nanofbReceived;
    quint64 nanofbStamp;

    s_TimeCode oTimeCode;
    s_TimeCode fbTimeCode;

    QHostAddress inAddress;
    quint16 inPort;
};

#endif // RECEIVETHREAD_H
