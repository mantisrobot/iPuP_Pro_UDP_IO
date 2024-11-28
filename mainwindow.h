#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>
#include <QElapsedTimer>
#include <QMdiSubWindow>
#include <QCommandLineParser>
#include <memory.h>
#include <QDebug>
#include <receivethread.h>
#include <outputdlg.h>
#include <inputdlg.h>
#include <output.h>
#include <xp2defs.h>
#include <math.h>

#define UDP_MAX_SIZE 516    // http://www.danzig.jct.ac.il/tcp-ip-lab/ibm-tutorial/3376c211.html

#define UDP_SYNC_PACKET_TRIGGER

// INPUT port is for data coming in from Xpress, e.g. Xpress baked output data
//#define UDP_INPUT_BIND_ADDRESS  "192.168.7.1" //"0.0.0.0"  // ANY
//#define UDP_INPUT_SEND_ADDRESS  "10.0.0.2"
#define UDP_INPUT_PORT          10000

// OUTPUT port is for data going to Xpress, e.g. inputs to drive expressions
//#define UDP_OUTPUT_BIND_ADDRESS   "192.168.7.1" //"0.0.0.0" // ANY
#define UDP_OUTPUT_SEND_ADDRESS   "10.0.0.2"
#define UDP_OUTPUT_PORT         20000

#define UDP_OUTPUT_HEADER       0xAA02
#define	UDP_OUTPUT_SYNC_HEADER  0xAA03

#define FPS 50



namespace Ui {
class MainWindow;
}
/*
const	quint16 cCrc16Table[16] = {
                0x0000, 0xcc01, 0xd801, 0x1400,
                0xf001, 0x3c00, 0x2800, 0xe401,
                0xa001, 0x6c00, 0x7800, 0xb401,
                0x5000, 0x9c01, 0x8801, 0x4400
                };
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void ctrlRemoteTx(bool);

private slots:

    void sendPacket( QHostAddress address );
    void packetCallBack(void);
    void packetFBCallBack(void);
    void osync(void);
    void ctrlsync(void);
    void infoRefresh();
    void displayRefresh(void);
    void inputSliderMoved(int index, float value);
    void enablePort();
    void enableTx() {ctrlRemoteTx(true);}
    void disableTx() {ctrlRemoteTx(false);}
    void requestOutputNames();
    void cbSineWave();

private:
    Ui::MainWindow *ui;

    QUdpSocket *osocket;
    QUdpSocket *ctrlsocket;
    QTimer *tick,*tickUpdate;
    QTimer *tick1hz;
    QElapsedTimer timer;
    quint32 syncTimeCode;
    s_TimeCode syncTimeCodeBuffer;
    quint16 syncStatusLength;
    quint16 syncStatusData[XP2_MAX_SYNC_DATA_LENGTH];
    quint16 inputStatusBits;
    int nanoMatch;
    int timeStampMatch;

    receiveThread   rxThread;

    quint16 inputValues[XP2_MAX_INPUTS];
    quint16 outputValues[XP2_MAX_OUTPUTS];
    XP2_FLOAT   outputValuesReal[XP2_MAX_OUTPUTS];
    char    outputNames[XP2_MAX_OUTPUTS][MAX_STR_LEN];
    quint16 outputfbValues[XP2_MAX_OUTPUTS];
    XP2_FLOAT   outputfbValuesReal[XP2_MAX_OUTPUTS];
    quint16 outputChannelCount;
    quint16 outputfbChannelCount;
    char    timeCode[100];
    char    timeCodeFb[100];

    quint16 crcGenerate( char *dPtr, unsigned pLength );
    int createUdpInputPacket(  quint16 channelCount, char *pInData, char *pOutData );
    int checkUdpSyncPacket( quint16 bytesIn, char *pData );

//    int checkUdpOutputPacket( quint16 bytesIn, char *pData );
//    int processUdpOutputPacket( quint16 bytesIn, char *pData );

    int     Sent;
    int     syncFps;

    OutputDlg   *oDlg;
    OutputDlg   *fbDlg;
    inputdlg    *iDlg;

    bool    portEnabled;

    int     inputCount;
    int     outputCount;
    int     outputFbCount;

    QHostAddress    inputSenderAddress;
    QHostAddress    outputSenderAddress ;

};

#endif // MAINWINDOW_H
