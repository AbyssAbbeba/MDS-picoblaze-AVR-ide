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
 * @file MScriptExprAlgebra.h
 */
// =============================================================================

#ifndef MSCRIPTEXPRALGEBRA_H
#define MSCRIPTEXPRALGEBRA_H

// Forward declarations.
class MScriptInterpretInterface;

// MScript language interpreter header files.
#include "MScriptExpr.h"
#include "MScriptValue.h"
#include "MScriptSrcLocation.h"

/**
 * @brief
 * @ingroup MoraviaScript
 * @class MScriptExprAlgebra
 */
class MScriptExprAlgebra
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] interpret
         */
        MScriptExprAlgebra ( MScriptInterpretInterface * interpret )
                           : m_interpret ( interpret ) {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] result
         * @param[in] input
         * @param[in] location
         * @param[in] oper
         */
        void unaryOperation ( MScriptValue & result,
                              const MScriptValue & input,
                              const MScriptSrcLocation & location,
                              const MScriptExpr::Operator oper );

        /**
         * @brief
         * @param[out] result
         * @param[in] left
         * @param[in] right
         * @param[in] location
         * @param[in] oper
         */
        void binaryOperation ( MScriptValue & result,
                               const MScriptValue & left,
                               const MScriptValue & right,
                               const MScriptSrcLocation & location,
                               const MScriptExpr::Operator oper );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         * @return
         */
        bool checkAssignment ( const MScriptSrcLocation & location,
                               const MScriptValue & value );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @return
         */
        void incompatibleType ( const MScriptSrcLocation & location,
                                const MScriptValue::Type type );

        /**
         * @brief
         * @param[in] location
         * @param[in] typeA
         * @param[in] typeB
         * @return
         */
        void incompatibleTypes ( const MScriptSrcLocation & location,
                                 const MScriptValue::Type typeA,
                                 const MScriptValue::Type typeB );

    ////    Inline Private Operations    ////
    private:
        /// @name
        //@{
            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void intInt ( MScriptValue & result,
                                 long long a,
                                 MScriptExpr::Operator oper,
                                 long long b,
                                 const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void intFloat ( MScriptValue & result,
                                   long long a,
                                   MScriptExpr::Operator oper,
                                   double b,
                                   const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void intString ( MScriptValue & result,
                                    long long a,
                                    MScriptExpr::Operator oper,
                                    const MScriptValue::Data::String & b,
                                    const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void intBool ( MScriptValue & result,
                                  long long a,
                                  MScriptExpr::Operator oper,
                                  bool b,
                                  const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void intComplex ( MScriptValue & result,
                                     long long a,
                                     MScriptExpr::Operator oper,
                                     const MScriptComplex & b,
                                     const MScriptSrcLocation & location );
        //@}

        /// @name
        //@{
            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void floatInt ( MScriptValue & result,
                                   double a,
                                   MScriptExpr::Operator oper,
                                   long long b,
                                   const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void floatFloat ( MScriptValue & result,
                                     double a,
                                     MScriptExpr::Operator oper,
                                     double b,
                                     const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void floatString ( MScriptValue & result,
                                      double a,
                                      MScriptExpr::Operator oper,
                                      const MScriptValue::Data::String & b,
                                      const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void floatBool ( MScriptValue & result,
                                    double a,
                                    MScriptExpr::Operator oper,
                                    bool b,
                                    const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void floatComplex ( MScriptValue & result,
                                       double a,
                                       MScriptExpr::Operator oper,
                                       const MScriptComplex & b,
                                       const MScriptSrcLocation & location );
        //@}

        /// @name
        //@{
            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void stringInt ( MScriptValue & result,
                                    const MScriptValue::Data::String & a,
                                    MScriptExpr::Operator oper,
                                    long long b,
                                    const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void stringFloat ( MScriptValue & result,
                                      const MScriptValue::Data::String & a,
                                      MScriptExpr::Operator oper,
                                      double b,
                                      const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void stringString ( MScriptValue & result,
                                       const MScriptValue::Data::String & a,
                                       MScriptExpr::Operator oper,
                                       const MScriptValue::Data::String & b,
                                       const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void stringBool ( MScriptValue & result,
                                     const MScriptValue::Data::String & a,
                                     MScriptExpr::Operator oper,
                                     bool b,
                                     const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void stringComplex ( MScriptValue & result,
                                        const MScriptValue::Data::String & a,
                                        MScriptExpr::Operator oper,
                                        const MScriptComplex & b,
                                        const MScriptSrcLocation & location );
        //@}

        /// @name
        //@{

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void boolInt ( MScriptValue & result,
                                  bool a,
                                  MScriptExpr::Operator oper,
                                  long long b,
                                  const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void boolFloat ( MScriptValue & result,
                                    bool a,
                                    MScriptExpr::Operator oper,
                                    double b,
                                    const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void boolString ( MScriptValue & result,
                                     bool a,
                                     MScriptExpr::Operator oper,
                                     const MScriptValue::Data::String & b,
                                     const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void boolBool ( MScriptValue & result,
                                   bool a,
                                   MScriptExpr::Operator oper,
                                   bool b,
                                   const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void boolComplex ( MScriptValue & result,
                                      bool a,
                                      MScriptExpr::Operator oper,
                                      const MScriptComplex & b,
                                      const MScriptSrcLocation & location );
        //@}

        /// @name
        //@{

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void complexInt ( MScriptValue & result,
                                     const MScriptComplex & a,
                                     MScriptExpr::Operator oper,
                                     long long b,
                                     const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void complexFloat ( MScriptValue & result,
                                       const MScriptComplex & a,
                                       MScriptExpr::Operator oper,
                                       double b,
                                       const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void complexString ( MScriptValue & result,
                                        const MScriptComplex & a,
                                        MScriptExpr::Operator oper,
                                        const MScriptValue::Data::String & b,
                                        const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void complexBool ( MScriptValue & result,
                                      const MScriptComplex & a,
                                      MScriptExpr::Operator oper,
                                      bool b,
                                      const MScriptSrcLocation & location );

            /**
             * @brief
             * @param[in,out] result
             * @param[in] a
             * @param[in] oper
             * @param[in] b
             * @param[in] location
             * @return
             */
            inline void complexComplex ( MScriptValue & result,
                                         const MScriptComplex & a,
                                         MScriptExpr::Operator oper,
                                         const MScriptComplex & b,
                                         const MScriptSrcLocation & location );
        //@}

        /**
         * @brief
         * @param[in] location
         * @param[in] div
         * @return
         */
        inline bool checkDivisor ( const MScriptSrcLocation & location,
                                   long long div );

        /// @overload
        inline bool checkDivisor ( const MScriptSrcLocation & location,
                                   double div );

        /// @overload
        inline bool checkDivisor ( const MScriptSrcLocation & location,
                                   const MScriptComplex & a,
                                   const MScriptComplex & b );

        /**
         * @brief
         * @param[in] location
         * @param[in] by
         * @return
         */
        inline bool checkShift ( const MScriptSrcLocation & location,
                                 long long by );

        /**
         * @brief
         * @param[in] location
         * @return
         */
        inline void undefinedOperation ( const MScriptSrcLocation & location );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         */
        inline void checkFloat ( const MScriptSrcLocation & location,
                                 double value );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         */
        inline void checkComplex ( const MScriptSrcLocation & location,
                                   const MScriptComplex & value );

        /**
         * @brief
         * @param[in] location
         * @param[in] a
         * @param[in] b
         * @param[in] c
         * @param[in] addition
         */
        inline void checkSignOverflow ( const MScriptSrcLocation & location,
                                        long long a,
                                        long long b,
                                        long long c,
                                        bool addition );

    ////    Private Attributes    ////
    private:
        /// @brief
        MScriptInterpretInterface * const m_interpret;
};

#endif // MSCRIPTEXPRALGEBRA_H
