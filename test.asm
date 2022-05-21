; nasm + link + LibC

; nasm "2-Nasm+Link.asm" -f win64 -l "2-Nasm+Link.lst"
; link "2-Nasm+Link.obj" libcmt.lib kernel32.lib /subsystem:console


; bits 32
[BITS 64]

NULL              EQU 0                         ; Constants
STD_OUTPUT_HANDLE EQU -11
STDOUT			equ		1
STDIN			equ		2


global std_printf
; extern _printf
; extern __cputs
; extern _putchar

section .text

global _start

std_printf:
                pop r10
                mov qword [printfSaveBp], r10
                pop r10
                mov qword [printfRetAddr], r10
                
                push r9
                push r8
                push rcx
                push rdx
                push rsi
                push rdi

                mov rbp, rsp

                xor r10, r10

                ; push rbp                ;making stack frame
                ; mov rbp, rsp
                ; add rbp, 16              

                xor rdx, rdx            
                mov rdi, buff           ;init buff
                ; mov rsi, [rbp]          ;init format string
				mov rsi, msg
                add rbp, byte 8
                mov rdx, 0
                
                ; mov rsi, msg
                ; mov rdi, buff
                
                ; mov cs, 

    .loop:      
                xor rdx, rdx
                xor rax, rax
                lodsb
                cmp al, 0
                je .endloop
                cmp al, '%'
                jne .standart
                inc r10
                ; je .endloop
                ; xor ecx, ecx
                ; call ExitProcess
                lodsb                
                
                ;do some switch shit
                cmp rax, 'b'            ;if less than b
                jl .standart
                ; je .endloop
                cmp rax, 's'            ;if grtr than s
                jg .standart
                ; jg .endloop

                sub rax, 'b'            ;middle part
                ; xor ecx, ecx
                ; call ExitProcess
                jmp [printf_switch + rax * 8]

        .char	mov rdx, [rbp]
                add rbp, 8
                stosb
                ; xor ecx, ecx
                ; call ExitProcess
                jmp .end_choice

        .bin 	push rdx
                push rsi
                mov rdx, bin_mask
                mov rcx, 1
                jmp .print_b

        .decimal push rdx
                push rsi
                mov rbx, 10
                mov rax, [rbp]
                call itoad
                pop rsi
                pop rdx
                add rbp, 8

                ; jmp .end_choice
                jmp .end_choice

        .hex 	push rdx
                push rsi
                mov rdx, hex_mask ;mask
                mov rcx, 4      ;shift offs
                jmp .print_b

        .oct	push rdx
                push rsi
                mov rdx, oct_mask ;mask
                mov rcx, 3      ;shift offs
                jmp .print_b

        .print_b mov rbx, [rbp]
                call itoaBin
                add rbp, 8
                pop rsi
                pop rdx
                jmp .end_choice

        .string	call dropbuff
                
                push rdi
                mov rdi, [rbp]
                call _strlen
                pop rdi
                
                push rsi
                mov rdx, rax
                mov rax, 1
                mov rdi, STDOUT
                mov rsi, [rbp]
                syscall
                pop rsi
                ;==========================================================
                        ; call __cputs
                ; add rsp, byte 8
                add rbp, byte 8
                mov rdi, buff
                xor rdx, rdx
                jmp .end_choice
                ; call dropbuff                


                
                je .endloop
    .standart:
                ; xor ecx, ecx
                ; call ExitProcess
                stosb
                inc rdx

    .end_choice:

                ; cmp rdx, buff_len
                ; jne .buff_undrop
                call dropbuff
    .buff_undrop:
                jmp .loop



    .endloop:	call dropbuff

                ; pop rbp
                ; pop rbp
                ; call ExitProcess

                inc r10
                cmp r10, 6
                jge .args_gr_six
                add rsp, 8 * 6
                jmp .skip_to_ret

        .args_gr_six mov rax, r10
                mov rcx, 8
                mul rcx
                add rsp, rax
                dec r10
                mov rax, r10

.skip_to_ret	push qword[printfRetAddr]
                push qword[printfSaveBp]
    ret


;==========================================================
; using last value of rdi from printf as pos to be set \0
; prints string
;==========================================================
dropbuff:
                mov byte [rdi], 0
                push rdi
                mov rdi, buff
                call _strlen
                pop rdi

                push rsi
                mov rdx, rax
                mov rax, 1
                mov rdi, STDOUT
                mov rsi, buff
                syscall
                pop rsi

                ; push qword buff
                ; ;==========================================================
                ; push STD_OUTPUT_HANDLE
                ; call GetStdHandle


                ; push 0
                ; push 
                ; mov rcx, rdx
                ; sub rsp, 40
                
                ; call __cputs
                ; add rsp, byte 8
                mov rdi, buff
                xor rdx, rdx
                
                
                ; push qword Text
                ; call _printf            ; printf (MsgText)
                ; add rsp, 8
                ret


