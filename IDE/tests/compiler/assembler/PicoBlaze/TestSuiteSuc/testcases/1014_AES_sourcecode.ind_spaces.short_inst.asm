

device kcpsm1

G               equ     4
SBOX_ROM               equ     4
STATE               equ     4
RESULT               equ     4
KEY               equ     4
INKEY               equ     4
B128               equ     4
XORROUNDKEY               equ     4
INTOSTATE               equ     4
X   equ                         4

; Rijndael encrypt entry
; plain  is assumed to be in {plain }, the key in {inkey}
; both will be copied, final state will be the result
encrypt:
                        CALL    inkeytokey
                        CALL    intostate               ; state = in

                        CALL    xorroundkey             ; XorRoundKey( state, k[0], Nc )
                        IN      sf, x                   ; x^(i-1) (i=1)
                        IN      s3, 9                   ; for round = 1 step 1 to Nn - 1
round:                                                  ;
                        CALL    subbytes                ; ..SubBytes( state, Nc )
                        CALL    shiftrows               ; ..ShiftRows( state, Nc )
                        CALL    mixcolumns              ; ..MixColumns( state, Nc )
                        CALL    nextroundkey            ; ..XorRoundKey( state, k[ round ], Nc )
                        CALL    xorroundkey
                        SUB     s3, #1                  ; ..step 1
                        JUMP    nz, round               ; end for
                        CALL    subbytes                ; SubBytes( state, Nc )
                        CALL    shiftrows               ; ShiftRows( state, Nc )
                        CALL    nextroundkey            ; XorRoundKey( state, k[ round ], Nc )
                        CALL    xorroundkey
                        CALL    statetoout
                        RET                             ; result  is last {state}

; result should be: (Gladman)
; R[10].k_sch d014f9a8c9ee2589e13f0cc8b6630ca6
; R[10].result  3925841d02dc09fbdc118597196a0b32

; XorRoundKey( state, k, Nc )
pkey    EQU     5
pstate  equ     4

xor128:                 IN      s0, b128                ; set up loop count
xornext:                LD      s4, #pkey               ; get key byte
                        LD      s5, #pstate             ; get state byte
                        XOR     s4, s5                  ; do the xor
                        ST      s4, pstate              ; save new state byte
                        ADD     pkey, #1                ; increment key pointer
                        ADD     pstate, #1              ; increment state pointer
                        SUB     s0, #1                  ; decrement loop counter
                        JUMP    nz, xornext             ; loop back if not done 16 times (128/8)
                        RET

; get pointer to state
                        JUMP    toscratch128

inkeytokey:
                        IN      pkey, inkey             ; get pointer to plain
                        IN      pstate, key             ; get pointer to state

toscratch128:
putnext:                LD      s4, #pkey               ; get plain  byte
                        ST      s4, pstate              ; save new state byte
                        ADD     pkey, #1                ; increment key pointer
                        ADD     pstate, #1              ; increment state pointer
                        SUB     s0, #1                  ; decrement loop counter
                        JUMP    nz, putnext             ; loop back if not done 16 times (128/8)
                        RET

statetoout:
                        IN      pkey, state             ; get pointer to state
                        IN      pstate, result          ; get pointer to result

                        IN      s0, b128                ; set up loop count
getnext:                LD      s4, #pkey               ; get plain  byte
                        ST      s4, pstate              ; save new state byte
                        ADD     pkey, #1                ; increment key pointer
                        ADD     pstate, #1              ; increment state pointer
                        SUB     s0, #1                  ; decrement loop counter
                        JUMP    nz, getnext             ; loop back if not done 16 times (128/8)
                        RET

nextroundkey:
; temp = k[i - 1]
                        LD      s4, #key + 12           ; get last word of previous key
                        LD      s5, #key + 13
                        LD      s6, #key + 14
                        LD      s7, #key + 15

                        IN      s8, @s4                 ; RotWord
                        IN      s4, @s5
                        IN      s5, @s6
                        IN      s6, @s7
                        IN      s7, @s8

                        IN      s8, @s4                 ; temp=SubWord( RotWord( temp ) )
                        CALL    sbox
                        IN      s4, @s8

                        XOR     s4, sf                  ; xor Rcon( i / Nk )
                        SL0     sf                      ; x^(i-1) (i+=1)
                        JUMP    nc, nowrap
                        XOR     sf, #g
