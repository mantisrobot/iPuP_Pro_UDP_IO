
/*
 * This is a thread that deals with incomming UDP output packlets from the iPuP-PRO
 * this inclused the feedb back position packet if available.
 */

#include "receivethread.h"

receiveThread::receiveThread(QObject *parent) :
    QThread(parent)
{
    outputChannelCount = 0;
    feedbackChannelCount = 0;
    bzero(outputBuffer,XP2_UDP_OUTPUT_BUFFER_SIZE);
    nanoReceived = 0;
    nanoStamp = 0;
    nanofbStamp = 0;
    nanofbReceived = 0;
    quit = false;
    QMutexLocker locker(&mutex);

}

receiveThread::~receiveThread()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}

int receiveThread::startThread(QString bindAddress, quint16 port)
{

    qDebug() << "Receive Thread Start";
    if( bindAddress == NULL ) bindAddress = "0.0.0.0";
    this->bindAddress = bindAddress;
    this->port = port;
    quit = false;
    if (!isRunning()) start(QThread::HighPriority);

    return 0;
}

void receiveThread::stopThread()
{
    if( isRunning() )
    {
        mutex.lock();
        quit = true;
        mutex.unlock();
    }
}

void receiveThread::run()
{
    qDebug() << "Create isocket";
    isocket = new QUdpSocket();

    qDebug() << "Bind Socket" << bindAddress << ":" << port;
    isocket->bind(QHostAddress(bindAddress), port);

    while(!quit)
    {

        isocket->waitForReadyRead(1000);
        processPendingDatagrams();

    }

    qDebug() << "Close isocket";
    isocket->close();
    delete isocket;
}


int receiveThread::processPendingDatagrams()
{
    while( isocket->hasPendingDatagrams() )
    {
        char iBuffer[XP2_UDP_OUTPUT_BUFFER_SIZE];
        int r = isocket->readDatagram(iBuffer,XP2_UDP_OUTPUT_BUFFER_SIZE,&inAddress, &inPort);

        if( this->signalsBlocked() == false )
        {
        //qDebug() << inAddress.toString();
            switch( checkUdpOutputPacket(r,iBuffer) )
            {
            default:
                break;
            case -1:
                nanoStamp = timer.nsecsElapsed();
                memcpy(outputBuffer,iBuffer+(XP2_UDP_OUTPACKET_DATA_OFFSET*2),sizeof(quint16)*outputChannelCount);
                emit this->datagramReceived();
                break;
            case -2:
                nanofbStamp = timer.nsecsElapsed();
                memcpy(feedbackBuffer,iBuffer+(XP2_UDP_OUTPACKET_DATA_OFFSET*2),sizeof(quint16)*outputChannelCount);
                emit this->fbDatagramReceived();
                break;
            }
        }
    }

    return 0;
}


int		receiveThread::checkUdpOutputPacket( quint16 bytesIn, char *pData )
{
    if( pData == NULL ) return 0;

    //qDebug() << "checkUdpOutputPacket()";
    quint16	*pWordData = (quint16 *)pData;

    if( bytesIn < XP2_UDP_OUTPACKET_DATA_OFFSET*2 )
    {
       qDebug("Packet Header Size Error %d != %d\n",bytesIn,XP2_UDP_OUTPACKET_DATA_OFFSET*2);
        return 0;
    }

    int exitCode = -1;
    switch( *pWordData )
    {
        case XP2_UDP_OUTPUT_FEEDBACK_HEADER:
            exitCode = -2;
            break;
        case XP2_UDP_OUTPUT_HEADER:
            break;
        default:
            qDebug("Packet Header Error %4X != %4X\n",*pWordData,XP2_UDP_OUTPUT_HEADER);
            return 0;
            break;
    }


    //	get time stamp and count data
    pWordData++;
    memcpy(&oTimeCode,pWordData,sizeof(s_TimeCode));
    pWordData += sizeof(s_TimeCode)/2;

    quint16 Status = *pWordData++;
    quint16 ChannelCount = *pWordData++;

    if( ChannelCount > UDP_OUTPUT_CHANNELS )
    {
        qDebug("Packet Channel Count Error %d > %d\n",ChannelCount,UDP_OUTPUT_CHANNELS);
        return 0;
    }


    //	CHECK PACKET SIZE
    // maybe should be < instead of != ?
    if( bytesIn != (XP2_UDP_OUTPACKET_DATA_OFFSET*2)+(ChannelCount*2)+2 )
    {
        qDebug("Packet Length Error %d != %d\n",bytesIn,(XP2_UDP_OUTPACKET_DATA_OFFSET*2)+(ChannelCount*2)+2);
        return 0;
    }


    //	CHECK PACKET CRC
    //	INCLUDING HEADER
    quint16 crc16 = crcGenerate( pData , (XP2_UDP_OUTPACKET_DATA_OFFSET*2)+(ChannelCount*2)+2 );

    if( crc16 != 0 )
    {
        qDebug("Packet CRC Error %4X != %4X\n",crc16,*(((quint16 *)pData) + XP2_UDP_OUTPACKET_CRC_OFFSET + ChannelCount));
        return 0;
    }

    switch( exitCode )
    {
        case -1:
            outputStatus = Status;
            outputChannelCount = ChannelCount;
            break;
        case -2:
            feedbackStatus = Status;
            feedbackChannelCount = ChannelCount;
            break;
        default:
            qDebug("Packet exitCode Error");
            return 0;
    }

    return exitCode;
}

quint16 receiveThread::crcGenerate(char *dPtr, unsigned pLength)
{
    quint16 rl;
    quint16 crc16 = 0;

    for( unsigned t = 0; t < pLength; t++, dPtr++ )
    {
        rl = cCrc16Table[crc16 & 0x0f];
        crc16 = (crc16>>4) & 0x0fff;
        crc16 = crc16 ^ rl ^ cCrc16Table[*dPtr & 0x0f];

        rl = cCrc16Table[crc16 & 0x0f];
        crc16 = (crc16>>4) & 0x0fff;
        crc16 = crc16 ^ rl ^ cCrc16Table[(*dPtr>>4) & 0x0f];
    }

return crc16;
}

