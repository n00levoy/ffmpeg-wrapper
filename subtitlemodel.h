#ifndef SUBTITLEMODEL_H
#define SUBTITLEMODEL_H

#include "mediadata.h"

#include <QAbstractListModel>
#include <QHash>

class SubtitleModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LanguageRole,
        TitleRole,
        FilenameRole
    };

public:
    SubtitleModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    void addSubtitle(const Subtitle& subtitle);
    Q_INVOKABLE void addSubtitle(const QString& filename);
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void clear();

    QList<Subtitle> rawData() const { return m_subtitles; }

private:
    QList<Subtitle> m_subtitles;
};
Q_ENUMS(SubtitleModel::Roles)

#endif // SUBTITLEMODEL_H
