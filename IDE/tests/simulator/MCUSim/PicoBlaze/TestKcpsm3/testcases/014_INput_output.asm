; testing of INPUT and OUTPUT instructions

port_id         PORT            20h

inout_data      REG             s0
pointer_in      REG             s1
pointer_out     REG             s2


org 00h
; testing here

start:          LOAD            pointer_in,#00h
                LOAD            pointer_out,#00h
                LOAD            inout_data,#ffh

                INPUT           inout_data,port_id


                
dataIN:         INPUT           inout_data,@pointer_in
                ADD             pointer_in,#1
                CMP             pointer_in,#2
                JUMP            NZ,dataIN

dataOUT:        OUTPUT          inout_data,@pointer_out
                ADD             pointer_in,#1
                CMP             pointer_in,#255
                JUMP            NZ,dataOUT

                