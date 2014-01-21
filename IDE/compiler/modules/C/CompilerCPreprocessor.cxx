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

#ifndef __linux__
#include <cassert>
constexpr size_t MIN_CHUNK = 64;
int getstr ( char ** lineptr,
             size_t * n,
             FILE * stream,
             char terminator,
             int offset )
{
    int nchars_avail;
    char * read_pos;
    int ret;

    if (!lineptr || !n || !stream)
    {
        errno = EINVAL;
        return -1;
    }

  if (!*lineptr)
    {
      *n = MIN_CHUNK;
      *lineptr = (char*) malloc (*n);
      if (!*lineptr)
        {
          errno = ENOMEM;
          return -1;
        }
    }

  nchars_avail = *n - offset;
  read_pos = *lineptr + offset;

  for (;;)
    {
      int save_errno;
      register int c = getc (stream);

      save_errno = errno;

      /* We always want at least one char left in the buffer, since we
         always (unless we get an error while reading the first char)
         NUL-terminate the line buffer.  */

      assert((*lineptr + *n) == (read_pos + nchars_avail));
      if (nchars_avail < 2)
        {
          if (*n > MIN_CHUNK)
            *n *= 2;
          else
            *n += MIN_CHUNK;

          nchars_avail = *n + *lineptr - read_pos;
          *lineptr = (char*) realloc (*lineptr, *n);
          if (!*lineptr)
            {
              errno = ENOMEM;
              return -1;
            }
          read_pos = *n - nchars_avail + *lineptr;
          assert((*lineptr + *n) == (read_pos + nchars_avail));
        }

      if (ferror (stream))
        {
          /* Might like to return partial line, but there is no
             place for us to store errno.  And we don't want to just
             lose errno.  */
          errno = save_errno;
          return -1;
        }

      if (c == EOF)
        {
          /* Return partial line, if any.  */
          if (read_pos == *lineptr)
            return -1;
          else
            break;
        }

      *read_pos++ = c;
      nchars_avail--;

      if (c == terminator)
        /* Return the line.  */
        break;
    }

  /* Done - NUL terminate and return the number of chars read.  */
  *read_pos = '\0';

  ret = read_pos - (*lineptr + offset);
  return ret;
}

int getline (
     char **lineptr,
     size_t *n,
     FILE *stream )
{
  return getstr (lineptr, n, stream, '\n', 0);
}
#endif //  __linux__

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

char * CompilerCPreprocessor::processFiles ( const std::vector<FILE *> & inputFiles )
{
    // Input buffer for reading the sourceFile.
    ssize_t lineLen;                // Number of characters loaded in the buffer; including EOLs, without NULL(s).
    size_t  inBufferSize = 0;       // Overall size of the input buffer.
    char *  inBuffer     = nullptr; // Buffer pointer, used for single line only.

    // Output buffer for stroring preprocessor output.
    size_t  outBufferCurP = 0;                             // Current position in the buffer.
    size_t  outBufferSize = 1024;                          // Overall size of the buffer.
    ssize_t outBufferCap  = ( outBufferSize - 1 );         // Number of free bytes in the buffer.
    char *  outBuffer     = (char*) malloc(outBufferSize); // Buffer pointer, used to store the entire output at once.

    outBuffer[outBufferCurP] = '\0';

    // Iterate over all given input files.
    for ( auto sourceFile : inputFiles )
    {
        // Iterate over lines in the source file.
        while ( -1 != ( lineLen = getline(&inBuffer, &inBufferSize, sourceFile) ) )
        {
            // Process input and generate output.
            if ( false == processLine ( lineLen, inBuffer, inBufferSize ) )
            {
                // Error -> terminate preprocessor.
                free(inBuffer);
                free(outBuffer);
                return nullptr;
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

        // Check for error condition on the sourceFile.
        if ( 0 != ferror(sourceFile) )
        {
            m_compilerCore->preprocessorMessage ( CompilerSourceLocation(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "I/O error, cannot read the source file properly" )
                                                              . toStdString() );

            // Error -> terminate preprocessor.
            free(inBuffer);
            free(outBuffer);
            return nullptr;
        }
    }

    // Dispose of the input buffer.
    free(inBuffer);

    // Save the output from preprocessor into a file specified by CompilerOptions.
    if ( false == m_opts->m_cunit.empty() )
    {
        FILE * file = fopen(m_opts->m_cunit.c_str(), "w+");
        if ( nullptr != file )
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

