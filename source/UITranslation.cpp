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

#include "UITranslation.h"

QString UITranslation::translationTools_version = "0.3.2.1";

//If no translations are found, the default translation is displayed
QString UITranslation::text_unused = "unused";
QString UITranslation::text_goodWorks = "No translation needed, Good Work!";
QString UITranslation::error_no_source_file_title = "No XML files found";
QString UITranslation::error_no_source_file_text = "No xml files are found. Make sure you launch TranslationTool in source folder or that your custom configuration is correct.";
QString UITranslation::error_no_file_title = "File not found";
QString UITranslation::error_no_file_text = "The file %1 was not found, make sure you launch TranslationTool in source folder or that your custom configuration is correct.";
QString UITranslation::label_language = "Language:";
QString UITranslation::label_rom_name = "ROM name:";
QString UITranslation::scan_button = "Scan";
QString UITranslation::column_title_apps_name = "Apps name";
QString UITranslation::column_title_untranslated = "Untranslated";
QString UITranslation::column_title_line_number = "Line";
QString UITranslation::column_title_filename = "Filename";
QString UITranslation::column_title_type = "Type";
QString UITranslation::label_progress = "Progress:";
QString UITranslation::information_title = "Informations:";
QString UITranslation::information_source_text_title = "Source text:";
QString UITranslation::info_scan_translation_finished = "Scan translation finished successfully";
QString UITranslation::info_project_XML_file_loaded = "project.xml file was loaded successfully";
QString UITranslation::info_user_config_loaded = "User configuration loaded successfully";
QString UITranslation::menu_tools = "Tools";
QString UITranslation::menu_display = "Display";
QString UITranslation::option_show_unused_string = "Show unused string";
QString UITranslation::menu_options = "Options";
QString UITranslation::editor_command = "Editor command:";
QString UITranslation::custom_source_title = "Custom folder:";
QString UITranslation::use_custom_source = "Use a custom source folder";
QString UITranslation::tips_options = "Use %1 for the path file and %2 for the line (for example : gedit +%2 %1)";
QString UITranslation::button_open_text = "Open";
QString UITranslation::title_open_folder = "Open folder";
QString UITranslation::menu_right_click_string = "Open file";
QString UITranslation::menu_right_click_folder = "Open source folder";
QString UITranslation::about_TranslationTools_Title = "About TranslationTools";
QString UITranslation::about_TranslationTools_Text = "Translation Tools is a program to assist translators in translating Android ROM into their language.<br/><br/>TranslationTools is under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.<br/><br/>This program is provided without warranty; for the source please see <a href=\"https://github.com/DelphinPETER/TranslationTools\">Github website</a>.";

UITranslation::UITranslation()
{

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

        QString about_TranslationTools_Text_1;
        QString about_TranslationTools_Text_2;
        QString about_TranslationTools_Text_3;
        QString about_TranslationTools_Text_link;

        while (!stringsItem.isNull()){

            if (stringsItem.attribute("name") == "label_language") {
                label_language = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "label_rom_name") {
                label_rom_name = stringsItem.text();
            }


            else if (stringsItem.attribute("name") == "scan_button") {
                scan_button = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "column_title_apps_name") {
                column_title_apps_name = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "column_title_untranslated") {
                column_title_untranslated = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "column_title_line_number") {
                column_title_line_number = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "column_title_filename") {
                column_title_filename = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "column_title_type") {
                column_title_type = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "label_progress") {
                label_progress = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "unused") {
                text_unused = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "good_works") {
                text_goodWorks = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "information_title") {
                information_title = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "information_source_text_title") {
                information_source_text_title = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "menu_tools") {
                menu_tools = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "menu_display") {
                menu_display = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "option_show_unused_string") {
                option_show_unused_string = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "menu_options") {
                menu_options = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "error_no_source_file_title") {
                error_no_source_file_title = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "error_no_source_file_text") {
                error_no_source_file_text = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "error_no_file_title") {
                error_no_file_title = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "error_no_file_text") {
                error_no_file_text = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "editor_command") {
                editor_command = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "custom_source_title") {
                editor_command = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "use_custom_source") {
                use_custom_source = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "tips_options") {
                tips_options = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "button_open_text") {
                button_open_text = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "info_scan_translation_finished") {
                info_scan_translation_finished = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "info_project_XML_file_loaded") {
                info_project_XML_file_loaded = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "info_user_config_loaded") {
                info_user_config_loaded = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "title_open_folder") {
                title_open_folder = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "menu_right_click_string") {
                menu_right_click_string = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "menu_right_click_folder") {
                menu_right_click_folder = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "about_TranslationTools_Title") {
                about_TranslationTools_Title = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "about_TranslationTools_Text_1") {
                about_TranslationTools_Text_1 = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "about_TranslationTools_Text_2") {
                about_TranslationTools_Text_2 = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "about_TranslationTools_Text_3") {
                about_TranslationTools_Text_3 = stringsItem.text();
            }

            else if (stringsItem.attribute("name") == "about_TranslationTools_Text_link") {
                about_TranslationTools_Text_link = stringsItem.text();
            }


            stringsItem = stringsItem.nextSiblingElement();
        }

        about_TranslationTools_Text_3.replace( "%1", "<a href=\"https://github.com/DelphinPETER/TranslationTools\">" + about_TranslationTools_Text_link + "</a>");
        about_TranslationTools_Text = "TranslationTools - " + translationTools_version + "<br/><br/>" +
                about_TranslationTools_Text_1 + "<br/><br/>" +
                "Copyright (C) 2014 - Delphin PETER - delphinpeter@gmail.com" + "<br/><br/>" +
                about_TranslationTools_Text_2 + "<br/><br/>" + about_TranslationTools_Text_3;

    }
}
