#ifndef REGWATCHEREXPORTSTRUCT_H
#define REGWATCHEREXPORTSTRUCT_H

#include <QString>

typedef struct RegWatcherExportStruct
{
    QString name;
    int type;
    int address;
    int regbank;
} RegWatcherExportStruct;


#endif