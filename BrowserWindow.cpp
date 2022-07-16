/*
 * Copyright (c) 2022, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2022, Matthew Costa <ucosty@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "BrowserWindow.h"
#include "WebView.h"
#include <LibCore/EventLoop.h>
#include <QAction>
#include <QPlainTextEdit>

extern String s_serenity_resource_root;

BrowserWindow::BrowserWindow(Core::EventLoop& event_loop)
    : m_event_loop(event_loop)
{
    m_tabs_container = new QTabWidget;
    m_tabs_container->setElideMode(Qt::TextElideMode::ElideRight);
    m_tabs_container->setMovable(true);
    m_tabs_container->setTabsClosable(true);

    m_tabs_bar = m_tabs_container->findChild<QTabBar*>();
    m_tabs_bar->hide();

    auto* menu = menuBar()->addMenu("&File");

    auto* new_tab_action = new QAction("New &Tab");
    new_tab_action->setIcon(QIcon(QString("%1/res/icons/16x16/new-tab.png").arg(s_serenity_resource_root.characters())));
    new_tab_action->setShortcut(QKeySequence(QKeySequence::AddTab)); // "Ctrl+N"
    menu->addAction(new_tab_action);

    auto* close_current_tab_action = new QAction("&Close Tab");
    close_current_tab_action->setIcon(QIcon(QString("%1/res/icons/16x16/close-tab.png").arg(s_serenity_resource_root.characters())));
    close_current_tab_action->setShortcut(QKeySequence(QKeySequence::Close));
    menu->addAction(close_current_tab_action);

    auto* quit_action = new QAction("&Quit");
    quit_action->setShortcut(QKeySequence("Ctrl+Q"));
    menu->addAction(quit_action);

    auto* edit_menu = menuBar()->addMenu("&Edit");

    auto* cut_action = new QAction("Cu&t");
    cut_action->setIcon(QIcon(QString("%1/res/icons/16x16/edit-cut.png").arg(s_serenity_resource_root.characters())));
    cut_action->setShortcut(QKeySequence(QKeySequence::Cut)); // "Ctrl+X"
    cut_action->setEnabled(false);
    edit_menu->addAction(cut_action);

    auto* copy_action = new QAction("&Copy");
    copy_action->setIcon(QIcon(QString("%1/res/icons/16x16/edit-copy.png").arg(s_serenity_resource_root.characters())));
    copy_action->setShortcut(QKeySequence(QKeySequence::Copy)); // "Ctrl+C"
    copy_action->setEnabled(false);
    edit_menu->addAction(copy_action);

    auto* paste_action = new QAction("&Paste");
    paste_action->setIcon(QIcon(QString("%1/res/icons/16x16/paste.png").arg(s_serenity_resource_root.characters())));
    paste_action->setShortcut(QKeySequence(QKeySequence::Paste)); // "Ctrl+V"
    paste_action->setEnabled(false);
    edit_menu->addAction(paste_action);

    edit_menu->addSeparator();

    auto* select_all_action = new QAction("Select &All");
    select_all_action->setIcon(QIcon(QString("%1/res/icons/16x16/select-all.png").arg(s_serenity_resource_root.characters())));
    select_all_action->setShortcut(QKeySequence(QKeySequence::SelectAll)); // "Ctrl+A"
    select_all_action->setEnabled(false);
    edit_menu->addAction(select_all_action);

    auto* view_menu = menuBar()->addMenu("&View");

    auto* view_source_action = new QAction("View &Source");
    view_source_action->setIcon(QIcon(QString("%1/res/icons/16x16/filetype-html.png").arg(s_serenity_resource_root.characters())));
    view_source_action->setShortcut(QKeySequence("Ctrl+U"));
    view_menu->addAction(view_source_action);
    QObject::connect(view_source_action, &QAction::triggered, this, [this] {
        if (m_current_tab) {
            auto source = m_current_tab->view().source();

            auto* text_edit = new QPlainTextEdit;
            text_edit->setFont(QFontDatabase::systemFont(QFontDatabase::SystemFont::FixedFont));
            text_edit->resize(800, 600);
            text_edit->setPlainText(source.characters());
            text_edit->show();
        }
    });

    auto* go_menu = menuBar()->addMenu("&Go");

    auto* back_action = new QAction("Back");
    back_action->setIcon(QIcon(QString("%1/res/icons/16x16/go-back.png").arg(s_serenity_resource_root.characters())));
    back_action->setShortcut(QKeySequence(QKeySequence::Back)); // "Alt+Left"
    back_action->setEnabled(false);
    go_menu->addAction(back_action);

    auto* forward_action = new QAction("Forward");
    forward_action->setIcon(QIcon(QString("%1/res/icons/16x16/go-forward.png").arg(s_serenity_resource_root.characters())));
    forward_action->setShortcut(QKeySequence(QKeySequence::Forward)); // "Alt+Right"
    forward_action->setEnabled(false);
    go_menu->addAction(forward_action);

    auto* home_action = new QAction("Home");
    home_action->setIcon(QIcon(QString("%1/res/icons/16x16/go-home.png").arg(s_serenity_resource_root.characters())));
    home_action->setEnabled(false);
    go_menu->addAction(home_action);

    auto* options_menu = menuBar()->addMenu("&Options");

    auto* dump_dom_tree_action = new QAction("Dump DOM Tree");
    dump_dom_tree_action->setIcon(QIcon(QString("%1/res/icons/browser/dom-tree.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_dom_tree_action);
    QObject::connect(dump_dom_tree_action, &QAction::triggered, this, [this] {
        debug_request("dump-dom-tree");
    });

    auto* dump_layout_tree_action = new QAction("Dump Layout Tree");
    dump_layout_tree_action->setIcon(QIcon(QString("%1/res/icons/16x16/layout.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_layout_tree_action);
    QObject::connect(dump_layout_tree_action, &QAction::triggered, this, [this] {
        debug_request("dump-layout-tree");
    });

    auto* dump_stacking_context_tree_action = new QAction("Dump Stacking Context Tree");
    dump_stacking_context_tree_action->setIcon(QIcon(QString("%1/res/icons/16x16/layers.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_stacking_context_tree_action);
    QObject::connect(dump_stacking_context_tree_action, &QAction::triggered, this, [this] {
        debug_request("dump-stacking-context-tree");
    });

    auto* dump_style_sheets_action = new QAction("Dump Style Sheets");
    dump_style_sheets_action->setIcon(QIcon(QString("%1/res/icons/16x16/filetype-css.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_style_sheets_action);
    QObject::connect(dump_style_sheets_action, &QAction::triggered, this, [this] {
        debug_request("dump-style-sheets");
    });

    auto* dump_history_action = new QAction("Dump History");
    dump_history_action->setIcon(QIcon(QString("%1/res/icons/16x16/history.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_history_action);
    QObject::connect(dump_history_action, &QAction::triggered, this, [this] {
        debug_request("dump-history");
    });

    auto* dump_cookies_action = new QAction("Dump Cookies");
    dump_cookies_action->setIcon(QIcon(QString("%1/res/icons/browser/cookie.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_cookies_action);
    QObject::connect(dump_cookies_action, &QAction::triggered, this, [this] {
        debug_request("dump-cookies");
    });

    auto* dump_local_storage_action = new QAction("Dump Local Storage");
    dump_local_storage_action->setIcon(QIcon(QString("%1/res/icons/browser/local-storage.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(dump_local_storage_action);
    QObject::connect(dump_local_storage_action, &QAction::triggered, this, [this] {
        debug_request("dump-local-storage");
    });

    options_menu->addSeparator();

    auto* show_line_box_borders_action = new QAction("Show Line Box Borders");
    show_line_box_borders_action->setCheckable(true);
    options_menu->addAction(show_line_box_borders_action);
    QObject::connect(show_line_box_borders_action, &QAction::triggered, this, [this, show_line_box_borders_action] {
        bool state = show_line_box_borders_action->isChecked();
        debug_request("set-line-box-borders", state ? "on" : "off");
    });

    options_menu->addSeparator();

    auto* collect_garbage_action = new QAction("Collect Garbage!!");
    collect_garbage_action->setIcon(QIcon(QString("%1/res/icons/16x16/trash-can.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(collect_garbage_action);
    QObject::connect(collect_garbage_action, &QAction::triggered, this, [this] {
        debug_request("collect-garbage");
    });

    auto* clear_cache_action = new QAction("Clear Cache");
    clear_cache_action->setIcon(QIcon(QString("%1/res/icons/browser/clear-cache.png").arg(s_serenity_resource_root.characters())));
    options_menu->addAction(clear_cache_action);
    QObject::connect(clear_cache_action, &QAction::triggered, this, [this] {
        debug_request("clear-cache");
    });

    options_menu->addSeparator();

    auto* enable_scripting_action = new QAction("Enable Scripting");
    enable_scripting_action->setCheckable(true);
    enable_scripting_action->setChecked(true);
    options_menu->addAction(enable_scripting_action);
    QObject::connect(enable_scripting_action, &QAction::triggered, this, [this, enable_scripting_action] {
        bool state = enable_scripting_action->isChecked();
        debug_request("scripting", state ? "on" : "off");
    });

    auto* enable_same_origin_policy_action = new QAction("Enable Same-Origin Policy");
    enable_same_origin_policy_action->setCheckable(true);
    options_menu->addAction(enable_same_origin_policy_action);
    QObject::connect(enable_same_origin_policy_action, &QAction::triggered, this, [this, enable_same_origin_policy_action] {
        bool state = enable_same_origin_policy_action->isChecked();
        debug_request("same-origin-policy", state ? "on" : "off");
    });

    auto* help_menu = menuBar()->addMenu("&Help");

    auto* help_action = new QAction("&Help");
    help_action->setIcon(QIcon(QString("%1/res/icons/16x16/app-help.png").arg(s_serenity_resource_root.characters())));
    help_action->setShortcut(QKeySequence(QKeySequence::HelpContents));
    help_action->setEnabled(false);
    help_menu->addAction(help_action);

    auto* about_action = new QAction("&About Coccinellidae");
    about_action->setIcon(QIcon(QString("../icons/app-coccinellidae.png")));
    about_action->setEnabled(false);
    help_menu->addAction(about_action);



    QObject::connect(new_tab_action, &QAction::triggered, this, &BrowserWindow::new_tab);
    QObject::connect(quit_action, &QAction::triggered, this, &QMainWindow::close);
    QObject::connect(m_tabs_container, &QTabWidget::currentChanged, [this](int index) {
        setWindowTitle(QString("%1 - Coccinellidae").arg(m_tabs_container->tabText(index)));
        setWindowIcon(m_tabs_container->tabIcon(index));
    });
    QObject::connect(m_tabs_container, &QTabWidget::tabCloseRequested, this, &BrowserWindow::close_tab);

    QObject::connect(close_current_tab_action, &QAction::triggered, this, &BrowserWindow::close_current_tab);

    new_tab();

    setCentralWidget(m_tabs_container);
    

}

void BrowserWindow::debug_request(String const& request, String const& argument)
{
    if (!m_current_tab)
        return;
    m_current_tab->debug_request(request, argument);
}

void BrowserWindow::new_tab()
{
    auto tab = make<Tab>(this);
    auto tab_ptr = tab.ptr();
    m_tabs.append(std::move(tab));

    if (m_current_tab == nullptr) {
        m_current_tab = tab_ptr;
    }

    m_tabs_container->addTab(tab_ptr, "New Tab");
    m_tabs_container->setCurrentWidget(tab_ptr);

    QObject::connect(tab_ptr, &Tab::title_changed, this, &BrowserWindow::tab_title_changed);
    QObject::connect(tab_ptr, &Tab::favicon_changed, this, &BrowserWindow::tab_favicon_changed);

    if (m_tabs_container->count() > 1)
        m_tabs_bar->show();
}

void BrowserWindow::close_tab(int index)
{
    auto* tab = m_tabs_container->widget(index);
    m_tabs_container->removeTab(index);
    m_tabs.remove_first_matching([&](auto& entry) {
        return entry == tab;
    });

    if (m_tabs_container->count() <= 1)
        m_tabs_bar->hide();
}

void BrowserWindow::close_current_tab()
{
    auto count = m_tabs_container->count() - 1;
    if (!count)
        close();
    else
        close_tab(m_tabs_container->currentIndex());
}

int BrowserWindow::tab_index(Tab* tab)
{
    return m_tabs_container->indexOf(tab);
}

void BrowserWindow::tab_title_changed(int index, QString const& title)
{
    if (title.isEmpty()) {
        m_tabs_container->setTabText(index, "...");
        setWindowTitle("Coccinellidae");
    } else {
        m_tabs_container->setTabText(index, title);
        setWindowTitle(QString("%1 - Coccinellidae").arg(title));
    }
}

void BrowserWindow::tab_favicon_changed(int index, QIcon icon)
{
    m_tabs_container->setTabIcon(index, icon);
    setWindowIcon(icon);
}

void BrowserWindow::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);

    // FIXME: Coccinellidae only supports one window at the moment. When we support
    //        multiple windows, we'll only want to fire off the quit event when
    //        all of the browser windows have closed.
    m_event_loop.quit(0);
}
