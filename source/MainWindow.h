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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QWidget>
#include <QString>
#include <QMap>
#include <QFile>
#include <QTreeWidget>
#include <QDomDocument>
#include <QProcess>
#include <QSettings>
#include <QDesktopServices>

#include <OptionWindow.h>
#include <AboutWindow.h>
#include <Project.h>
#include <UITranslation.h>

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

private slots:
    //Window action
    void on_combo_Language_activated(const QString &arg1);
    void on_treeApps_itemSelectionChanged();
    void on_treeApps_itemDoubleClicked();
    void on_treeApps_itemExpanded();
    void on_treeApps_itemPressed(QTreeWidgetItem *item);
    void on_actionOptions_triggered();
    void on_actionShow_unused_string_triggered();
    void on_actionAbout_TranslationTools_triggered();
    void on_pushButton_Scan_clicked();
    void display_result();
    void clean_value();
    void openProjectFolder();

    //Loading XML file
    void load_configXMLFile();
    void load_ProjectXMLFile();
    void load_UITranslation();
    QDomElement loadXMLFile(QString p_Filepath);

private:
    Ui::MainWindow *uiMainWindow;
    QAction *mRightClickProject;
    QAction *mRightClickString;
    QString mRootDir;
    QString mLanguage;
    QString mEditor;
    QMap<QString, QString > mLanguageSubst;
    QMap<QString, Project> mProjectIDProject;

    //Options
    bool mCustomMode;
    QString mCustomSourcePath;
};

#endif
