﻿/*
The MIT License (MIT)

Copyright (c) 2014-2018 Oleg Linkin <maledictusdemagog@gmail.com>
Copyright (c) 2017-2018 Maciej Janiszewski <chleb@krojony.pl>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "bookmarksmodel.h"
#include <QtDebug>

namespace LinksBag
{
BookmarksModel::BookmarksModel(QObject *parent)
: QAbstractListModel(parent)
{
}

BookmarksModel::~BookmarksModel()
{
}

int BookmarksModel::GetCount() const
{
    return rowCount();
}

QVariant BookmarksModel::data(const QModelIndex& index, int role) const
{
    if(index.row() < 0 || index.row() > m_Bookmarks.count())
    {
        return QVariant();
    }

    auto bookmark = m_Bookmarks.at(index.row());
    switch(role)
    {
    case BRID:
        return bookmark->GetID();
    case BRUrl:
        return bookmark->GetUrl();
    case BRTitle:
        return bookmark->GetTitle();
    case BRDescription:
        return bookmark->GetDescription();
    case BRImageUrl:
        return bookmark->GetImageUrl();
    case BRFavorite:
        return bookmark->IsFavorite();
    case BRRead:
        return bookmark->IsRead();
    case BRTags:
        return bookmark->GetTags().join(',');
    case BRAddTime:
        return bookmark->GetAddTime();
    case BRUpdateTime:
        return bookmark->GetUpdateTime();
    case BRStatus:
        return bookmark->GetStatus();
    case BRThumbnail:
        return bookmark->GetThumbnail();
    case BRHasContent:
        return bookmark->HasContent();
    case BRCoverImage:
        return bookmark->GetCoverImageUrl();
    case BRImages:
        return bookmark->GetImagesVar();
    case BRVideos:
        return bookmark->GetVideosVar();
    case BRContentType:
        return bookmark->GetContentType();
    case BRBookmark:
        return QVariant::fromValue(bookmark.get());
    case BRSelected:
        return m_SelectedBookmarksIds.contains(index.row());
    default:
        return QVariant();
    }
}

int BookmarksModel::rowCount(const QModelIndex&) const
{
    return m_Bookmarks.count();
}

QHash<int, QByteArray> BookmarksModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles [BRID] = "bookmarkID";
    roles [BRUrl] = "bookmarkUrl";
    roles [BRTitle] = "bookmarkTitle";
    roles [BRDescription] = "bookmarkDescription";
    roles [BRImageUrl] = "bookmarkImageUrl";
    roles [BRFavorite] = "bookmarkFavorite";
    roles [BRRead] = "bookmarkRead";
    roles [BRTags] = "bookmarkTags";
    roles [BRAddTime] = "bookmarkAddTime";
    roles [BRUpdateTime] = "bookmarkUpdateTime";
    roles [BRStatus] = "bookmarkStatus";
    roles [BRThumbnail] = "bookmarkThumbnail";
    roles [BRHasContent] = "bookmarkHasContent";
    roles [BRCoverImage] = "bookmarkCoverImage";
    roles [BRImages] = "bookmarkImages";
    roles [BRVideos] = "bookmarkVideos";
    roles [BRContentType] = "bookmarkContentType";
    roles [BRBookmark] = "bookmarkBookmark";
    roles [BRSelected] = "bookmarkSelected";

    return roles;
}

void BookmarksModel::Clear()
{
    beginResetModel();
    m_Bookmarks.clear();
    endResetModel();
    emit countChanged();
}

void BookmarksModel::RemoveBookmarks(const QStringList& ids)
{
    for (const auto& id : ids)
    {
        auto it = std::find_if(m_Bookmarks.begin(), m_Bookmarks.end(),
                [id](decltype(m_Bookmarks.front()) bookmark)
                {
                    return id == bookmark->GetID();
                });
        if(it != m_Bookmarks.end())
        {
            const int pos = std::distance(m_Bookmarks.begin(), it);
            beginRemoveRows(QModelIndex(), pos, pos);
            m_Bookmarks.removeAt(pos);
            endRemoveRows();
        }
    }
    emit countChanged();
}

void BookmarksModel::AddBookmarks(const Bookmarks_t& bookmarks)
{
    Bookmarks_t bmss = bookmarks;
    for(int i = bmss.count() - 1; i >= 0; --i)
    {
        auto bms = bmss.at(i);
        auto it = std::find_if(m_Bookmarks.begin(), m_Bookmarks.end(),
                [bms](decltype(m_Bookmarks.front()) bookmark)
                {
                    return bms->GetID() == bookmark->GetID();
                });
        if (it != m_Bookmarks.end())
        {
            const int pos = std::distance(m_Bookmarks.begin(), it);
            switch(bms->GetStatus())
            {
            case Bookmark::SDeleted:
                RemoveBookmarks({ bms->GetID() });
                break;
            case Bookmark::SArchived:
            {
                (*it)->SetIsRead(true);
                emit dataChanged(index(pos), index(pos));
                break;
            }
            default:
            {
                (*it)->SetUrl(bms->GetUrl());
                (*it)->SetTitle(bms->GetTitle());
                (*it)->SetDescription(bms->GetDescription());
                (*it)->SetIsFavorite(bms->IsFavorite());
                (*it)->SetIsRead(bms->IsRead());
                (*it)->SetAddTime(bms->GetAddTime());
                (*it)->SetUpdateTime(bms->GetUpdateTime());
                (*it)->SetTags(bms->GetTags());
                (*it)->SetImageUrl(bms->GetImageUrl());
                (*it)->SetStatus(bms->GetStatus());
                (*it)->SetImages(bms->GetImages());
                (*it)->SetVideos(bms->GetVideos());
                (*it)->SetContentType(bms->GetContentType());

                emit dataChanged(index(pos), index(pos));
                break;
            }
            }
        }
        else if (bms->GetStatus() != Bookmark::SDeleted)
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_Bookmarks.append(bms);
            endInsertRows();
        }
    }
    emit countChanged();
}

void BookmarksModel::SetBookmarks(const Bookmarks_t& bookmarks)
{
    beginResetModel();
    m_Bookmarks = bookmarks;
    endResetModel();
    emit countChanged();
}

void BookmarksModel::MarkBookmarksAsFavorite(const QStringList& ids, bool favorite)
{
    for (const auto& id : ids)
    {
        auto it = std::find_if(m_Bookmarks.begin(), m_Bookmarks.end(),
                [id](decltype(m_Bookmarks.front()) bookmark)
                {
                    return bookmark->GetID() == id;
                });
        if (it != m_Bookmarks.end())
        {
            const int pos = std::distance(m_Bookmarks.begin(), it);
            (*it)->SetIsFavorite(favorite);
            emit dataChanged(index(pos, 0), index(pos, 0));
        }
    }
}

void BookmarksModel::MarkBookmarksAsRead(const QStringList& ids, bool read)
{
    for (const auto& id : ids)
    {
        auto it = std::find_if(m_Bookmarks.begin(), m_Bookmarks.end(),
                [id](decltype(m_Bookmarks.front()) bookmark)
                {
                    return bookmark->GetID() == id;
                });
        if (it != m_Bookmarks.end())
        {
            const int pos = std::distance(m_Bookmarks.begin(), it);
            (*it)->SetIsRead(read);
            emit dataChanged(index(pos, 0), index(pos, 0));
        }
    }
}

void BookmarksModel::UpdateTags(const QString& id, const QString& tags)
{
    for(int i = 0, size = m_Bookmarks.count(); i < size; ++i)
    {
        auto& bm = m_Bookmarks[i];
        if(bm->GetID() == id)
        {
            bm->SetTags(tags.split(',', QString::SkipEmptyParts));
            emit dataChanged(index(i, 0), index(i, 0));
            break;
        }
    }
}

void BookmarksModel::UpdatePublishDate(const QString& id, const QString& date)
{
    for(int i = 0, size = m_Bookmarks.count(); i < size; ++i)
    {
        auto& bm = m_Bookmarks[i];
        if(bm->GetID() == id)
        {
            bm->SetPublishedDate(date);
            emit dataChanged(index(i, 0), index(i, 0));
            break;
        }
    }
}

void BookmarksModel::RefreshBookmark(const QString& id)
{
    auto it = std::find_if(m_Bookmarks.begin(), m_Bookmarks.end(),
            [id](decltype(m_Bookmarks.front()) bookmark)
            {
                return bookmark->GetID() == id;
            });
    if (it != m_Bookmarks.end())
    {
        const int pos = std::distance(m_Bookmarks.begin(), it);
        emit dataChanged(index(pos), index(pos));
    }
}

Bookmarks_t BookmarksModel::GetBookmarks() const
{
    return m_Bookmarks;
}

void BookmarksModel::SelectBookmark(const QModelIndex& index)
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_Bookmarks.count()) {
        return;
    }

    const auto bookmark = m_Bookmarks[idx];
    if (!m_SelectedBookmarksIds.contains(idx)) {
        m_SelectedBookmarksIds.insert(idx, bookmark->GetID());
        dataChanged(index, index, { BRSelected });
    }
}

void BookmarksModel::DeselectBookmark(const QModelIndex& index)
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_Bookmarks.count()) {
        return;
    }

    if (m_SelectedBookmarksIds.contains(idx)) {
        m_SelectedBookmarksIds.remove(idx);
        dataChanged(index, index, { BRSelected });
    }
}

void BookmarksModel::SelectAllBookmarks()
{
}

void BookmarksModel::DeselectAllBookmarks()
{
    m_SelectedBookmarksIds.clear();
    emit dataChanged(index(0), index(m_Bookmarks.count() - 1), { BRSelected });
}

QStringList BookmarksModel::GetSelectedBookmarks() const
{
    return m_SelectedBookmarksIds.values();
}

void BookmarksModel::handleArticlesCacheReset()
{
    beginResetModel();
    endResetModel();
}
} // namespace LinksBag
