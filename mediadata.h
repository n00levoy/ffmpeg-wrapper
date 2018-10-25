#ifndef MEDIADATA_H
#define MEDIADATA_H

#include <QString>
#include <QList>

struct Subtitle
{
    int id = -1;
    QString title;
    QString lang;
    QString filename;
};
Q_DECLARE_METATYPE(Subtitle)

struct MediaData
{
    double duration = 0;
    int frame_width = -1;
    int frame_height = -1;

    QList<Subtitle> subs_list;
    QMap<int, QString> streams_map;
};
Q_DECLARE_METATYPE(MediaData)

#endif // MEDIADATA_H
