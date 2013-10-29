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
#include <vector>
#include <ostream>

/**
 * @brief
 * @class MScriptNamespaces
 * @ingroup MoraviaScript
 */
class MScriptNamespaces
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const MScriptNamespaces & namespaces );

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class NsDesc
        {
            ////    Friends    ////
            friend std::ostream & operator << ( std::ostream & out,
                                                const NsDesc * nsDesc );

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 * @param[in] name
                 * @param[in] parent
                 */
                NsDesc ( const std::string & name,
                         NsDesc * parent,
                         const MScriptSrcLocation & location )
                       :
                         m_name ( name ),
                         m_parent ( parent ),
                         m_location ( location ) {}

                /**
                 * @brief
                 */
                ~NsDesc();

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 */
                void clear();

                /**
                 * @brief
                 * @param[in,out]
                 * @param[in] level
                 * @param[in] lineString
                 */
                void print ( std::ostream & out,
                             int level = 0,
                             std::string lineString =  "1" ) const;

                /**
                 * @brief
                 * @return
                 */
                std::string toString() const;

                /**
                 * @brief
                 * @param[in] name
                 * @return
                 */
                NsDesc * getChildByName ( const std::string & name ) const;

                /**
                 * @brief
                 * @param[in] ns
                 * @return
                 */
                int inheritsFrom ( const NsDesc * ns ) const;

            ////    Public Attributes    ////
            public:
                /// @brief
                const std::string m_name;

                /// @brief
                NsDesc * const m_parent;

                /// @brief
                const MScriptSrcLocation m_location;

                /// @brief
                std::vector<NsDesc*> m_contains;
        };

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
         * @return
         */
        NsDesc * define ( const MScriptSrcLocation & location,
                          const std::string & ns );

        /**
         * @brief
         * @param[in] location
         * @param[in,out] ns
         */
        void enter ( NsDesc * ns );

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
        NsDesc * current() const;

        /**
         * @brief Determinate namespace and ID without namespace specification(s) from general ID.
         * @param[in] id
         * @param[out] bareId
         * @param[in] location
         * @return Namespace descriptor; or NULL, if determination was not possible.
         */
        NsDesc * analyseId ( const std::string & id,
                             std::string & bareId,
                             const MScriptSrcLocation * location = NULL ) const;

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
        NsDesc * m_rootNs;
};

/// @name Tracing operators.
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
