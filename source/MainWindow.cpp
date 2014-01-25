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

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QDateTime"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uiOpenWindow(new Ui::MainWindow)
{
    uiOpenWindow->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete uiOpenWindow;
}


void MainWindow::noui(QStringList argument)
{
    QString language;
    QStringListIterator itrArgument(argument);
    while(itrArgument.hasNext()) {
        language = itrArgument.next();
        if(language == "lang:all") {
            QVectorIterator<QString> itrLanguage(m_languageList);
            while(itrLanguage.hasNext()) {
                m_language = itrLanguage.next();
                on_pushButton_Scan_clicked();
            }
        } else if (language.contains("lang:")) {
            m_language = language;
            on_pushButton_Scan_clicked();
        }
    }

    exit(0);
}

void MainWindow::on_pushButton_Scan_clicked()
{

    cleanValue();
    int appNumber = 0;

    load_ProjectXMLFile();
    load_LanguageXMLFile();

    if (m_customMode) {
        m_rootDir = m_customSourcePath;
    } else {
        m_rootDir = QCoreApplication::applicationDirPath().section("/", 0, -4);
    }

    //Save user configuration
    save_PersonalConfig();

    if(m_stringID_stringName.isEmpty()){
        m_translatableCounter = 0;
        QMapIterator<QString, QString> itrSourceApps(m_resourceID_Name);
        while(itrSourceApps.hasNext()) {
            itrSourceApps.next();

            //Scan source file
            scan_TranslationPath(itrSourceApps.key(), "source");
        }
    }


    //Scan application list
    QMapIterator<QString, QString> itrApps(m_resourceID_Name);
    while(itrApps.hasNext()) {
        itrApps.next();
        appNumber++;

        //Scan translation file
        scan_TranslationPath(itrApps.key(), m_language);

        if(m_language_subst.contains(m_language)) {
            scan_TranslationPath(itrApps.key(), m_language_subst.value(m_language));
        }

        //Define current value of progress bar
        uiOpenWindow->progressBar_scan->setValue(appNumber);

    }

    //Compare source and translation strings
    compare_SourceTranslation();

    if(m_stringID_stringName.isEmpty()){
        QMessageBox::warning(this, m_text_error_no_source_file_title, m_text_error_no_source_file_text, QMessageBox::Ok);
        qDebug() << "ERROR: No files found";
        return;
    }

    //Display the result
    set_TreeProject();

}

void MainWindow::cleanValue()
{
    //Clear application window and value

    m_untranslatedString.clear();
    m_unUsedString.clear();
    QMapIterator<QString, QString> itrStringID(m_stringID_stringName);
    while(itrStringID.hasNext()){
        QString stringID = itrStringID.next().key();
        if(!stringID.contains("@source@")) {
            m_stringID_stringName.remove(stringID);
            m_stringID_lineNumber.remove(stringID);
            m_stringID_file.remove(stringID);
            m_stringID_text.remove(stringID);
            m_stringID_type.remove(stringID);
        }
    }

    uiOpenWindow->treeApps->clear();
    uiOpenWindow->progressBar_scan->setValue(0);
    uiOpenWindow->label_FilePathText->clear();
    uiOpenWindow->label_InfoTypeText->clear();
    uiOpenWindow->label_LineNumberText->clear();
    uiOpenWindow->label_progressCounter->clear();

}

