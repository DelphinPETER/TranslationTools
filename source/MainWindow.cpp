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

#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uiMainWindow(new Ui::MainWindow)
{
    uiMainWindow->setupUi(this);

    UITranslation uiTranslation;

    load_UITranslation();
    load_configXMLFile();
    load_PersonalConfig();

    mRightClickString = new QAction(UITranslation::menu_right_click_string, this);
    mRightClickProject = new QAction(UITranslation::menu_right_click_folder, this);
    uiMainWindow->treeApps->setContextMenuPolicy(Qt::ActionsContextMenu);

    QObject::connect(mRightClickString, SIGNAL(triggered()), this, SLOT(on_treeApps_itemDoubleClicked()));
    QObject::connect(mRightClickProject, SIGNAL(triggered()), this, SLOT(openProjectFolder()));
}

MainWindow::~MainWindow()
{
    delete uiMainWindow;
}

/*
 * Launch the scan process
 */
void MainWindow::on_pushButton_Scan_clicked()
{
    clean_value();

    load_ProjectXMLFile();

    int appNumber = 0;
    QMapIterator<QString, Project> itrProject(mProjectIDProject);
    while(itrProject.hasNext()){
        itrProject.next();
        appNumber++;
        Project project = mProjectIDProject.value( itrProject.key() );
        project.scanSource();

        //If language have subst, the substitute language is scanned first
        if(mLanguageSubst[mLanguage] != ""){
            project.scanTranslation(mLanguageSubst.value(mLanguage));
        }

        project.scanTranslation(mLanguage);
        mProjectIDProject[itrProject.key()] = project;
        uiMainWindow->progressBar_scan->setValue(appNumber);
    }

    display_result();

}

/*
 * Clean interface and purge project already scanned
 */
void MainWindow::clean_value()
{
    Project::clear();
    uiMainWindow->treeApps->clear();
    uiMainWindow->progressBar_scan->setValue(0);
    uiMainWindow->label_progressCounter->clear();
}

/*
 * Display the result on the MainWindow
 */
void MainWindow::display_result()
{

    //If m_TotalString is empty that because TTools have not found source files
    if(Project::m_TotalString == 0){
        QMessageBox::warning(this,
                             UITranslation::error_no_source_file_title,
                             UITranslation::error_no_source_file_text,
                             QMessageBox::Ok);
        return;
    }


    int untranslatedCount =0;

    QMapIterator<QString, Project> itrProject(mProjectIDProject);
    while(itrProject.hasNext()) {
        itrProject.next();
        Project project = itrProject.value();

        if( !project.isTranslated() || project.hasUnused() ){

            QTreeWidgetItem* itemProject = new QTreeWidgetItem;
            itemProject->setText(0, project.getName() );
            itemProject->setText(1, QString::number(project.getUntranslatedNumber()) );
            itemProject->setText(4, project.getFolder().remove(mRootDir));
            itemProject->setText(5, itrProject.key() );

            QMapIterator<QString, TranslationItem> itrUntranslatedString(project.getTranslationItems());
            while(itrUntranslatedString.hasNext()){
                itrUntranslatedString.next();
                TranslationItem translationItem = itrUntranslatedString.value();
                QTreeWidgetItem* untranslatedItem = new QTreeWidgetItem;
                untranslatedItem->setText(0, translationItem.getStringName());
                untranslatedItem->setText(2, QString::number(translationItem.getLineNumber()) );
                untranslatedItem->setText(3, translationItem.getType());
                QString filename = translationItem.getFilename().remove(mRootDir);
                filename = filename.remove(itemProject->text(4)+"/");
                untranslatedItem->setText(4, filename);
                untranslatedItem->setText(5, itrProject.key() );
                itemProject->addChild(untranslatedItem);
                untranslatedCount++;
            }

            if(uiMainWindow->actionShow_unused_string->isChecked()){

                QMapIterator<QString, TranslationItem> itrUnusedString(project.getUnusedItems());
                while(itrUnusedString.hasNext()){
                    itrUnusedString.next();
                    TranslationItem translationItem = itrUnusedString.value();
                    QTreeWidgetItem* unusedItem = new QTreeWidgetItem;
                    unusedItem->setText(0, translationItem.getStringName());
                    unusedItem->setText(1, UITranslation::text_unused);
                    unusedItem->setText(2, QString::number(translationItem.getLineNumber()) );
                    unusedItem->setText(3, translationItem.getType());
                    unusedItem->setText(4, translationItem.getFilename().remove(mRootDir));
                    unusedItem->setText(5, itrProject.key() );
                    unusedItem->setTextColor(0, Qt::red);
                    unusedItem->setTextColor(1, Qt::red);
                    unusedItem->setTextColor(2, Qt::red);
                    unusedItem->setTextColor(3, Qt::red);
                    unusedItem->setTextColor(4, Qt::red);
                    itemProject->setTextColor(0, Qt::red);
                    itemProject->addChild(unusedItem);
                }

            }

            if(itemProject->childCount()>0){
                uiMainWindow->treeApps->addTopLevelItem(itemProject);
            }
        }
    }

    //Add statistics
    int pourcent = ( (Project::m_TotalString - untranslatedCount) * 100 / Project::m_TotalString );
    uiMainWindow->label_progressCounter->setText(" " + QString::number(pourcent) + "% (" + QString::number(untranslatedCount) + "/" + QString::number(Project::m_TotalString) + ")");
    uiMainWindow->label_Progression->setVisible(true);

    //No translation find, good work !!!
    if(uiMainWindow->treeApps->topLevelItemCount() == 0) {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0,UITranslation::text_goodWorks);
        uiMainWindow->treeApps->addTopLevelItem(item);
    }

    uiMainWindow->statusbar->showMessage(UITranslation::info_scan_translation_finished, 0);

    for(int i = 0; i < 2; i++){
        uiMainWindow->treeApps->resizeColumnToContents(i);
    }

}

