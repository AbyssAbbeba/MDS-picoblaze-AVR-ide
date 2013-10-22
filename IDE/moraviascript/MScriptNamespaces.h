// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MoraviaScript
 * @file MScriptNamespaces.h
 */
// =============================================================================

#ifndef MSCRIPTNAMESPACES_H
#define MSCRIPTNAMESPACES_H

// Forward declarations.
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptSrcLocation.h"

// Standard header files.
#include <string>
#include <ostream>

/**
 * @brief
 * @class MScriptNamespaces
 * @ingroup MoraviaScript
 */
class MScriptNamespaces
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct NsDesc
        {
            ////    Constructors and Destructors    ////
            /**
             * @brief
             * @param[in] name
             * @param[in] parent
             */
            NsDesc ( const std::string & name,
                     NsDesc * parent )
                   :
                     m_name ( name ),
                     m_parent ( parent ) {}

            /**
             * @brief
             */
            ~NsDesc();

            ////    Public Operations    ////
            /**
             * @brief
             */
            void clear();

            ////    Public Attributes    ////
            /// @brief
            const std::string m_name;

            /// @brief
            NsDesc * const m_parent;

            /// @brief
            std::vector<NsDesc*> m_contains;
        };

    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const MScriptNamespaces & namespaces );

    friend std::ostream & operator << ( std::ostream & out,
                                        const NsDesc * nsDesc );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptNamespaces ( MScriptInterpretInterface * interpret );

        /**
         * @brief
         */
        ~MScriptNamespaces();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] ns
         */
        void define ( const MScriptSrcLocation & location,
                      const std::string & ns );

        /**
         * @brief
         * @param[in] location
         * @param[in] ns
         */
        void enter ( const MScriptSrcLocation & location,
                     const std::string & ns );

        /**
         * @brief
         * @param[in] location
         * @param[in] ns
         */
        void defineEnter ( const MScriptSrcLocation & location,
                           const std::string & ns );

        /**
         * @brief
         */
        void leave();

        /**
         * @brief
         * @return
         */
        const NsDesc * current() const;

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;

        /// @brief
        std::vector<NsDesc*> m_current;

        /// @brief
        NsDesc * m_ns;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] namespaces
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptNamespaces & namespaces );

    /**
     * @brief
     * @param[in,out] out
     * @param[in] nsDesc
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const MScriptNamespaces::NsDesc * nsDesc );
//@}

#endif // MSCRIPTNAMESPACES_H
