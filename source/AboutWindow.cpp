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

#include "AboutWindow.h"
#include "ui_AboutWindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    uiAboutWindow(new Ui::AboutWindow)
{
    uiAboutWindow->setupUi(this);

    setWindowTitle(UITranslation::about_TranslationTools_Title);
    uiAboutWindow->label_AboutWindowText->setText(UITranslation::about_TranslationTools_Text);
    uiAboutWindow->label_AboutWindowText->adjustSize();
    adjustSize();
    uiAboutWindow->verticalLayout->setSizeConstraint( QLayout::SetFixedSize );
}

AboutWindow::~AboutWindow()
{
    delete uiAboutWindow;
}
