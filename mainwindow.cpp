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


    this->setWindowTitle("XP2 UDP I/O Test Application");

    // configure ip/port text
    ui->edtAddress->setText(UDP_OUTPUT_SEND_ADDRESS);    
    ui->edtPort->setText(QString::number(UDP_OUTPUT_PORT));
    ui->edtPortOp->setText(QString::number(UDP_INPUT_PORT));

    //  output socket, actually sends input type packet to iPuP-PRO Engine
    osocket = new QUdpSocket(this);
    //  connect the output readyread signal to look for sync packets from Xpress Engine
    connect(osocket,SIGNAL(readyRead()),this,SLOT(osync()));

    //  control socket, this is the Xpress Output socket + 1 and is used for transport control and output information data.
    ctrlsocket = new QUdpSocket(this);
    //  connect the output readyread signal to look for sync packets from Xpress Engine
    connect(ctrlsocket,SIGNAL(readyRead()),this,SLOT(ctrlsync()));

    // connect start/stop button
    connect(ui->pbEnable,SIGNAL(clicked()),this,SLOT(enablePort()));

    // connect clear log button
    connect(ui->pbClear,SIGNAL(clicked()),ui->ptLog,SLOT(clear()));

    // connect enable/disable stream buttons
    connect(ui->pbStartTx,SIGNAL(clicked()),this,SLOT(enableTx()));
    connect(ui->pbStopTx,SIGNAL(clicked()),this,SLOT(disableTx()));
    connect(ui->pbGetOutputNames,SIGNAL(clicked()),this,SLOT(requestOutputNames()));

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

    connect(&rxThread,SIGNAL(datagramReceived()),this,SLOT(packetCallBack()));//, Qt::DirectConnection);
    connect(&rxThread,SIGNAL(fbDatagramReceived()),this,SLOT(packetFBCallBack()));//, Qt::DirectConnection);
    //rxThread.startThread(UDP_INPUT_BIND_ADDRESS,UDP_INPUT_SEND_ADDRESS,UDP_INPUT_PORT);

#ifndef UDP_SYNC_PACKET_TRIGGER
    // configure and start refresh timer
    connect(tick,SIGNAL(timeout()),this,SLOT(sendPacket()) );
    tick->start();
#endif

    // start timers
    tick1hz->start();
    tickUpdate->start();

    portEnabled = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cbSineWave()
{
        iDlg->setDisabled(ui->cbSineWave->isChecked());

}

void MainWindow::enablePort()
{
    if( portEnabled )
    {
        portEnabled = false;
        qDebug() << "enablePort()" << portEnabled;
        // send transport control to disable Xpress stream
        ctrlRemoteTx(false);

        rxThread.blockSignals(true);
        // stop listening for Xpress output packets
        rxThread.stopThread();
        // close input stream socket.
        osocket->close();
        // close transport stream socket.
        ctrlsocket->close();

        qApp->processEvents();
        QThread::usleep(100000);
        qApp->processEvents();

        outputChannelCount = 0;
        outputfbChannelCount = 0;

        ui->edtAddress->setEnabled(true);
        ui->edtSendTo->setEnabled(true);
        ui->edtPort->setEnabled(true);
        ui->edtPortOp->setEnabled(true);
        ui->pbStartTx->setEnabled(false);
        ui->pbStopTx->setEnabled(false);

        ui->lblFps->setText("Sync FPS: ???");
        ui->lblInfo->setText("Output Count: ???");


        ui->pbEnable->setText("Enable Ports");

    }
    else
    {
        portEnabled = true;
        qDebug() << "enablePort()" << portEnabled;

        rxThread.blockSignals(true);
        // start the recevie thread, this thread listens for baked output packets from Xpress Engine
//        rxThread.startThread(ui->edtSendTo->text(),ui->edtAddress->text(),ui->edtPortOp->text().toInt());
        rxThread.startThread("0.0.0.0",ui->edtPortOp->text().toInt());

        // bind output socket for sending input data to Xpress
        if( osocket->bind(QHostAddress("0.0.0.0"), ui->edtPort->text().toInt() ) == false )
        {
            qDebug() << "Failed to bind output socket!";
            rxThread.stopThread();
            portEnabled = false;
            return;
        }

        // bind ctrl socket for sending tranport conrols to Xpress
        if( ctrlsocket->bind(QHostAddress("0.0.0.0"), ui->edtPortOp->text().toInt() + 1 ) == false )
        {
            qDebug() << "Failed to bind control socket!";
        }

        // send transport control to enable Xpress stream
        ctrlRemoteTx(true);

        // get the output names
        requestOutputNames();

        ui->edtAddress->setEnabled(false);
        ui->edtSendTo->setEnabled(false);
        ui->edtPort->setEnabled(false);
        ui->edtPortOp->setEnabled(false);
        ui->pbStartTx->setEnabled(true);
        ui->pbStopTx->setEnabled(true);

        ui->pbEnable->setText("Disable Ports");

        rxThread.blockSignals(false);

    }
}

