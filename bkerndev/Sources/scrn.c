#include <system.h>

/* Nuoroda į teksto atmintį,  teskto ir fono spalvos,  x ir y kursorių koordinatės*/
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/* Paslenka ekraną */
void scroll(void)
{
    unsigned blank, temp;

    /* Tuščia vieta yra tarpas, kuris gali turėti fono spalvą */
    blank = 0x20 | (attrib << 8);

    /* 25 eilutė paskutė, reiškia reikia paslinkti į viršų */
    if(csr_y >= 25)
    {
        /* Patraukia dabartinį tekstą viena eilute atgal */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Paskutinę eilutę užpildom tarpais */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Atnaujina kursorių: mirksinti linija akrane
*  po paskutinio atspausdinto simbolio */
void move_csr(void)
{
    unsigned temp;

    /* Formulė indekso radimui linijinėj atminty, išreikšta:
    *  Indeksas = [(y * plotis) + x] */
    temp = csr_y * 80 + csr_x;

  /* Nusiunčia komandą į 14 ir 15 indeksus esančius
    * VGA kontrolerio CRT valdymo registre (Control Register). 
    * Tai yra aukštesnieji ir žemesnieji baiti, kurie rodo, kur
    *  kursorius turi mirksėti.
    *  Detaliau:  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* Išvalo ekraną */
void cls()
{
    unsigned blank;
    int i;

    /* Short tipo kintamasis, kuris reiškia tarpą*/
    blank = 0x20 | (attrib << 8);

    /* Visą ekraną užpildo tarpais */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

    /* Atnaujina virtualų kursorių ir pastumia tikrąjį*/
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* Išveda vieną simbolį */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Backspace simbolio valdymas, grįžtam viena pozicija atgal */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Tab simbolio valdymas vykdomas paiddintant kursorių x. Bet į 
	* tokią poziciją, kuri dalinasi iš 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* 'Carriage Return' apdorjimas - kursorius grąžinamas į 
	* eilutės pradžią */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* Nauja eilutė: grįžtam į pradžią eilutės horizontaliai,
	vertikaliai viena eilute žemyn*/
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Simbolių išvedimas */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Simbolis ir atributas - spalva */
        csr_x++;
    }

    /* Jei kursorius pasiekė eilutės pabaigą, tai pereina į naują eilutę */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Paslenka ekraną, jei reikia ir pajudina kursorių */
    scroll();
    move_csr();
}

/* Išveda teksto gabalą naudodamas aukščiau esančią funkciją 
* vienam simboliui išvesti*/
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Spalvų nustatymas*/
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Viršutiniai 4 bitai fonas, apatiniai 4 teksto spalva */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Nustato tekstinės atminties adresą ir išvalo ekraną */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
