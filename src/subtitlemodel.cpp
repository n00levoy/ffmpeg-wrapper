#include "subtitlemodel.h"

#include <QDebug>

SubtitleModel::SubtitleModel(QObject *parent) : QAbstractListModel(parent)
{

}

int SubtitleModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_subtitles.count();
}

QVariant SubtitleModel::data(const QModelIndex& index, int role) const
{
    auto subtitle = m_subtitles.at(index.row());

    switch(role)
    {
    case Roles::IdRole:
        return subtitle.id;

    case Roles::LanguageRole:
        return subtitle.lang;

    case Roles::TitleRole:
        return subtitle.title;

    case Roles::FilenameRole:
        return subtitle.filename;

    case Qt::DisplayRole:
        return QString::number(subtitle.id) + "-" + subtitle.lang;

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SubtitleModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Roles::IdRole] = "id";
    roles[Roles::LanguageRole] = "language";
    roles[Roles::TitleRole] = "title";
    roles[Roles::FilenameRole] = "filename";

    return roles;
}

bool SubtitleModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch(role)
    {
    case Roles::LanguageRole:
        if(m_subtitles[index.row()].lang == value.toString())
            return true;

        m_subtitles[index.row()].lang = value.toString();
        break;

    case Roles::TitleRole:
        if(m_subtitles[index.row()].title == value.toString())
            return true;

        m_subtitles[index.row()].title = value.toString();
        break;

    default:
        break;
    }

    emit dataChanged(index, index);
    return true;
}

void SubtitleModel::addSubtitle(const Subtitle& subtitle)
{
    int rowIndex = rowCount();

    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    m_subtitles.append(subtitle);
    endInsertRows();
}

void SubtitleModel::addSubtitle(const QString& filename)
{
    Subtitle subtitle;
    subtitle.filename = filename;

    addSubtitle(subtitle);
}

bool SubtitleModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row < 0 || row >= rowCount() || count < 0 || (row + count) > rowCount())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    int countLeft = count;
    while(countLeft--)
        m_subtitles.takeAt(row + (count - 1));
    endRemoveRows();

    return true;
}

void SubtitleModel::clear()
{
     removeRows(0, rowCount());
}
