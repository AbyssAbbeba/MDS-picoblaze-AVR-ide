// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDefGui.h
 */
// =============================================================================

#ifndef ADJSIMPROCDEFGUI_H
#define ADJSIMPROCDEFGUI_H

#include "ui_AdjSimProcDefGuiWidget.h"

#include <QWidget>
#include <QString>

/**
 * @brief
 * @ingroup AdjSimProcDef
 * @class AdjSimProcDefGui
 */
class AdjSimProcDefGui : public QWidget,
                         private Ui::AdjSimProcDefGuiWidget
{
    Q_OBJECT

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] parent
         */
        AdjSimProcDefGui ( QWidget * parent = nullptr );

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] fileName
         */
        void openFile ( const QString & fileName );

        /**
         * @brief
         * @return
         */
        const QString & getFileName() const;

    ////    Private Operations    ////
    public:
        /**
         * @brief
         * @param[in] fileName
         */
        void setFileName ( const QString & fileName );

        /**
         * @brief
         * @param[in] fileName
         */
        void saveFile ( const QString & fileName );

        /**
         * @brief
         */
        void clearModified();

        /**
         * @brief
         */
        inline void setupConnections();

    ////    Private Attributes    ////
    private:
        /// @brief
        QString m_fileName;

        /// @brief
        bool m_modified;

    ////    Private Slots    ////
    private slots:
        void setModified();
        void setModified(int);
        void setModified(bool);
        void setModified ( const QString & );

        /**
         * @brief
         * @param[in] checkState
         */
        void disenaRegFile ( int checkState );

        /**
         * @brief
         * @param[in] checkState
         */
        void disenaDataMem ( int checkState );

        void on_pushButtonOpenFile_clicked();
        bool on_pushButtonSaveAs_clicked();
};

#endif // ADJSIMPROCDEFGUI_H
