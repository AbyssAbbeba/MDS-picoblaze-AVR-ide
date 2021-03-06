/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CompatibilityMode
 * @file compatibilitymode.cpp
 */


#include <QtGui>
#include <stdexcept>
#include "compatibilitymode.h"

CompatibilityMode::CompatibilityMode()
{
    //this->setFont(QFont("Andale Mono", 10));
}


int CompatibilityMode::getOrigLine(QString file, int newLine)
{
    qDebug() << "CompatibilityMode: getOrigLine from file" << file;
    for (int i = 0; i < m_maps.count(); i++)
    {
        if (file == m_maps.at(i).first)
        {
            try
            {
                return m_maps.at(i).second.at(newLine);
            }
            catch (const std::out_of_range& oor)
            {
                break;
            }
        }
    }
    return -1;
}


int CompatibilityMode::getNewLine(QString file, int origLine)
{
    qDebug() << "CompatibilityMode: getNewLine from file" << file;
    for (int i = 0; i < m_mapsReverse.count(); i++)
    {
        if (file == m_mapsReverse.at(i).first)
        {
            try
            {
                qDebug() << "CompatibilityMode: line" << m_mapsReverse.at(i).second.at(origLine);
                return m_mapsReverse.at(i).second.at(origLine);
            }
            catch (const std::out_of_range& oor)
            {
                break;
            }
        }
    }
    return -1;
}


void CompatibilityMode::appendFileMap(QString file, std::map<unsigned int, unsigned int> map)
{
    QPair<QString, std::map<unsigned int, unsigned int>> fileMap;
    fileMap.first = QDir::cleanPath(file);
    fileMap.second = map;
    m_maps.append(fileMap);
}


void CompatibilityMode::appendReverseFileMap(QString file, std::map<unsigned int, unsigned int> map)
{
    QPair<QString, std::map<unsigned int, unsigned int>> fileMap;
    fileMap.first = QDir::cleanPath(file);
    fileMap.second = map;
    m_mapsReverse.append(fileMap);
}


void CompatibilityMode::clearFileMaps()
{
    m_maps.clear();
    m_mapsReverse.clear();
}


bool CompatibilityMode::loaded()
{
    return (m_maps.count() > 0) ? true : false;
}