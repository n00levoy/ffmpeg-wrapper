#include "converter.h"

#include "probeprocess.h"
#include "muxprocess.h"

#include <QFileInfo>

#include <QListView>
#include <QDebug>

Converter::Converter(QObject *parent) : QObject(parent),
    m_stream_count{-1},
    m_vb{-1}, m_fw{-1}, m_fh{-1},
    m_subtitle_model(new SubtitleModel(this)),
    m_process_time(0, 0)
{
    connect(&m_process_timer, &QTimer::timeout, [this]()
    {
        m_process_time = m_process_time.addSecs(1);
        emit processTimeElapsed(m_process_time.toString("hh:mm:ss"));
    });
}

void Converter::setInputFilename(QString name)
{
    if(m_input_filename == name)
        return;

    m_input_filename = name;
    emit inputFilenameChanged();

    loadMediaDataFromFile();
}

void Converter::setOutputFilename(QString name)
{
    if(m_output_filename == name)
        return;

    m_output_filename = name;
    emit outputFilenameChanged();
}

void Converter::loadMediaDataFromFile()
{
    QFileInfo file_info(m_input_filename);
    if(!file_info.exists())
        return;

    ProbeProcess* process = new ProbeProcess;
    connect(process, &ProbeProcess::resultReady, this, &Converter::readMediaData);
    connect(process, &ProbeProcess::finished, process, &ProbeProcess::deleteLater);

    process->setFilename(file_info.absoluteFilePath());
    process->start();
}

void Converter::setVideoBitrate(int bitrate)
{
    if(m_vb == bitrate)
        return;

    m_vb = bitrate;
    emit videoBitrateChanged();
}

void Converter::setFrameWidth(int width)
{
    if(m_fw == width)
        return;

    m_fw = width;
    emit frameWidthChanged();
}

void Converter::setFrameHeight(int height)
{
    if(m_fh == height)
        return;

    m_fh = height;
    emit frameHeightChanged();
}

void Converter::readMediaData(MediaData data)
{
    m_fw = data.frame_width;
    m_fh = data.frame_height;

    m_original_data = data;

    m_subtitle_model->clear();
    for(auto subtitle: data.subs_list)
        m_subtitle_model->addSubtitle(subtitle);

    emit mediaDataLoaded();
}

void Converter::mux()
{
    MuxProcess* process = new MuxProcess;
    connect(process, &MuxProcess::finished, process, &MuxProcess::deleteLater);
    connect(process, &MuxProcess::dataReadyRead, this, &Converter::readMuxingState);
    connect(process, &MuxProcess::firstPassFinished, this, &Converter::firstPassFinished);
    connect(process, &MuxProcess::muxingFinished, this, &Converter::muxingFinished);
    connect(process, &MuxProcess::muxingStarted, this, &Converter::muxingStarted);

    process->setInputFile(m_input_filename);
    process->setOutputFile(m_output_filename);

    if(m_vb > 0)
        process->setVideoBitrate(m_vb);

    process->setOriginalData(m_original_data);

    MediaData target_data;
    target_data.frame_height = m_fh;
    target_data.frame_width = m_fw;
    target_data.duration = m_original_data.duration;

    for(int key: m_original_data.streams_map.keys())
    {
        auto val = m_original_data.streams_map[key];

        if(val == "subtitle")
        {
            for(auto sub: m_subtitle_model->rawData())
            {
                if(sub.id == key)
                {
                    target_data.streams_map[key] = val;
                    break;
                }
            }
        }
        else
            target_data.streams_map[key] = val;
    }

    target_data.subs_list = m_subtitle_model->rawData();

    process->setTargetData(target_data);

    m_process_time = QTime(0, 0);
    m_process_timer.start(1000);

    process->start();
}

void Converter::readMuxingState(QString data)
{
    if(data.contains("time="))
    {
        auto cur_times = data.split("time=").at(1).split(" bitrate").at(0).split(QRegExp("\\D"), QString::SkipEmptyParts);
        double time = cur_times.at(0).toInt() * 3600 + cur_times.at(1).toInt() * 60 + cur_times.at(2).toInt();

        double percent = time / m_original_data.duration * 100;

        emit muxingStateUpdated(percent);
    }
}