/*
 * Action with a right click on the project item
 */
void MainWindow::openProjectFolder()
{
    QTreeWidgetItem *item = uiMainWindow->treeApps->currentItem();
    Project project = mProjectIDProject.value(item->text(5));
    QDesktopServices::openUrl(QUrl("file://" + project.getFolder()));
}

/*
 * If a item is selected, the informations about this item is displayed
 */
void MainWindow::on_treeApps_itemSelectionChanged()
{
    QTreeWidgetItem *item = uiMainWindow->treeApps->currentItem();

    uiMainWindow->label_sourceTextText->clear();

    if(item->text(0) != UITranslation::text_goodWorks){

        if(uiMainWindow->treeApps->indexOfTopLevelItem(item) == -1) {

            Project project = mProjectIDProject.value(item->parent()->text(5));
            QMap<QString, TranslationItem> translationItemList;
            if( item->text(1) == UITranslation::text_unused ){
                translationItemList = project.getUnusedItems();
            } else {
                translationItemList = project.getTranslationItems();
            }
            TranslationItem translationItem = translationItemList.value(item->text(0));
            uiMainWindow->label_sourceTextText->setText(translationItem.getSourceText());
        }

    }

}

/*
 * If a item is double clicked, we check if it's project item or a translation item.
 * - For project item, we expand project item and we resize the column
 * - For translation item we open the translation file with the text editor command
 */
void MainWindow::on_treeApps_itemDoubleClicked()
{
    QTreeWidgetItem *item = uiMainWindow->treeApps->currentItem();

    //If the item is a string we open the editor
    if(uiMainWindow->treeApps->indexOfTopLevelItem(item) == -1) {
        QProcess process;

        //Open the translation file
        QString openFile = mEditor;


        Project project = mProjectIDProject.value(item->parent()->text(5));
        QMap<QString, TranslationItem> translationItemList;
        if( item->text(1) == UITranslation::text_unused ){
            translationItemList = project.getUnusedItems();
        } else {
            translationItemList = project.getTranslationItems();
        }
        TranslationItem translationItem = translationItemList.value(item->text(0));

        if( translationItem.isUnused() ){
            openFile.replace( "%1", translationItem.getFilename() );
            openFile.replace( "%2", QString::number(translationItem.getLineNumber()) );
            process.startDetached(openFile);
        } else {

            //Open translation file
            openFile.replace( "%1", translationItem.getFilename() );
            openFile.replace( "%2", QString::number(translationItem.getLineNumber()) );
            process.startDetached(openFile);

            //Open source file
            openFile = mEditor;
            QString translation_folder = translationItem.getFilename();
            translation_folder = translation_folder.replace("values", "values-" + mLanguage);
            openFile.replace( "%1", translation_folder );
            openFile.replace( "%2", QString::number( 1 ) );
            process.startDetached(openFile);
        }
    } else if (!item->isExpanded()){
        item->setExpanded(true);
    }

}

