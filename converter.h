#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>

#include "mediadata.h"
#include "subtitlemodel.h"

class Converter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString inputFilename READ inputFilename WRITE setInputFilename NOTIFY inputFilenameChanged)
    Q_PROPERTY(QString outputFilename READ outputFilename WRITE setOutputFilename NOTIFY outputFilenameChanged)
    Q_PROPERTY(int videoBitrate READ videoBitrate WRITE setVideoBitrate NOTIFY videoBitrateChanged)
    Q_PROPERTY(int frameWidth READ frameWidth WRITE setFrameWidth NOTIFY frameWidthChanged)
    Q_PROPERTY(int frameHeight READ frameHeight WRITE setFrameHeight NOTIFY frameHeightChanged)

public:
    explicit Converter(QObject *parent = nullptr);

    QString inputFilename() const { return m_input_filename; }
    QString outputFilename() const { return m_output_filename; }

    int videoBitrate() const { return m_vb; }
    int frameWidth() const { return m_fw; }
    int frameHeight() const { return m_fh; }

    void setInputFilename(QString name);
    void setOutputFilename(QString name);

    void setVideoBitrate(int bitrate);
    void setFrameWidth(int width);
    void setFrameHeight(int height);

    Q_INVOKABLE SubtitleModel* subtitleModel() const { return m_subtitle_model; }

    Q_INVOKABLE void mux();

private:
    void loadMediaDataFromFile();

private:
    QString m_input_filename;
    QString m_output_filename;

    int m_stream_count;

    int m_vb;
    int m_fw;
    int m_fh;

    MediaData m_original_data;

    SubtitleModel* m_subtitle_model;

signals:
    void inputFilenameChanged();
    void outputFilenameChanged();

    void videoBitrateChanged();
    void frameWidthChanged();
    void frameHeightChanged();

    void mediaDataLoaded();

public slots:

private slots:
    void readMediaData(MediaData data);
};

#endif // CONVERTER_H
