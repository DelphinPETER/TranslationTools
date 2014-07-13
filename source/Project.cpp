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

#include "Project.h"

Project::Project(){}


int Project::m_TotalString = 0;

Project::Project(QString p_ProjectID, QString p_ProjectName, QString p_SourceFolder){
    m_ProjectID = p_ProjectID;
    m_ProjectName = p_ProjectName;
    m_ProjectSourceFolder = p_SourceFolder;
}

void Project::setProjectID(QString p_ProjectID)
{
    m_ProjectID = p_ProjectID;

    QTextStream(stdout) << "========== " << m_ProjectID << "\n";
    QTextStream(stdout) << "=========================================================" << "\n";
}

void Project::clear()
{
    m_TotalString = 0;
}

void Project::setProjectName(QString p_ProjectName)
{
    m_ProjectName = p_ProjectName;
}

QString Project::getFolder() const
{

    return m_ProjectSourceFolder;
}

void Project::setSourceFolder(QString p_SourceFolder)
{
    m_ProjectSourceFolder = p_SourceFolder;
}

void Project::addSkipFile(QString p_SkipFile)
{
    m_skipFile.push_back(p_SkipFile);
}

void Project::addSkipString(QString p_SkipStringName, QString p_StringType)
{
    m_skipStringType[p_SkipStringName] = p_StringType;
}

void Project::scanSource()
{
    //Lists files in folder
    QStringList fileList = getFileList(m_ProjectSourceFolder);
    QStringListIterator itrListFile(fileList);
    while(itrListFile.hasNext()) {
        QString filename = itrListFile.next();
        if(!m_skipFile.contains(filename)){
            loadSourceFile(filename);
        }
    }
}

void Project::scanTranslation(QString p_language)
{
    QString filepath = m_ProjectSourceFolder + "-" + p_language;

    //Lists files in folder
    QStringList fileList = getFileList(filepath);

    QStringListIterator itrListFile(fileList);
    while(itrListFile.hasNext()) {
        QString filename = itrListFile.next();
        if(!m_skipFile.contains(filename)){
            loadTranslationFile(filename, p_language);
        }
    }

}

QString Project::getName() const
{
    return m_ProjectName;
}

int Project::getUntranslatedNumber() const
{
    return m_UntranslatedItemList.size();
}

bool Project::isTranslated() const
{
    if(m_UntranslatedItemList.size() == 0){
        return true;
        QTextStream(stdout) << "Project translated " << "\n";
    } else {
        return false;
        QTextStream(stdout) << "Project untranslated " << "\n";
    }
}

bool Project::hasUnused() const
{
    if(m_UnusedItemList.size() == 0){
        return false;
        QTextStream(stdout) << "Project has unused " << "\n";
    } else {
        return true;
        QTextStream(stdout) << "Project has not unused " << "\n";
    }
}

QMap<QString, TranslationItem> Project::getTranslationItems() const
{
    return m_UntranslatedItemList;
}

QMap<QString, TranslationItem> Project::getUnusedItems() const
{
    return m_UnusedItemList;
}

void Project::loadTranslationFile(QString p_translationFileDirectory, QString p_Language)
{

    QDomElement root = loadXMLFile(m_ProjectSourceFolder + "-" + p_Language + "/" + p_translationFileDirectory);
    QDomElement parentItem = root.firstChildElement();

    while( !parentItem.isNull() ) {

        if(parentItem.tagName() == "string" || parentItem.tagName() == "string-array" || parentItem.tagName() == "plurals") {

            if (parentItem.attribute("translatable") != "false" && parentItem.attribute("translate") != "false") {

                QString itemName;
                if(parentItem.hasAttribute("product")){
                    itemName = parentItem.attribute("name") + "-" + parentItem.attribute("product");
                } else {
                    itemName = parentItem.attribute("name");
                }

                if(!m_skipStringType.contains(itemName) ){
                    if(m_UntranslatedItemList.contains(itemName)){
                        TranslationItem translationItem = m_UntranslatedItemList.value(itemName);
                        if(translationItem.getType() == parentItem.tagName()){
                            //translationItem.setSourceFileName(p_TranslationFileFilepath);
                            m_UntranslatedItemList.remove(itemName);
                        }
                    } else {
                        TranslationItem translationItem(parentItem);
                        translationItem.setSourceFileName(m_ProjectSourceFolder + "-" + p_Language + "/" + p_translationFileDirectory);
                        translationItem.setUnusedItem(true);
                        m_UnusedItemList[itemName] = translationItem;
                    }
                }

            }

        }

        parentItem = parentItem.nextSiblingElement();
    }

}

void Project::loadSourceFile(QString p_SourceFile)
{

    QDomElement root = loadXMLFile(m_ProjectSourceFolder + "/" + p_SourceFile);
    QDomElement parentItem = root.firstChildElement();

    while( !parentItem.isNull() )
    {
        if(parentItem.tagName() == "string" || parentItem.tagName() == "string-array" || parentItem.tagName() == "plurals"){

            QString itemName;
            if(parentItem.hasAttribute("product")){
                itemName = parentItem.attribute("name") + "-" + parentItem.attribute("product");
            } else {
                itemName = parentItem.attribute("name");
            }

            if (parentItem.attribute("translatable") != "false" && parentItem.attribute("translate") != "false"){
                if(!m_skipStringType.contains(parentItem.attribute("name"))){
                    TranslationItem translationItem(parentItem);
                    translationItem.setSourceFileName(m_ProjectSourceFolder + "/" + p_SourceFile);

                    m_TotalString++;
                    m_UntranslatedItemList[itemName] = translationItem;
                }
            }
        }

        parentItem = parentItem.nextSiblingElement();

    }

}

QStringList Project::getFileList(QString filepath)
{
    //Define QDir path
    QDir dir(filepath);

    //Add xml file filter
    QStringList filters;
    filters << "*.xml";
    dir.setNameFilters(filters);

    return dir.entryList();
}

QDomElement Project::loadXMLFile(QString p_Filepath)
{

    QFile file(p_Filepath);
    if( !file.open( QIODevice::ReadOnly ) ) {
        QTextStream(stdout) << "ERROR:" << p_Filepath << " not found";
        exit(0);
    }

    QDomDocument domDocument( "domDocument" );
    if( !domDocument.setContent( &file ) )
    {
        QTextStream(stdout) << "ERROR:" << p_Filepath << " not found";
        file.close();
    }
    file.close();

    QDomElement root = domDocument.documentElement();

    if( root.tagName() != "resources" ) {
        QTextStream(stdout) << "ERROR: " + p_Filepath + "<===== file corrupt";
    }

    return root;

}
