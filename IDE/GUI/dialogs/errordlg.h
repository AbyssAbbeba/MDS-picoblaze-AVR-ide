#ifndef ERRORDLG_H
#define ERRORDLG_H

#define ERR_OPENFILE 2000
#define ERR_XML_CONTENT 2001
#define ERR_XML_ASSIGN 2002

#include <QMessageBox>

void error(int errCode);

#endif
