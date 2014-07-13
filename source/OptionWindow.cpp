/*
 * Copyright (C) 2014 - Delphin PETER - delphinpeter@gmail.com
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
    uiOptionWindow(new Ui::OptionWindow)
{
    uiOptionWindow->setupUi(this);

    //loading UI Translation
    setWindowTitle( UITranslation::menu_options);
    uiOptionWindow->label_EditorCommand->setText( UITranslation::editor_command );
    uiOptionWindow->label_optionCustomSourceTitle->setText( UITranslation::custom_source_title );
    uiOptionWindow->groupBox_customSource->setTitle( UITranslation::use_custom_source );
    uiOptionWindow->label_TipsOptions->setText( UITranslation::tips_options );
    uiOptionWindow->pushButton_OpenFolder->setText( UITranslation::button_open_text );

    //Load user configuration
    QSettings settings("TranslationTools", "config");
    uiOptionWindow->lineEdit_EditorCommand->setText( settings.value("editor").toString() );
    uiOptionWindow->groupBox_customSource->setChecked( settings.value("custom_mode").toBool() );
    uiOptionWindow->lineEdit_optionCustomSourceValue->setText( settings.value("custom_source_path").toString() );

}

OptionWindow::~OptionWindow()
{
    delete uiOptionWindow;
}

void OptionWindow::on_buttonBox_accepted()
{
    QSettings settings("TranslationTools", "config");
    settings.setValue("editor", uiOptionWindow->lineEdit_EditorCommand->text());
    settings.setValue("custom_mode", uiOptionWindow->groupBox_customSource->isChecked());
    settings.setValue("custom_source_path", uiOptionWindow->lineEdit_optionCustomSourceValue->text());
}

void OptionWindow::on_pushButton_OpenFolder_clicked()
{
    QString custom_source_path = uiOptionWindow->lineEdit_optionCustomSourceValue->text();

    if(custom_source_path == ""){
        uiOptionWindow->lineEdit_optionCustomSourceValue->setText( QFileDialog::getExistingDirectory(this,
                                                                                                     UITranslation::title_open_folder,
                                                                                                     QDir::homePath()));
    } else {
        uiOptionWindow->lineEdit_optionCustomSourceValue->setText( QFileDialog::getExistingDirectory(this,
                                                                                                     UITranslation::title_open_folder,
                                                                                                     custom_source_path));
    }

}
