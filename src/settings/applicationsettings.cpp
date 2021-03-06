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

#include "applicationsettings.h"

#include <mlite5/MDConfGroup>

namespace LinksBag
{
ApplicationSettings::ApplicationSettings(QObject *parent)
: QObject(parent)
, m_ApplicationGroup(new MDConfGroup("/apps/harbour-linksbag", this))
{
}

ApplicationSettings* ApplicationSettings::Instance(QObject *parent)
{
    static ApplicationSettings *appSettings = nullptr;
    if (!appSettings)
    {
        appSettings = new ApplicationSettings(parent);
    }
    return appSettings;
}

QVariant ApplicationSettings::value(const QString& key, const QVariant& def) const
{
    return m_ApplicationGroup ? m_ApplicationGroup->value(key, def) : def;
}

void ApplicationSettings::setValue(const QString& key, const QVariant& value)
{
    if (m_ApplicationGroup)
    {
        m_ApplicationGroup->setValue(key, value);
    }
}

void ApplicationSettings::remove(const QString& key)
{
    setValue(key, QVariant());
}

void ApplicationSettings::removeAll()
{
    if (m_ApplicationGroup)
    {
        m_ApplicationGroup->clear();
    }
}
} // namespace LinksBag

