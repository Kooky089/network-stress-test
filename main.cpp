#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    QCoreApplication::setApplicationName("Network Tester");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Network Tester");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("hosts", QCoreApplication::translate("main", "Hosts to connect."), "[hosts...]");    
    parser.addOption(QCommandLineOption("t", "Test for <seconds>", "seconds"));
    parser.addOption(QCommandLineOption("f", "Write output to logfile at <path>", "path"));
    parser.process(a);
    QStringList hosts = parser.positionalArguments();
    for (auto &host : hosts) {
        w.connectToHostViaCMD(host);
    }
    w.startTimer(parser.value("f"), parser.value("t").toLongLong());
    return a.exec();
}