/*
 * Load interface translation
 */
void MainWindow::load_UITranslation()
{

    QStringList ColumnNames;
    ColumnNames << UITranslation::column_title_apps_name <<
                   UITranslation::column_title_untranslated <<
                   UITranslation::column_title_line_number <<
                   UITranslation::column_title_type <<
                   UITranslation::column_title_filename;
    uiMainWindow->treeApps->setHeaderLabels(ColumnNames);
    uiMainWindow->treeApps->setColumnHidden(5, true);

    uiMainWindow->label_Language->setText( UITranslation::label_language );
    uiMainWindow->label_ROMName_title->setText( UITranslation::label_rom_name );
    uiMainWindow->pushButton_Scan->setText( UITranslation::scan_button );
    uiMainWindow->label_Progression->setText(UITranslation::label_progress);
    uiMainWindow->groupBox_Information->setTitle(UITranslation::information_title);
    uiMainWindow->label_sourceTextTitle->setText(UITranslation::information_source_text_title);
    uiMainWindow->menuOption->setTitle(UITranslation::menu_options);
    uiMainWindow->menuDisplay->setTitle(UITranslation::menu_display);
    uiMainWindow->actionShow_unused_string->setText(UITranslation::option_show_unused_string);
    uiMainWindow->actionAbout_TranslationTools->setText(UITranslation::about_TranslationTools_Title);
    uiMainWindow->actionOptions->setText(UITranslation::menu_options);

}

/*
 * Loading the language supported
 */
void MainWindow::load_configXMLFile()
{

    mLanguageSubst.clear();

    QDomElement rootItem = loadXMLFile(QCoreApplication::applicationDirPath() + "/config/config.xml");
    QDomElement item = rootItem.firstChildElement();

    while( !item.isNull() )
    {
        if(item.tagName() == "rom-name"){
            uiMainWindow->label_ROMName_text->setText( item.text() );
        }
        else if (item.tagName() == "languages") {

            QDomElement itemLanguage = item.firstChildElement();
            while ( !itemLanguage.isNull() ) {
                mLanguageSubst[itemLanguage.attribute("value")] = itemLanguage.attribute("subst");
                uiMainWindow->combo_Language->addItem(itemLanguage.attribute("value"));
                itemLanguage = itemLanguage.nextSiblingElement();
            }

        }
        item = item.nextSiblingElement();
    }

}

/*
 * Loading the project XML file with all attributes: project name and relative path, string missing, etc...
 */
void MainWindow::load_ProjectXMLFile()
{

    mProjectIDProject.clear();

    QDomElement projectRoot = loadXMLFile(QCoreApplication::applicationDirPath() + "/config/project.xml");
    QDomElement projectItem = projectRoot.firstChildElement();

    while( !projectItem.isNull() )
    {

        Project project(projectItem.attribute("resourceID"),
                        projectItem.attribute("name"),
                        mRootDir + "/" + projectItem.attribute("resourceID"));

        if(projectItem.hasChildNodes()){

            QDomElement skipItem = projectItem.firstChildElement();

            while(!skipItem.isNull())
            {
                if(skipItem.tagName() == "skip-file") {
                    project.addSkipFile(skipItem.attribute("name"));
                }
                else if (skipItem.tagName() == "skip-string") {
                    project.addSkipString(skipItem.attribute("name"), skipItem.attribute("type"));
                }
                skipItem = skipItem.nextSiblingElement();
            }
        }

        mProjectIDProject[projectItem.attribute("resourceID")] = project;
        projectItem = projectItem.nextSiblingElement();
    }

    //Define maximum in progressbar
    uiMainWindow->progressBar_scan->setMaximum(mProjectIDProject.size());
    uiMainWindow->statusbar->showMessage(UITranslation::info_project_XML_file_loaded, 0);
}

