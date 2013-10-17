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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uiOpenWindow(new Ui::MainWindow)
{
    uiOpenWindow->setupUi(this);

    //Fix column width/hidden
    uiOpenWindow->treeApps->setColumnWidth(0, 400);
    uiOpenWindow->treeApps->setColumnWidth(1, 200);
    uiOpenWindow->treeApps->setColumnHidden(2, true);

    //Text selectable
    uiOpenWindow->label_FilePathText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    uiOpenWindow->label_InfoTypeText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    uiOpenWindow->label_LineNumberText->setTextInteractionFlags(Qt::TextSelectableByMouse);

}

MainWindow::~MainWindow()
{
    delete uiOpenWindow;
}

void MainWindow::on_pushButton_Scan_clicked()
{
    //Save user configuration
    save_PersonalConfig();

    //Clear application window and value
    m_resourceID_sourceString.clear();
    m_resourceID_translationString.clear();
    m_resourceID_untranslatedString.clear();
    m_translatableCounter = 0;
    int appNumber = 0;
    uiOpenWindow->treeApps->clear();
    uiOpenWindow->progressBar_scan->setValue(0);
    uiOpenWindow->label_FilePathText->clear();
    uiOpenWindow->label_InfoTypeText->clear();
    uiOpenWindow->label_LineNumberText->clear();

    //Scan application list
    QMapIterator<QString, QString> itrApps(m_resourceID_Name);
    while(itrApps.hasNext()) {
        itrApps.next();
        appNumber++;

        //Scan source file
        scan_SourcePath(itrApps.key());

        //Scan translation file
        scan_TranslationPath(itrApps.key(), m_language);

        if(m_language_subst.contains(m_language)){
            scan_TranslationPath(itrApps.key(), m_language_subst.value(m_language));
        }

        //Compare source and translation strings
        compare_SourceTranslation(itrApps.key());

        //Define current value of progress bar
        uiOpenWindow->progressBar_scan->setValue(appNumber);

    }

    if(m_stringID_stringName.isEmpty()){
        QMessageBox::warning(this, tr("No XML files found"), tr("No xml files are found. Make sure you launch TranslationTool in source folder or that your custom configuration is correct."), QMessageBox::Ok);
        qDebug() << "ERROR: No files found";
        return;
    }

    //Display the result
    set_TreeProject();

}

void MainWindow::compare_SourceTranslation(QString resourceID)
{
    QVector<QString> sourceStringList = m_resourceID_sourceString.value(resourceID);
    QVector<QString> translateStringList = m_resourceID_translationString.value(resourceID);
    QVector<QString> skipStringList = m_resourceID_skipStringList.value(resourceID);

    //Lists untranslated strings
    QVector<QString> untranslatedStringList;
    QVectorIterator<QString> itrSourceString(sourceStringList);
    while(itrSourceString.hasNext()) {
        QString sourceString = itrSourceString.next();
        if(!skipStringList.contains(sourceString)) {
            if(!translateStringList.contains(sourceString)) {
                untranslatedStringList.push_back(sourceString);
            }
        }
    }
    m_resourceID_untranslatedString[resourceID] = untranslatedStringList;

    //List unused strings
    QVector<QString> unusedStringList;
    QVectorIterator<QString> itrTranslationString(translateStringList);
    while(itrTranslationString.hasNext()) {
        QString translationString = itrTranslationString.next();
        if(!skipStringList.contains(translationString)) {
            if(!sourceStringList.contains(translationString)) {
                unusedStringList.push_back(translationString);
            }
        }
    }
    m_resourceID_unusedString[resourceID] = unusedStringList;

}

