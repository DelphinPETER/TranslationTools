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

#ifndef UITRANSLATION_H
#define UITRANSLATION_H

#include <QDomDocument>
#include <QFile>
#include <QCoreApplication>
#include <QLocale>

class UITranslation
{
public:
    UITranslation();

    //UI translation
    static QString text_unused;
    static QString text_goodWorks;
    static QString error_no_source_file_title;
    static QString error_no_source_file_text;
    static QString error_no_file_title;
    static QString error_no_file_text;
    static QString label_language;
    static QString label_rom_name;
    static QString scan_button;
    static QString column_title_apps_name;
    static QString column_title_untranslated;
    static QString column_title_line_number;
    static QString column_title_filename;
    static QString column_title_type;
    static QString label_progress;
    static QString information_title;
    static QString information_source_text_title;
    static QString menu_tools;
    static QString menu_display;
    static QString option_show_unused_string;
    static QString menu_options;
    static QString editor_command;
    static QString custom_source_title;
    static QString use_custom_source;
    static QString tips_options;
    static QString button_open_text;
    static QString info_scan_translation_finished;
    static QString info_project_XML_file_loaded;
    static QString info_user_config_loaded;
    static QString title_open_folder;
    static QString menu_right_click_string;
    static QString menu_right_click_folder;
    static QString about_TranslationTools_Title;
    static QString about_TranslationTools_Text;
    static QString translationTools_version;

};

#endif // UITRANSLATION_H
