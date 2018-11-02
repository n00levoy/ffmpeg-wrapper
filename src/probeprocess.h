#ifndef PROBEPROCESS_H
#define PROBEPROCESS_H

#include "mediadata.h"

#include <QThread>

class ProbeProcess : public QThread
{
    Q_OBJECT

public:
    ProbeProcess(QObject* parent = nullptr) : QThread(parent) {}

    void setFilename(QString filename) { m_filename = filename; }

private:
    void run() override;

signals:
    void resultReady(const MediaData &s);

private:
    QString m_filename;
};

#endif // PROBEPROCESS_H
