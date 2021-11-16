#include <QApplication>
#include <QCommandLineParser>

#include "NonogramQt.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(NonogramQt::tr("Nonogram Qt"));
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(NonogramQt::tr("[file]"), NonogramQt::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    NonogramQt nonogramQt;
    if (!commandLineParser.positionalArguments().isEmpty()
        && !nonogramQt.loadFile(commandLineParser.positionalArguments().front())) {
        return -1;
    }
    nonogramQt.show();
    return app.exec();
}