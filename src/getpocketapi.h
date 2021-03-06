﻿/*
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

#include <QObject>
#include <QJsonDocument>
#include <QNetworkReply>

#include "src/enumsproxy.h"
#include "src/bookmark.h"

class QNetworkAccessManager;

namespace LinksBag
{
class GetPocketApi : public QObject
{
    Q_OBJECT

    const QString m_ConsumerKey;
    static const int m_InvalidRequestError = 400;
    static const int m_AuthError = 401;
    static const int m_PermissionsRateError = 403;
    static const int m_MaintenanceError = 503;

    QNetworkAccessManager *m_NAM;
    QString m_RequestToken;

public:
    explicit GetPocketApi(QObject *parent = 0);

    void ObtainRequestToken();

    void RequestAccessToken();

    void LoadBookmarks(int lastUpdate);
    void RemoveBookmarks(const QStringList& ids);
    void MarkBookmarksAsFavorite(const QStringList& ids, bool favorite);
    void MarkBookmarksAsRead(const QStringList& ids, bool read);
    void UpdateTags(const QString& id, const QString& tags);

    void ResetAccount();
private:
    QJsonDocument PreparsingReply(QObject *sender, bool& ok);

private slots:
    void handleObtainRequestToken();
    void handleRequestAccessToken();

    void handleLoadBookmarks();
    void handleRemoveBookmarks(const QStringList& ids);
    void handleMarkBookmarksAsFavorite(const QStringList& id, bool favorite);
    void handleMarkBookmarksAsRead(const QStringList& id, bool read);
    void handleTagsUpdated(const QString& id, const QString& tags);

signals:
    void requestFinished(bool success, const QString& errorMsg = QString());
    void error(const QString& msg, int code = 0, ErrorType type = ETGeneral);

    void requestTokenChanged(const QString& requestToken);

    void logged(bool logged, const QString& accessToken, const QString& userName);

    void gotBookmarks(const Bookmarks_t& bookmarks, quint64 since);
    void bookmarksRemoved(const QStringList& ids);
    void bookmarksMarkedAsFavorite(const QStringList& ids, bool favorite);
    void bookmarksMarkedAsRead(const QStringList& ids, bool read);
    void tagsUpdated(const QString& id, const QString& tags);
};
} // namespace LinksBag
