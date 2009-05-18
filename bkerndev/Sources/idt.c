/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* Apibr��ia PDL �ra�� */
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

/* 256 PDL �ra�� deklaravimas. Ta�iau pavyzdin� programa naudos 
 * tik 32. Jei �vyksta neapibr��tas pertraukimas, tuomet tai sukelia
 * "Unhandled Interrupt" klaid�. Be to, kiekvienas deskriptorius, 
 * kurio naudojamumo bitas 0, generuoja toki� pat klaid� */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* �i funkcija apra�yta start.asm. Ji skirta PDL u�krovimui */
extern void idt_load();

/* Funkcija skirta PDL �ra�� nustatymui */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* Pertraukimo popragram�s bazinis adresas */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
	
	/* Segmentas arba selektorius (angl. selector), kur�
	* naudos �is PDL �ra�as */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* �diega PDL */
void idt_install()
{
    /* Sukuria speciali� nuorod�, reikaling� PDL u�krovimui.
	 * Identi�ka GDL nuorodai. */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Visus PDL �ra�us "nunulina" */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* �ia galima �traukti naujas pertraukim� apdorojimo paprogrames,
	 * naudojantis funkcija idt_set_gate */



    /* Nustato procesoriaus registr� � PDL */
    idt_load();
}
