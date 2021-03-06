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
 * @file MScriptComplex.h
 */
// =============================================================================

#ifndef MSCRIPTCOMPLEX_H
#define MSCRIPTCOMPLEX_H

#include <complex>

/**
 * @brief
 * @class MScriptComplex
 * @ingroup MoraviaScript
 */
class MScriptComplex
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in] r
         * @param[in] i
         */
        MScriptComplex ( double r = 0.0d,
                         double i = 0.0d )
                       :
                         m_r ( r ),
                         m_i ( i ) {}

        /**
         * @brief
         * @param[in] std_complex
         */
        MScriptComplex ( const std::complex<double> & std_complex )
                       :
                         m_r ( std_complex.real() ),
                         m_i ( std_complex.imag() ) {}

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