nowrap:
                        IN      s8, @s5                 ; SubWord( RotWord( temp ) )
                        CALL    sbox
                        IN      s5, @s8

                        IN      s8, @s6                 ; SubWord( RotWord( temp ) )
                        CALL    sbox
                        IN      s6, @s8

                        IN      s8, @s7                 ; SubWord( RotWord( temp ) )
                        CALL    sbox
                        IN      s7, @s8

                        IN      pkey, @key

                        IN      s0, @b128
key96:                  LD      s8, #pkey               ; k[i]=k[i - Nk] ^ temp
                        XOR     s4, s8
                        ST      s4, @pkey
                        ADD     pkey, #1

                        LD      s8, #pkey               ; k[i]=k[i - Nk] ^ temp
                        XOR     s5, s8
                        ST      s5, @pkey
                        ADD     pkey, #1

                        LD      s8, #pkey               ; k[i]=k[i - Nk] ^ temp
                        XOR     s6, s8
                        ST      s6, @pkey
                        ADD     pkey, #1

                        LD      s8, #pkey               ; k[i]=k[i - Nk] ^ temp
                        XOR     s7, s8
                        ST      s7, @pkey
                        ADD     pkey, #1

                        SUB     s0, #4
                        JUMP    nz, key96
                        RET

; Sub bytes of one 32b word pointed at by pKey
subword:
                        IN      s0, 4
subword1:               LD      s8, #pkey
                        CALL    sbox
                        ST      s8, @pkey
                        ADD     pkey, #1
                        SUB     s0, #1
                        JUMP    nz, subword1
                        RET

; SubBytes( state, Nc )
subbytes:
                        IN      pstate, @state          ; get pointer to state

                        IN      s0, b128                ; set up loop count
sub128:                 LD      s8, #pstate             ; get state byte
                        CALL    sbox
                        ST      s8, pstate              ; save new state byte
                        ADD     pstate, #1              ; increment state pointer
                        SUB     s0, #1                  ; decrement loop counter
                        JUMP    nz, sub128              ; loop back if not done 16 times (128/8)
                        RET

; SBox( s )
sbox:
                        OUT     s8, sbox_rom            ; set index
                        IN      s8, sbox_rom            ; get data
                        RET

; soft version of SBOX, very slow
sbox_soft:
                        CALL    mulinverse              ; .    x = sbox_affine(mul_inverse(in));
sboxaffine:
; for(counter = 1; counter > (DEGREE - 1)) | (s << 1); s &= MASK;
                        XOR     s8, s9                  ; in ^= s;
                        RL      s9
                        XOR     s8, s9
                        RL      s9
                        XOR     s8, s9
                        RL      s9
                        XOR     s8, s9
                        XOR     s8, #63                 ; in ^= 0x63;
                        RET                             ; return in;
; }

; MulInverse by trial and error
mulinverse:
                        IN      s9, 0                   ; int result = 0;
                        OR      s8, s8                  ; if (in == 0)
                        RET                             ; return 0;
mulinverse1:            ADD     s9, #1                  ; result = 1; result++
                        RET                             ; result < MOD
                        IN      sc, @s8                 ; in
                        IN      sd, @s9                 ; result
                        CALL    gmul                    ; gmul( in, result, ...)
                        SUB     se, #1                  ; == 1
                        JUMP    nz, mulinverse1         ; == 1?
                        RET                             ; return result

gmul:
                        IN      se, 0
gmul1:
                        SR0     sd
                        JUMP    c, gmul2                ; ; last bit was 1
                        RET                             ; ; i2 was 0 already ?
                        JUMP    gmul3

gmul2:                  XOR     se, sc
gmul3:                  SL0     sc
                        JUMP    nc, gmul1
                        XOR     sc, #g                  ; ; i1 ^= field;
                        JUMP    gmul1

