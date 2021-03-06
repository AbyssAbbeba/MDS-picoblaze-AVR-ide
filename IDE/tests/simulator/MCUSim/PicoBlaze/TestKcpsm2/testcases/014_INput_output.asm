; testing of INPUT and OUTPUT instructions
device kcpsm2
port_id         PORT            20h

inout_data      REG             s0
pointer_in      REG             s1
pointer_out     REG             s2


org 00h
; testing here
                ;; flag[C] == false
                ;; flag[Z] == false
start:          LOAD            pointer_in,#00h
                LOAD            pointer_out,#00h
                LOAD            inout_data,#ffh
                ;; step 3
                ;; reg[0] == 0xff
                ;; reg[1] == 0
                ;; reg[2] == 0
                ;; flag[C] == false
                ;; flag[Z] == false
                INPUT           inout_data,port_id
                ;; step
                

                
dataIN:         INPUT           inout_data,@pointer_in
                ;; STEP
               
                ;; reg[1] == 0
                ;; flag[C] == false
                ;; flag[Z] == false
                ADD             pointer_in,#1
                ;; step
                ;; reg[1] == 1
                ;; flag[C] == false
                ;; flag[Z] == false

                JUMP            NZ,dataIN
                ;; step

                ;; flag[Z] == false

                ;; # jumped to dataIN
                ;; step 

                ;; flag[Z] == false
                ;; step
                ;; # add instructions
                ;; flag[C] == false
                ;; flag[Z] == false
                ;; step
                ;; # cmp instruction
                ;; flag[C] == false

                ;; step
                ;; # jump instruction
                ;; flag[C] == false


                ;; step 3

                ;; flag[Z] == false
                 
dataOUT:        OUTPUT          inout_data,@pointer_out
                ADD             pointer_in,#1
                JUMP            NZ,dataOUT

                