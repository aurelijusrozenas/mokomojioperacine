#include <system.h>

/* Nuoroda � teksto atmint�,  teskto ir fono spalvos,  x ir y kursori� koordinat�s*/
unsigned short *textmemptr;
int attrib = 0x0F;
int csr_x = 0, csr_y = 0;

/* Paslenka ekran� */
void scroll(void)
{
    unsigned blank, temp;

    /* Tu��ia vieta yra tarpas, kuris gali tur�ti fono spalv� */
    blank = 0x20 | (attrib << 8);

    /* 25 eilut� paskut�, rei�kia reikia paslinkti � vir�� */
    if(csr_y >= 25)
    {
        /* Patraukia dabartin� tekst� viena eilute atgal */
        temp = csr_y - 25 + 1;
        memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        /* Paskutin� eilut� u�pildom tarpais */
        memsetw (textmemptr + (25 - temp) * 80, blank, 80);
        csr_y = 25 - 1;
    }
}

/* Atnaujina kursori�: mirksinti linija akrane
*  po paskutinio atspausdinto simbolio */
void move_csr(void)
{
    unsigned temp;

    /* Formul� indekso radimui linijin�j atminty, i�reik�ta:
    *  Indeksas = [(y * plotis) + x] */
    temp = csr_y * 80 + csr_x;

  /* Nusiun�ia komand� � 14 ir 15 indeksus esan�ius
    * VGA kontrolerio CRT valdymo registre (Control Register). 
    * Tai yra auk�tesnieji ir �emesnieji baiti, kurie rodo, kur
    *  kursorius turi mirks�ti.
    *  Detaliau:  http://www.brackeen.com/home/vga */
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

/* I�valo ekran� */
void cls()
{
    unsigned blank;
    int i;

    /* Short tipo kintamasis, kuris rei�kia tarp�*/
    blank = 0x20 | (attrib << 8);

    /* Vis� ekran� u�pildo tarpais */
    for(i = 0; i < 25; i++)
        memsetw (textmemptr + i * 80, blank, 80);

    /* Atnaujina virtual� kursori� ir pastumia tikr�j�*/
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

/* I�veda vien� simbol� */
void putch(unsigned char c)
{
    unsigned short *where;
    unsigned att = attrib << 8;

    /* Backspace simbolio valdymas, gr��tam viena pozicija atgal */
    if(c == 0x08)
    {
        if(csr_x != 0) csr_x--;
    }
    /* Tab simbolio valdymas vykdomas paiddintant kursori� x. Bet � 
	* toki� pozicij�, kuri dalinasi i� 8 */
    else if(c == 0x09)
    {
        csr_x = (csr_x + 8) & ~(8 - 1);
    }
    /* 'Carriage Return' apdorjimas - kursorius gr��inamas � 
	* eilut�s prad�i� */
    else if(c == '\r')
    {
        csr_x = 0;
    }
    /* Nauja eilut�: gr��tam � prad�i� eilut�s horizontaliai,
	vertikaliai viena eilute �emyn*/
    else if(c == '\n')
    {
        csr_x = 0;
        csr_y++;
    }
    /* Simboli� i�vedimas */
    else if(c >= ' ')
    {
        where = textmemptr + (csr_y * 80 + csr_x);
        *where = c | att;	/* Simbolis ir atributas - spalva */
        csr_x++;
    }

    /* Jei kursorius pasiek� eilut�s pabaig�, tai pereina � nauj� eilut� */
    if(csr_x >= 80)
    {
        csr_x = 0;
        csr_y++;
    }

    /* Paslenka ekran�, jei reikia ir pajudina kursori� */
    scroll();
    move_csr();
}

/* I�veda teksto gabal� naudodamas auk��iau esan�i� funkcij� 
* vienam simboliui i�vesti*/
void puts(unsigned char *text)
{
    int i;

    for (i = 0; i < strlen(text); i++)
    {
        putch(text[i]);
    }
}

/* Spalv� nustatymas*/
void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    /* Vir�utiniai 4 bitai fonas, apatiniai 4 teksto spalva */
    attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Nustato tekstin�s atminties adres� ir i�valo ekran� */
void init_video(void)
{
    textmemptr = (unsigned short *)0xB8000;
    cls();
}
