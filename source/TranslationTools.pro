#
# Copyright (C) 2014 - Delphin PETER - delphinpeter@gmail.com
#
#   This file is part of TranslationTools.
#
#   TranslationTools is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

QT       += core gui xml widgets

TARGET = TranslationTools

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    OptionWindow.cpp \
    Project.cpp \
    TranslationItem.cpp \
    UITranslation.cpp \
    AboutWindow.cpp

HEADERS  += \
    MainWindow.h \
    OptionWindow.h \
    Project.h \
    TranslationItem.h \
    UITranslation.h \
    AboutWindow.h

FORMS    += \
    MainWindow.ui \
    OptionWindow.ui \
    AboutWindow.ui

RESOURCES += \
    icon.qrc