/*
 * Loading personal config save in USER config folder
 * by default: /home/USERNAME/.config/TranslationTools/config.conf
 */
void MainWindow::load_PersonalConfig()
{

    QSettings settings("TranslationTools", "config");
    mLanguage = settings.value("language").toString();
    mEditor = settings.value("editor").toString();
    mCustomMode = settings.value("custom_mode").toBool();
    mCustomSourcePath = settings.value("custom_source_path").toString();
    uiMainWindow->actionShow_unused_string->setChecked(settings.value("show_unused_string").toBool());

    int temp = uiMainWindow->combo_Language->findText(mLanguage);
    uiMainWindow->combo_Language->setCurrentIndex(temp);

    if (mCustomMode) {
        mRootDir = mCustomSourcePath;
    } else {
        mRootDir = QCoreApplication::applicationDirPath().section("/", 0, -3);
    }

    uiMainWindow->statusbar->showMessage(UITranslation::info_user_config_loaded, 0);

}

/*
 * Display Option window
 */
void MainWindow::on_actionOptions_triggered()
{
    OptionWindow *optionWindow = new OptionWindow(this);
    optionWindow->show();
    load_PersonalConfig();
}

/*
 * When option "Show unused string" is triggered, the window is update and config is saved
 */
void MainWindow::on_actionShow_unused_string_triggered()
{
    if(uiMainWindow->treeApps->topLevelItemCount() != 0) {
        uiMainWindow->treeApps->clear();
        uiMainWindow->label_sourceTextText->clear();
        display_result();
    }

    //Save config
    QSettings settings("TranslationTools", "config");
    settings.setValue("show_unused_string", uiMainWindow->actionShow_unused_string->isChecked());

}

/*
 * When selected language changed, it's saving in personal config
 */
void MainWindow::on_combo_Language_activated(const QString &arg1)
{
    mLanguage = arg1;

    QSettings settings("TranslationTools", "config");
    settings.setValue("language", mLanguage);

}

/*
 * Generic XML file loading process, it's return the root item of XML tree
 */
QDomElement MainWindow::loadXMLFile(QString p_Filepath)
{

    QFile file(p_Filepath);
    if( !file.open( QIODevice::ReadOnly ) ) {
        QString message = UITranslation::error_no_file_text;
        message = message.replace("%1", p_Filepath);
        QMessageBox::warning(this, UITranslation::error_no_file_title, message, QMessageBox::Ok);
        QTextStream(stdout) << "ERROR:" << p_Filepath << " not found";
        exit(0);
    }

    QDomDocument domDocument( "domDocument" );
    if( !domDocument.setContent( &file ) )
    {
        QTextStream(stdout) << "ERROR: "<< p_Filepath << " not found";
        file.close();
    }
    file.close();

    return domDocument.documentElement();

}

/*
 * Simply resize the first colum when a root item is expanded
 */
void MainWindow::on_treeApps_itemExpanded()
{
    uiMainWindow->treeApps->resizeColumnToContents(0);
}

/*
 * When item selected change, the right menu is adapted, string or project item.
 */
void MainWindow::on_treeApps_itemPressed(QTreeWidgetItem *item)
{
    uiMainWindow->treeApps->removeAction(mRightClickString);
    uiMainWindow->treeApps->removeAction(mRightClickProject);
    if(item->text(0) != UITranslation::text_goodWorks){
        if(uiMainWindow->treeApps->indexOfTopLevelItem(item) == -1) {
            uiMainWindow->treeApps->addAction(mRightClickString);
        } else {
            uiMainWindow->treeApps->addAction(mRightClickProject);
        }
    }

}

void MainWindow::on_actionAbout_TranslationTools_triggered()
{
    AboutWindow*aboutDialog=new AboutWindow(this);
    aboutDialog->show();
}
