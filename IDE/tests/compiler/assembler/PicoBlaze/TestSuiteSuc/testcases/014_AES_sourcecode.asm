

device KCPSM3

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
Encrypt:
                    CALL      InkeyToKey
                    CALL      InToState           ; state = in

                    CALL      XorRoundKey         ; XorRoundKey( state, k[0], Nc )
                    INPUT      sF, X               ; x^(i-1) (i=1)
                    INPUT      s3, 9               ; for round = 1 step 1 to Nn - 1
Round:                                            ;
                    CALL      SubBytes            ; ..SubBytes( state, Nc )
                    CALL      ShiftRows           ; ..ShiftRows( state, Nc )
                    CALL      MixColumns          ; ..MixColumns( state, Nc )
                    CALL      NextRoundKey        ; ..XorRoundKey( state, k[ round ], Nc )
                    CALL      XorRoundKey
                    SUB       s3, 1               ; ..step 1
                    JUMP      NZ, Round           ; end for
                    CALL      SubBytes            ; SubBytes( state, Nc )
                    CALL      ShiftRows           ; ShiftRows( state, Nc )
                    CALL      NextRoundKey        ; XorRoundKey( state, k[ round ], Nc )
                    CALL      XorRoundKey
                    CALL      StateToOut
                    RETURN                           ; result  is last {state}

; result should be: (Gladman)
; R[10].k_sch d014f9a8c9ee2589e13f0cc8b6630ca6
; R[10].result  3925841d02dc09fbdc118597196a0b32

; XorRoundKey( state, k, Nc )
pkey    EQU     5
pstate  equ     4

xor128:             INPUT      s0, b128            ; set up loop count
xornext:            LOAD        s4, pKey            ; get key byte
                    LOAD        s5, pState          ; get state byte
                    XOR       s4, s5              ; do the xor
                    STORE        s4, pState          ; save new state byte
                    ADD       pKey, 1             ; increment key pointer
                    ADD       pState, 1           ; increment state pointer
                    SUB       s0, 1               ; decrement loop counter
                    JUMP      NZ, xornext         ; loop back if not done 16 times (128/8)
                    RETURN

  ; get pointer to state
                    JUMP      ToScratch128

InkeyToKey:
                    INPUT      pKey, inkey         ; get pointer to plain
                    INPUT      pState, key         ; get pointer to state

ToScratch128:     
putnext:            LOAD        s4, pKey            ; get plain  byte
                    STORE        s4, pState          ; save new state byte
                    ADD       pKey, 1             ; increment key pointer
                    ADD       pState, 1           ; increment state pointer
                    SUB       s0, 1               ; decrement loop counter
                    JUMP      NZ, putnext         ; loop back if not done 16 times (128/8)
                    RETURN

StateToOut:
                    INPUT      pKey, state         ; get pointer to state
                    INPUT      pState, result      ; get pointer to result

                    INPUT      s0, b128            ; set up loop count
getnext:            LOAD        s4, pKey            ; get plain  byte
                    STORE        s4, pState          ; save new state byte
                    ADD       pKey, 1             ; increment key pointer
                    ADD       pState, 1           ; increment state pointer
                    SUB       s0, 1               ; decrement loop counter
                    JUMP      NZ, getnext         ; loop back if not done 16 times (128/8)
                    RETURN

NextRoundKey:
; temp = k[i - 1]
                    LOAD        s4, key + 12        ; get last word of previous key
                    LOAD        s5, key + 13
                    LOAD        s6, key + 14
                    LOAD        s7, key + 15

                    INPUT      s8, @s4              ; RotWord
                    INPUT      s4, @s5
                    INPUT      s5, @s6
                    INPUT      s6, @s7
                    INPUT      s7, @s8

                    INPUT      s8, @s4              ; temp=SubWord( RotWord( temp ) )
                    CALL      SBox
                    INPUT      s4, @s8

                    XOR       s4, sF              ; xor Rcon( i / Nk )
                    SL0       sF                  ; x^(i-1) (i+=1)
                    JUMP      NC, nowrap
                    XOR       sF, G
