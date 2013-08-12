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
 * @ingroup MCUSim
 * @file MCUSimEventLogger.h
 */
// =============================================================================

#ifndef MCUSIMEVENTLOGGER_H
#define MCUSIMEVENTLOGGER_H

/**
 * @brief This class is the observer of events occurring inside the simulator. Basically it acts as a queue.
 * @ingroup MCUSim
 * @class MCUSimEventLogger
 */
class MCUSimEventLogger
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        MCUSimEventLogger ( )
                          : m_size   ( 10 ),
                            m_inPos  (  1 ),
                            m_outPos (  0 ),
                            m_subsysId ( new int [ m_size ] ),
                            m_eventId  ( new int [ m_size ] ),
                            m_location ( new int [ m_size ] ),
                            m_detail   ( new int [ m_size ] )
        {
        }

        /**
         * @brief
         */
        ~MCUSimEventLogger()
        {
            delete [] m_subsysId;
            delete [] m_eventId;
            delete [] m_location;
            delete [] m_detail;
        }

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         */
        inline void clear();

        /**
         * @brief
         * @param[in] subsysId
         * @param[in] eventId
         * @param[in] location
         * @param[in] detail
         */
        inline void logEvent ( int subsysId,
                               int eventId,
                               int location, /* or reason */
                               int detail );

        /**
         * @brief This operation removes the read item.
         * @param[out] subsysId
         * @param[out] eventId
         * @param[out] location
         * @param[out] detail
         * @return How many entries was there before this method was called, 0 means that there was nothing.
         */
        inline int getEvent ( int & subsysId,
                              int & eventId,
                              int & location,
                              int & detail );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void enlargeQueue();

    ////    Private Attributes    ////
    private:
        int m_size;    ///<
        int m_inPos;   ///<
        int m_outPos;  ///<

        int * m_subsysId;  ///<
        int * m_eventId;   ///<
        int * m_location;  ///< (or reason)
        int * m_detail;    ///<
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void MCUSimEventLogger::clear()
{
    m_inPos = 1;
    m_outPos = 0;
}

inline void MCUSimEventLogger::logEvent ( int subsysId,
                                          int eventId,
                                          int location, /* or reason */
                                          int detail )
{
    if ( m_inPos == m_outPos )
    {
        // The queue is full -> enlarge it.
        enlargeQueue();

        // OR we can simply:
        // return;
    }

    m_subsysId [ m_inPos ] = subsysId;
    m_eventId  [ m_inPos ] = eventId;
    m_location [ m_inPos ] = location;
    m_detail   [ m_inPos ] = detail;

    m_inPos++;
    m_inPos %= m_size;
}

inline int MCUSimEventLogger::getEvent ( int & subsysId,
                                         int & eventId,
                                         int & location,
                                         int & detail )
{
    m_outPos++;
    m_outPos %= m_size;

    if ( m_outPos == m_inPos )
    {
        // The queue is empty
        m_outPos--;
        if ( -1 == m_outPos )
        {
            m_outPos += m_size;
        }
        return 0;
    }

    subsysId = m_subsysId [ m_outPos ];
    eventId  = m_eventId  [ m_outPos ];
    location = m_location [ m_outPos ];
    detail   = m_detail   [ m_outPos ];

    int result = m_inPos - m_outPos - 1;
    if ( result < 0 )
    {
        result += m_size;
    }
    return result;
}

inline void MCUSimEventLogger::enlargeQueue()
{
    int newSize = m_size * 2;
    int * m_subsysIdNew = new int [ newSize ];
    int * m_eventIdNew  = new int [ newSize ];
    int * m_locationNew = new int [ newSize ];
    int * m_detailNew   = new int [ newSize ];

    int i = 1;
    for ( int j = m_outPos + 1;
          j != m_inPos;
          j = ( ( j + 1 ) % m_size ) )
    {
        m_subsysIdNew [ i ] = m_subsysId [ j ];
        m_eventIdNew  [ i ] = m_eventId  [ j ];
        m_locationNew [ i ] = m_location [ j ];
        m_detailNew   [ i ] = m_detail   [ j ];

        i++;
        i %= m_size;
    }

    m_outPos = 0;
    m_inPos = i;
    m_size = newSize;

    delete [] m_subsysId;
    delete [] m_eventId;
    delete [] m_location;
    delete [] m_detail;

    m_subsysId = m_subsysIdNew;
    m_eventId  = m_eventIdNew;
    m_location = m_locationNew;
    m_detail   = m_detailNew;
}

#endif // MCUSIMEVENTLOGGER_H
