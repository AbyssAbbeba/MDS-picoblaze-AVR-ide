        device  kcpsm6

my_port portout 3
my_str  string  "Omnubus Formosior Semper In Te Glorior."

        org     0

        ldret   S0, my_str
        ldret   S1, "Hac In Hora, Sine Mora."
        outputk my_str, my_port
        outputk "O Fortuna, Venut Luna", my_port

        end
