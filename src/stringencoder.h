#ifndef STRINGENCODER_H
#define STRINGENCODER_H

#include <QString>

class StringEncoder
{
public:
    StringEncoder(QString primaryCodec, QString secondaryCodec);

    void encodeToUtf(QString filename);

private:
    QByteArray loadContent(QString filename);
    void saveContent(QString filename, QByteArray data);

private:
    QString m_primaryCodec;
    QString m_secondaryCodec;
};

#endif // STRINGENCODER_H
