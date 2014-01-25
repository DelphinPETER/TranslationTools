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
    uiOptionWindow(new Ui::OptionWindow)
{
    uiOptionWindow->setupUi(this);
    setFixedSize(size());

    QString language = QLocale::system().name().section('_', 0, 0);

    QFile uiTranslationFile(QCoreApplication::applicationDirPath() + "/res/strings-" + language  + ".xml");

    if(!uiTranslationFile.exists()){
        uiTranslationFile.setFileName(QCoreApplication::applicationDirPath() + "/res/strings.xml");
    }

    if( uiTranslationFile.open( QIODevice::ReadOnly ) ) {

        QDomDocument uiTranslationDocument;
        uiTranslationDocument.setContent( &uiTranslationFile);

        //Define xml element
        QDomElement uiTranslationRoot = uiTranslationDocument.documentElement();
        QDomElement stringsItem = uiTranslationRoot.firstChildElement();

        while (!stringsItem.isNull()){

            if (stringsItem.attribute("name") == "menu_options") {
                setWindowTitle(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "editor_command") {
                uiOptionWindow->label_EditorCommand->setText( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "custom_source_title") {
                uiOptionWindow->label_optionCustomSourceTitle->setText( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "use_custom_source") {
                uiOptionWindow->groupBox_customSource->setTitle( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "tips_options") {
                uiOptionWindow->label_TipsOptions->setText( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "button_open_text") {
                uiOptionWindow->pushButton_OpenFolder->setText( stringsItem.text() );
            }

            stringsItem = stringsItem.nextSiblingElement();
        }
    }


}

OptionWindow::~OptionWindow()
{
    delete uiOptionWindow;
}

void OptionWindow::set_Editor(QString editor)
{
    m_editor = editor;
    uiOptionWindow->lineEdit_EditorCommand->setText(m_editor);
}

void OptionWindow::set_customMode(bool customMode)
{
    m_customMode = customMode;
    uiOptionWindow->groupBox_customSource->setChecked(customMode);
}

void OptionWindow::set_customSourcePath(QString customSourcePath)
{
    m_customSourcePath = customSourcePath;
    uiOptionWindow->lineEdit_optionCustomSourceValue->setText(customSourcePath);
}

void OptionWindow::on_buttonBox_accepted()
{
    m_editor = uiOptionWindow->lineEdit_EditorCommand->text();
    m_customSourcePath = uiOptionWindow->lineEdit_optionCustomSourceValue->text();
    m_customMode = uiOptionWindow->groupBox_customSource->isChecked();
}

QString OptionWindow::get_Editor()
{
    return m_editor;
}

QString OptionWindow::get_customSourcePath()
{
    return m_customSourcePath;
}

bool OptionWindow::get_customMode()
{
    return m_customMode;
}

void OptionWindow::on_pushButton_OpenFolder_clicked()
{
    QString folder = QFileDialog::getExistingDirectory(this);
    uiOptionWindow->lineEdit_optionCustomSourceValue->setText(folder);
}
