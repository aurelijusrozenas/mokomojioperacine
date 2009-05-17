/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Global Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* Defines a GDT entry */
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gp;

/* Funkcija, kuri apibr��ta faile start.asm, 
* kurios paskirtis yra, tinkamai u�krauti nauj� GDL */
extern void gdt_flush();

/* Nustato deskriptori� GDL viduje */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Nustato deskriptoriaus bazin� adres� */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Nustato deksriptoriaus limitus */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Nustato granuli�kum� ir pri�jimo teises */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* Funkcijos paskirtis yra sukurti nauj� GDL. 
*  �i funkcija sukuria specialios strukt�ros nuorod�, kurios pagalba 
*  galima u�krauti nauj�j� GDL. 
*  Taip pat ji �ra�o tris deskriptorius.
*  Pa�ioje pabaigoje i�kvie�ia gdt_flush(), kuris nurodo procesoriui,
*  kur yra naujoji GDL  bei perstato segment� registrus */
void gdt_install()
{
    /* Nustato GDL lentel�s nuorod� ir limit�*/
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    /* NULL deskriptorius*/
    gdt_set_gate(0, 0, 0, 0, 0);

    /* Antrasis �ra�as yra kodo segmentas. 
	* Bazinis adresas 0, limitas 4 gigabaitai, granuli�kumas 4 kilobaitai,
	*  naudoja 32 bit�  operacijas */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* Lygiai tas pats, kas ankstesnis �ra�as, ta�iau deskriptoriaus tipas
	* nurodo, kad tai duomen� segmentas */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Sen�j� GDL pakei�ia naujuoju! */
    gdt_flush();
}