void MainWindow::set_TreeProject()
{
    uiOpenWindow->treeApps->clear();
    int untranslatedCounterFull = 0;
    int untranslatedCounterApp = 0;

    QMapIterator<QString, QVector<QString> > itr(m_resourceID_untranslatedString);
    while(itr.hasNext()) {
        itr.next();
        untranslatedCounterApp = 0;
        QTreeWidgetItem* item = new QTreeWidgetItem;

        item->setText(0,m_resourceID_Name.value(itr.key()));
        item->setText(2, itr.key());

        //Add item of string UNTRANSLATED
        QVector<QString> untranslatedList = itr.value();
        QVectorIterator<QString> itrUntranslatedList(untranslatedList);
        while (itrUntranslatedList.hasNext()) {
            QString stringName = itrUntranslatedList.next();
            QTreeWidgetItem* untransItem = new QTreeWidgetItem;
            untransItem->setText(0, m_stringID_stringName.value(stringName));
            untransItem->setText(2, stringName);
            item->addChild(untransItem);
            untranslatedCounterApp++;
        }

        //Add item of string UNUSED
        QVector<QString> unusedStringList = m_resourceID_unusedString.value(itr.key());
        if(!unusedStringList.isEmpty()) {
            QVectorIterator<QString> itrUnusedList(unusedStringList);
            while (itrUnusedList.hasNext()) {
                QString unUsedStringName = itrUnusedList.next();
                QTreeWidgetItem* unusedItem = new QTreeWidgetItem;
                unusedItem->setText(0, m_stringID_stringName.value(unUsedStringName));
                unusedItem->setTextColor(0, Qt::red);
                unusedItem->setText(1, tr("unused"));
                unusedItem->setTextColor(1, Qt::red);
                unusedItem->setText(2, unUsedStringName);
                unusedItem->setTextColor(0, Qt::red);
                item->setTextColor(0, Qt::red);
                item->addChild(unusedItem);
            }
        }

        item->setText(1,QString::number(untranslatedCounterApp));


        untranslatedCounterFull += untranslatedCounterApp;

        if(untranslatedCounterApp > 0) {
            uiOpenWindow->treeApps->addTopLevelItem(item);
        } else if (!unusedStringList.isEmpty()) {
            uiOpenWindow->treeApps->addTopLevelItem(item);
        }

    }

    //No translation find, good work !!!
    if(uiOpenWindow->treeApps->topLevelItemCount() == 0) {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(0,tr("No translation needed, good work!"));
        uiOpenWindow->treeApps->addTopLevelItem(item);
        uiOpenWindow->pushButton_saveCurrentList->setEnabled(false);
    } else {
        uiOpenWindow->pushButton_saveCurrentList->setEnabled(true);
    }

    //Add statistics
    int pourcent(((m_translatableCounter - untranslatedCounterFull)*100) / m_translatableCounter );
    uiOpenWindow->label_ProgressionTranslated->setText(tr("Progress:") + QString::number(pourcent) + "%");
    QStringList columnTitle;
    columnTitle << tr("Apps name") << (tr("Untranslated") + " (" + QString::number(untranslatedCounterFull) + "/" +QString::number(m_translatableCounter) + ")" );
    uiOpenWindow->treeApps->setHeaderLabels(columnTitle);
    uiOpenWindow->treeApps->resizeColumnToContents(1);
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

        if(m_stringID_file.contains(stringID)) {
            uiOpenWindow->label_FilePathText->setText(resourceID + "/" + m_stringID_file.value(stringID) );
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
    }

}

void MainWindow::on_treeApps_itemDoubleClicked()
{
    QTreeWidgetItem *item = uiOpenWindow->treeApps->currentItem();

    //If the item is a string we open the editor
    if(uiOpenWindow->treeApps->indexOfTopLevelItem(item) == -1) {
        QString resourceID = item->parent()->text(2);
        QProcess process;

        //Open the translation file
        QString openTransaltionFile = m_editor;
        //Replace file path
        openTransaltionFile.replace( "%1", m_rootDir + "/" + resourceID + "-" + m_language + "/" + m_stringID_file.value(item->text(2)) );
        //replace line number
        openTransaltionFile.replace( "%2", QString::number(1) );
        //launch command
        process.startDetached(openTransaltionFile);

        //Open the source file
        QString openSourceFile = m_editor;
        //Replace file path
        openSourceFile.replace( "%1", m_rootDir + "/" + resourceID + "/" + m_stringID_file.value(item->text(2)) );
        //replace line number
        openSourceFile.replace( "%2", QString::number(m_stringID_lineNumber.value(item->text(2))) );
        //launch command
        process.startDetached(openSourceFile);

    }

}

void MainWindow::scan_SourcePath(QString sourcePath)
{
    //Lists files in source folder
    QStringList fileList = get_FileList(sourcePath);

    QStringListIterator itrListFile(fileList);
    while(itrListFile.hasNext()) {
        QString filename = itrListFile.next();
        QVector<QString> skipFileList = m_resourceID_skipFileList.value(sourcePath);
        if(!skipFileList.contains(filename)) {
            load_SourceFile(sourcePath, filename);
        }
    }

}