void MainWindow::compare_SourceTranslation()
{
    m_untranslatedString.clear();
    m_unUsedString.clear();

    QVector<QString> sourceStringList;
    QVector<QString> translationStringList;

    QMapIterator<QString, QString> itrStringsList(m_stringID_stringName);
    while(itrStringsList.hasNext()){
        itrStringsList.next();
        QString item = itrStringsList.key();
        if ( item.contains("@source@")){
            sourceStringList.push_back(item);
        } else if (item.contains("@"+m_language+"@")){
            translationStringList.push_back(item);
        }
    }

    //Lists untranslated strings
    QVectorIterator<QString> itrSourceString(sourceStringList);
    while(itrSourceString.hasNext()) {
        QString sourceString = itrSourceString.next();
        if(!translationStringList.contains(sourceString.replace("@source@","@" + m_language + "@"))) {
            m_untranslatedString.push_back(sourceString);
        }
    }

    //Lists unused strings
    QVectorIterator<QString> itrUnusedString(translationStringList);
    while (itrUnusedString.hasNext()){
        QString unusedString = itrUnusedString.next();
        QString testString = unusedString;
        if(!sourceStringList.contains(testString.replace("@"+m_language+"@","@source@"))){
            m_unUsedString.push_back(unusedString);
        }
    }

}

void MainWindow::set_TreeProject()
{
    uiOpenWindow->treeApps->clear();
    int untranslatedCounterFull = 0;
    int untranslatedCounterApp = 0;

    QMapIterator<QString, QString> itrProjectList(m_resourceID_Name);
    while(itrProjectList.hasNext()) {
        itrProjectList.next();
        untranslatedCounterApp = 0;
        QString project = itrProjectList.key();

        QTreeWidgetItem* itemProject = new QTreeWidgetItem;

        //Display the untranslated strings
        QVectorIterator<QString> itrUntranslatedString(m_untranslatedString);
        while(itrUntranslatedString.hasNext()){
            QString untranslatedString = itrUntranslatedString.next();
            if(untranslatedString.contains(project)){
                QTreeWidgetItem* untranslatedItem = new QTreeWidgetItem;
                QString sourceItem = untranslatedString;
                sourceItem = sourceItem.replace("@"+m_language+"@","@source@");
                if(!m_filterString.contains(sourceItem.section("-",0,0))){
                    untranslatedItem->setText(0, m_stringID_stringName.value(sourceItem));
                    untranslatedItem->setText(2, sourceItem);
                    itemProject->addChild(untranslatedItem);
                    untranslatedCounterApp++;
                    QString message = project + " - " + m_language + " - " + m_stringID_stringName.value(sourceItem);
                    QTextStream(stdout) << ( message + "\n" );
                    qDebug() << message;
                }
            }
        }

        //Display the uunused strings
        if(uiOpenWindow->actionShow_unused_string->isChecked()) {
            QVectorIterator<QString> itrUnusedString(m_unUsedString);
            while(itrUnusedString.hasNext()){
                QString unusedString =itrUnusedString.next();
                if(unusedString.contains(project)){
                    QTreeWidgetItem* unusedItem = new QTreeWidgetItem;
                    QString sourceUnusedItem = unusedString;
                    if(!m_filterString.contains(sourceUnusedItem.section("-",0,0))){
                        unusedItem->setText(0, m_stringID_stringName.value(sourceUnusedItem));
                        unusedItem->setText(2, unusedString);
                        itemProject->addChild(unusedItem);
                        unusedItem->setTextColor(0, Qt::red);
                        unusedItem->setText(1, m_text_unused);
                        unusedItem->setTextColor(1, Qt::red);
                        unusedItem->setText(2, sourceUnusedItem);
                        unusedItem->setTextColor(0, Qt::red);
                        itemProject->setTextColor(0, Qt::red);
                    }
                }
            }
        }

        untranslatedCounterFull += untranslatedCounterApp;
        itemProject->setText(1,QString::number(untranslatedCounterApp));

        itemProject->setText(0, m_resourceID_Name.value(project));
        if(itemProject->childCount() >0){
            uiOpenWindow->treeApps->addTopLevelItem(itemProject);
        }

    }

    //Add statistics
    int pourcent(((m_translatableCounter - untranslatedCounterFull)*100) / m_translatableCounter );
    uiOpenWindow->label_progressCounter->setText(" " + QString::number(pourcent) + "% (" + QString::number(untranslatedCounterFull) + "/" + QString::number(m_translatableCounter) + ")");
    uiOpenWindow->label_Progression->setVisible(true);

    //No translation find, good work !!!
    if(uiOpenWindow->treeApps->topLevelItemCount() == 0) {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0,m_text_goodWorks);
        uiOpenWindow->treeApps->addTopLevelItem(item);
        qDebug() << "Language:" + m_language + " have no translation needed";
        cout << "Language:" + m_language.toStdString() + " have no translation needed\n";
    }
}

