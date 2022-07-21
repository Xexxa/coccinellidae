/*
 * Copyright (c) 2022, Filiph Sandström <filiph.sandstrom@filfatstudios.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Settings.h"

namespace Browser {

Settings::Settings()
{
    m_qsettings = new QSettings("Serenity", "Coccinellidae");
}

QString Settings::homepage()
{
    return m_qsettings->value("homepage", "https://home.coccinellidae.serenityos.net").toString();
}

void Settings::set_homepage(QString const& homepage)
{
    m_qsettings->setValue("homepage", homepage);
}

}
