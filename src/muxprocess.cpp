#include "muxprocess.h"

#include <QProcess>
#include <QFile>
#include <QFileInfo>
#include <QTime>

#include "stringencoder.h"

void MuxProcess::run()
{
    QProcess process;

    #if defined(Q_OS_WIN)
        process.setProgram("bin/ffmpeg.exe");
    #elif defined(Q_OS_LINUX)
        process.setProgram("ffmpeg");
    #endif

    process.setReadChannelMode(QProcess::MergedChannels);

    bool two_pass = (m_vb > 0);

    connect(&process, &QProcess::readyRead, [this, &process]() {
        auto data = QString::fromLatin1(process.readAll());
        emit dataReadyRead(data);
    });

    QStringList input_args;
    input_args << "-i"
               << m_in_filename;

    StringEncoder encoder("UTF-8", "Windows-1251");
    for(const auto& sub: m_target_data.subs_list)
    {
        QString filename = sub.filename;
        if(!filename.isEmpty())
        {
            encoder.encodeToUtf(filename);
            input_args << "-i" << filename;
        }
    }

    int vs_count = 0;
    int as_count = 0;

    QStringList map_args;
    for(auto key: m_target_data.streams_map.keys())
    {
        auto val = m_target_data.streams_map[key];

        if(val != "data")
            map_args << "-map" << "0:" + QString::number(key);

        if(val == "video")
            vs_count++;
        if(val == "audio")
            as_count++;
    }

    int sub_counter = 1;
    for(const auto& sub: m_target_data.subs_list)
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
    {
        codec_args << "-c:v:" + QString::number(i);
        if(i == 0 && two_pass)
            codec_args << "libx264";
        else
            codec_args << "copy";
    }
    for(int i = 0; i < as_count; i++)
        codec_args << "-c:a:" + QString::number(i)
                   << "copy";

    int ss_counter = 0;
    for(const auto& sub: m_target_data.subs_list)
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
                           << "title=" + sub.title;

            ss_counter++;
        }
    }

    for(const auto& sub: m_target_data.subs_list)
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
                           << "title=" + sub.title;

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
    if(two_pass)
        bitrate_args << "-b:v:0" << QString::number(m_vb) + "k";

    QStringList output_args = QStringList() << m_out_filename;

    QFileInfo info(m_in_filename);
    QString source_suffix = info.suffix();

    emit muxingStarted(two_pass);

    if(two_pass)
    {
        QStringList first_run_args;
        first_run_args << "-y"
                       << "-i"
                       << m_in_filename;

        if(!resize_args.isEmpty())
            first_run_args << resize_args;

        first_run_args << "-b:v" << QString::number(m_vb) + "k"
                       << "-pass" << "1"
                       << "-an" << "-sn";

        if(source_suffix == "mp4")
            first_run_args << "-f" << "mp4";
        else if(source_suffix == "mkv")
            first_run_args << "-f" << "matroska";

        first_run_args << "nul";

        process.setArguments(first_run_args);
        process.start();
        process.waitForFinished(-1);

        emit firstPassFinished();
    }

    QStringList run_args;
    run_args << "-y";
    run_args << input_args
             << map_args
             << codec_args;

    if(!resize_args.isEmpty())
        run_args << resize_args;

    if(two_pass)
    {
        run_args << bitrate_args
                 << "-pass" << "2";
    }

    bool same_file = (m_in_filename == m_out_filename);
    info.fileName();

    QFile file(info.path() + "/" + info.baseName() + "-tmp." + source_suffix);
    if(same_file)
    {
        if(file.open(QIODevice::WriteOnly))
            run_args << file.fileName();
        file.close();
    }
    else
        run_args << output_args;

    process.setArguments(run_args);

    process.start();
    process.waitForFinished(-1);

    if(process.exitStatus() == QProcess::ExitStatus::NormalExit && process.exitCode() == 0)
    {
        if(same_file)
        {
            QFile source(m_in_filename);
            source.remove();

            file.rename(m_in_filename);
        }
    }
    else
        emit ffmpegFailed();

    emit muxingFinished();
}
