/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file errordlg.cpp
 */


#include "errordlg.h"
#include <QStyle>
#include <QDebug>
#include "ui_errordlg.h"

/**
 * @brief Shows error according to given code
 * @param errCode Error code
 */
void error(int errCode, QString detail)
{
    QDialog msgBox;
    Ui_ErrorDialog ui;
    ui.setupUi(&msgBox);
    ui.teDetail->clear();
    switch (errCode)
    {
        case ERR_OPENFILE:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            //ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("File not found or cannot be opened");
            ui.teDetail->appendPlainText("File: " + detail);
            qDebug() << "Error: file not found - " << detail;
            break;
        }
        case ERR_XML_ASSIGN:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("XML file cannot be assigned");
            break;
        }
        case ERR_XML_CONTENT:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("XML file has wrong content");
            break;
        }
        case ERR_NO_PROJECT:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Active project is not set");
            break;
        }


        //simulation errors
        //stack
        case ERR_STACK_OVERFLOW:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Stack overflow");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_STACK_UNDERFLOW:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Stack underflow");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        //mem
        case ERR_MEM_WR_NONEXISTENT:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Write to nonexistent memory location");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_MEM_RD_NONEXISTENT:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Read from nonexistent memory location");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_MEM_WR_NOT_IMPLEMENTED:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Write to unimplemented memory location");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_MEM_RD_NOT_IMPLEMENTED:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Read from unimplemented memory location");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_MEM_RD_UNDEFINED:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Uninitialized value read from memory");
            ui.teDetail->appendPlainText("Simulation halted");
            ui.teDetail->appendPlainText("Address: 0x" + detail);
            break;
        }
        //cpu
        case ERR_CPU_PC_OVERFLOW:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Program counter overflow");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_PC_UNDERFLOW:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Program counter underflow");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_SYS_FATAL:
        {
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("System fatal error");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_OPCODE:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid opcode");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_UNDEFINED_OPCODE:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Undefined opcode");
            ui.teDetail->appendPlainText("Simulation halted");
            ui.teDetail->appendPlainText("Address: 0x" + detail);
            break;
        }
        case ERR_CPU_INVALID_JUMP:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid jump");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_CALL:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid call");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_IRQ:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid IRQ");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_RET:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid return");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_RETI:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid return from interrupt");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INVALID_OPSET:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Invalid opset");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_UNSUPPORTED_INST:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Unsupported instruction");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }
        case ERR_CPU_INST_IGNORED:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Instruction ignored");
            ui.teDetail->appendPlainText("Simulation halted");
            break;
        }


        case ERR_UNTRACKED_PROJECT:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Action not available for untracked project");
            break;
        }
        case ERR_SIM_NOSTART:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Simulation could not start");
            ui.teDetail->appendPlainText("Try to recompile or check dbg file integrity.");
            ui.teDetail->appendPlainText("Dbg file: " + detail);
            break;
        }
        case ERR_SIM_NOT_COMPILED:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Simulation could not start, project is not compiled.");
            break;
        }
        case ERR_SIM_NOT_COMPILED_MODIFIED:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Simulation could not start, source files were modified.");
            break;
        }
        case ERR_SIM_RECOMPILE_FAILED:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Simulation could not start, recompilation failed. ");
            ui.teDetail->appendPlainText("See compiler output for possible errors.");
            break;
        }
        case ERR_NO_MAINFILE:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Project main file is not set");
            break;
        }
        case ERR_NO_LICENSE:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("License file not loaded.");
            break;
        }
        case ERR_COMPILE_DURING_SIMULATION:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Can not compile during active simulation.");
            break;
        }
        case ERR_END_OF_DOCUMENT:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("End of document reached.");
            break;
        }
        case ERR_START_OF_DOCUMENT:
        {
            msgBox.setWindowTitle("Warning");
            QIcon icon = QIcon::fromTheme("dialog-warning", QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Beginning of document reached.");
            break;
        }
        default:
        {
            msgBox.setWindowTitle("Error");
            QIcon icon = QIcon::fromTheme("dialog-error", QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));
            ui.lblIcon->setPixmap(icon.pixmap(QSize(32,32)));
            ui.lblError->setText("Error not specified");
            break;
        }
    }
    msgBox.exec();
}