void MainWindow::load_SourceFile(QString sourcePath, QString filename)
{
    QFile fileToParse(m_rootDir + "/" + sourcePath + "/" + filename);
    if( !fileToParse.open( QIODevice::ReadOnly ) ) {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "/" + filename + "<===== file not found";
    }

    QDomDocument xmlFile( "Ficher de Traduction" );
    if( !xmlFile.setContent( &fileToParse ) )
    {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "/" + filename + "<===== file not loading";
        fileToParse.close();
    }
    fileToParse.close();

    //Define root
    QDomElement root = xmlFile.documentElement();
    if( root.tagName() != "resources" ) {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "/" + filename + "<===== file corrupt";
    }

    QDomElement parentItem = root.firstChildElement();

    QString type;
    QVector<QString> stringFound;

    while( !parentItem.isNull() )
    {
        type = parentItem.tagName();
        if (type == "string" || type == "plurals" || type == "string-array") {
            if( parentItem.hasAttribute("name") && parentItem.attribute("translatable") != "false") {
                QString NameID;
                //Define the ID
                if(parentItem.hasAttribute("product")) {
                    NameID = sourcePath + "@" + parentItem.tagName() + "@" + parentItem.attribute("name") + "-" + parentItem.attribute("product");
                    m_stringID_stringName[NameID] = parentItem.attribute("name") + "-" + parentItem.attribute("product");
                } else {
                    NameID = sourcePath + "@" + parentItem.tagName() + "@" + parentItem.attribute("name");
                    m_stringID_stringName[NameID] = parentItem.attribute("name");
                }
                stringFound.push_back(NameID);
                m_stringID_lineNumber[NameID] = parentItem.lineNumber();
                m_stringID_file[NameID] = filename;
                m_stringID_type[NameID] = parentItem.tagName();

                m_translatableCounter++;

            }

        }
        parentItem = parentItem.nextSiblingElement();
    }

    if( !m_resourceID_sourceString.value(sourcePath).empty() ) {
        stringFound += m_resourceID_sourceString.value(sourcePath);
    }

    m_resourceID_sourceString[sourcePath] = stringFound;

}