void MainWindow::infoRefresh()
{
    QString foo;
    inputStatusBits = 0;
    syncTimeCode = 0;

    ui->lblFps->setText("Sync FPS: " + QString::number(syncFps));
    syncFps = 0;

    ui->lblInfo->setText("Output Count: " + QString::number(outputChannelCount));

}

// we have an output packet to process from Xpress2
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

// we have an output packet to process from Xpress2
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

// a datagram is ready to read on input sync stream from Xpress2

void MainWindow::osync()
{
    static float sinpos = 0;
    char    iBuffer[XP2_UDP_INPUT_SYNC_PACKET_SIZE];

    //qDebug() << "Sync Trigger";
    if( osocket->hasPendingDatagrams() )
    {
        int r = osocket->readDatagram(iBuffer,XP2_UDP_INPUT_SYNC_PACKET_SIZE,&inputSenderAddress,NULL);
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
/*
            char b[100];
            sprintf(b,"Sync TC= %3.3d:%2.2d:%2.2d:%3.3d",syncTimeCodeBuffer.hours,syncTimeCodeBuffer.minutes,syncTimeCodeBuffer.seconds,syncTimeCodeBuffer.frames);
            qDebug() << b;
            */
        }
    }
}

void MainWindow::sendPacket( QHostAddress destinationAddress )
{
    char       lBuffer[512];

    bzero(lBuffer,512);
    int l = createUdpInputPacket(XP2_MAX_INPUTS,(char *)inputValues,lBuffer);
    // or send as many inputs as you reqire up to MAX_INPUTS
    //int l = createUdpInputPacket(4,(char *)inputValues,lBuffer);

    //  send data
//    if( osocket->writeDatagram((char*)lBuffer, l,  QHostAddress(ui->edtAddress->text()), ui->edtPort->text().toInt()) >= 0 )
    if( osocket->writeDatagram((char*)lBuffer, l,  destinationAddress, ui->edtPort->text().toInt()) >= 0 )
    {
       // qDebug() << "Sent" << Sent++;
       // nanoSent = timer.nsecsElapsed();
    }
    else
    {
        qDebug() << osocket->errorString();
    }

}


