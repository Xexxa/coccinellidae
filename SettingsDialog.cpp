/*
 * Copyright (c) 2022, Filiph Sandström <filiph.sandstrom@filfatstudios.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Settings.h"
#include "SettingsDialog.h"
#include <QCloseEvent>
#include <QLabel>
#include <QComboBox>

extern Browser::Settings* s_settings;

SettingsDialog::SettingsDialog(QMainWindow* window)
    : m_window(window)
{
    m_layout = new QFormLayout;
    m_homepage = new QLineEdit;
    m_search_engine = new QComboBox; // FIXME: Saved search engine should be preselected in QComboBox
    m_ok_button = new QPushButton("&Save");

    m_layout->addWidget(new QLabel("Homepage"));
    m_layout->addWidget(m_homepage);
    m_layout->addWidget(new QLabel("Search engine"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-bing.png")), "Bing", QVariant("bing"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-brave.png")), "Brave (Default)", QVariant("brave"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/app-coccinellidae.png")), "Coccinellidae SerenityOS Search", QVariant("coccinellidae-serenityos-search"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-duckduckgo.png")), "DuckDuckGo", QVariant("duckduckgo"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-frogfind.png")), "FrogFind", QVariant("frogfind"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-github.png")), "GitHub", QVariant("github"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-google.png")), "Google", QVariant("google"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-metager.png")), "metaGer", QVariant("metager"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-mojeek.png")), "Mojeek", QVariant("mojeek"));
    m_search_engine->addItem(QIcon(QString("../icons/16x16/website-yandex.png")), "Yandex", QVariant("yandex"));
    m_layout->addWidget(m_search_engine);
    // FIXME: Should add field for custom search engine.
    m_layout->addWidget(m_ok_button);

    m_homepage->setText(s_settings->homepage());

    QObject::connect(m_ok_button, &QPushButton::released, this, [this] {
        close();
    });
    
    setWindowTitle("Preferences");
    setFixedWidth(300);
    setLayout(m_layout);
    show();
    setFocus();
}

void SettingsDialog::closeEvent(QCloseEvent *event)
{
    save();
    event->accept();
}

void SettingsDialog::save()
{
    // FIXME: Validate data.
    s_settings->set_homepage(m_homepage->text());

    s_settings->set_search_engine(m_search_engine->currentData().toString());
}
