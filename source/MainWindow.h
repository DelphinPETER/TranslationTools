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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QMap>
#include <QFile>
#include <QTreeWidget>
#include <QDomDocument>
#include <QProcess>
#include <iostream>

#include <OptionWindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void load_PersonalConfig();
    void load_ApplicationFile();
    void noui(QStringList);

private slots:

    //Window action
    void on_combo_Language_currentIndexChanged(const QString &arg1);
    void on_treeApps_itemSelectionChanged();
    void on_treeApps_itemDoubleClicked();
    void on_actionOptions_triggered();
    void on_actionShow_unused_string_triggered();
    void on_pushButton_Scan_clicked();
    void set_TreeProject();

    //File loading and working
    void scan_TranslationPath(QString translationPath, QString language);
    void load_TranslationFile(QString sourcePath, QString filename, QString language);
    void compare_SourceTranslation();
    QStringList get_FileList(QString filepath);
    void cleanValue();

    //User configuration
    void save_PersonalConfig();

    //Loading XML file
    void load_LanguageXMLFile();
    void load_ProjectXMLFile();
    void load_UITranslation();

private:
    Ui::MainWindow *uiOpenWindow;

    QString m_rootDir;
    QString m_language;
    QString m_editor;
    QVector<QString> m_languageList;
    QVector<QString> m_untranslatedString;
    QVector<QString> m_unUsedString;
    QVector<QString> m_filterString;
    QMap<QString, QString > m_resourceID_Name;
    QMap<QString, QVector<QString> > m_resourceID_skipFileList;
    QMap<QString, QString> m_stringID_stringName;
    QMap<QString, int> m_stringID_lineNumber;
    QMap<QString, QString> m_stringID_file;
    QMap<QString, QString> m_stringID_text;
    QMap<QString, QString> m_stringID_type;
    QMap<QString, QString > m_language_subst;
    int m_translatableCounter;

    //Options
    bool m_customMode;
    QString m_customSourcePath;

    //UI translation
    QString m_text_unused;
    QString m_text_goodWorks;
    QString m_text_error_no_source_file_title;
    QString m_text_error_no_source_file_text;
    QString m_text_error_no_file_title;
    QString m_text_error_no_project_text;
    QString m_text_error_no_language_text;
};

#endif
