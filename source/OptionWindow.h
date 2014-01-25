/*
 * Copyright (C) 2013 - Delphin PETER - delphinpeter@gmail.com
 *
 *   This file is part of TranslationTools.
 *
 *   TranslationTools is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QDomDocument>

namespace Ui {
class OptionWindow;
}

class OptionWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionWindow(QWidget *parent = 0);
    ~OptionWindow();
    void set_Editor(QString editor);
    void set_customMode(bool customMode);
    void set_customSourcePath(QString customSourcePath);
    QString get_Editor();
    QString get_customSourcePath();
    bool get_customMode();
    
private slots:
    void on_buttonBox_accepted();
    void on_pushButton_OpenFolder_clicked();

private:
    Ui::OptionWindow *uiOptionWindow;
    QString m_editor;
    bool m_customMode;
    QString m_customSourcePath;
};

#endif
