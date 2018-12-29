#include "stringencoder.h"

#include <QFile>
#include <QTextCodec>

#include <stdexcept>

StringEncoder::StringEncoder(QString primaryCodec, QString secondaryCodec)
{
    m_primaryCodec   = primaryCodec;
    m_secondaryCodec = secondaryCodec;
}

void StringEncoder::encodeToUtf(QString filename)
{
    auto content = loadContent(filename);

    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName(m_primaryCodec.toUtf8());
    codec->toUnicode(content.constData(), content.size(), &state);
    if(state.invalidChars > 0)
    {
        QTextCodec::ConverterState rus_state;
        QTextCodec* rus_codec = QTextCodec::codecForName(m_secondaryCodec.toUtf8());
        if(!codec)
           return;

        auto encoded_data = rus_codec->toUnicode(content.constData(), content.size(), &rus_state);
        if(rus_state.invalidChars > 0)
            throw std::runtime_error("Undetected codec");
        else
            saveContent(filename, encoded_data.toUtf8());
    }
    else
        return;
}

QByteArray StringEncoder::loadContent(QString filename)
{
    QFile file(filename);

    QByteArray content;
    if(file.exists() && file.open(QIODevice::ReadOnly))
    {
        content = file.readAll();
        file.close();
    }
    else
        throw std::runtime_error("Cannot open subtitle file");

    return content;
}

void StringEncoder::saveContent(QString filename, QByteArray data)
{
    QFile file(filename);

    QByteArray content;
    if(file.exists() && file.open(QIODevice::WriteOnly))
    {
        file.write(data);
        file.close();
    }
    else
        throw std::runtime_error("Cannot write in subtitle file");

    return;
}
