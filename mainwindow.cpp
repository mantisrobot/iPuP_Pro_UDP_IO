#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    oDlg = new OutputDlg(this,"Output Positions");
    oDlg->show();

    fbDlg = new OutputDlg(this,"Feedback Positions");
    fbDlg->show();

    iDlg = new inputdlg(this);
    iDlg->show();

    connect(ui->pbShowOutput,SIGNAL(clicked()),oDlg,SLOT(show()));
    connect(ui->pbShowOutput,SIGNAL(clicked()),fbDlg,SLOT(show()));
    connect(ui->pbShowInput,SIGNAL(clicked()),iDlg,SLOT(show()));


    this->setWindowTitle(QString("iPuP-PRO UDP I/O Test Application - ") + VERSION);

    // configure ip/port text
    ui->edtAddress->setText(UDP_OUTPUT_SEND_ADDRESS);    
    ui->edtPort->setText(QString::number(UDP_OUTPUT_PORT));
    ui->edtPortOp->setText(QString::number(UDP_INPUT_PORT));

    //  output socket, actually sends input type packet to iPuP-PRO Engine
    inputSocket = new QUdpSocket(this);
    //  connect the output readyread signal to look for sync packets from Xpress Engine
    connect(inputSocket,SIGNAL(readyRead()),this,SLOT(osync()));

    // connect start/stop button
    connect(ui->pbEnable,SIGNAL(clicked()),this,SLOT(enablePort()));

    // configure tick timer at FPS rate, only used when UDP sync packet trigger is not in use.
    tick = new QTimer(this);
    tick->setInterval(1000/FPS);
    tick->setTimerType(Qt::PreciseTimer);

    // configure 1Hz tick timer
    tick1hz = new QTimer(this);
    tick1hz->setInterval(1000);
    connect(tick1hz,SIGNAL(timeout()),this,SLOT(infoRefresh()) );

    // configure screen refresh timer
    tickUpdate = new QTimer(this);
    tickUpdate->setInterval(40);
    connect(tickUpdate,SIGNAL(timeout()),this,SLOT(displayRefresh()));

    // connect input slider moved signals to slider moved slot
    connect(iDlg,SIGNAL(sliderMoved(int,float)),this,SLOT(inputSliderMoved(int,float)));

    // clear input values to default values
    for( int t = 0; t < XP2_MAX_INPUTS; t++ ) inputValues[t] = 65535/2;

    // reset variables
    outputChannelCount = 0;
    outputfbChannelCount = 0;
    strcpy(timeCode,"Is Xpress2 TX ON?");
    syncFps = 0;

    connect(ui->cbSineWave,SIGNAL(clicked(bool)),this,SLOT(cbSineWave()));

    // connect datagram thread receive signals to the call pack routine
    connect(&rxThread,SIGNAL(datagramReceived()),this,SLOT(packetCallBack()));//, Qt::DirectConnection);
    connect(&rxThread,SIGNAL(fbDatagramReceived()),this,SLOT(packetFBCallBack()));//, Qt::DirectConnection);

#ifndef UDP_SYNC_PACKET_TRIGGER
    // configure and start refresh timer
    connect(tick,SIGNAL(timeout()),this,SLOT(sendPacket()) );
    tick->start();
#endif

    // start timers
    tick1hz->start();
    tickUpdate->start();

    portEnabled = false;

    ui->edtAddress->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->edtSendTo->setAttribute(Qt::WA_TransparentForMouseEvents);

}

MainWindow::~MainWindow()
{
    delete ui;
}


// enable / disable the ports
void MainWindow::enablePort()
{
    if( portEnabled )
    {
        portEnabled = false;
        qDebug() << "enablePort()" << portEnabled;

        rxThread.blockSignals(true);
        // stop listening for Xpress output packets
        rxThread.stopThread();
        // close input stream socket.
        inputSocket->close();

        qApp->processEvents();
        QThread::usleep(100000);
        qApp->processEvents();

        outputChannelCount = 0;
        outputfbChannelCount = 0;

        ui->edtAddress->setEnabled(true);
        ui->edtSendTo->setEnabled(true);
        ui->edtPort->setEnabled(true);
        ui->edtPortOp->setEnabled(true);

        ui->lblFps->setText("Sync FPS: ???");
        ui->lblInfo->setText("Output Count: ???");


        ui->pbEnable->setText("Enable Ports");

    }
    else
    {
        portEnabled = true;
        qDebug() << "enablePort()" << portEnabled;

        rxThread.blockSignals(true);
        // start the recevie thread, this thread listens for baked output packets from iPuP-Pro
        rxThread.startThread("0.0.0.0",ui->edtPortOp->text().toInt());

        // bind output socket for sending input data to iPuP-Pro
        if( inputSocket->bind(QHostAddress("0.0.0.0"), ui->edtPort->text().toInt() ) == false )
        {
            qDebug() << "Failed to bind output socket!";
            rxThread.stopThread();
            portEnabled = false;
            return;
        }

        ui->edtAddress->setEnabled(false);
        ui->edtSendTo->setEnabled(false);
        ui->edtPort->setEnabled(false);
        ui->edtPortOp->setEnabled(false);

        ui->pbEnable->setText("Disable Ports");

        rxThread.blockSignals(false);

    }
}


