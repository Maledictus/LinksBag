/*
The MIT License (MIT)

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

Item {
    property bool isBusy: true
    property string entry: ""
    property string date: ""
    property string bookmarkImage: ""

    function setArticle(article_url) {
        isBusy = true;
        makeRequest(article_url);
    }

    function makeRequest(url) {
        var doc = new XMLHttpRequest();
        entry = "";
        date = ""
        doc.onreadystatechange = function() {
            if (doc.readyState === XMLHttpRequest.DONE && doc.status == 200) {
                var json = JSON.parse(doc.responseText)
                var entry_text = json.content;
                if (entry_text) {
                    entry_text = entry_text.replace(new RegExp("\%20[0-9].*x[a-z]?", "g"), "");
                    entry_text = entry_text.replace(new RegExp("<img(?!\/)", "g"), "<img width='" +
                            (mainWindow.width - 2*Theme.horizontalPageMargin) + "'");
                    entry_text = entry_text.replace(bookmarkImage, "");
                    entry = entry_text;
                    date = json.date_published !== null ? json.date_published : ""
                }
                isBusy = false;
            }
            else if (doc.readyState === XMLHttpRequest.DONE) {
                entry = doc.statusText
                isBusy = false;
            }
        }

        doc.open("GET", "https://g5q08g5xqk.execute-api.us-east-1.amazonaws.com/prod/parser?url=" + encodeURIComponent(url));
        doc.setRequestHeader("Content-Type", "application/json");
        doc.setRequestHeader("x-api-key", "kmEINFMf17L8zYYZlrOzsfL6XaNXCMqd2gx7JxTT");
        doc.send();
    }
}