void MainWindow::on_combo_Language_currentIndexChanged(const QString &arg1)
{
    m_language = arg1;
}

void MainWindow::on_treeApps_itemSelectionChanged()
{
    QTreeWidgetItem *item = uiOpenWindow->treeApps->currentItem();

    QString stringID;
    QString resourceID;

    if(uiOpenWindow->treeApps->indexOfTopLevelItem(item) == -1) {
        stringID = item->text(2);
        resourceID = item->parent()->text(2);

        uiOpenWindow->label_InfoTypeText->setText(m_stringID_type.value(stringID) );
        uiOpenWindow->label_sourceTextText->setText(m_stringID_text.value(stringID));

        if(m_stringID_file.contains(stringID)) {
            uiOpenWindow->label_FilePathText->setText(m_stringID_file.value(stringID) );
        } else {
            uiOpenWindow->label_FilePathText->clear();
        }

        if(m_stringID_lineNumber.contains(stringID)) {
            uiOpenWindow->label_LineNumberText->setText(QString::number(m_stringID_lineNumber.value(stringID)) );
        } else {
            uiOpenWindow->label_LineNumberText->clear();
        }

    }
    else {
        uiOpenWindow->label_InfoTypeText->clear();
        uiOpenWindow->label_FilePathText->setText(item->text(2) );
        uiOpenWindow->label_LineNumberText->clear();
        uiOpenWindow->label_sourceTextText->clear();
    }

}

void MainWindow::on_treeApps_itemDoubleClicked()
{
    QTreeWidgetItem *item = uiOpenWindow->treeApps->currentItem();

    //If the item is a string we open the editor
    if(uiOpenWindow->treeApps->indexOfTopLevelItem(item) == -1) {
        QProcess process;

        //Open the translation file
        QString openTransaltionFile = m_editor;

        if(!m_unUsedString.contains(item->text(2))){
            //Replace file path
            QString translateFolder = m_stringID_file.value(item->text(2));
            translateFolder = translateFolder.replace("values", "values-"+m_language);
            openTransaltionFile.replace( "%1", translateFolder );
            //replace line number
            openTransaltionFile.replace( "%2", QString::number(1) );
            //launch command
            process.startDetached(openTransaltionFile);

            //Open the source file
            QString openSourceFile = m_editor;
            //Replace file path
            openSourceFile.replace( "%1", m_stringID_file.value(item->text(2)) );
            //replace line number
            openSourceFile.replace( "%2", QString::number(m_stringID_lineNumber.value(item->text(2))) );
            //launch command
            process.startDetached(openSourceFile);

        } else {
            //Replace file path
            QString translateFolder = m_stringID_file.value(item->text(2));
            //translateFolder = translateFolder.replace("values", "values-"+m_language);
            openTransaltionFile.replace( "%1", translateFolder );
            //replace line number
            openTransaltionFile.replace( "%2", QString::number(m_stringID_lineNumber.value(item->text(2)))  );
            //launch command
            process.startDetached(openTransaltionFile);

        }

    }

}

void MainWindow::scan_TranslationPath(QString translationPath, QString language)
{
    //Lists files in folder
    QStringList fileList;
    if (language == "source"){
        fileList = get_FileList(translationPath);
    } else {
        fileList = get_FileList(translationPath + "-" + language);
    }

    QStringListIterator itrListFile(fileList);
    while(itrListFile.hasNext()) {
        QString filename = itrListFile.next();
        QVector<QString> skipFileList = m_resourceID_skipFileList.value(translationPath);
        if(!skipFileList.contains(filename)) {
            load_TranslationFile(translationPath, filename, language);
        }
    }

}

