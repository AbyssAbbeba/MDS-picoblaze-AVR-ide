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


void CompatibilityMode::appendFileMap(QString file, std::map<unsigned int, unsigned int> map)
{
    QPair<QString, std::map<unsigned int, unsigned int>> fileMap;
    fileMap.first = file;
    fileMap.second = map;
    m_maps.append(fileMap);
}


void CompatibilityMode::clearFileMaps()
{
    m_maps.clear();
}


bool CompatibilityMode::loaded()
{
    return (m_maps.count() > 0) ? true : false;
}