        device  kcpsm6

my_port portout 3
my_str  string  "Omnibus Formosior Semper In Te Glorior."

        org     0

        ldret   S0, my_str
        ldret   S1, "Hac In Hora, Sine Mora."
        outk    my_str, my_port
        outk    "O Fortuna, Venut Luna", my_port

        jump    $
        db      my_str

        end
