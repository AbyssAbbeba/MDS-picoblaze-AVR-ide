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
    if ( false == file.is_open() )
    {
        std::cerr << "Error: unable to open file: \"" << argv[1] << '"' << std::endl;
        return 2;
    }

    LicenseCertificate crt(file);

    if ( false == crt.m_isValid )
    {
        std::cerr << "Error: certificate did not pass validation." << std::endl;
        return 3;
    }

    std::cout << "Certificate is valid." << std::endl;
    std::cout << std::endl;
    std::cout << "<certificate>" << std::endl;
    std::cout << "  ├─ version: " << crt.m_version << std::endl;
    std::cout << "  ├─ <product>" << std::endl;
    std::cout << "  │    ├─ designation: \"" << crt.m_product.m_designation << '"' << std::endl;
    std::cout << "  │    ├─ licences: \"" << crt.m_product.m_licences << '"' << std::endl;
    std::cout << "  │    ├─ variant: \"" << crt.m_product.m_variant << '"' << std::endl;
    std::cout << "  │    ├─ grade: \"" << crt.m_product.m_grade << '"' << std::endl;
    std::cout << "  │    ├─ target: \"" << crt.m_product.m_target << '"' << std::endl;
    std::cout << "  ├─ <licensee>" << std::endl;
    std::cout << "  │    ├─ name: \"" << crt.m_licensee.m_name << '"' << std::endl;
    std::cout << "  │    ├─ type: \"" << crt.m_licensee.m_type << '"' << std::endl;
    std::cout << "  │    ├─ email: \"" << crt.m_licensee.m_email << '"' << std::endl;
    std::cout << "  │    ├─ phone: \"" << crt.m_licensee.m_phone << '"' << std::endl;
    std::cout << "  │    ╰─ <address>" << std::endl;
    std::cout << "  │         ├─ country: " << crt.m_licensee.m_address.m_country << std::endl;
    std::cout << "  │         ├─ state: \"" << crt.m_licensee.m_address.m_state << '"' << std::endl;
    std::cout << "  │         ├─ municipality: \"" << crt.m_licensee.m_address.m_municipality << '"' << std::endl;
    std::cout << "  │         ├─ street: \"" << crt.m_licensee.m_address.m_street << '"' << std::endl;
    std::cout << "  │         ├─ number: \"" << crt.m_licensee.m_address.m_number << '"' << std::endl;
    std::cout << "  │         ╰─ post: \"" << crt.m_licensee.m_address.m_post << '"' << std::endl;
    std::cout << "  ╰─ <system>" << std::endl;
    std::cout << "       ├─ timestamp: \"" << crt.m_system.m_timestamp << '"' << std::endl;
    std::cout << "       ├─ userid: \"" << crt.m_system.m_userid << '"' << std::endl;
    std::cout << "       ╰─ orderid: \"" << crt.m_system.m_orderid << '"' << std::endl;

    return 0;
}
