/*
 * Copyright (c) 2022, Filiph Sandström <filiph.sandstrom@filfatstudios.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define AK_DONT_REPLACE_STD

#include <AK/String.h>
#include <QSettings>

namespace Browser {

class Settings {
public:
    Settings();

    QString homepage();
    void set_homepage(QString const& homepage);

    QString search_engine();
    void set_search_engine(QString const& search_engine);

private:
    QSettings* m_qsettings;
};

}
