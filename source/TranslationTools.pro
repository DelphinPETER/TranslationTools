#
# Copyright (C) 2013 - The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

QT       += core gui xml widgets

TARGET = TranslationTools

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp \
    OptionWindow.cpp

HEADERS  += \
    MainWindow.h \
    OptionWindow.h

FORMS    += \
    MainWindow.ui \
    OptionWindow.ui

RESOURCES += \
    icon.qrc

TRANSLATIONS = \
    language_da.ts \
    language_el.ts \
    language_es.ts \
    language_fr.ts \
    language_sk.ts
