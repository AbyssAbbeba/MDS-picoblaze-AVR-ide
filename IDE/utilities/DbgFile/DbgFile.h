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
 * @file DbgFile.h
 */
// =============================================================================

#ifndef DBGFILE_H
#define DBGFILE_H

#include <vector>
#include <string>

/**
 * @brief
 * @ingroup DbgFile
 * @class DbgFile
 */
class DbgFile
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        class Exception
        {
            ////    Public Datatypes    ////
            public:
                /**
                 * @brief
                 */
                enum Type
                {
                    IO_ERROR,           ///<
                    PARSE_ERROR,        ///<
                    COMPATIBILITY_ERROR ///<
                };

            ////    Constructors and Destructors    ////
            public:
                /**
                 * @brief
                 * @param[in] errorType
                 * @param[in] errorInfo
                 */
                Exception ( Type errorType,
                            const std::string & errorInfo )
                          : m_errorType ( errorType ),
                            m_errorInfo ( errorInfo )
                {
                }

            private:
                /**
                 * @brief Forbidden constructor.
                 */
                Exception();

            ////    Public Operations    ////
            public:
                /**
                 * @brief
                 * @return
                 */
                std::string toString() const;

            ////    Public Attributes    ////
            public:
                /**
                 * @brief
                 */
                const Type m_errorType;

                /**
                 * @brief
                 */
                const std::string m_errorInfo;
        };

        /**
         * @brief
         */
        enum AddressSpace
        {
            ADDRSP_EXTERNAL_STACK,       ///<
            ADDRSP_INTERNAL_STACK,       ///<
            ADDRSP_CODE,                 ///<
            ADDRSP_CODE__STATIC_SEGMENT, ///<
            ADDRSP_INTERNAL_RAM__LOWER,  ///<
            ADDRSP_EXTERNAL_RAM,         ///<
            ADDRSP_INTERNAL_RAM,         ///<
            ADDRSP_BIT_ADDRESSABLE,      ///<
            ADDRSP_SFR,                  ///<
            ADDRSP_SBIT,                 ///<
            ADDRSP_REGISTER,             ///<
            ADDRSP_OTHER                 ///<
        };

        /**
         * @brief
         */
        enum Scope
        {
            SCOPE_GLOBAL,  ///<
            SCOPE_FILE,    ///<
            SCOPE_LOCAL,   ///<
            SCOPE_NONE     ///<
        };

        /**
         * @brief
         */
        struct TypeChain
        {
            /**
             * @brief
             */
            struct DCLType
            {
                /**
                 * @brief
                 */
                enum Type
                {
                    TYPE_DA, ///< Array of \<n\> elements
                    TYPE_DF, ///< Function
                    TYPE_DG, ///< Generic pointer
                    TYPE_DC, ///< Code pointer
                    TYPE_DX, ///< External ram pointer
                    TYPE_DD, ///< Internal ram pointer
                    TYPE_DP, ///< Paged pointer
                    TYPE_DI, ///< Upper 128 byte pointer
                    TYPE_SL, ///< long
                    TYPE_SI, ///< int
                    TYPE_SC, ///< char
                    TYPE_SS, ///< short
                    TYPE_SV, ///< void
                    TYPE_SF, ///< float
                    TYPE_ST, ///< Structure of name \<m_name\>
                    TYPE_SX, ///< sbit
                    TYPE_SB  ///< Bit field of \<n\> bits
                } m_type;

                /**
                 * @brief
                 */
                int m_n;

                /**
                 * @brief
                 */
                std::string m_name;
            };

            /**
             * @brief
             */
            unsigned int m_size;

            /**
             * @brief
             */
            std::vector<DCLType> m_DCLType;

            /**
             * @brief
             */
            bool m_signed;
        };

        /**
         * @brief
         */
        struct ModuleRecord
        {
            ///
            int m_fileNumber;
        };

        /**
         * @brief
         */
        struct LineRecord
        {
            /**
             * @brief
             */
            LineRecord()
            {
                m_fileNumber = -1;
                m_lineNumber = -1;
                m_level      = -1;
                m_block      = -1;
                m_address    = -1;
            }

            /**
             * @brief
             */
            LineRecord ( int fileNumber,
                         int lineNumber,
                         int level,
                         int block,
                         int address )
            {
                m_fileNumber = fileNumber;
                m_lineNumber = lineNumber;
                m_level      = level;
                m_block      = block;
                m_address    = address;
            }

            ///
            int m_fileNumber;

            ///
            int m_lineNumber;

            ///
            int m_level;

            ///
            int m_block;

            ///
            int m_address;
        };

        /**
         * @brief
         */
        struct SymbolRecord
        {
            /**
             * @brief
             */
            SymbolRecord()
            {
                m_fileNumber   = -1;
                m_startAddress = -1;
                m_endAddress   = -1;
            }

            ///
            Scope m_scope;

            ///
            std::string m_function; // Valid only for local scope

            ///
            std::string m_name;

            ///
            int m_fileNumber; // Valid only for file scope

            ///
            int m_level;

            ///
            int m_block;

            ///
            int m_startAddress;

            ///
            int m_endAddress;

            ///
            TypeChain m_typeChain;

            ///
            AddressSpace m_addressSpace;

            ///
            bool m_onStack;

            ///
            int m_stackOffset;

            ///
            std::vector<std::string> m_registers;
        };

        /**
         * @brief
         */
        struct FunctionRecord
        {
            ///
            Scope m_scope;

            ///
            std::string m_function; // Valid only for local scope

            ///
            std::string m_name;

            ///
            int m_fileNumber; // Valid only for file scope

            ///
            int m_level;

            ///
            int m_block;

            ///
            TypeChain m_typeChain;

            ///
            AddressSpace m_addressSpace;

            ///
            int m_stackOffset;

            ///
            int m_interruptNum;

            ///
            int m_registerBank;

            ///
            bool m_isInterrupt;

            ///
            bool m_onStack;
        };

        /**
         * @brief
         */
        struct TypeRecord
        {
            /**
             * @brief
             */
            struct TypeMember
            {
                /**
                 * @brief
                 * @param[in] offset
                 */
                TypeMember ( int offset )
                {
                    m_offset = offset;
                }

                ///
                int m_offset;

                ///
                SymbolRecord m_memberSymbol;
            };

            ///
            int m_fileNumber;

            ///
            std::string m_name;

            ///
            std::vector<TypeMember> m_typeMembers;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DbgFile()
        {
            m_lastAddress = -1;
        }

        /**
         * @brief
         */
        virtual ~DbgFile() {};

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] obj
         * @return
         */
        bool operator == ( const DbgFile & obj );

        /**
         * @brief
         * @param[in] filename
         */
        virtual void openFile ( const std::string & filename ) throw ( Exception ) = 0;

        /**
         * @brief
         * @param[in] addr
         * @return
         */
        virtual int getLineByAddr ( unsigned int addr ) const = 0;

        /**
         * @brief
         * @param[in] addr
         * @param[out] fileName
         * @return
         */
        int getLineByAddr ( unsigned int addr,
                            std::string & fileName ) const;

        /**
         * @brief
         * @param[in] addr
         * @param[in] filename
         * @return
         */
        virtual int getAddrByLine ( unsigned int line,
                                    const std::string & filename ) const = 0;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        const std::vector<LineRecord> & getLineRecords() const
        {
            return m_lineRecords;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<SymbolRecord> & getSymbolRecords() const
        {
            return m_symbolRecords;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<FunctionRecord> & getFunctionRecords() const
        {
            return m_functionRecords;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<ModuleRecord> & getModuleRecords() const
        {
            return m_moduleRecords;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<TypeRecord> & getTypeRecords() const
        {
            return m_typeRecords;
        }

        /**
         * @brief
         * @return
         */
        const std::vector<std::string> & getFileNames() const
        {
            return m_fileNames;
        }

        /**
         * @brief
         * @param[in] fileNumber
         * @return
         */
        const std::string & fileNumber2Name ( int fileNumber ) const
        {
            static const std::string emptyString = "";
            if ( -1 == fileNumber )
            {
                    return emptyString;
            }
            return m_fileNames[fileNumber];
        }

        /**
         * @brief
         * @return
         */
        const std::string getSourceFileName() const
        {
            return m_sourceFile;
        }

    ////    Protected Attributes    ////
    protected:
        /**
         * @brief
         */
        std::vector<unsigned int> m_numberOfLines;

        /**
         * @brief
         */
        int m_lastAddress;

        /**
         * @brief
         */
        std::string m_sourceFile;

        /**
         * @brief
         */
        std::vector<LineRecord> m_lineRecords;

        /**
         * @brief
         */
        std::vector<SymbolRecord> m_symbolRecords;

        /**
         * @brief
         */
        std::vector<FunctionRecord> m_functionRecords;

        /**
         * @brief
         */
        std::vector<ModuleRecord> m_moduleRecords;

        /**
         * @brief
         */
        std::vector<TypeRecord> m_typeRecords;

        /**
         * @brief
         */
        std::vector<std::string> m_fileNames;
};

#endif // DBGFILE_H
