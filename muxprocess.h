#ifndef MUXPROCESS_H
#define MUXPROCESS_H

#include <QThread>

#include "mediadata.h"

class MuxProcess : public QThread
{
    Q_OBJECT

public:
    MuxProcess(QObject* parent = nullptr) : QThread(parent), m_vb{-1} {}

    void setOutputFile(QString name) { m_out_filename = name; }
    void setInputFile(QString name) { m_in_filename = name; }

    void setOriginalData(MediaData data) { m_original_data = data; }
    void setTargetData(MediaData data) { m_target_data = data; }

    void setVideoBitrate(int bitrate) { m_vb = bitrate; }

private:
    void run() override;

signals:
    void dataReadyRead(QString);
    void firstPassFinished();
    void muxStarted(bool two_pass);
    void muxingFinished();

private:
    int m_vb;

    QString m_in_filename;
    QString m_out_filename;

    MediaData m_original_data;
    MediaData m_target_data;
};

#endif // MUXPROCESS_H
