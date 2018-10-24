#include "muxprocess.h"

#include <QProcess>

#include <QDebug>

void MuxProcess::run()
{
    QProcess process;
    process.setProgram("bin/ffmpeg.exe");
    process.setReadChannelMode(QProcess::MergedChannels);
    connect(&process, &QProcess::readyRead, [&process]() {
        qDebug() << process.readAll();
    });

    int input_count = 1;

    QStringList input_args;
    input_args << "-i"
               << m_in_filename;

    for(auto sub: m_target_data.subs_list)
    {
        qDebug() << sub.filename << sub.title;

        if(!sub.filename.isEmpty())
        {
            input_args << "-i" << sub.filename;
            input_count++;
        }
    }

    int vs_count = 0;
    int as_count = 0;

    QStringList map_args;
    for(auto key: m_target_data.streams_map.keys())
    {
        auto val = m_target_data.streams_map[key];

        map_args << "-map"
                 << "0:" + QString::number(key);

        if(val == "video")
            vs_count++;
        if(val == "audio")
            as_count++;
    }

    int sub_counter = 1;
    for(auto sub: m_target_data.subs_list)
    {
        if(!sub.filename.isEmpty())
        {
            map_args << "-map"
                     << QString::number(sub_counter);
            sub_counter++;
        }
    }

    QStringList codec_args;
    for(int i = 0; i < vs_count; i++)
        codec_args << "-c:v:" + QString::number(i)
                   << "copy";
    for(int i = 0; i < as_count; i++)
        codec_args << "-c:a:" + QString::number(i)
                   << "copy";

    int ss_counter = 0;
    for(auto sub: m_target_data.subs_list)
    {
        if(sub.filename.isEmpty())
        {
            QString stream_id = QString::number(ss_counter);

            codec_args << "-c:s:" + QString::number(ss_counter)
                       << "copy";

            if(!sub.lang.isEmpty())
                codec_args << "-metadata:s:s:" + stream_id
                           << "language=" + sub.lang;
            if(!sub.title.isEmpty())
                codec_args << "-metadata:s:s:" + stream_id
                           << "title=\"" + sub.title + "\"";

            ss_counter++;
        }
    }

    for(auto sub: m_target_data.subs_list)
    {
        if(!sub.filename.isEmpty())
        {
            QString stream_id = QString::number(ss_counter);

            codec_args << "-c:s:" + stream_id
                       << "srt";
            if(!sub.lang.isEmpty())
                codec_args << "-metadata:s:s:" + stream_id
                           << "language=" + sub.lang;
            if(!sub.title.isEmpty())
                codec_args << "-metadata:s:s:" + stream_id
                           << "title=\"" + sub.title + "\"";

            ss_counter++;
        }
    }

    int ofh = m_original_data.frame_height;
    int ofw = m_original_data.frame_width;
    int tfh = m_target_data.frame_height;
    int tfw = m_target_data.frame_width;

    QStringList resize_args;
    if(ofh != tfh || ofw != tfw)
        resize_args << "-s:v:0" << QString::number(tfw) + "x" + QString::number(tfh);

    QStringList bitrate_args;
    if(m_vb > 0)
        bitrate_args << "-b:v:0" << QString::number(m_vb) + "k";

    QStringList output_args = QStringList() << m_out_filename;

    qDebug() << input_args;
    qDebug() << map_args;
    qDebug() << codec_args;
    qDebug() << resize_args;
    qDebug() << bitrate_args;
    qDebug() << output_args;

//    QStringList tp_fr_args;
//    tp_fr_args << "-y"
//               << "-i"
//               << m_in_filename;

//    if(!frame_args.isEmpty())
//        args_firstrun << frame_args;
//    args_firstrun << "-b:v"
//                  << bitrate
//                  << "-pass"
//                  << "1"
//                  << "-an"
//                  << "-f"
//                  << "mp4"
//                  << "nul";


    process.start();
    process.waitForFinished();
}
