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

// Forward declarations.
class QCloseEvent;
class QTreeWidgetItem;
class QRegExpValidator;

// Standard header files.
#include <map>

// MDS processor definition.
#include "AdjSimProcDef.h"

// UI header files.
#include "ui_AdjSimProcDefGuiWidget.h"

// Qt header files.
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

        /**
         * @brief
         */
        ~AdjSimProcDefGui();

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
         * @return
         */
        bool maybeSave();

        /**
         * @brief
         * @return
         */
        int checkIDs();

        /**
         * @brief
         */
        inline void setupConnections();

        /**
         * @brief
         */
        inline void setupValidators();

    ////    Private Static Constants    ////
    private:
        /**
         * @brief
         */
        static const AdjSimProcDef::Instruction m_emptyInstruction;

    ////    Private Attributes    ////
    private:
        /// @brief
        QString m_fileName;

        /// @brief
        std::map<const QTreeWidgetItem *, AdjSimProcDef::Instruction *> m_instructions;

        /// @brief
        unsigned int m_idCounter;

        /// @brief
        bool m_lock;

    ////    Private Slots    ////
    private slots:
        /**
         * @brief
         * @param[in,out] event
         */
        void closeEvent ( QCloseEvent * event );

        /**
         * @brief
         */
        void setModified();

        /**
         * @brief
         */
        void instModified();

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

        /**
         * @brief
         * @param[in] -
         */
        void disenaStack ( bool );

        /**
         * @brief
         * @param[in] checkState
         */
        void disenaPorts ( int checkState );

        /**
         * @brief
         * @param[in] checkState
         */
        void disenaInterrupts ( int checkState );

        /**
         * @brief
         * @return
         */
        bool on_pushButtonSaveAs_clicked();

        /**
         * @brief
         */
        void on_pushButtonOpenFile_clicked();

        /**
         * @brief
         */
        void on_pushButtonNewInstruction_clicked();

        /**
         * @brief
         */
        void on_pushButtonRemoveInstruction_clicked();

        /**
         * @brief
         */
        void on_treeWidgetInstructions_itemSelectionChanged();
};

#endif // ADJSIMPROCDEFGUI_H
