/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file compatibilitymode.h
 */


#ifndef COMPATIBILITYMODE_H
#define COMPATIBILITYMODE_H


#include <QList>
#include <QString>
#include <QPair>
#include <map>





/**
 * @brief
 * @ingroup GUI
 * @class CompatibilityMode
 */
class CompatibilityMode
{ 
    public:
        CompatibilityMode();
        int getOrigLine(QString file, int newLine);
        void appendFileMap(QString file, std::map<unsigned int, unsigned int> map);
        void clearFileMaps();
        bool loaded();

    private:
        QList<QPair<QString, std::map<unsigned int, unsigned int>>> m_maps;
        
};


#endif
