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
    void load_ApplicationXMLFile();

private slots:

    //Window action
    void on_combo_Language_currentIndexChanged(const QString &arg1);
    void on_pushButton_Scan_clicked();
    void on_pushButton_saveCurrentList_clicked();
    void on_treeApps_itemSelectionChanged();
    void on_treeApps_itemDoubleClicked();
    void on_actionOptions_triggered();

    //File loading and working
    void set_TreeProject();
    void scan_SourcePath(QString sourcePath);
    void load_SourceFile(QString sourcePath, QString filename);
    void scan_TranslationPath(QString translationPath, QString language);
    void load_TranslationFile(QString sourcePath, QString filename, QString language);
    void compare_SourceTranslation(QString resourceID);
    QStringList get_FileList(QString filepath);

    //User configuration
    void save_PersonalConfig();

    //Loading XML file
    void load_FilterXMLFile();
    void load_LanguageXMLFile();
    void load_ProjectXMLFile();

private:
    Ui::MainWindow *uiOpenWindow;

    QString m_rootDir;
    QString m_language;
    QString m_editor;
    QVector<QString> m_languageList;
    QMap<QString, QVector<QString> > m_resourceID_sourceString;
    QMap<QString, QVector<QString> > m_resourceID_translationString;
    QMap<QString, QVector<QString> > m_resourceID_untranslatedString;
    QMap<QString, QVector<QString> > m_resourceID_unusedString;
    QMap<QString, QString > m_resourceID_Name;
    QMap<QString, QVector<QString> > m_resourceID_skipStringList;
    QMap<QString, QVector<QString> > m_resourceID_skipFileList;
    QMap<QString, QString> m_stringID_stringName;
    QMap<QString, int> m_stringID_lineNumber;
    QMap<QString, QString> m_stringID_file;
    QMap<QString, QString> m_stringID_type;
    QMap<QString, QString > m_language_subst;
    int m_translatableCounter;

    //Custom Mode
    bool m_customMode;
    QString m_customSourcePath;
};

#endif