void MainWindow::ctrlsync()
{
    unsigned char    iBuffer[512];

    // datagram was recevied from Xpress Transport control port
    if( ctrlsocket->hasPendingDatagrams() )
    {
        //qDebug() << "Have CTRL datagram";
        QHostAddress ctrlSenderAddress;
        int r = ctrlsocket->readDatagram((char*)iBuffer,512,&ctrlSenderAddress,NULL);
        if( r <= 0)
        {
            return;
        }
        qDebug() << "ctrlsync()" << " Received from " << ctrlSenderAddress << r << "bytes";
        if( iBuffer[0] == BYTE_HEADER)
        {
            if( iBuffer[3] == BYTE_TRAILER )
            {
                switch(iBuffer[1])
                {
                default:
                    qDebug() << "Unknown CTRL Command! " << iBuffer[1];
                    ui->ptLog->appendPlainText("Unknown CTRL Command!");
                    break;
                case BYTE_TRANSPORT_STOP:
                    qDebug() << "Transport STOP";
                    ui->ptLog->appendPlainText("Transport STOP");
                    break;
                case BYTE_TRANSPORT_PLAY:
                    qDebug() << "Transport PLAY";
                    if( iBuffer[2] == BYTE_TRANSPORT_RECD )
                        ui->ptLog->appendPlainText("Transport RECORD");
                    else
                        ui->ptLog->appendPlainText("Transport PLAY");
                    break;
                case BYTE_TRANSPORT_PLAY_FOUR:
                    qDebug() << "Transport PLAY 4x";
                    ui->ptLog->appendPlainText("Transport PLAY 4x");
                    break;
                case BYTE_TRANSPORT_PLAY_FULL:
                    qDebug() << "Transport PLAY 1x";
                    ui->ptLog->appendPlainText("Transport PLAY 1x");
                    break;
                case BYTE_TRANSPORT_PLAY_DOUBLE:
                    qDebug() << "Transport PLAY 2x";
                    ui->ptLog->appendPlainText("Transport PLAY 2x");
                    break;
                case BYTE_TRANSPORT_PLAY_HALF:
                    qDebug() << "Transport PLAY 1/2";
                    ui->ptLog->appendPlainText("Transport PLAY 1/2");
                    break;
                case BYTE_TRANSPORT_PLAY_QUATER:
                    qDebug() << "Transport PLAY 1/4";
                    ui->ptLog->appendPlainText("Transport PLAY 1/4");
                    break;
                case BYTE_TRANSPORT_FWD:
                    qDebug() << "Transport FWD";
                    ui->ptLog->appendPlainText("Transport FWD");
                    break;
                case BYTE_TRANSPORT_RWD:
                    qDebug() << "Transport RWD";
                    ui->ptLog->appendPlainText("Transport RWD");
                    break;
                case BYTE_TRANSPORT_GOTO:
                    qDebug() << "Transport GOTO";
                    ui->ptLog->appendPlainText("Transport GOTO");
                    break;
                case BYTE_TRANSPORT_RECD:
                    qDebug() << "Transport RECD";
                    ui->ptLog->appendPlainText("Transport RECD");
                    break;
                case BYTE_TRANSPORT_RWDPLAY:
                    qDebug() << "Transport RWD PLAY";
                    ui->ptLog->appendPlainText("Transport RWD PLAY");
                    break;
                case BYTE_TRANSPORT_STEP_BKWD:
                    qDebug() << "Transport STEP BACK";
                    ui->ptLog->appendPlainText("Transport STEP BACK");
                    break;
                case BYTE_TRANSPORT_STEP_FWD:
                    qDebug() << "Transport STEP FWD";
                    ui->ptLog->appendPlainText("Transport STEP FWD");
                    break;
                }
            }
            else
            {
                qDebug() << "Bad CTRL Trailer";
            }

        }
        else
        {
            ui->ptLog->appendPlainText("Bad CTRL Header: " + QString::number(iBuffer[0]) + " Lenght: " + QString::number(r));
            qDebug() << "Have CTRL datagram";
            qDebug() << "Received " << r << "byes";
            qDebug() << "Bad CTRL Header";
            for( int t = 0; t < r; t++ )
               fprintf(stderr,"%2.2x ",iBuffer[t]);
            fprintf(stderr,"\n");
            fflush(stderr);
            //flush();

        }

    }
}


void MainWindow::ctrlRemoteTx(bool enable)
{


    unsigned char	lBuffer[ MAX_REQUEST_SIZE ];
    int count=0;

    if( ctrlsocket->state() != QAbstractSocket::BoundState  ) return;

    lBuffer[count++] = BYTE_HEADER;
    if( enable )
        lBuffer[count++] = BYTE_STREAM_START;
    else
        lBuffer[count++] = BYTE_STREAM_STOP;
    lBuffer[count++] = 0;
    lBuffer[count++] = BYTE_TRAILER;

    qDebug() << "Xpress VP Start/Stop Stream";

    //  send data
    if( ctrlsocket->writeDatagram((char*)lBuffer, count,  QHostAddress(ui->edtAddress->text()), ui->edtPortOp->text().toInt() + 1) >= 0 )
    {
       // qDebug() << "Sent" << Sent++;
       // nanoSent = timer.nsecsElapsed();
    }
    else
    {
        qDebug() << ctrlsocket->errorString();
    }


}


