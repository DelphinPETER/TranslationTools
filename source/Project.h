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

#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QDomDocument>
#include <QVector>

#include <TranslationItem.h>

class Project
{
public:
    Project();
    Project(QString p_ProjectID, QString p_ProjectName, QString p_SourceFolder);
    static int m_TotalString;
    static void clear();

public slots:
    void setProjectID(QString p_ProjectID);
    void setProjectName(QString p_ProjectName);
    void setSourceFolder(QString p_SourceFolder);
    void addSkipFile(QString p_SkipFile);
    void addSkipString(QString p_SkipString, QString p_StringType);
    QString getName() const;
    QString getFolder() const;
    QMap<QString, TranslationItem> getTranslationItems() const;
    QMap<QString, TranslationItem> getUnusedItems() const;
    int getUntranslatedNumber() const;
    bool hasUnused() const;
    bool isTranslated() const;
    void scanSource();
    void scanTranslation(QString p_language);

private slots:
    QStringList getFileList(QString filepath);
    void loadSourceFile(QString m_SourceFileFilepath);
    void loadTranslationFile(QString p_translationFileDirectory, QString p_Language);
    QDomElement loadXMLFile(QString p_Filepath);

private:
    QString m_ProjectID;
    QString m_ProjectName;
    QString m_ProjectSourceFolder;
    QMap<QString, TranslationItem> m_UntranslatedItemList;
    QMap<QString, TranslationItem> m_UnusedItemList;
    QVector<QString> m_skipFile;
    QMap<QString, QString> m_skipStringType;

};

#endif // PROJECT_H
