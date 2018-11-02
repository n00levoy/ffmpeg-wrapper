#include "probeprocess.h"

#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonValueRef>

void ProbeProcess::run()
{
    QProcess process;

    #if defined(Q_OS_WIN)
        process.setProgram("bin/ffprobe.exe");
    #elif defined(Q_OS_LINUX)
        process.setProgram("ffprobe");
    #endif

    process.setReadChannelMode(QProcess::MergedChannels);
    process.setArguments(QStringList()
                           << "-v"
                           << "quiet"
                           << "-print_format"
                           << "json"
                           << "-show_format"
                           << "-show_streams"
                           << m_filename);

    process.start();
    process.waitForFinished();

    QJsonObject json = QJsonDocument::fromJson(process.readAll()).object();
    QJsonArray streams_list = json["streams"].toArray();
    double duration = json["format"].toObject()["duration"].toString().split(".").at(0).toDouble();

    MediaData result;
    result.duration = duration;

    bool video_found = false;
    for(auto stream_ref: streams_list)
    {
        auto stream = stream_ref.toObject();
        int index = stream["index"].toInt();
        QString type = stream["codec_type"].toString();

        result.streams_map[index] = type;

        if(type == "video" && !video_found)
        {
            int width = stream["width"].toInt();
            int height = stream["height"].toInt();

            result.frame_width = width;
            result.frame_height = height;

            video_found = true;
        }

        if(type == "subtitle")
        {
            auto tags = stream["tags"].toObject();
            QString language = tags["language"].toString();
            QString title = tags["title"].toString();

            Subtitle subtitle;
            subtitle.id = index;
            subtitle.lang = language;
            subtitle.title = title;

            result.subs_list.append(subtitle);
        }
    }

    emit resultReady(result);
}