// *********************************************************************************
// Below are routines to deal with the UDP output stream from the iPuP-Pro
// *********************************************************************************

// we have an output packet to process from iPuP-Pro
void MainWindow::packetCallBack()
{
    //qDebug() << "packetCallBack()";
    outputChannelCount = rxThread.getOutputData((char *)outputValues);
    if( outputCount != outputChannelCount )
    {
        outputCount = outputChannelCount;
    }
    rxThread.getTimeCode( timeCode );
    qDebug() << "Packet TC:" << timeCode;

    //qDebug() << outputChannelCount;
    if( outputChannelCount > XP2_MAX_OUTPUTS ) outputChannelCount = XP2_MAX_OUTPUTS;

    int t;
    for( t = 0; t < outputChannelCount; t++ )
    {
        // avoid 0 this is used by feedback display to show disconencted device
        if( outputValues[t] == 0 ) outputValues[t] = 1;
        outputValuesReal[t] = outputValues[t] / 65535.0;
    }

    outputSenderAddress = rxThread.getSocketSenderAddress();
}

// we have an output feedback position packet to process from iPuP-Pro
void MainWindow::packetFBCallBack()
{
    //qDebug() << "packetCallBack()";
    outputfbChannelCount = rxThread.getFeedbackData((char *)outputfbValues);
    if( outputFbCount != outputfbChannelCount )
    {
        outputFbCount = outputfbChannelCount;
    }
    rxThread.getfbTimeCode( timeCodeFb );
    qDebug() << "Packet FB:" << timeCodeFb;

    //qDebug() << outputChannelCount;
    if( outputfbChannelCount > XP2_MAX_OUTPUTS ) outputfbChannelCount = XP2_MAX_OUTPUTS;

    int t;
    for( t = 0; t < outputfbChannelCount; t++ )
    {
        outputfbValuesReal[t] = outputfbValues[t] / 65535.0;
    }

    outputSenderAddress = rxThread.getSocketSenderAddress();
}

// display information refresh
void MainWindow::infoRefresh()
{
    QString foo;
    inputStatusBits = 0;
    syncTimeCode = 0;

    ui->lblFps->setText("Sync FPS: " + QString::number(syncFps));
    syncFps = 0;

    ui->lblInfo->setText("Output Count: " + QString::number(outputChannelCount));

}

// refresh teh output and feedback slider displays
void MainWindow::displayRefresh()
{

    oDlg->updateSliders(outputValuesReal,outputChannelCount);
    fbDlg->updateSliders(outputfbValuesReal,outputfbChannelCount);

    ui->lblTc->setText(timeCode);
    char b[100];
    sprintf(b,"Sync TC= %3.3d:%2.2d:%2.2d:%3.3d",syncTimeCodeBuffer.hours,syncTimeCodeBuffer.minutes,syncTimeCodeBuffer.seconds,syncTimeCodeBuffer.frames);
    ui->lblSTc->setText(b);
    //qDebug() << b;
    ui->edtSendTo->setText(outputSenderAddress.toString());

}


// *********************************************************************************
// Below are routines to deal with the UDP input stram from this app to the iPuP-Pro
//
// The address that the iPuP-Pro sends sync packets to us configured on the iPuP-Pro
// SD card ipup_cfg.ini.
// The iPuP-Pro sends a sync packet to the configured ports at the start of each frame
// receving clients then send back the input packet to the iPuP-Pro.
// *********************************************************************************

// a datagram is ready to read on input sync stream from iPuP-Pro
// we have a sync trigger so we need tos end an input packet to iPuP-PRO
void MainWindow::osync()
{
    static float sinpos = 0;
    char    iBuffer[XP2_UDP_INPUT_SYNC_PACKET_SIZE];

    //qDebug() << "Sync Trigger";
    if( inputSocket->hasPendingDatagrams() )
    {
        int r = inputSocket->readDatagram(iBuffer,XP2_UDP_INPUT_SYNC_PACKET_SIZE,&inputSenderAddress,NULL);
        if( r <= 0)
        {
            return;
        }

        if( checkUdpSyncPacket(r,iBuffer) )
        {
            if( ui->cbSineWave->isChecked() )
            {
                for( int t = 0; t < XP2_MAX_INPUTS; t++ )
                {
                    float pos = sin(sinpos-(t*((3.14/180)*10)));
                    iDlg->setSliderPosition(t,pos);
                    pos = 0.5 + (pos/2);
                    inputValues[t] = pos * 65535;

                }
                sinpos+= 3.14/180;
            }

            memcpy(&syncTimeCodeBuffer,&syncTimeCode,sizeof(s_TimeCode));
#ifdef UDP_SYNC_PACKET_TRIGGER
            syncFps++;
            sendPacket( inputSenderAddress );
#endif

        }
    }
}

