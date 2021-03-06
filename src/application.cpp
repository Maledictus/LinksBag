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

#include "application.h"

#include "sailfishapp.h"

#include <QQuickView>
#include <QQmlContext>
#include <QtDebug>
#include <QtQml>
#include <QStandardPaths>

#include "authserver.h"
#include "bookmarksmodel.h"
#include "enumsproxy.h"
#include "filterproxymodel.h"
#include "linksbagmanager.h"
#include "settings/applicationsettings.h"

namespace LinksBag
{
Application::Application(QObject *parent)
: QObject(parent)
, m_View(nullptr)
, m_AuthServer(new AuthServer(this))
{
    connect(m_AuthServer, &AuthServer::gotAuthAnswer,
            this, &Application::handleAuthAnswerGot);

    const QString settingsPath = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
            .filePath(QCoreApplication::applicationName()) + "/linksbag.conf";
    const bool migrate = ApplicationSettings::Instance(this)->value("settingsMigration", true).toBool();
    if (QFile(settingsPath).exists() && migrate) {
        QSettings settings (settingsPath, QSettings::IniFormat);
        ApplicationSettings::Instance(this)->setValue("lastUpdate", settings.value("last_update", 0));
        ApplicationSettings::Instance(this)->setValue("statusFilter", settings.value("bookmarks_filter", "all"));
        ApplicationSettings::Instance(this)->setValue("showSearchField", settings.value("show_search_field", false));
        ApplicationSettings::Instance(this)->setValue("accessToken", settings.value("access_token"));
        ApplicationSettings::Instance(this)->setValue("userName",  settings.value("user_name"));
        ApplicationSettings::Instance(this)->setValue("settingsMigration", false);
    }

    if (migrate) {
        QFile(settingsPath).remove();
    }
}

void Application::ShowUI()
{
    if (!m_View)
    {
        m_View = SailfishApp::createView();
        m_View->setTitle("LinksBag");
        m_View->rootContext()->setContextProperty("linksbagManager",
                LinksBagManager::Instance(this));

        const bool result = m_AuthServer->StartListening(QHostAddress::LocalHost, PORT);
        m_View->rootContext()->setContextProperty("authServerRunning", result);

        m_View->setSource(SailfishApp::pathTo("qml/harbour-linksbag.qml"));
        m_View->showFullScreen();

        connect(LinksBagManager::Instance(this), &LinksBagManager::loggedChanged,
                this, &Application::handleLogged);
    }
    else
    {
        m_View->raise();
        m_View->requestActivate();
    }
}

QString Application::GetPath(ApplicationDirectory subdirectory)
{
    QString cacheDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    switch (subdirectory) {
    case AppDataDirectory:
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    case CacheDirectory:
        return cacheDirectory;
    case CoverCacheDirectory:
        return cacheDirectory + "/covers/";
    case ThumbnailCacheDirectory:
        return cacheDirectory + "/thumbnails/";
    case ArticleCacheDirectory:
        return cacheDirectory + "/articles/";
    default:
        return "";
    }
}

void Application::handleAuthAnswerGot(const QString& data)
{
    LinksBagManager::Instance(this)->handleGotAuthAnswer(data);
}

void Application::handleLogged()
{
    if (m_AuthServer)
    {
        m_AuthServer->SendAnswer(LinksBagManager::Instance(this)->GetLogged() ?
                tr("Authorized") : tr("Not authorized"));
    }
}

void Application::start()
{
    qRegisterMetaType<BookmarksModel*>("BookmarksModel*");
    qRegisterMetaType<FilterProxyModel*>("FilterProxyModel*");
    qRegisterMetaType<Bookmark*>("Bookmark*");
    qRegisterMetaType<Bookmarks_t>("Bookmarks_t");
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qmlRegisterType<Bookmark>("harbour.linksbag", 1, 0, "Bookmark");
    qmlRegisterUncreatableType<LinksBagManager>("harbour.linksbag", 1, 0,
            "LinksBagManager", "LinksBagManager static uncreatable type");
    qmlRegisterUncreatableType<LinksBag::EnumsProxy>("harbour.linksbag", 1, 0,
            "LinksBag", "This exports otherwise unavailable \
                    LinksBag datatypes to QML");
    ShowUI();
}

void Application::handleAboutToQuit()
{
    LinksBagManager::Instance(this)->Stop();
    LinksBagManager::Instance(this)->saveBookmarks();
}
} // namespace LinksBag
