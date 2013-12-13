// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @ingroup Compiler
 * @file CompilerCPreprocessor.cxx
 */
// =============================================================================

#include "CompilerCPreprocessor.h"

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerParserInterface.h"

// Standard headers.
#include <cstdlib>
#include <cstring>

CompilerCPreprocessor::CompilerCPreprocessor ( CompilerParserInterface * compilerCore,
                                               CompilerOptions * opts )
                                              :
                                                m_compilerCore ( compilerCore ),
                                                m_opts ( opts )
{
}

CompilerCPreprocessor::~CompilerCPreprocessor()
{
}

char * CompilerCPreprocessor::processFile ( FILE * sourceFile )
{
    // Input buffer for reading the sourceFile.
    ssize_t lineLen;             // Number of character currently loaded in the buffer; including EOLs, without NULL(s).
    size_t  inBufferSize  = 0;    // Overall size of the input buffer.
    char *  inBuffer      = NULL; // Buffer pointer, used for single line only.

    // Output buffer for stroring preprocessor output.
    size_t  outBufferCurP = 0;                             // Current position in the buffer.
    size_t  outBufferSize = 1024;                          // Overall size of the buffer.
    ssize_t outBufferCap  = ( outBufferSize - 1 );         // Number of free bytes in the buffer.
    char *  outBuffer     = (char*) malloc(outBufferSize); // Buffer pointer, used to store the entire output at once.

    outBuffer[outBufferCurP] = '\0';

    // Iterate over lines in the source file.
    while ( -1 != ( lineLen = getline(&inBuffer, &inBufferSize, sourceFile) ) )
    {
        // Process input and generate output.
        if ( false == processLine ( lineLen, inBuffer, inBufferSize ) )
        {
            // Error -> terminate preprocessor.
            free(inBuffer);
            free(outBuffer);
            return NULL;
        }

        // Safely copy contents of the input buffer to the output buffer.
        outBufferCap -= lineLen;
        if ( outBufferCap < 0 )
        {
            size_t incr = 1024 + ( 10 * -outBufferCap );
            outBufferSize += incr;
            outBufferCap  += incr;

            outBuffer = (char*) realloc ( outBuffer, outBufferSize );
        }
        strcpy((outBuffer + outBufferCurP), inBuffer);
        outBufferCurP += lineLen;
    }

    // Dispose of the input buffer.
    free(inBuffer);

    // Check for error condition on the sourceFile.
    if ( 0 != ferror(sourceFile) )
    {
        m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "I/O error, cannot read the source file properly" )
                                                          . toStdString() );
    }

    // Save the output from preprocessor into a file specified by CompilerOptions.
    if ( false == m_opts->m_cunit.empty() )
    {
        FILE * file = fopen(m_opts->m_cunit.c_str(), "w");
        if ( NULL != file )
        {
            fwrite(outBuffer, sizeof(char), outBufferCurP, file);
            if ( 0 != ferror(file) )
            {
                m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "unable to save file: `%1'" )
                                                                  . arg ( m_opts->m_cunit.c_str() )
                                                                  . toStdString() );
            }
            fclose(file);
        }
        else
        {
            m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "unable to open file: `%1'" )
                                                              . arg ( m_opts->m_cunit.c_str() )
                                                              . toStdString() );
        }
    }

    // Done.
    return outBuffer;
}

inline bool CompilerCPreprocessor::processLine ( ssize_t & length,
                                                 char * & line,
                                                 size_t & bufferSize )
{
    return true;
}

