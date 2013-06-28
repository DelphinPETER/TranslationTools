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

#include <QtGui/QApplication>
#include <QTextCodec>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

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
