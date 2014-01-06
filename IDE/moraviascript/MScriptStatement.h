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
 * @ingroup MoraviaScript
 * @file MScriptStatement.h
 */
// =============================================================================

#ifndef MSCRIPTSTATEMENT_H
#define MSCRIPTSTATEMENT_H

// Forward declarations.
class MScriptExpr;

// MScript language interpreter header files.
#include "MScriptStmtTypes.h"
#include "MScriptSrcLocation.h"
#include "MScriptSerializable.h"

// Standard header files.
#include <ostream>

/**
 * @brief
 * @class MScriptStatement
 * @ingroup MoraviaScript
 */
class MScriptStatement : public MScriptSerializable
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MScriptStatement();

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] args
         */
        MScriptStatement ( const MScriptSrcLocation & location,
                           MScriptStmtTypes::Type type,
                           MScriptExpr * args = NULL );

        /**
         * @brief
         * @param[in,out] input
         */
        MScriptStatement ( MScriptSerializer & input );

        /**
         * @brief
         */
        virtual ~MScriptStatement();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void completeDelete();

        /**
         * @brief
         * @param[in,out] stmt
         */
        static void completeDelete ( MScriptStatement * stmt );

        /**
         * @brief
         * @param[in,out] branch
         * @return
         */
        MScriptStatement * createBranch ( MScriptStatement * branch );

        /**
         * @brief
         * @return
         */
        MScriptStatement * first();

        /**
         * @brief
         * @return
         */
        MScriptStatement * last();

        /**
         * @brief
         * @return this
         */
        MScriptStatement * unlink();

        /**
         * @brief
         */
        MScriptStatement * copyEntireChain() const;

        /**
         * @brief
         */
        MScriptStatement * copyChainLink() const;

        /**
         * @brief Insert link to the chain right after the link represented by this object.
         * @param[in,out] next
         * @return
         */
        MScriptStatement * insertLink ( MScriptStatement * chainLink );

        /**
         * @brief
         * @param[in,out] chainLink
         * @return
         */
        MScriptStatement * appendLink ( MScriptStatement * chainLink );

        /**
         * @brief Insert link before the first link in the chain.
         * @param[in,out] chainLink
         * @return
         */
        MScriptStatement * prependLink ( MScriptStatement * chainLink );

        /**
         * @brief
         * @param[in,out] chainLink
         * @return
         */
        MScriptStatement * appendArgsLink ( MScriptExpr * chainLink );

        /// @name Serialization methods.
        //@{
            /**
             * @brief
             * @param[in,out]
             */
            virtual void serialize ( MScriptSerializer & output ) const;

            /**
             * @brief
             * @param[in,out]
             */
            virtual void deserialize ( MScriptSerializer & input );

            /**
             * @brief
             * @param[in,out]
             */
            void serializeTree ( MScriptSerializer & output ) const;
        //@}

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MScriptStatement * prev() const
        {
            return m_prev;
        }

        /**
         * @brief
         * @return
         */
        MScriptStatement * next() const
        {
            return m_next;
        }

        /**
         * @brief
         * @return
         */
        MScriptStatement * branch() const
        {
            return m_branch;
        }

        /**
         * @brief
         * @return
         */
        const MScriptSrcLocation & location() const
        {
            return m_location;
        }

        /**
         * @brief
         * @return
         */
        MScriptStmtTypes::Type type() const
        {
            return m_type;
        }

        /**
         * @brief
         * @return
         */
        MScriptExpr * args() const
        {
            return m_args;
        }

    ////    Private Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] out
         * @param[in] level
         * @param[in] lineString
         * @return
         */
        std::ostream & print ( std::ostream & out,
                               int level = 0,
                               std::string lineString = "1" ) const;

    ////    Public Attributes    ////
    public:
        /**
         * @brief
         */
        MScriptStmtTypes::Type m_type;

        /**
         * @brief
         */
        MScriptSrcLocation m_location;

        /**
         * @brief
         */
        MScriptExpr * m_args;

        /**
         * @brief
         */
        MScriptStatement * m_prev;

        /**
         * @brief
         */
        MScriptStatement * m_next;

        /**
         * @brief
         */
        MScriptStatement * m_branch;
};

/**
 * @brief Tracing operator
 * @param[in,out] out
 * @param[in] stmt
 * @return
 */
std::ostream & operator << ( std::ostream & out,
                             const MScriptStatement * stmt );

#endif // MSCRIPTSTATEMENT_H