void MainWindow::load_TranslationFile(QString sourcePath, QString filename, QString language)
{

    QString directory;
    if(language == "source") {
        directory =  m_rootDir + "/" + sourcePath + "/" + filename;
    } else {
        directory =  m_rootDir + "/" + sourcePath + "-" + language + "/" + filename;
    }


    QFile fileToParse(directory);
    if( !fileToParse.open( QIODevice::ReadOnly ) ) {
        qDebug() << "ERROR: " + directory + "<===== file not found";
    }

    QDomDocument xmlFile( "Translation_file" );
    if( !xmlFile.setContent( &fileToParse ) )
    {
        qDebug() << "ERROR: " + directory + "<===== file not loading";
        fileToParse.close();
    }
    fileToParse.close();

    //Define root
    QDomElement root = xmlFile.documentElement();
    if( root.tagName() != "resources" ) {
        qDebug() << "ERROR: " + directory + "<===== file corrupt";
    }

    QDomElement parentItem = root.firstChildElement();
    //QVector<QString> stringFound;

    while( !parentItem.isNull() )
    {

        if (parentItem.attribute("translatable") != "false" && parentItem.attribute("translate") != "false"){

            if (parentItem.tagName() == "string"){
                QString NameID;
                //Define string ID
                if(parentItem.hasAttribute("product")) {
                    NameID = sourcePath + "@" + language + "@" + parentItem.attribute("name") + "-" + parentItem.attribute("product");
                    m_stringID_stringName[NameID] = parentItem.attribute("name") + "-" + parentItem.attribute("product");

                } else {
                    NameID = sourcePath + "@" + language + "@" + parentItem.attribute("name");
                    m_stringID_stringName[NameID] = parentItem.attribute("name");
                }

                m_stringID_file[NameID] = directory;
                m_stringID_lineNumber[NameID] = parentItem.lineNumber();
                m_stringID_type[NameID] = "string";
                m_stringID_text[NameID] = parentItem.text();

            } else if (parentItem.tagName() == "plurals"){
                QDomElement pluralItem = parentItem.firstChildElement();
                while (!pluralItem.isNull()){
                    //Define plural ID
                    QString  NameID = sourcePath + "@" + language + "@" + parentItem.attribute("name");
                    if(!m_stringID_stringName.contains(NameID)) {
                        m_stringID_stringName[NameID] = parentItem.attribute("name") + "-" + pluralItem.attribute("quantity");
                        m_stringID_file[NameID] = directory;
                        m_stringID_lineNumber[NameID] = pluralItem.lineNumber();
                        m_stringID_type[NameID] = "plurals";
                        m_stringID_text[NameID] = pluralItem.text();
                    }
                    pluralItem = pluralItem.nextSiblingElement();
                }

            } else if (parentItem.tagName() == "string-array"){
                QDomElement arrayItem = parentItem.firstChildElement();
                int arrayNumber = 1;
                while (!arrayItem.isNull()){
                    //Define array ID
                    QString  NameID = sourcePath + "@" + language + "@" + parentItem.attribute("name") + "-" + QString::number(arrayNumber);
                    if(!m_stringID_stringName.contains(NameID)) {
                        m_stringID_stringName[NameID] = parentItem.attribute("name") + "-" + QString::number(arrayNumber);
                        m_stringID_file[NameID] = directory;
                        m_stringID_lineNumber[NameID] = arrayItem.lineNumber();
                        m_stringID_type[NameID] = "string-array";
                        m_stringID_text[NameID] = arrayItem.text();
                    }
                    arrayItem = arrayItem.nextSiblingElement();
                    ++arrayNumber;
                }
            }
            if(language == "source") {
                m_translatableCounter ++;
            }

        }
        parentItem = parentItem.nextSiblingElement();

    }
}

QStringList MainWindow::get_FileList(QString filepath)
{
    //Define QDir path
    QDir dir(m_rootDir + "/" + filepath);

    //Add xml file filter
    QStringList filters;
    filters << "*.xml";
    dir.setNameFilters(filters);

    return dir.entryList();
}