void MainWindow::scan_TranslationPath(QString translationPath, QString language)
{
    //Lists files in translation folder
    QStringList fileList = get_FileList(translationPath + "-" + language);

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

    QFile fileToParse(m_rootDir + "/" + sourcePath + "-" + language + "/" + filename);
    if( !fileToParse.open( QIODevice::ReadOnly ) ) {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "-" + language + "/" + filename + "<===== file not found";
    }

    QDomDocument xmlFile( "Translation_file" );
    if( !xmlFile.setContent( &fileToParse ) )
    {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "-" + language + "/" + filename + "<===== file not loading";
        fileToParse.close();
    }
    fileToParse.close();

    //Define root
    QDomElement root = xmlFile.documentElement();
    if( root.tagName() != "resources" ) {
        qDebug() << "ERROR: " + m_rootDir + "/" + sourcePath + "-" + language + "/" + filename + "<===== file corrupt";
    }

    QDomElement parentItem = root.firstChildElement();

    QString type;
    QVector<QString> stringFound;

    while( !parentItem.isNull() )
    {
        type = parentItem.tagName();
        if (type == "string" || type == "plurals" || type == "string-array") {
            if( parentItem.hasAttribute("name") && parentItem.attribute("translatable") != "false") {
                QString NameID;
                //Define string ID
                if(parentItem.hasAttribute("product")) {
                    NameID = sourcePath + "@" + parentItem.tagName() + "@" + parentItem.attribute("name") + "-" + parentItem.attribute("product");
                    if(!m_stringID_stringName.contains(NameID)) {
                        m_stringID_stringName[NameID] = parentItem.attribute("name") + "-" + parentItem.attribute("product");
                    }
                } else {
                    NameID = sourcePath + "@" + parentItem.tagName() + "@" + parentItem.attribute("name");
                    if(!m_stringID_stringName.contains(NameID)) {
                        m_stringID_stringName[NameID] = parentItem.attribute("name");
                    }
                }
                stringFound.push_back(NameID);

                if(!m_stringID_type.contains(NameID)) {
                    m_stringID_type[NameID] = parentItem.tagName();
                }

            }

        }
        parentItem = parentItem.nextSiblingElement();
    }


    if( !m_resourceID_translationString.value(sourcePath).empty() ) {
        stringFound += m_resourceID_translationString.value(sourcePath);
    }

    m_resourceID_translationString[sourcePath] = stringFound;

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

void MainWindow::load_ApplicationXMLFile()
{
    load_ProjectXMLFile();

    load_LanguageXMLFile();

    load_FilterXMLFile();

}

void MainWindow::load_FilterXMLFile()
{
    QFile globalFilterFile(QCoreApplication::applicationDirPath() + "/filter.xml");
    if( !globalFilterFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning(this, tr("File not found"), tr("The file filter.xml was not found, make sure you launch TranslationTool in source folder or that your custom configuration is correct."), QMessageBox::Ok);
        qDebug() << "ERROR: filter.xml file not found";
        exit(0);
    }

    QDomDocument globalFilterDocument( "Global_Filter" );
    if( !globalFilterDocument.setContent( &globalFilterFile ) )
    {
        qDebug() << "ERROR: filter.xml file not found";
        globalFilterFile.close();
    }
    globalFilterFile.close();

    //Define xml element
    QDomElement globalFilterRoot = globalFilterDocument.documentElement();
    QDomElement projectItem = globalFilterRoot.firstChildElement();

    QVector<QString> skipFileList;
    QVector<QString> skipStringList;

    while( !projectItem.isNull() ) {

        QDomElement skipItem = projectItem.firstChildElement();

        while(!skipItem.isNull())
        {
            if(skipItem.tagName() == "skip-file") {
                skipFileList.push_back(skipItem.attribute("name"));
            }
            else if (skipItem.tagName() == "skip-string") {
                skipStringList.push_back(m_resourceID_Name.key(projectItem.attribute("name")) + "@" + skipItem.attribute("type") + "@" + skipItem.attribute("name"));
            }
            skipItem = skipItem.nextSiblingElement();
        }

        m_resourceID_skipStringList[m_resourceID_Name.key(projectItem.attribute("name"))] = skipStringList;
        m_resourceID_skipFileList[m_resourceID_Name.key(projectItem.attribute("name"))] = skipFileList;

        //Init
        skipStringList.clear();
        skipFileList.clear();

        projectItem = projectItem.nextSiblingElement();
    }

}

void MainWindow::load_LanguageXMLFile()
{
    QFile languageFile(QCoreApplication::applicationDirPath() + "/language.xml");
    if( !languageFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning(this, tr("File not found"), tr("The file language.xml was not found, make sure you launch TranslationTool in source folder or that your custom configuration is correct."), QMessageBox::Ok);
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
    QFile projectFile(QCoreApplication::applicationDirPath() + "/project.xml");
    if( !projectFile.open( QIODevice::ReadOnly ) ) {
        QMessageBox::warning(this, tr("File not found"), tr("The file project.xml was not found, make sure you launch TranslationTool in source folder or that your custom configuration is correct."), QMessageBox::Ok);
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

        //Define custom_source_path
        if(itemSetting.attribute("name") == "custom_source_path") {
            m_customSourcePath = itemSetting.text();
        }

        itemSetting = itemSetting.nextSiblingElement();
    }

    if (m_customMode) {
        m_rootDir = m_customSourcePath;
    } else {
        m_rootDir = QCoreApplication::applicationDirPath().remove("/tools/TranslationTools/binary");
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
    newWindow.exec();
    m_editor = newWindow.get_New();
    save_PersonalConfig();
}

void MainWindow::on_pushButton_saveCurrentList_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save current list"), QDir::homePath());

    //Define head of xml file
    QDomDocument writeXML;
    QDomNode xmlNode = writeXML.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    writeXML.insertBefore(xmlNode, writeXML.firstChild());

    //Add comment header
    QString commentHeader;
    commentHeader.push_back("\nTranslation Tools export\n");
    commentHeader.push_back(QDateTime::currentDateTime().toString() + "\n\n");
    commentHeader.push_back("Source directory = " + m_rootDir + "\n");
    commentHeader.push_back("Language = " + m_language + "\n");
    writeXML.appendChild(writeXML.createComment(commentHeader));
    writeXML.appendChild(writeXML.createTextNode("\n"));

    //Define item : root
    QDomElement globalExport = writeXML.createElement("export");
    writeXML.appendChild(globalExport);

    QMapIterator<QString, QVector<QString> > itr(m_resourceID_untranslatedString);
    while(itr.hasNext()) {
        itr.next();

        //Define item : project
        QDomElement itemProject = writeXML.createElement("project");
        itemProject.setAttribute("name", itr.key());

        QVectorIterator<QString> itrUntranslatedList = itr.value();
        if(itrUntranslatedList.hasNext()) {
            while (itrUntranslatedList.hasNext()) {

                //Define item : string
                QString StringID = itrUntranslatedList.next();
                QDomElement itemString = writeXML.createElement("item");
                itemString.setAttribute("type", m_stringID_type.value(StringID));
                QDomText string = writeXML.createTextNode(m_stringID_stringName.value(StringID));
                itemString.appendChild(string);
                itemProject.appendChild(itemString);
            }
        }

        if(itemProject.hasChildNodes()) {
            globalExport.appendChild(itemProject);
        }
    }

    //Open the file for write
    QFile writeFile(filename);
    if (!writeFile.open(QIODevice::WriteOnly)) {
        qDebug() << "ERROR: unable to open file for writing";
        return;
    }

    //Save the flux in the file
    QTextStream flux(&writeFile);
    writeXML.save(flux, 4);
    writeFile.close();

}
