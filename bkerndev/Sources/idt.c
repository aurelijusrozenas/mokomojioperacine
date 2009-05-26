/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* Apibrėžia PDL įrašą */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Branduolio segmentas */
    unsigned char always0;     /* Visada nulis */
    unsigned char flags;       
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* 256 PDL įrašų deklaravimas. Tačiau pavyzdinė programa naudos 
 * tik 32. Jei įvyksta neapibrėžtas pertraukimas, tuomet tai sukelia
 * "Unhandled Interrupt" klaidą. Be to, kiekvienas deskriptorius, 
 * kurio naudojamumo bitas 0, generuoja tokią pat klaidą */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* Ši funkcija aprašyta start.asm. Ji skirta PDL užkrovimui */
extern void idt_load();

/* Funkcija skirta PDL įrašų nustatymui */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* Pertraukimo popragramės bazinis adresas */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
	
	/* Segmentas arba selektorius (angl. selector), kurį
	* naudos šis PDL įrašas */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Įdiega PDL */
void idt_install()
{
    /* Sukuria specialią nuorodą, reikalingą PDL užkrovimui.
	 * Identiška GDL nuorodai. */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Visus PDL įrašus "nunulina" */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Čia galima įtraukti naujas pertraukimų apdorojimo paprogrames,
	 * naudojantis funkcija idt_set_gate */



    /* Nustato procesoriaus registrą į PDL */
    idt_load();
}
