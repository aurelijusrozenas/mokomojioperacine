; bkerndev - Bran's Kernel Development Tutorial
; By:   Brandon F. (friesenb@gmail.com)
; Desc: Kernel entry point, stack, and Interrupt Service Routines.
;
; Notes: No warranty expressed or implied. Use at own risk.
;
; This is the kernel's entry point. We could either call main here,
; or we can use this to setup the stack or other nice stuff, like
; perhaps setting up the GDT and segments. Please note that interrupts
; are disabled at this point: More on interrupts later!
[BITS 32]
global start
start:
    mov esp, _sys_stack     ; This points the stack to our new stack area
    jmp stublet

; This part MUST be 4byte aligned, so we solve that issue using 'ALIGN 4'
ALIGN 4
mboot:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM
    
    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd mboot
    dd code
    dd bss
    dd end
    dd start

; This is an endless loop here. Make a note of this: Later on, we
; will insert an 'extern _main', followed by 'call _main', right
; before the 'jmp $'.
stublet:
    extern _main
    call _main
    jmp $

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush();'
global _gdt_flush     ; leid�ia C kodui naudotis �ia funkcija
extern _gp            ;  nurodo, kad gp kintamasis apibr��tas kitame faile
_gdt_flush:
    lgdt [gp]         ; u�krauna GDL naudodamasis m�s� specialia nuoroda
    mov ax, 0x10      ; 0x10 yra duomen� segmento poslinkis naujoje GDL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2   ; 0x08 yra kodo segmento poslinkis
flush2:
    ret               ; gr��ta � C kod�

; U�krauna PDL, pasinaudodama specialia nuoroda idtp.
; �i funkcija kvie�iama i� C kodo
global idt_load
extern idtp
idt_load:
    lidt [idtp]
    ret

; Pertraukim� apdorojimo �ym�s
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

;  0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

;  1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

;  3: Int 3 Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

;  4: INTO Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

;  7: Coprocessor Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

;  8: Double Fault Exception (su klaidos kodu)
isr8:
    cli
    push byte 8
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Bad TSS Exception (su klaidos kodu)
isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (su klaidos kodu)
isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack Fault Exception (su klaidos kodu)
isr12:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (su klaidos kodu)
isr13:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (su klaidos kodu)
isr14:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Reserved Exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: Floating Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; 18: Machine Check Exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub


; C faile esanti funkcija
extern fault_handler

;  I�saugo procesoriaus b�sen�, i�kvie�ia C kalbos funkcij�,
; galiausiai atstato b�sen� i� steko
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10	; u�krauna branduolio duomen� segmento deskriptori�
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, fault_handler	; u�krauna branduolio duomen� segmento deskriptori�
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8 ; I�valo klaidos kod� ir PAP numer�
    iret       ; i� steko paima penkias reik�mes: CS, EIP, EFLAGS, SS ir ESP


global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; 32: IRQ0
_irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

; 33: IRQ1
_irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

; 34: IRQ2
_irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

; 35: IRQ3
_irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

; 36: IRQ4
_irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

; 37: IRQ5
_irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

; 38: IRQ6
_irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

; 39: IRQ7
_irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

; 40: IRQ8
_irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

; 41: IRQ9
_irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

; 42: IRQ10
_irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

; 43: IRQ11
_irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

; 44: IRQ12
_irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

; 45: IRQ13
_irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

; 46: IRQ14
_irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

; 47: IRQ15
_irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub

extern _irq_handler

irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp

    push eax
    mov eax, _irq_handler
    call eax
    pop eax

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; Here is the definition of our BSS section. Right now, we'll use
; it just to store the stack. Remember that a stack actually grows
; downwards, so we declare the size of the data before declaring
; the identifier '_sys_stack'
SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:

