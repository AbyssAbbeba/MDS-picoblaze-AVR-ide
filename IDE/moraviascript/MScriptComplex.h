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
 * @file MScriptComplex.h
 */
// =============================================================================

#ifndef MSCRIPTCOMPLEX_H
#define MSCRIPTCOMPLEX_H

/**
 * @brief
 * @class MScriptComplex
 * @ingroup MoraviaScript
 */
class MScriptComplex
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] n
         * @return
         */
        bool divisableBy ( const MScriptComplex & n ) const;

        /// @name Overloaded operators.
        //@{
            /**
             * @brief
             * @param[in] n
             * @return
             */
            bool operator == ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            bool operator != ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            bool operator && ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            bool operator || ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            MScriptComplex operator + ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            MScriptComplex operator - ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            MScriptComplex operator * ( const MScriptComplex & n ) const;

            /**
             * @brief
             * @param[in] n
             * @return
             */
            MScriptComplex operator / ( const MScriptComplex & n ) const;
        //@}

    ////    Public Attributes    ////
    public:
        ///
        double m_r;

        ///
        double m_i;
};

#endif // MSCRIPTCOMPLEX_H