// Send the UDP input type packet to iPuP-Pro
void MainWindow::sendPacket( QHostAddress destinationAddress )
{
    char       lBuffer[512];

    bzero(lBuffer,512);
    int l = createUdpInputPacket(XP2_MAX_INPUTS,(char *)inputValues,lBuffer);
    // or send as many inputs as you reqire up to MAX_INPUTS

    //  send data
    if( inputSocket->writeDatagram((char*)lBuffer, l,  destinationAddress, ui->edtPort->text().toInt()) >= 0 )
    {
       // qDebug() << "Sent" << Sent++;
       // nanoSent = timer.nsecsElapsed();
    }
    else
    {
        qDebug() << inputSocket->errorString();
    }

}


// create the UDP input packet for iPuP-Pro
int		MainWindow::createUdpInputPacket(  quint16 channelCount, char *pInData, char *pOutData )
{
    if( pInData == NULL || pOutData == NULL ) return 0;

    if( channelCount > XP2_MAX_INPUTS ) return 0;

    quint16	*pBuff = (quint16 *)pOutData;

    //	header
    *pBuff++ = UDP_OUTPUT_HEADER;

    //	time stamp
    memcpy( (char *)pBuff, (char *)&syncTimeCode, sizeof(quint32) );
    pBuff += sizeof(quint32)/2;

    // status bits
    *pBuff++ = inputStatusBits;

    //	channel count
    *pBuff++ = channelCount;

    //	data
    memcpy((char *)pBuff,(char *)pInData,sizeof(quint16)*channelCount);
    pBuff += channelCount;

    //	generate crc
    quint16 crc16 = crcGenerate(pOutData,(XP2_UDP_INPACKET_DATA_OFFSET*2) + (channelCount*2));
    memcpy((char *)pBuff,(char *)&crc16,sizeof(quint16));

    return ((XP2_UDP_INPACKET_DATA_OFFSET*2) + (channelCount*2) + 2);

}


// check validity of UDP input sync packet from iPuP-Pro
int		MainWindow::checkUdpSyncPacket( quint16 bytesIn, char *pData )
{
    if( pData == NULL ) return 0;

    quint16	*pWordData = (quint16 *)pData;

    if( bytesIn < XP2_UDP_INPUT_SYNC_PACKET_SIZE )
    {
       qDebug("Input Sync Size Error %d != %d\n",bytesIn,XP2_UDP_INPUT_SYNC_PACKET_SIZE);
        return 0;
    }

    if( *pWordData != UDP_OUTPUT_SYNC_HEADER )
    {

        qDebug("Input Sync packet Header Error %4X != %4X\n",*pWordData,UDP_OUTPUT_SYNC_HEADER);
        return 0;
    }

    //	get time stamp and count data

    pWordData++;

    memcpy(&syncTimeCode,pWordData,sizeof(quint32));
    pWordData += sizeof(quint32)/2;

    // get status data length
    syncStatusLength = *pWordData++;

    if( syncStatusLength != 0 && syncStatusLength < XP2_MAX_SYNC_DATA_LENGTH )
    {
        memcpy(syncStatusData,pWordData,sizeof(quint16)*syncStatusLength);
        pWordData+=syncStatusLength;
    }

    //	CHECK PACKET CRC
    //	INCLUDING HEADER
    quint16 crc16 = crcGenerate( pData , XP2_UDP_INPUT_SYNC_PACKET_SIZE + (sizeof(quint16)*syncStatusLength));

    if( crc16 != 0 )
    {
        qDebug("Input Sync packet CRC Error %4X != %4X\n",crc16,*(((quint16 *)pData) + (XP2_UDP_INPUT_SYNC_PACKET_SIZE-sizeof(quint16))));
        return 0;
    }

    return -1;
}


// input slider moved callback
void MainWindow::inputSliderMoved(int index, float value)
{
    //qDebug() << index << " " << value;
    //return( outLo + ( ((inX-inLo)*(outHi-outLo)) / (inHi-inLo) ) );
    inputValues[index] = ((value - -1.0)*65535) / 2.0;
}


// UDP input crc generator
quint16 MainWindow::crcGenerate(char *dPtr, unsigned pLength)
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



// set sinewave test mode for input sliders
void MainWindow::cbSineWave()
{
        iDlg->setDisabled(ui->cbSineWave->isChecked());

}
