#include <system.h>

/* Nuoroda á teksto atmintá,  teskto ir fono spalvos,  x ir y kursoriø koordinatës*/
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/* Paslenka ekranà */
void scroll(void)
{
    unsigned blank, temp;

    /* Tuðèia vieta yra tarpas, kuris gali turëti fono spalvà */
    blank = 0x20 | (attrib << 8);

    /* 25 eilutë paskutë, reiðkia reikia paslinkti á virðø */
    if(csr_y >= 25)
    {
        /* Patraukia dabartiná tekstà viena eilute atgal */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Paskutinæ eilutæ uþpildom tarpais */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Atnaujina kursoriø: mirksinti linija akrane
*  po paskutinio atspausdinto simbolio */
void move_csr(void)
{
    unsigned temp;

    /* Formulë indekso radimui linijinëj atminty, iðreikðta:
    *  Indeksas = [(y * plotis) + x] */
    temp = csr_y * 80 + csr_x;

  /* Nusiunèia komandà á 14 ir 15 indeksus esanèius
    * VGA kontrolerio CRT valdymo registre (Control Register). 
    * Tai yra aukðtesnieji ir þemesnieji baiti, kurie rodo, kur
    *  kursorius turi mirksëti.
    *  Detaliau:  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Iðvalo ekranà */
void cls()
{
    unsigned blank;
    int i;

    /* Short tipo kintamasis, kuris reiðkia tarpà*/
    blank = 0x20 | (attrib << 8);

    /* Visà ekranà uþpildo tarpais */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

    /* Atnaujina virtualø kursoriø ir pastumia tikràjá*/
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Iðveda vienà simbolá */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Backspace simbolio valdymas, gráþtam viena pozicija atgal */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Tab simbolio valdymas vykdomas paiddintant kursoriø x. Bet á 
	* tokià pozicijà, kuri dalinasi ið 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* 'Carriage Return' apdorjimas - kursorius gràþinamas á 
	* eilutës pradþià */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* Nauja eilutë: gráþtam á pradþià eilutës horizontaliai,
	vertikaliai viena eilute þemyn*/
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Simboliø iðvedimas */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Simbolis ir atributas - spalva */
        csr_x++;
    }

    /* Jei kursorius pasiekë eilutës pabaigà, tai pereina á naujà eilutæ */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Paslenka ekranà, jei reikia ir pajudina kursoriø */
    scroll();
    move_csr();
}

/* Iðveda teksto gabalà naudodamas aukðèiau esanèià funkcijà 
* vienam simboliui iðvesti*/
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Spalvø nustatymas*/
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Virðutiniai 4 bitai fonas, apatiniai 4 teksto spalva */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Nustato tekstinës atminties adresà ir iðvalo ekranà */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
