/*
The MIT License (MIT)

Copyright (c) 2014-2018 Oleg Linkin <maledictusdemagog@gmail.com>

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

#pragma once

#include <QSortFilterProxyModel>
#include <QSet>

#include "enumsproxy.h"

namespace LinksBag
{
class BookmarksModel;

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    BookmarksModel *m_BookmarksModel;
    BookmarksStatusFilter m_StatusFilter;
    BookmarksContentTypeFilter m_ContentTypeFilter;

    QList<int> m_UnreadSelectedBookmarksIds;
    QList<int> m_UnfavoriteSelectedBookmarksIds;
    Q_PROPERTY(bool unreadBookmarksSelected READ IsUnreadBookmarksSelected NOTIFY unreadBookmarksSelectedChanged)
    Q_PROPERTY(bool unfavoriteBookmarksSelected READ IsUnfavoriteBookmarksSelected NOTIFY unfavoriteBookmarksSelectedChanged)
public:
    explicit FilterProxyModel(BookmarksModel *bookmarksModel, QObject *parent = 0);

    virtual bool filterAcceptsRow (int sourceRow, const QModelIndex& sourceParent) const;
    virtual bool lessThan (const QModelIndex& left, const QModelIndex& right) const;

    bool IsUnreadBookmarksSelected() const;
    bool IsUnfavoriteBookmarksSelected() const;
    Q_INVOKABLE QStringList selectedBookmarks() const;
public slots:
    void filterBookmarks(int statusFilter, int contentTypeFilter);

    void selectBookmark(int row);
    void deselectBookmark(int row);
    void selectAllBookmarks();
    void deselectAllBookmarks();

signals:
    void unreadBookmarksSelectedChanged();
    void unfavoriteBookmarksSelectedChanged();
};
} // namespace LinksBag
