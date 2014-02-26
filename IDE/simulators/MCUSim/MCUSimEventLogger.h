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
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Flags
        {
            FLAG_NORMAL  = 0x01,
            FLAG_HI_PRIO = 0x02,

            FLAG_ALL     = 0xff
        };

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
                            m_detail   ( new int [ m_size ] ),
                            m_eventFilter ( FLAG_ALL )
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
         * @param[in] flags
         */
        inline void logEvent ( int subsysId,
                               int eventId,
                               int location, /* or reason */
                               int detail,
                               Flags flags );

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

        /**
         * @brief
         * @param[in] eventFilter
         */
        void setFilter ( Flags eventFilter )
        {
            m_eventFilter = eventFilter;
        }

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

        Flags m_eventFilter; ///<
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void MCUSimEventLogger::clear()
{
    m_inPos = 1;
    m_outPos = 0;
}
#include<iostream>//debug
inline void MCUSimEventLogger::logEvent ( int subsysId,
                                          int eventId,
                                          int location, /* or reason */
                                          int detail,
                                          Flags flags )
{
std::cout << "m_eventFilter = " << m_eventFilter << ", flags = " << flags << "\n" << std::flush;
    if ( !( m_eventFilter & flags ) )
    {
        return;
    }

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
    int * subsysIdNew = new int [ newSize ];
    int * eventIdNew  = new int [ newSize ];
    int * locationNew = new int [ newSize ];
    int * detailNew   = new int [ newSize ];

    int i = 1;
    for ( int j = m_outPos + 1;
          j != m_inPos;
          j = ( ( j + 1 ) % m_size ) )
    {
        subsysIdNew [ i ] = m_subsysId [ j ];
        eventIdNew  [ i ] = m_eventId  [ j ];
        locationNew [ i ] = m_location [ j ];
        detailNew   [ i ] = m_detail   [ j ];

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

    m_subsysId = subsysIdNew;
    m_eventId  = eventIdNew;
    m_location = locationNew;
    m_detail   = detailNew;
}

#endif // MCUSIMEVENTLOGGER_H