;==========================================================
; using last value of rdi from printf as pos to be set \0
; prints string
;==========================================================
itoaBin:
                ; push rdx
                push rsi
                ; xor rdx, rdx

                xor rax, rax
                .extract_sym:
                mov rsi, rbx
                and rsi, rdx ;rdx
                push qword rsi ; push sym
                shr rbx, cl
                inc rax                     ; cnt of syms               
                test rbx, rbx
                jnz .extract_sym            ; end of number 

                .write_sym:
                pop qword rbx
                mov bl, [itoa_symbols_table + rbx]

                mov [rdi], bl
                inc rdi
                dec rax
                test rax, rax
                jnz .write_sym
                pop rsi

                ret


delim:

itoad			push rdx
                
                xor rcx, rcx


                .extract_sym:
                xor rdx, rdx
                div rbx
                inc rcx                         ; cnt of syms
                push rdx                        ; push sym
                test rax, rax
                jnz .extract_sym                ; end of number 

                .write_sym:
                pop qword rbx
                mov bl, [itoa_symbols_table + rbx]       ;load char from table
                mov [rdi], bl                           ;mov char to memory
                inc rdi                                 ;inc buff ptr
                dec rcx                                 ;now need to print 1 less sym
                test rcx, rcx                           ;test if we dont need print anymore
                jnz .write_sym

                pop rdx
                ret



_strlen:

                push rbx
                push rcx

                mov   rbx, rdi            
                xor   al, al                               
                mov   rcx, 0xffffffff     
                                                                
                repne scasb               ; REPeat while Not Equal [edi] != al

                sub   rdi, rbx            ; length = offset of (edi - ebx)
                mov   rax, rdi            

                pop rbx
                pop rcx
                ret 

my_scan_dec:
	xor rax, rax      ; rax <- 0 (syscall number for 'read')
	xor rdi, rdi      ; edi <- 0 (stdin file descriptor)
	mov rsi, buff   ; rsi <- address of the buffer.  lea rsi, [rel buffer]
	mov rdx, 20  ; rdx <- size of the buffer
	syscall  

	mov rdx, buff

	xor rcx, rcx
        mov cl, BYTE  [rdx]
        xor rax, rax

        xor cl, '0'  ;Translates '0'..'9' to 0..9 (nasty trick I'd say Smile )
        cmp cl, 10
        jae end

	convert_loop:
		lea rax, [rax+4*rax]  ; eax*=5
		inc rdx
		lea rax, [rcx+2*rax]  ; eax=eax*2+ecx

		mov cl, BYTE  [rdx]
		xor cl, '0'
		cmp cl, 10
		jb convert_loop

	end:

	ret



_start:

	call my_scan_dec

	

	; push qword 15
	push rax
	mov rdi, msg
	mov rsi, rax
	; push msg
	call std_printf
    ; mov rdi, decm
    ; push rdi
    ; call _strlen
    ; pop rdi
    ; mov rsi, rdi
    ; mov rdx, rax
    ; mov rax, 1
    ; mov rdi, STDOUT

    ; syscall
	
	; pop rax
	push 2
	push 1
	pop rcx
    pop rbx
    cmp rcx, rbx
    jg .if_true
    push 0
    jle .if_end
        .if_true:
        push 1
        .if_end:

	pop rsi
	mov rdi, msg
	call std_printf

    mov	rax, 60	                    ; exit
    mov	rdi, 0                      ; with success
    syscall

    ret


; _printf_caller:
        

       
;         ; call printf
;         ; mov byte [rdi], 0
;         push rdi
;         call _strlen
;         pop rdi
;         mov rsi, rdi
;         mov rdx, rax
;         mov rax, 1
;         mov rdi, STDOUT
        
;         syscall



        

;         ret






section .data
itoa_symbols_table   db "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
decm                db "/decm/", 0
hexm                db "hexm", 0
charm               db "charm", 0ah, 0
stringm             db "string", 0ah, 0
octm                db "oct", 0ah, 0
Text                db "", 0
; msg                 db "%s | %% %d %b %c", 0
msg                 db "%d", 0ah, 0
len             	equ $-msg	
Message        db "Console Message 64", 0Dh, 0Ah
MessageLength  EQU $-Message   


section .const
buff_len        equ 120
buff_lenend     equ 120 + 33                        ; max len of base output type
                                                ; + 1 for \0
bin_mask        equ 0001h 
oct_mask        equ 0007h 
hex_mask        equ 000fh 

STD_OUTPUT_HANDLE   equ -11
STD_INPUT_HANDLE    equ -10

printf_switch: 
                            dq std_printf.bin
                            dq std_printf.char
                            dq std_printf.decimal
                times 3d    dq std_printf.standart
                            dq std_printf.hex
                times 6d    dq std_printf.standart
                            dq std_printf.oct
                times 3d    dq std_printf.standart
                            dq std_printf.string

section .bss
printfRetAddr resq 1
printfSaveBp resq 1

buff                    resb buff_lenend
StandardHandle          resq 1
char_written            resb    4
chars                   resb 4 