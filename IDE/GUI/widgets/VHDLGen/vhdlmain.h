// =============================================================================
/**
* @brief
* C++ interface:  Automatic generation of VHDL code for Picoblaze hardware components
*
*
* (C) copyright 2013 Moravia Microsystems, s.r.o.
*
* @author Martin Madron <martin.madron@moravia-microsystems.com>
* @ingroup PicoBlaze
* @file vhdlmain.h
*/
// =============================================================================
#ifndef VHDLMAIN_H
#define VHDLMAIN_H

#include <QWidget>
#include <QtGui>
#include "ui_createcomponent.h"
#include "ui_rectclicked.h"
#include "xmlparser.h"

namespace Ui {
class VhdlMain;
}
class XmlParser;
/**
* @brief
* @ingroup PicoBlaze
* @class VhdlMain
*/

class VhdlMain : public QWidget
{
    Q_OBJECT
    XmlParser xmlParser;
    
    //// Public Datatypes ////
    public:
    explicit VhdlMain(QWidget *parent = 0);
    ~VhdlMain();

    //QPoint p1(int u,int z);
    /**
    * @brief Variable list
    */

     QStringList printedComponents, printedComponentsDeclaration;
     QStringList longerList, splitList2, splitList1;
     QStringList portList, portInList, portOutList;
     QStringList rectList;
     QStringList bootDeviceList;

     QStringList existingNamesList;

     unsigned int portCount,portInCount,portOutCount;
     unsigned int componentCnt;
     unsigned int xPos,yPos, yPosNew;
     unsigned int m_spaceNum;
     unsigned int m_xmlEditSpaceNum;
     bool m_componentRectHighlight;
     bool m_saveAddFlag;
     bool m_xmlNumberFlag;
     bool m_xmlEditFlag;
     bool createWidget;
     unsigned int m_xmlNumber,m_editNumber;

     enum genericType {
         LOGIC = 0,
         LOGIC_VECTOR = 1,
         INTEGER = 2,
         POSITIVE = 3,
         NATURAL = 4,
         REAL = 5,
         STRING = 6,
         INVALID = 7
     };

     /**
     * @brief Structures containing user defined hardware component data
     */
     struct port
     {
         QString name;
         QString type;
         int value;
     } portInOut[512], portIn[256], portOut[256];

     struct component
     {
         QString name;
         QStringList portName;
         QStringList portValue;
         QStringList portDirection;
         bool bus[17];
         bool signal[17];
         QStringList msbNumber;
         QStringList lsbNumber;
         //QStringList attributes;
     } definedComponent[21],picoBlaze;

     struct generic
     {
         QStringList genericName;
         QStringList genericValue;
         QStringList portType;
         bool bus[8];
         bool constant[8];
         QStringList msbNumber;
         QStringList lsbNumber;

     } genericComponent[21];

     QString entityName;
     QString loadPath,savePath;
     //functions
     void splitList(unsigned int count, unsigned int portType);
     void componentWizardInitialization();
     void componentWizardConstruct();
     void clearStructures();
     unsigned int tabsNumber(QString inputString);
     void addExistingComponent(unsigned int);
     void validateName(unsigned int);
     genericType getEnum(QString & inString);
     int getNumberOfInstances(QString & componentToInstantiate, bool Declaration);
     void editSelectedComponent(unsigned int position);

private slots:
   void customMenuRequested(QPoint pos);
   void RemoveXml();
   void pushEditSlot();
   void saveAdd();
   void busChecked();
   void saveFile();
   void loadFile();
   void createComponent1();
   void printToFile();
   void pushOk();
   void pushCancel();
   void pushDelete();
   void insertItem(QListWidgetItem *componentItem);
   void typeIndexChanged1(int index);
   void typeIndexChanged2(int index);
   void typeIndexChanged3(int index);
   void typeIndexChanged4(int index);
   void typeIndexChanged5(int index);
   void typeIndexChanged6(int index);
   void typeIndexChanged7(int index);
   void typeIndexChanged8(int index);
   /**
   * @brief Validation of inserted number (bus/logic)
   */
   void valueEditingFinished(QString textValue);
    
private:
    QRegExpValidator *valueBinValidator;
    QRegExpValidator *valueValidator;
    bool KCPSMhighlight;
    bool inHighlight;
    bool outHighlight;
    bool componentHighlight[20];
    QRect *KCPSM3;
    QRect *KCPSM;
    QRect *MUX_in;
    QRect *MUX_out;
    QRect *componentRect;
    QRect *componentObject[20];
    QWidget *componentWizard;
    QWidget *RectInfo;
    Ui::VhdlMain *ui;
    Ui_CreateComponent ui2;
    Ui_RectClick ui3;

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent* pressEvent);
    void mouseMoveEvent ( QMouseEvent * moveEvent );
};

#endif // VHDLMAIN_H