;; ShiftRows( state, Nc )
shiftrows:
                        LD      s7, #state + 1
                        LD      s4, #state + 1 + 4
                        LD      s5, #state + 1 + 4 + 4
                        LD      s6, #state + 1 + 4 + 4 + 4
                        ST      s4, state + 1
                        ST      s5, state + 1 + 4
                        ST      s6, state + 1 + 4 + 4
                        ST      s7, state + 1 + 4 + 4 + 4

                        LD      s6, #state + 2
                        LD      s7, #state + 2 + 4
                        LD      s4, #state + 2 + 4 + 4
                        LD      s5, #state + 2 + 4 + 4 + 4
                        ST      s4, state + 2
                        ST      s5, state + 2 + 4
                        ST      s6, state + 2 + 4 + 4
                        ST      s7, state + 2 + 4 + 4 + 4

                        LD      s5, #state + 3
                        LD      s6, #state + 3 + 4
                        LD      s7, #state + 3 + 4 + 4
                        LD      s4, #state + 3 + 4 + 4 + 4
                        ST      s4, state + 3
                        ST      s5, state + 3 + 4
                        ST      s6, state + 3 + 4 + 4
                        ST      s7, state + 3 + 4 + 4 + 4

                        RET

;; MixColumns( state, Nc )
mixcolumns:

                        LD      s4, #state + 0
                        LD      s5, #state + 1
                        LD      s6, #state + 2
                        LD      s7, #state + 3
                        CALL    mixcolumn
                        ST      s4, state + 0
                        ST      s5, state + 1
                        ST      s6, state + 2
                        ST      s7, state + 3

                        LD      s4, #state + 0 + 4
                        LD      s5, #state + 1 + 4
                        LD      s6, #state + 2 + 4
                        LD      s7, #state + 3 + 4
                        CALL    mixcolumn
                        ST      s4, state + 0 + 4
                        ST      s5, state + 1 + 4
                        ST      s6, state + 2 + 4
                        ST      s7, state + 3 + 4

                        LD      s4, #state + 0 + 4 + 4
                        LD      s5, #state + 1 + 4 + 4
                        LD      s6, #state + 2 + 4 + 4
                        LD      s7, #state + 3 + 4 + 4
                        CALL    mixcolumn
                        ST      s4, state + 0 + 4 + 4
                        ST      s5, state + 1 + 4 + 4
                        ST      s6, state + 2 + 4 + 4
                        ST      s7, state + 3 + 4 + 4

                        LD      s4, #state + 0 + 4 + 4 + 4
                        LD      s5, #state + 1 + 4 + 4 + 4
                        LD      s6, #state + 2 + 4 + 4 + 4
                        LD      s7, #state + 3 + 4 + 4 + 4
                        CALL    mixcolumn
                        ST      s4, state + 0 + 4 + 4 + 4
                        ST      s5, state + 1 + 4 + 4 + 4
                        ST      s6, state + 2 + 4 + 4 + 4
                        ST      s7, state + 3 + 4 + 4 + 4

                        RET

mixcolumn:
                        IN      s9, @s4                 ; ; t = c[0] ^ c[3]
                        XOR     s9, s7
                        IN      sa, @s5                 ; ; u = c[1] ^ c[2]
                        XOR     sa, s6
                        IN      sb, @s9                 ;  ; v = t ^ u
                        XOR     sb, sa

                        IN      s8, @s4                 ; ; c[0] = c[0] ^ v ^ FFmul(0x02, c[0] ^ c[1])
                        XOR     s8, s5
                        SL0     s8
                        JUMP    nc, mcf1
                        XOR     s8, #g
mcf1:                   XOR     s8, sb
                        XOR     s4, s8

                        IN      s8, @sa                 ;  ; c[1] = c[1] ^ v ^ FFmul(0x02, u)
                        SL0     s8
                        JUMP    nc, mcf2
                        XOR     s8, #g
mcf2:                   XOR     s8, sb
                        XOR     s5, s8

                        IN      s8, @s6                 ; ; c[2] = c[2] ^ v ^ FFmul(0x02, c[2] ^ c[3])
                        XOR     s8, s7
                        SL0     s8
                        JUMP    nc, mcf3
                        XOR     s8, #g
mcf3:                   XOR     s8, sb
                        XOR     s6, s8

                        IN      s8, @s9                 ; ; c[3] = c[3] ^ v ^ FFmul(0x02, t)
                        SL0     s8
                        JUMP    nc, mcf4
                        XOR     s8, #g
mcf4:                   XOR     s8, sb
                        XOR     s7, s8

                        RET
