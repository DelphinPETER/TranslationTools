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

#include "OptionWindow.h"
#include "ui_OptionWindow.h"

OptionWindow::OptionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

void OptionWindow::set_Editor(QString editor)
{
    m_editor = editor;
    ui->lineEdit_EditorCommand->setText(m_editor);
}

void OptionWindow::on_buttonBox_accepted()
{
    m_editor = ui->lineEdit_EditorCommand->text();
}

QString OptionWindow::get_New()
{
    return m_editor;
}
