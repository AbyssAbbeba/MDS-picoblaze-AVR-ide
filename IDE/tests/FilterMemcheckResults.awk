#! /usr/bin/gawk -f

BEGIN {
    errors=0
    a=0
}

/^==[0-9]+== Invalid read of size 8$/ {
    getline line
    buffer = line
    if ( line ~ /^==[0-9]+==    at 0x[0-9a-fA-F]+: wcscmp \([\.[:alnum:]]+:[0-9]+\)$/ ) {
        getline line
        buffer = buffer "\n" line
        if ( line ~ /^==[0-9]+==    by 0x[0-9a-fA-F]+: std::moneypunct<wchar_t, (true)|(false)>::~moneypunct\(\) \(in [^\)]+\)$/ ) {
            getline line
            buffer = buffer "\n" line
            if ( line ~ /^==[0-9]+==    by 0x[0-9a-fA-F]+: std::moneypunct<wchar_t, (true)|(false)>::~moneypunct\(\) \(in [^\)]+\)$/ ) {
                getline line
                buffer = buffer "\n" line
                if ( line ~ /^==[0-9]+==    by 0x[0-9a-fA-F]+: std::locale::_Impl::~_Impl\(\) \(in [^\)]+\)$/ ) {
                    getline line
                    buffer = buffer "\n" line
                    if ( line ~ /^==[0-9]+==    by 0x[0-9a-fA-F]+: std::locale::~locale\(\) \(in [^\)]+\)$/ ) {
                        buffer = ""
                        getline line
                        errors += 1
                        while ( line !~ /^==[0-9]+== *$/ ) {
                            getline line
                        }
                        print line "(libboost related error automatically removed)"
                        next
                    }
                }
            }
        }
    }

    print ( $0 )
    print ( buffer )
    next
}

/^==[0-9]+== ERROR SUMMARY: [0-9]+ errors/ {
    $4 = ( $4 - errors )
}

{
    print ( $0 )
}