void MainWindow::load_ApplicationFile()
{    
    load_UITranslation();
    load_LanguageXMLFile();
}

void MainWindow::load_UITranslation()
{

    //Fix column width/hidden
    uiOpenWindow->treeApps->setColumnWidth(0, 400);
    uiOpenWindow->treeApps->setColumnWidth(1, 200);
    uiOpenWindow->treeApps->setColumnHidden(2, true);

    //Text selectable
    uiOpenWindow->label_FilePathText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    uiOpenWindow->label_InfoTypeText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    uiOpenWindow->label_LineNumberText->setTextInteractionFlags(Qt::TextSelectableByMouse);

    uiOpenWindow->label_Progression->hide();


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

        QString column_title_apps_name;
        QString column_title_untranslated;

        while (!stringsItem.isNull()){

            if (stringsItem.attribute("name") == "label_language") {
                uiOpenWindow->label_Language->setText( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "scan_button") {
                uiOpenWindow->pushButton_Scan->setText( stringsItem.text() );
            }

            if (stringsItem.attribute("name") == "column_title_apps_name") {
                column_title_apps_name = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "column_title_untranslated") {
                column_title_untranslated = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "label_progress") {
                uiOpenWindow->label_Progression->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "unused") {
                m_text_unused = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "good_works") {
                m_text_goodWorks = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "information_title") {
                uiOpenWindow->groupBox_Information->setTitle(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "information_line_number_title") {
                uiOpenWindow->label_LineNumberTitle->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "information_file_path_title") {
                uiOpenWindow->label_FilePathTitle->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "information_type_title") {
                uiOpenWindow->label_InfoTypeTitle->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "information_source_text_title") {
                uiOpenWindow->label_sourceTextTitle->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "menu_tools") {
                uiOpenWindow->menuOption->setTitle(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "menu_display") {
                uiOpenWindow->menuDisplay->setTitle(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "option_show_unused_string") {
                uiOpenWindow->actionShow_unused_string->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "menu_options") {
                uiOpenWindow->actionOptions->setText(stringsItem.text());
            }

            if (stringsItem.attribute("name") == "error_no_source_file_title") {
                m_text_error_no_source_file_title = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "error_no_source_file_text") {
                m_text_error_no_source_file_text = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "error_no_file_title") {
                m_text_error_no_file_title = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "error_no_project_text") {
                m_text_error_no_project_text = stringsItem.text();
            }

            if (stringsItem.attribute("name") == "error_no_language_text") {
                m_text_error_no_language_text = stringsItem.text();
            }

            stringsItem = stringsItem.nextSiblingElement();
        }

        QStringList columnTitle;
        columnTitle << column_title_apps_name << column_title_untranslated;
        uiOpenWindow->treeApps->setHeaderLabels(columnTitle);
        uiOpenWindow->treeApps->resizeColumnToContents(1);
    }
}

void MainWindow::load_LanguageXMLFile()
{

    m_languageList.clear();

    QFile languageFile(QCoreApplication::applicationDirPath() + "/language.xml");
    if( !languageFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning(this, m_text_error_no_file_title, m_text_error_no_language_text, QMessageBox::Ok);
        qDebug() << "ERROR: language.xml file not found";
        exit(0);
    }

    QDomDocument languageDocument( "Language" );
    if( !languageDocument.setContent( &languageFile ) )
    {
        qDebug() << "ERROR: language.xml file not found";
        languageFile.close();
    }
    languageFile.close();

    //Define xml element
    QDomElement LanguageRoot = languageDocument.documentElement();
    QDomElement languageItem = LanguageRoot.firstChildElement();

    while( !languageItem.isNull() )
    {
        m_languageList.push_back(languageItem.attribute("value"));
        if(languageItem.hasAttribute("subst")) {
            m_language_subst[languageItem.attribute("value")] = languageItem.attribute("subst");
        }
        languageItem = languageItem.nextSiblingElement();
    }

    //Definition of language list
    QVectorIterator<QString> itrLanguage(m_languageList);
    while(itrLanguage.hasNext()) {
        QString langue = itrLanguage.next();
        uiOpenWindow->combo_Language->addItem(langue);
    }

}

void MainWindow::load_ProjectXMLFile()
{

    m_resourceID_Name.clear();
    m_filterString.clear();
    m_resourceID_skipFileList.clear();

    QFile projectFile(QCoreApplication::applicationDirPath() + "/project.xml");
    if( !projectFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning(this, m_text_error_no_file_title, m_text_error_no_project_text , QMessageBox::Ok);
        qDebug() << "ERROR: project.xml file not found";
        exit(0);
    }

    QDomDocument projectDocument( "Project" );
    if( !projectDocument.setContent( &projectFile ) )
    {
        qDebug() << "ERROR: project.xml file not found";
        projectFile.close();
    }
    projectFile.close();

    //Define xml element
    QDomElement projectRoot = projectDocument.documentElement();
    QDomElement projectItem = projectRoot.firstChildElement();
    int appNumber = 0;

    while( !projectItem.isNull() )
    {
        m_resourceID_Name[projectItem.attribute("resourceID")] = projectItem.attribute("name");

        if(projectItem.hasChildNodes()){

            QVector<QString> skipFileList;

            QDomElement skipItem = projectItem.firstChildElement();

            while(!skipItem.isNull())
            {
                if(skipItem.tagName() == "skip-file") {
                    skipFileList.push_back(skipItem.attribute("name"));
                }
                else if (skipItem.tagName() == "skip-string") {
                    m_filterString.push_back(projectItem.attribute("resourceID") + "@" + "source" + "@" + skipItem.attribute("name"));
                }
                skipItem = skipItem.nextSiblingElement();
            }

            m_resourceID_skipFileList[m_resourceID_Name.key(projectItem.attribute("name"))] = skipFileList;

            //Init
            skipFileList.clear();
        }

        projectItem = projectItem.nextSiblingElement();
        appNumber++;
    }

    //Define maximum in progressbar
    uiOpenWindow->progressBar_scan->setMaximum(appNumber);
}

void MainWindow::load_PersonalConfig()
{

    //Loading file in personal folder
    QFile parameterFile(QDir::homePath() + "/.config/translation-tools/translation-tools-config.xml");
    if( !parameterFile.open( QIODevice::ReadOnly ) ) {
        qDebug() << "ERROR: " + QDir::homePath() + "/.config/translation-tools/translation-tools-config.xml <===== file not found";
        m_customMode = false;
        m_language = uiOpenWindow->combo_Language->currentText();
    }

    //Loading content file
    QDomDocument parameterXML( "config_file" );
    if( !parameterXML.setContent( &parameterFile ) ) {
        qDebug() << "ERROR: " + QDir::homePath() + "/.config/translation-tools/translation-tools-config.xml <===== file not loading";
        parameterFile.close();
    }
    parameterFile.close();

    //Define root item
    QDomElement root = parameterXML.documentElement();
    if ( root.tagName() != "settings" ) {
        qDebug() << "ERROR: " + QDir::homePath() + "/.config/translation-tools/translation-tools-config.xml <===== corrupted file";
    }

    //Define first item
    QDomElement itemSetting = root.firstChildElement();

    while( !itemSetting.isNull() )
    {
        //Define language
        if(itemSetting.attribute("name") == "language") {
            m_language = itemSetting.text();
            int temp = uiOpenWindow->combo_Language->findText(m_language);
            uiOpenWindow->combo_Language->setCurrentIndex(temp);
        }

        //Define editor command
        if(itemSetting.attribute("name") == "editor") {
            m_editor = itemSetting.text();
        }

        //Define status of custom_mode
        if(itemSetting.attribute("name") == "custom_mode") {
            if (itemSetting.attribute("value") == "true") {
                m_customMode = true;
            } else {
                m_customMode = false;
            }
        }

        //Define status of show_unused_string
        if(itemSetting.attribute("name") == "show_unused_string") {
            if (itemSetting.attribute("value") == "true") {
                uiOpenWindow->actionShow_unused_string->setChecked(true);
            } else {
                uiOpenWindow->actionShow_unused_string->setChecked(false);
            }
        }

        //Define custom_source_path
        if(itemSetting.attribute("name") == "custom_source_path") {
            m_customSourcePath = itemSetting.text();
        }

        itemSetting = itemSetting.nextSiblingElement();
    }

}

void MainWindow::save_PersonalConfig()
{
    //Define header of xml file
    QDomDocument writeXML;
    QDomNode xmlNode = writeXML.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    writeXML.insertBefore(xmlNode, writeXML.firstChild());

    //Define item : root
    QDomElement globalSettings = writeXML.createElement("settings");
    writeXML.appendChild(globalSettings);

    //Define item : language
    QDomElement itemSettingLanguage = writeXML.createElement("setting");
    itemSettingLanguage.setAttribute("name", "language");
    QDomText language = writeXML.createTextNode(m_language);
    itemSettingLanguage.appendChild(language);
    globalSettings.appendChild(itemSettingLanguage);

    //Define item : editor_command
    QDomElement itemEditor = writeXML.createElement("setting");
    itemEditor.setAttribute("name", "editor");
    QDomText editor = writeXML.createTextNode(m_editor);
    itemEditor.appendChild(editor);
    globalSettings.appendChild(itemEditor);

    //Define item : custom_mode
    QDomElement itemCustomMode = writeXML.createElement("setting");
    if(m_customMode) {
        itemCustomMode.setAttribute("value", "true");
    } else {
        itemCustomMode.setAttribute("value", "false");
    }
    itemCustomMode.setAttribute("name", "custom_mode");
    globalSettings.appendChild(itemCustomMode);

    //Define item : show_unused_string
    QDomElement itemShowUnusedString = writeXML.createElement("setting");
    if(uiOpenWindow->actionShow_unused_string->isChecked()) {
        itemShowUnusedString.setAttribute("value", "true");
    } else {
        itemShowUnusedString.setAttribute("value", "false");
    }
    itemShowUnusedString.setAttribute("name", "show_unused_string");
    globalSettings.appendChild(itemShowUnusedString);

    //Define item : custom_source_path
    QDomElement itemCustomSourcePath = writeXML.createElement("setting");
    itemCustomSourcePath.setAttribute("name", "custom_source_path");
    QDomText customSourcePath = writeXML.createTextNode(m_customSourcePath);
    itemCustomSourcePath.appendChild(customSourcePath);
    globalSettings.appendChild(itemCustomSourcePath);

    //Define the save path
    QDir configPath = QDir::homePath() + "/.config/translation-tools/";
    if(!configPath.exists()) {
        //if it doesn't exit, it created
        configPath.mkdir(QDir::homePath() + "/.config/translation-tools/");
    }

    //Open the file for write
    QFile writeConfig(QDir::homePath() + "/.config/translation-tools/translation-tools-config.xml");
    if (!writeConfig.open(QIODevice::WriteOnly)) {
        qDebug() << "ERROR: unable to open config file for writing";
        return;
    }

    //Save the flux in the file
    QTextStream flux(&writeConfig);
    writeXML.save(flux, 4);
    writeConfig.close();
}

void MainWindow::on_actionOptions_triggered()
{
    OptionWindow newWindow;
    newWindow.set_Editor(m_editor);
    newWindow.set_customMode(m_customMode);
    newWindow.set_customSourcePath(m_customSourcePath);
    newWindow.exec();
    m_editor = newWindow.get_Editor();
    m_customMode = newWindow.get_customMode();
    m_customSourcePath = newWindow.get_customSourcePath();
    save_PersonalConfig();
}

void MainWindow::on_actionShow_unused_string_triggered()
{
    if(uiOpenWindow->treeApps->topLevelItemCount() != 0) {
        set_TreeProject();
    }
    save_PersonalConfig();
}
