#include <QApplication>
#include <QtMsgHandler>
#include "mainwindow.h"
#ifdef _WIN32_WINNT
    #include <Windows.h>
#endif

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    char    buffer[1000];

    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        sprintf(buffer,"%s::%s\n", context.function, localMsg.constData());
        //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sprintf(buffer, "Warning: %s (%s:%u)\n", localMsg.constData(), context.function, context.line);
        break;
    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sprintf(buffer, "Critical: %s (%s:%u)\n", localMsg.constData(), context.function, context.line);
        break;
#if QT_VERSION > QT_VERSION_CHECK(5, 4, 0)
    case QtInfoMsg:
        sprintf(buffer, "Info: %s (%s:%u)\n", localMsg.constData(), context.function, context.line);
        break;
#endif
    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sprintf(buffer, "Fatal: %s (%s:%u)\n", localMsg.constData(), context.function, context.line);
        // write to file
        abort();
    }

    //  write to sdtout
    fprintf(stderr,"%s",buffer);
    fflush(stderr);




}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //qInstallMessageHandler(myMessageOutput);


#ifdef _WIN32_WINNT
    HWND hwnd = GetConsoleWindow();
    // increase screen buffer width to 100, 400 default is 80,300
    CONSOLE_SCREEN_BUFFER_INFO  csbi;
    COORD size = {120,400};
    SetConsoleScreenBufferSize(hwnd,size);
    //GetConsoleScreenBufferInfo( hwnd, &csbi );
    //SetConsoleWindowInfo(hwnd, TRUE, &csbi.dwMaximumWindowSize    );
    //RECT rect = {100, 100, 300, 500};
    //MoveWindow(hwnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,TRUE);
    ShowWindow(hwnd, 0);
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription("iPUP_PRO_UI");
    // A boolean option with a single name (-d)
    QCommandLineOption showDebugOption("d", QCoreApplication::translate("main", "Show Debug console"));
    parser.addOption(showDebugOption);
    parser.process(a);

#ifdef _WIN32_WINNT
    bool showDebug = parser.isSet(showDebugOption);
    if( showDebug )
    {
       qDebug() << "Show debug console";
       ShowWindow(hwnd, 1);
    }
    else
    {
       ShowWindow(hwnd, 0);
    }
#endif

    MainWindow w;
    w.show();

    
    return a.exec();
}
