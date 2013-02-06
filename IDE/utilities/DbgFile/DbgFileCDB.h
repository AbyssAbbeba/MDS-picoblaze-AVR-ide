// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup DbgFile
 * @file DbgFileCDB.h
 */
// =============================================================================

#ifndef DBGFILECDB_H
#define DBGFILECDB_H

#include "DbgFile.h"

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFileCDB
 */
class DbgFileCDB : public DbgFile
{
    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 4095;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DbgFileCDB();

        /**
         * @brief
         */
        ~DbgFileCDB();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         */
        void openFile ( const std::string & filename ) throw ( DbgFileException );

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        int getLineByAddr ( unsigned int addr ) const;

        /**
         * @brief
         * @param[in] line
         * @param[in] filename
         * @return
         */
        int getAddrByLine ( unsigned int line,
                            const std::string & filename ) const;

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] filename
         */
        inline void loadFile ( const std::string & filename );

        /**
         * @brief
         */
        inline void clear();

        /**
         * @brief
         * @param[in,out] line
         */
        inline void handleModuleRecord ( char * line );

        /**
         * @brief
         * @param[in,out] line
         */
        inline void handleFunctionRecord ( char * line );

        /**
         * @brief
         * @param[in,out] line
         * @param[in,out] target
         */
        inline void handleSymbolRecord ( char * line,
                                         SymbolRecord * target = NULL );

        /**
         * @brief
         * @param[in,out] line
         */
        inline void handleTypeRecord ( char * line );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] startAddress
         */
        inline void handleLinkerSymbolAddressRecord ( char * line,
                                                      bool startAddress );

        /**
         * @brief
         * @param[in,out] line
         */
        inline void handleLinkerRecord ( char * line );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] asmRecord
         */
        inline void handleLinkerLineRecord ( char * line,
                                             bool asmRecord );

        /**
         * @brief
         * @param[in] filename
         * @return
         */
        inline int getOrInsertFileNumber ( const std::string & filename );

        /**
         * @brief
         * @param[in,out] line 
         * @param[in] delimiter
         * @return
         */
        inline char * extractToken ( char * & line,
                                     char delimiter = '$' ) const;

        /**
         * @brief
         * @param[in] line
         * @return
         */
        inline const char * checkHexNumber ( const char * line ) const;

        /**
         * @brief
         * @param[in] line
         * @return
         */
        inline const char * checkNumber ( const char * line ) const;

        /**
         * @brief
         * @param[in] filename
         * @return
         */
        inline int getFileNumber ( const std::string & filename ) const;

        /**
         * @brief
         * @param[in,out] line
         * @param[out] fileNumber
         * @param[out] functionName
         * @return
         */
        inline Scope parseScope ( char * & line,
                                  int & fileNumber,
                                  std::string & functionName );

        /**
         * @brief
         * @param[in,out] typeChain
         * @return
         */
        inline TypeChain parseTypeChain ( char * typeChain ) const;

        /**
         * @brief
         * @param[in] dclType
         * @return
         */
        inline TypeChain::DCLType parseDCLType ( const char * dclType ) const;

        /**
         * @brief
         * @param[in] addressSpace
         * @return
         */
        inline AddressSpace parseAddressSpace ( const char * addressSpace ) const;

        /**
         * @brief
         * @param[in,out] typeMemberDef
         * @param[in,out] typeMembers
         * @return
         */
        inline void parseMemberSymbol ( char * typeMemberDef,
                                        std::vector<TypeRecord::TypeMember> & typeMembers );

        /**
         * @brief
         */
        inline void generateLineAddressMaps();

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        int * m_addrToLineMap;

        /**
         * @brief
         */
        int ** m_lineToAddrMap;
};

#endif // DBGFILECDB_H
