/*
 * Copyright (c) 2022, Filiph Sandström <filiph.sandstrom@filfatstudios.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <QFormLayout>
#include <QDialog>
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

#pragma once

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QMainWindow* window);
    
    void save();

    virtual void closeEvent(QCloseEvent*) override;

private:
    QFormLayout* m_layout;
    QPushButton* m_ok_button { nullptr };
    QLineEdit* m_homepage { nullptr };
    QComboBox* m_search_engine { nullptr };
    QMainWindow* m_window { nullptr };
};
