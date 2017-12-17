/*
The MIT License(MIT)

Copyright(c) 2016 Oleg Linkin <maledictusdemagog@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
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

#include <memory>

#include <QObject>
#include <QVariantMap>

namespace LinksBag
{
class BookmarksModel;
class FilterProxyModel;
class GetPocketApi;

class LinksBagManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LinksBagManager)

    std::shared_ptr<GetPocketApi> m_Api;

    bool m_IsBusy;
    bool m_IsLogged;

    QString m_RequesToken;
    QString m_UserName;
    QString m_Filter;
    bool m_Authorized;
    BookmarksModel *m_BookmarksModel;
    FilterProxyModel *m_FilterProxyModel;

    Q_PROPERTY(bool busy READ GetBusy NOTIFY busyChanged)
    Q_PROPERTY(bool logged READ GetLogged NOTIFY loggedChanged)
    Q_PROPERTY(BookmarksModel* bookmarksModel READ GetBookmarksModel
            NOTIFY bookmarksModelChanged)
    Q_PROPERTY(FilterProxyModel* filterModel READ GetFilterModel
            NOTIFY filterModelChanged)

    explicit LinksBagManager(QObject *parent = 0);
public:
    static LinksBagManager* Instance(QObject *parent = 0);
    bool GetBusy() const;
    bool GetLogged() const;

    BookmarksModel* GetBookmarksModel() const;
    FilterProxyModel* GetFilterModel() const;

    void SaveBookmarks();
private:
    void MakeConnections();
    void SetBusy(const bool busy);
    void SetLogged(const bool logged);

public slots:
    void obtainRequestToken();
    void requestAccessToken();

    void filterBookmarks(const QString& text);

    void loadBookmarksFromCache();
    void refreshBookmarks();
    void removeBookmark(const QString& id);
    void markAsFavorite(const QString& id, bool favorite);
    void markAsRead(const QString& id, bool read);
    void updateTags(const QString& id, const QString& tags);

signals:
    void busyChanged();
    void loggedChanged();
    void requestTokenChanged(const QString& requestToken);

    void bookmarksModelChanged();
    void filterModelChanged();

    void bookmarkReadStateChanged(const QString& id, bool readState);
    void bookmarkFavoriteStateChanged(const QString& id, bool favoriteState);

    void error(const QString& msg, int type);
    void notify(const QString& msg);
};
} // namespace LinskBag
