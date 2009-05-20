/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Request management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* Tai yra PAP, kurie rodo á specialø PU dorotojà vietoj to,
* kad rodytø á tipinæ funkijà "fault_handler" kitoms PAP */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Funkcijø nuorodø masyvas, kurio paskirtis saugoti
* funkcijø adresus. Vëliau ðie adresai naudojami
* PU apdorojimui */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* Ádiegia nestandartinæ PU doroktlæ pateiktam PU pagal numerá*/
void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* Paðalina pateikto PU doroklæ ið funkcijø nuorodø sàraðo */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* Standartiðkai, PU  nuo 0 iki 7 rodo á PDL áraðus nuo 8 iki 15. 
* Tai yra problema,  nes ðios reiðkmës yra jau uþimtos,
* PU nuo  0 iki 15 perprogramuojamas á PDL áraðus nuo 32 iki 47 */
void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/* Pirma perprogramuojami pertraukimø valdikliai (irq_remap).
* Po to PAP nustatomos á atitinkamus PDL áraðus. */
void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);

    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

/* Kiekviena PU paprogramë rodo á ðià funkcijà, o ne "fault_handler".
*  PU valdikliams reikia praneðti, kada jie buvo aptarnauti, t.y.,
*  nusiøsti jiems komandà - "Pertraukimo pabaiga" (PA).
*  Yra  du lustai: pirmas adresu 0x20, antras - 0xA0. Jei ávyksta 
*  pertraukimas ðalutiniam valdiklyje (PU nuo 8 iki 15, tuomet 
*  apie apdorojimo pabaigà reikia informuoti abu valdiklius.
*  Kitu atveju reikia tik nusiøsti PA á pirmàjá valdiklá.
*  Jei ðis praneðimas nebus nusiøstas, tai neávyks në vienas
* naujas PU */
void irq_handler(struct regs *r)
{
    /* Tuðèia funkcijos nuoroda */
    void (*handler)(struct regs *r);

    /* IÐsiaiðkina ar yra paruoðta konkreèiam PU skirta doroklë */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* Jei ávykio aukðtesnio numerio pertraukimas nei 40
	* (atitinkamai PU 8 - 15),  tada reikia nusiøsti PA á antràjá
	* valdiklá */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* Bet kuriuo atveju PA reikia siøsti á pagrindiná valdiklá */
    outportb(0x20, 0x20);
}
