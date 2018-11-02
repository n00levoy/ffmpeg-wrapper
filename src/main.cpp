#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickStyle>
#include <QQuickWindow>
#include <QQmlContext>
#include <QQmlFileSelector>

#ifdef Q_OS_WIN
    #include <QWinTaskbarButton>
    #include <QWinTaskbarProgress>
#endif

#include "converter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qRegisterMetaType<MediaData>("MediaData");
    qRegisterMetaType<Subtitle>("Subtitle");
    qmlRegisterInterface<SubtitleModel>("SubtitleModel");
    qmlRegisterType<Converter>("FFMpeg.Wrapper", 1, 0, "Converter");

    QQmlApplicationEngine engine;
    new QQmlFileSelector(&engine);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if(engine.rootObjects().isEmpty())
        return -1;

#ifdef Q_OS_WIN
    auto* window = static_cast<QQuickWindow*>(engine.rootObjects().first());

    QWinTaskbarButton* task_button = new QWinTaskbarButton(window);
    task_button->setWindow(window);

    QWinTaskbarProgress* task_progress = task_button->progress();
    task_progress->setVisible(true);
    task_progress->setValue(0);

    engine.rootContext()->setContextProperty("taskbarButton", task_button);
#endif

    return app.exec();
}
