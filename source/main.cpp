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

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    //internationalization support
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("translation/language_") + locale);
    application.installTranslator(&translator);

    //UTF-8 support
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    MainWindow openWindow;

    //Loading application XML file
    openWindow.load_ApplicationXMLFile();

    //Loading user configuration
    openWindow.load_PersonalConfig();

    //Display the window
    openWindow.show();

    return application.exec();
}