nowrap:
                    INPUT      s8, @s5              ; SubWord( RotWord( temp ) )
                    CALL      SBox
                    INPUT      s5, @s8

                    INPUT      s8, @s6              ; SubWord( RotWord( temp ) )
                    CALL      SBox
                    INPUT      s6, @s8

                    INPUT      s8, @s7              ; SubWord( RotWord( temp ) )
                    CALL      SBox
                    INPUT      s7, @s8

                    INPUT      pKey, @key

                    INPUT      s0, @b128
key96:              LOAD        s8, pKey            ; k[i]=k[i - Nk] ^ temp
                    XOR       s4, s8
                    STORE        s4, @pKey
                    ADD       pKey, 1

                    LOAD        s8, pKey            ; k[i]=k[i - Nk] ^ temp
                    XOR       s5, s8
                    STORE        s5, @pKey
                    ADD       pKey, 1

                    LOAD        s8, pKey            ; k[i]=k[i - Nk] ^ temp
                    XOR       s6, s8
                    STORE        s6, @pKey
                    ADD       pKey, 1

                    LOAD        s8, pKey            ; k[i]=k[i - Nk] ^ temp
                    XOR       s7, s8
                    STORE        s7, @pKey
                    ADD       pKey, 1

                    SUB       s0, 4
                    JUMP      NZ, key96
                    RETURN

; Sub bytes of one 32b word pointed at by pKey
SubWord:
                    INPUT      s0, 4
SubWord1:           LOAD        s8, pKey
                    CALL      SBox
                    STORE        s8, @pKey
                    ADD       pKey, 1
                    SUB       s0, 1
                    JUMP      NZ, SubWord1
                    RETURN

; SubBytes( state, Nc )
SubBytes:
                    INPUT      pState, @state       ; get pointer to state

                    INPUT      s0, b128            ; set up loop count
sub128:             LOAD        s8, pState          ; get state byte
                    CALL      SBox
                    STORE        s8, pState          ; save new state byte
                    ADD       pState, 1           ; increment state pointer
                    SUB       s0, 1               ; decrement loop counter
                    JUMP      NZ, sub128          ; loop back if not done 16 times (128/8)
                    RETURN

; SBox( s )
SBox:
                    OUTPUT       s8, SBOX_ROM        ; set index
                    INPUT        s8, SBOX_ROM        ; get data
                    RETURN

; soft version of SBOX, very slow
SBox_Soft:
                    CALL      MulInverse          ; .    x = sbox_affine(mul_inverse(in));
SBoxAffine:
; for(counter = 1; counter > (DEGREE - 1)) | (s << 1); s &= MASK;
                    XOR       s8, s9              ; in ^= s;
                    RL        s9
                    XOR       s8, s9
                    RL        s9
                    XOR       s8, s9
                    RL        s9
                    XOR       s8, s9
                    XOR       s8, #63             ; in ^= 0x63;
                    RETURN                           ; return in;
; }

; MulInverse by trial and error
MulInverse:
                    INPUT      s9, 0               ; int result = 0;
                    OR        s8, s8              ; if (in == 0)
                    RETURN       Z                   ; return 0;
MulInverse1:        ADD       s9, 1               ; result = 1; result++
                    RETURN       Z                   ; result < MOD
                    INPUT      sC, @s8              ; in
                    INPUT      sD, @s9              ; result
                    CALL      GMul                ; gmul( in, result, ...)
                    SUB       sE, 1               ; == 1
                    JUMP      NZ, MulInverse1     ; == 1?
                    RETURN                           ; return result

GMul:
                    INPUT      sE, 0
GMul1:
                    SR0       sD
                    JUMP      C, GMul2           ; ; last bit was 1
                    RETURN       Z                  ; ; i2 was 0 already ?
                    JUMP      GMul3

GMul2:              XOR       sE, sC
GMul3:              SL0       sC
                    JUMP      NC, GMul1
                    XOR       sC, G              ; ; i1 ^= field;
                    JUMP      GMul1

