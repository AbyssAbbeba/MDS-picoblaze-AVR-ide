// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup LicenseCertificate
 * @file main.cxx
 */
// =============================================================================

// Library for validating and reading license certificates.
#include "LicenseCertificate.h"

// Standard header files.
#include <iostream>

int main ( int argc, char ** argv )
{
    if ( 2 != argc)
    {
        std::cerr << "Please provide certificate file." << std::endl;
        return 1;
    }

    std::ifstream file ( argv[1], std::ios_base::binary );
    LicenseCertificate crt(file);

    if ( false == crt.m_isValid )
    {
        std::cerr << "Error: certificate did not pass validation." << std::endl;
        return 2;
    }

    std::cout << "Certificate is valid." << std::endl;
    std::cout << std::endl;
    std::cout << "<certificate>" << std::endl;
    std::cout << "  ├─ version: " << crt.m_version << std::endl;
    std::cout << "  ├─ user ID: " << crt.m_userid << std::endl;
    std::cout << "  ├─ license ID: " << crt.m_licenseid << std::endl;
    std::cout << "  ├─ date: " << crt.m_date << std::endl;
    std::cout << "  ├─ expiry: " << crt.m_expiry << std::endl;
    std::cout << "  ├─ products:";
    for ( const auto & product : crt.m_products )
    {
        std::cout << ' ' << product;
    }
    std::cout << std::endl;
    std::cout << "  ├─ <address>" << std::endl;
    std::cout << "  │    ├─ street1: \"" << crt.m_addrStreet1 << '"' << std::endl;
    std::cout << "  │    ├─ street2: \"" << crt.m_addrstreet2 << '"' << std::endl;
    std::cout << "  │    ├─ city: \"" << crt.m_addrcity << '"' << std::endl;
    std::cout << "  │    ├─ post: \"" << crt.m_addrpost << '"' << std::endl;
    std::cout << "  │    ├─ state1: \"" << crt.m_addrstate1 << '"' << std::endl;
    std::cout << "  │    ├─ state2: \"" << crt.m_addrstate2 << '"' << std::endl;
    std::cout << "  │    ╰─ state3: \"" << crt.m_addrstate3 << '"' << std::endl;
    std::cout << "  ├─ name: \"" << crt.m_name << '"' << std::endl;
    std::cout << "  ╰─ <details>" << std::endl;
    std::cout << "       ├─ type: \"" << crt.m_detailsType << '"' << std::endl;
    std::cout << "       ├─ <contact>" << std::endl;
    std::cout << "       │    ├─ email: " << crt.m_detailsContactEmail << std::endl;
    std::cout << "       │    ├─ phone: \"" << crt.m_detailsContactPhone << '"' << std::endl;
    std::cout << "       │    ╰─ address: \"" << crt.m_detailsContactAddress << '"' << std::endl;
    std::cout << "       ╰─ department: \"" << crt.m_detailsDepartment << '"' << std::endl;

    return 0;
}
