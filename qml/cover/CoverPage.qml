/*
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

import QtQuick 2.0
import Sailfish.Silica 1.0

Cover {
    id: coverPage

    property alias model: articleList.model
    property alias image: article.image
    property alias title: article.title
    property alias currentFilter: articleList.filter
    property bool articleLayout: false

    anchors.fill: parent
    transparent: true

    BackgroundItem {
        anchors.fill: parent
        Image {
            id: coverBgImage
            fillMode: Image.PreserveAspectCrop
            anchors.fill: parent
            source: "qrc:/images/linksbag-cover"
            opacity: 0.1
        }
    }

    ArticleCover {
        id: article
        visible: articleLayout
    }

    ArticleListCover {
        id: articleList
        anchors.bottomMargin: coverActionArea.height;
        visible: !articleLayout
    }

    CoverActionList {
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                linksbagManager.refreshBookmarks()
            }
        }
    }
}