;; ShiftRows( state, Nc )
ShiftRows:
                    LOAD        s7, state + 1
                    LOAD        s4, state + 1 + 4
                    LOAD        s5, state + 1 + 4 + 4
                    LOAD        s6, state + 1 + 4 + 4 + 4
                    STORE        s4, state + 1
                    STORE        s5, state + 1 + 4
                    STORE        s6, state + 1 + 4 + 4
                    STORE        s7, state + 1 + 4 + 4 + 4

                    LOAD        s6, state + 2
                    LOAD        s7, state + 2 + 4
                    LOAD        s4, state + 2 + 4 + 4
                    LOAD        s5, state + 2 + 4 + 4 + 4
                    STORE        s4, state + 2
                    STORE        s5, state + 2 + 4
                    STORE        s6, state + 2 + 4 + 4
                    STORE        s7, state + 2 + 4 + 4 + 4

                    LOAD        s5, state + 3
                    LOAD        s6, state + 3 + 4
                    LOAD        s7, state + 3 + 4 + 4
                    LOAD        s4, state + 3 + 4 + 4 + 4
                    STORE        s4, state + 3
                    STORE        s5, state + 3 + 4
                    STORE        s6, state + 3 + 4 + 4
                    STORE        s7, state + 3 + 4 + 4 + 4

                    RETURN

;; MixColumns( state, Nc )
MixColumns:

                    LOAD        s4, state + 0
                    LOAD        s5, state + 1
                    LOAD        s6, state + 2
                    LOAD        s7, state + 3
                    CALL      MixColumn
                    STORE        s4, state + 0
                    STORE        s5, state + 1
                    STORE        s6, state + 2
                    STORE        s7, state + 3

                    LOAD        s4, state + 0 + 4
                    LOAD        s5, state + 1 + 4
                    LOAD        s6, state + 2 + 4
                    LOAD        s7, state + 3 + 4
                    CALL      MixColumn
                    STORE        s4, state + 0 + 4
                    STORE        s5, state + 1 + 4
                    STORE        s6, state + 2 + 4
                    STORE        s7, state + 3 + 4

                    LOAD        s4, state + 0 + 4 + 4
                    LOAD        s5, state + 1 + 4 + 4
                    LOAD        s6, state + 2 + 4 + 4
                    LOAD        s7, state + 3 + 4 + 4
                    CALL      MixColumn
                    STORE        s4, state + 0 + 4 + 4
                    STORE        s5, state + 1 + 4 + 4
                    STORE        s6, state + 2 + 4 + 4
                    STORE        s7, state + 3 + 4 + 4

                    LOAD        s4, state + 0 + 4 + 4 + 4
                    LOAD        s5, state + 1 + 4 + 4 + 4
                    LOAD        s6, state + 2 + 4 + 4 + 4
                    LOAD        s7, state + 3 + 4 + 4 + 4
                    CALL      MixColumn
                    STORE        s4, state + 0 + 4 + 4 + 4
                    STORE        s5, state + 1 + 4 + 4 + 4
                    STORE        s6, state + 2 + 4 + 4 + 4
                    STORE        s7, state + 3 + 4 + 4 + 4

                    RETURN

MixColumn:
                    INPUT      s9, @s4             ; ; t = c[0] ^ c[3]
                    XOR       s9, s7
                    INPUT      sA, @s5             ; ; u = c[1] ^ c[2]
                    XOR       sA, s6
                    INPUT      sB, @s9            ;  ; v = t ^ u
                    XOR       sB, sA

                    INPUT      s8, @s4             ; ; c[0] = c[0] ^ v ^ FFmul(0x02, c[0] ^ c[1])
                    XOR       s8, s5
                    SL0       s8
                    JUMP      NC, mcf1
                    XOR       s8, G
mcf1:               XOR       s8, sB
                    XOR       s4, s8

                    INPUT      s8, @sA            ;  ; c[1] = c[1] ^ v ^ FFmul(0x02, u)
                    SL0       s8
                    JUMP      NC, mcf2
                    XOR       s8, G
mcf2:               XOR       s8, sB
                    XOR       s5, s8

                    INPUT      s8, @s6             ; ; c[2] = c[2] ^ v ^ FFmul(0x02, c[2] ^ c[3])
                    XOR       s8, s7
                    SL0       s8
                    JUMP      NC, mcf3
                    XOR       s8, G
mcf3:               XOR       s8, sB
                    XOR       s6, s8

                    INPUT      s8, @s9             ; ; c[3] = c[3] ^ v ^ FFmul(0x02, t)
                    SL0       s8
                    JUMP      NC, mcf4
                    XOR       s8, G
mcf4:               XOR       s8, sB
                    XOR       s7, s8

                    RETURN