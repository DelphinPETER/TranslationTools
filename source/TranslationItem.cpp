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

#include "TranslationItem.h"

TranslationItem::TranslationItem(){
}

TranslationItem::TranslationItem(QDomElement p_DomElement){

    m_Product = p_DomElement.tagName();
    m_isUnused = false;

    m_Type = p_DomElement.tagName();
    m_StringName = p_DomElement.attribute("name");
    m_SourceLine = p_DomElement.lineNumber();

    if (p_DomElement.tagName() == "string"){

        if(p_DomElement.hasAttribute("product")) {
            m_Product = p_DomElement.attribute("product");
        }
        m_SourceText = p_DomElement.text();


    } else if (p_DomElement.tagName() == "plurals"){

        QDomElement item = p_DomElement.firstChildElement();
        while(!item.isNull()){
            m_PluralsSourceText[item.attribute("quantity")] = item.text();
            item = item.nextSiblingElement();
        }

    } else if (p_DomElement.tagName() == "string-array"){

        QDomElement item = p_DomElement.firstChildElement();
        while(!item.isNull()){
            m_ArraySourceText.push_back(item.text());
            item = item.nextSiblingElement();
        }
    }
}

void TranslationItem::setSourceFileName(QString p_SourceFile)
{
    m_Filename = p_SourceFile;
}

QString TranslationItem::getStringName() const
{
    return m_StringName;
}

QString TranslationItem::getProduct() const
{
    return m_Product;
}

QString TranslationItem::getSourceText() const
{
    QString result;

    if(m_Type == "string"){
        result = m_SourceText;

    } else if (m_Type == "plurals"){
        QMapIterator<QString, QString> itrTranslation(m_PluralsSourceText);
        while(itrTranslation.hasNext()){
            itrTranslation.next();
            result = result + "quantity=" + itrTranslation.key() + " : " + itrTranslation.value() + "\n";
        }


    } else if (m_Type == "string-array"){
        for (int i = 0; i < m_ArraySourceText.size(); ++i){
            result = result + "index=" + QString::number(i+1) + " : " + m_ArraySourceText.value(i) + "\n";
        }
    }

    return result;

}

int TranslationItem::getLineNumber() const
{
    return m_SourceLine;
}

QString TranslationItem::getFilename() const
{
    return m_Filename;
}

QString TranslationItem::getType() const
{
    return m_Type;
}

bool TranslationItem::isUnused() const
{
    return m_isUnused;
}

void  TranslationItem::setUnusedItem(bool p_unused)
{
    m_isUnused = p_unused;
}
