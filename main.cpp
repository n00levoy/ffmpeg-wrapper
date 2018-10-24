#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>
#include <QQmlContext>

#include "converter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qRegisterMetaType<MediaData>("MediaData");
    qRegisterMetaType<Subtitle>("Subtitle");
    qmlRegisterInterface<SubtitleModel>("SubtitleModel");
    qmlRegisterType<Converter>("FFMpeg.Wrapper", 1, 0, "Converter");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
