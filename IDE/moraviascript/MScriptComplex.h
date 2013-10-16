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
        bool operator == ( const MScriptComplex & n ) const;
        bool operator != ( const MScriptComplex & n ) const;
        bool operator <  ( const MScriptComplex & n ) const;
        bool operator >  ( const MScriptComplex & n ) const;
        bool operator <= ( const MScriptComplex & n ) const;
        bool operator >= ( const MScriptComplex & n ) const;
        bool operator && ( const MScriptComplex & n ) const;
        bool operator || ( const MScriptComplex & n ) const;

        MScriptComplex operator + ( const MScriptComplex & n ) const;
        MScriptComplex operator - ( const MScriptComplex & n ) const;
        MScriptComplex operator * ( const MScriptComplex & n ) const;
        MScriptComplex operator / ( const MScriptComplex & n ) const;


    ////    Public Attributes    ////
    public:
        ///
        double m_r;

        ///
        double m_i;
};

#endif // MSCRIPTCOMPLEX_H
