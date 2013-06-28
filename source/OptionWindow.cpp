/*
 * Copyright (C) 2013 - The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