void MainWindow::requestOutputNames()
{
    unsigned char	lPacket[ MAX_REQUEST_SIZE ];
    unsigned char   lBuffer[ MAX_BUFFER_SIZE ];

    if( ctrlsocket->state() != QAbstractSocket::BoundState  ) return;

    ctrlsocket->blockSignals(true);

    rxThread.blockSignals(true);

    // Build info packet for request
    // Output names are read 10 at a time.
    for( int t = 0; t < XP2_MAX_OUTPUTS/10; t++ )
    {
        lPacket[0] = BYTE_HEADER;
        lPacket[1] = BYTE_INFO_PACKET;
        lPacket[2] = t;
        lPacket[3] = BYTE_TRAILER;

        qDebug() << "Request ouptut names " << t+1 << " of " << XP2_MAX_OUTPUTS/10;
        if( ctrlsocket->writeDatagram((char*)lPacket, 4,  QHostAddress(ui->edtAddress->text()), ui->edtPortOp->text().toInt() + 1) >= 0 )
        {
           // qDebug() << "Sent" << Sent++;
           // nanoSent = timer.nsecsElapsed();
        }
        else
        {
            qDebug() << ctrlsocket->errorString();
            ctrlsocket->blockSignals(false);
            rxThread.blockSignals(false);
            return;
        }

        ctrlsocket->waitForReadyRead(100);
        if( ctrlsocket->hasPendingDatagrams() )
        {
            int r = ctrlsocket->readDatagram((char *)lBuffer,MAX_BUFFER_SIZE,NULL,NULL);
            if( r <= 0)
            {
                qDebug() << "Error reading CTRL socket name data!";
                ctrlsocket->blockSignals(false);
                rxThread.blockSignals(false);
                return;
            }

            // check header and ctrl byte
            if( lBuffer[0] == BYTE_HEADER && lBuffer[1] == BYTE_INFO_PACKET )
            {
                // get the output offset index
                int lAddress = lBuffer[2] * 10;

                // check against requested index
                if( lAddress == t*10 )
                {
                    // check received data count and trailer byte
                    if( r == (MAX_REQUEST_SIZE+(MAX_STR_LEN*10)) && lBuffer[MAX_REQUEST_SIZE+(MAX_STR_LEN*10)-1] == BYTE_TRAILER )
                    {
                        // all good so store the ouptu names
                        for( int i = 0; i < 10; i++ )
                        {
                            //SetName((t*10)+i,(char *)&lBuffer[(MAX_REQUEST_SIZE-1)+(i*MAX_STR_LEN)]);
                            strcpy(outputNames[(t*10)+i], (char *)&lBuffer[(MAX_REQUEST_SIZE-1)+(i*MAX_STR_LEN)] );
                            qDebug() <<  "Read output " << (t*10)+i << " name " << outputNames[(t*10)+i];
                            oDlg->updateSliderName((t*10)+i,outputNames[(t*10)+i]);
                        }
                    }
                    else
                    {
                        qDebug() << "Read Name Failed! " << lAddress << " , " << r << " Trailer = " << lBuffer[MAX_REQUEST_SIZE+(MAX_STR_LEN*10)-1];
                        ctrlsocket->blockSignals(false);
                        rxThread.blockSignals(false);
                        return;
                    }
                }
                else
                {
                    qDebug() << "Request address mismatch " << lAddress << " != " << t*10 ;
                    ctrlsocket->blockSignals(false);
                    rxThread.blockSignals(false);
                    return;
                }
            }
            else
            {
                qDebug() << "Error CTRL header mismatch!";
                ctrlsocket->blockSignals(false);
                rxThread.blockSignals(false);
                return;
            }

        }
        else
        {
            qDebug() << "Error waiting for CTRL socket name data!";
            ctrlsocket->blockSignals(false);
            rxThread.blockSignals(false);
            return;
        }
    }

    ctrlsocket->blockSignals(false);
    rxThread.blockSignals(false);

}


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

void MainWindow::inputSliderMoved(int index, float value)
{
    //qDebug() << index << " " << value;
    //return( outLo + ( ((inX-inLo)*(outHi-outLo)) / (inHi-inLo) ) );
    inputValues[index] = ((value - -1.0)*65535) / 2.0;
}


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
