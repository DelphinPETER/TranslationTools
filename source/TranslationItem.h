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

#ifndef TRANSLATIONITEM_H
#define TRANSLATIONITEM_H

#include <TranslationItem.h>
#include <QDomElement>
#include <QTextStream>

class TranslationItem
{
public:
    TranslationItem();
    TranslationItem(QDomElement p_DomElement);
    void setSourceFileName(QString m_SourceFile);
    void setUnusedItem(bool p_unused);
    QString getStringName() const;
    QString getProduct() const;
    QString getSourceText() const;
    QString getFilename() const;
    QString getType() const;
    int getLineNumber() const;
    bool isUnused() const;

private:
    QString m_StringName;
    QString m_Type;
    QString m_SourceText;
    QString m_TranslationText;
    QString m_Filename;
    QString m_Product;
    bool m_isUnused;
    int m_SourceLine;
    QMap<QString, QString> m_PluralsSourceText;
    QMap<QString, int> m_PluralsSourceLine;
    QStringList m_ArraySourceText;

};

#endif // TRANSLATIONITEM_H
