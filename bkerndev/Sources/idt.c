/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* Apibrëþia PDL áraðà */
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

/* 256 PDL áraðø deklaravimas. Taèiau pavyzdinë programa naudos 
 * tik 32. Jei ávyksta neapibrëþtas pertraukimas, tuomet tai sukelia
 * "Unhandled Interrupt" klaidà. Be to, kiekvienas deskriptorius, 
 * kurio naudojamumo bitas 0, generuoja tokià pat klaidà */
struct idt_entry idt[256];
struct idt_ptr idtp;

/* Ði funkcija apraðyta start.asm. Ji skirta PDL uþkrovimui */
extern void idt_load();

/* Funkcija skirta PDL áraðø nustatymui */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* Pertraukimo popragramës bazinis adresas */
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
	
	/* Segmentas arba selektorius (angl. selector), kurá
	* naudos ðis PDL áraðas */
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Ádiega PDL */
void idt_install()
{
    /* Sukuria specialià nuorodà, reikalingà PDL uþkrovimui.
	 * Identiðka GDL nuorodai. */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Visus PDL áraðus "nunulina" */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Èia galima átraukti naujas pertraukimø apdorojimo paprogrames,
	 * naudojantis funkcija idt_set_gate */



    /* Nustato procesoriaus registrà á PDL */
    idt_load();
}
