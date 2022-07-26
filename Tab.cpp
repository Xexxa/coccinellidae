/*
 * Copyright (c) 2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2022, Matthew Costa <ucosty@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Tab.h"
#include "BrowserWindow.h"
#include "Settings.h"
#include "History.h"
#include <QCoreApplication>
#include <QStatusBar>
#include <QLabel>

extern String s_serenity_resource_root;
extern Browser::Settings* s_settings;

Tab::Tab(QMainWindow* window)
    : m_window(window)
{
    m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_view = new WebView;
    m_toolbar = new QToolBar;
    m_toolbar_buttons = new QToolBar;
    m_toolbar_buttons->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_location_edit = new QLineEdit;
    m_search_edit = new QLineEdit;
    m_search_edit->setFixedWidth(150);
    m_search_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

    auto* focus_location_edit_action = new QAction("Edit Location");
    focus_location_edit_action->setShortcut(QKeySequence("Ctrl+L"));
    addAction(focus_location_edit_action);

    auto* focus_search_edit_action = new QAction("Edit Location");
    focus_search_edit_action->setShortcut(QKeySequence("Ctrl+K"));
    addAction(focus_search_edit_action);

    m_layout->addWidget(m_toolbar_buttons);
    m_layout->addWidget(m_toolbar);
    m_layout->addSpacing(5);
    m_layout->addWidget(m_view);

    QFont button_text_font;
    button_text_font.setPointSize(9);

    auto back_icon_path = QString("../icons/back.png");
    auto forward_icon_path = QString("../icons/forward.png");
    auto home_icon_path = QString("../icons/home.png");
    //auto reload_icon_path = QString("%1/res/icons/16x16/reload.png").arg(s_serenity_resource_root.characters());
    auto reload_icon_path = QString("../icons/reload.png");
    auto break_cache_icon_path = QString("../icons/break-cache.png");
    auto open_icon_path = QString("../icons/open.png");
    auto print_icon_path = QString("../icons/print.png");
    auto find_icon_path = QString("../icons/find.png");
    auto stop_icon_path = QString("../icons/stop.png");
    m_back_action = make<QAction>(QIcon(back_icon_path), "Back");
    m_back_action->setShortcut(QKeySequence(QKeySequence::Back)); // "Alt+Left"
    m_back_action->setFont(button_text_font);
    m_back_action->setEnabled(false);
    m_forward_action = make<QAction>(QIcon(forward_icon_path), "Forward");
    m_forward_action->setShortcut(QKeySequence(QKeySequence::Forward)); // "Alt+Right"
    m_forward_action->setFont(button_text_font);
    m_forward_action->setEnabled(false);
    m_home_action = make<QAction>(QIcon(home_icon_path), "Home");
    m_home_action->setFont(button_text_font);
    m_reload_action = make<QAction>(QIcon(reload_icon_path), "Reload");
    m_reload_action->setShortcut(QKeySequence(QKeySequence::Refresh)); // "Ctrl+R"
    m_reload_action->setFont(button_text_font);
    m_break_cache_action = make<QAction>(QIcon(break_cache_icon_path), "Break cache");
    m_break_cache_action->setFont(button_text_font);
    m_open_action = make<QAction>(QIcon(open_icon_path), "Open");
    m_open_action->setEnabled(false);
    m_open_action->setFont(button_text_font);
    m_print_action = make<QAction>(QIcon(print_icon_path), "Print");
    m_print_action->setEnabled(false);
    m_print_action->setFont(button_text_font);
    m_find_action = make<QAction>(QIcon(find_icon_path), "Find");
    m_find_action->setEnabled(false);
    m_find_action->setFont(button_text_font);
    m_stop_action = make<QAction>(QIcon(stop_icon_path), "Stop");
    m_stop_action->setEnabled(false);
    m_stop_action->setFont(button_text_font);





    auto m_location_label = new QLabel("Location: ");
    auto m_search_label = new QLabel("  Search: ");

    m_toolbar_buttons->addAction(m_back_action);
    m_toolbar_buttons->addAction(m_forward_action);
    m_toolbar_buttons->addAction(m_home_action);
    m_toolbar_buttons->addAction(m_reload_action);
    m_toolbar_buttons->addAction(m_break_cache_action);
    m_toolbar_buttons->addAction(m_open_action);
    m_toolbar_buttons->addAction(m_print_action);
    m_toolbar_buttons->addAction(m_find_action);
    m_toolbar_buttons->addAction(m_stop_action);

    m_toolbar->addWidget(m_location_label);
    m_toolbar->addWidget(m_location_edit);
    m_toolbar->addWidget(m_search_label);
    m_toolbar->addWidget(m_search_edit);

    QObject::connect(m_view, &WebView::linkUnhovered, m_window->statusBar(), &QStatusBar::clearMessage);

    QObject::connect(m_view, &WebView::loadStarted, [this](const URL& url) {
        m_location_edit->setText(url.to_string().characters());
        m_history.push(url, m_title.toUtf8().data());

        // Update buttons everytime on navigation!
        if (!m_history.can_go_back()) {
            m_back_action->setEnabled(false);
        } else {
            m_back_action->setEnabled(true);
        }

        if (!m_history.can_go_forward()) {
            m_forward_action->setEnabled(false);
        } else {
            m_forward_action->setEnabled(true);
        }
    });

    QObject::connect(m_location_edit, &QLineEdit::returnPressed, this, &Tab::location_edit_return_pressed);
    QObject::connect(m_search_edit, &QLineEdit::returnPressed, this, &Tab::search_edit_return_pressed);
    QObject::connect(m_view, &WebView::title_changed, this, &Tab::page_title_changed);
    QObject::connect(m_view, &WebView::favicon_changed, this, &Tab::page_favicon_changed);

    QObject::connect(m_back_action, &QAction::triggered, this, &Tab::back);
    QObject::connect(m_forward_action, &QAction::triggered, this, &Tab::forward);
    QObject::connect(m_home_action, &QAction::triggered, this, &Tab::home);
    QObject::connect(m_reload_action, &QAction::triggered, this, &Tab::reload);
    QObject::connect(focus_location_edit_action, &QAction::triggered, m_location_edit, qOverload<>(&QWidget::setFocus));
    QObject::connect(focus_location_edit_action, &QAction::triggered, m_location_edit, &QLineEdit::selectAll);
    QObject::connect(focus_search_edit_action, &QAction::triggered, m_search_edit, qOverload<>(&QWidget::setFocus));
    QObject::connect(focus_search_edit_action, &QAction::triggered, m_search_edit, &QLineEdit::selectAll);
}

void Tab::navigate(QString url)
{
    if (!url.startsWith("http://", Qt::CaseInsensitive) &&
        !url.startsWith("https://", Qt::CaseInsensitive) &&
        !url.startsWith("file://", Qt::CaseInsensitive))
        url = "https://" + url;
    view().load(url.toUtf8().data());
}

void Tab::back()
{
    if (!m_history.can_go_back())
        return;

    m_history.go_back();
    view().load(m_history.current().url.to_string());
}

void Tab::forward()
{
    if (!m_history.can_go_forward())
        return;

    m_history.go_forward();
    view().load(m_history.current().url.to_string());
}

void Tab::home()
{
    //navigate("https://home.coccinellidae.serenityos.net");
    navigate(s_settings->homepage());
}

void Tab::reload()
{
    view().reload();
}

void Tab::location_edit_return_pressed()
{
    navigate(m_location_edit->text());
}

void Tab::search_edit_return_pressed()
{
    QString user_input = m_search_edit->text();
    user_input.prepend("https://search.brave.com/search?q=");

    /*
     * Bing https://www.bing.com/search?q={}
     * Brave https://search.brave.com/search?q={} (Default)
     * Coccinellidae SerenityOS Search https://search.coccinellidae.serenityos.net/search?q=
     * DuckDuckGo https://duckduckgo.com/?q={}
     * FrogFind https://frogfind.com/?q={}
     * GitHub https://github.com/search?q={}
     * Google https://google.com/search?q={}
     * Mojeek https://www.mojeek.com/search?q={}
     * Yandex https://yandex.com/search/?text={}
     */

    navigate(user_input);
}

void Tab::page_title_changed(QString title)
{
    m_title = title;
    emit title_changed(tab_index(), std::move(title));
}

void Tab::page_favicon_changed(QIcon icon)
{
    emit favicon_changed(tab_index(), std::move(icon));
}

int Tab::tab_index()
{
    // FIXME: I hear you like footguns...
    //        There has to be a better way of doing this
    auto browser_window = reinterpret_cast<BrowserWindow*>(m_window);
    return browser_window->tab_index(this);
}

void Tab::debug_request(String const& request, String const& argument)
{
    m_view->debug_request(request, argument);
}
