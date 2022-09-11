#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<time.h>

struct cvor {
    
    struct cvor* otac;
    struct cvor* deca[100];
    int matrica[20][4];
    int br_dece;
    int nivo;
    int redni_broj;
    int resenje; // 1 ako je taj cvor resenje igre, 0 ako nije
    int putanja_do_resenja; // 1 ako se preko njega moze doci do resenja, 0 ako ne moze
    int ukupan_broj_cvorova;// ovo nam sluzi da imamo tu informaciju za koren
    int iz;
    int u;
}cvor;

struct Queue {
    struct cvor** array;
    int front;
    int rear;
    int size;
    int kapacitet;
};

struct Queue* formiraj(int kapacitet)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->kapacitet = kapacitet;
    queue->front = queue->size = 0;
    queue->rear = kapacitet - 1;
    queue->array = (struct cvor*)malloc(kapacitet*sizeof(cvor));
    return queue;
}

int pun(struct Queue* queue)
{
    if (queue->size == queue->kapacitet)
        return 1;
    return 0;
}

int isEmpty(struct Queue* queue)
{
    if (queue->size == 0)
        return 1;
    return 0;
}

void dodaj_u_red(struct Queue* queue, struct cvor* cvor)
{
    if (pun(queue))
        return;
    queue->rear = (queue->rear + 1)% queue->kapacitet;
    queue->array[queue->rear] = cvor;
    queue->size = queue->size + 1;
}

void izbaci(struct Queue* queue)
{
    if (isEmpty(queue))
        return 0;
    struct cvor* cvor = queue->array[queue->front];
    queue->front = (queue->front + 1)% queue->kapacitet;
    queue->size = queue->size - 1;
}

struct cvor* prvi(struct Queue* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->front];
}

struct cvor* poslednji(struct Queue* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->rear];
}


void ispis(int matrica[20][4],int vrste, int kolone,int nivo,int rb,int otac_rb)
{
    if (nivo != 0 && rb != 0 && otac_rb != 0)
    {
        printf("%d. element\n", rb);
        printf("Ovaj cvor se nalazi na nivou %d\n", nivo);
        printf("Otac je %d. element po redu\n", otac_rb);
    }
    //printf("\n-----------------");
    for (int i = 1; i <= vrste; i++)
    {
        printf("Bocica broj %d:  ", i);
        for (int j = 1; j <= kolone; j++)
            printf("%d ", matrica[i][j]);
        printf("\n");
    }
    printf("\n");
}

int resenje(int matrica[20][4], int vrste, int kolone)
{
    for(int i=1;i<=vrste;i++)
        for (int j = 1; j <= kolone-1; j++)
        {
            if (matrica[i][j] != matrica[i][j + 1])
                return 0;
        }
    return 1;
}

void preorder(struct cvor* koren,int br,int vrste, int kolone)
{
    struct Queue* red_posecenih = formiraj(br);
    struct Queue* stek = formiraj(br);

    dodaj_u_red(stek, koren);
    
    while (!isEmpty(stek))
    {
        struct cvor* pom = poslednji(stek);
        stek->size--;
        stek->rear = stek->rear - 1;
        dodaj_u_red(red_posecenih, pom);
        for (int i = pom->br_dece - 1; i >= 0; i--)
            dodaj_u_red(stek, pom->deca[i]);
    }
    printf("\n\n");
    for (int i = 0; i < red_posecenih->size; i++)
        ispis(red_posecenih->array[i]->matrica, vrste, kolone, red_posecenih->array[i]->nivo, red_posecenih->array[i]->redni_broj, red_posecenih->array[i]->otac->redni_broj);
    
    free(red_posecenih);
    free(stek);
        
}

struct cvor* formiraj_stablo(int n, int k, int p, int pocetna[20][4])
{
    struct Queue* queue = formiraj(1000);

    int po_redu = 1;
    struct cvor* koren;
    koren = malloc(sizeof(cvor));
    koren->otac = koren;
    koren->br_dece = 0;
    koren->nivo = 0;
    koren->redni_broj = po_redu;
    koren->resenje = 0;
    koren->putanja_do_resenja = 0;
    po_redu++;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= 4; j++)
            koren->matrica[i][j] = pocetna[i][j];
    
    int br_boja = n - k;
    int br_deteta = 0;
    dodaj_u_red(queue, koren);
    
    struct cvor* dete_iz_niza;

    while (isEmpty(queue) == 0)
    {
        dete_iz_niza = prvi(queue); //dohvata prvi iz reda
        int formiran_novi = 0;
        if (dete_iz_niza->nivo < p)
        {
            for (int x = 1; x <= n; x++)
            {
                int na_vrhu, indeks; // za bocicu X - IZ koje se presipa
                int sloj;
                for (sloj = 1; sloj <= 4; sloj++)
                    if (dete_iz_niza->matrica[x][sloj] == 0)
                        break;
                na_vrhu = dete_iz_niza->matrica[x][sloj - 1];
                indeks = sloj - 1;
                for (int y = 1; y <= n; y++) // bocica Y - U koju se presipa
                {
                    sloj = 1;
                    if (y == x) continue;
                    if (dete_iz_niza->matrica[y][4] != 0)
                        continue;
                    if (dete_iz_niza->matrica[y][1] != 0)
                    {
                        for (sloj = 2; sloj <= 4; sloj++)
                            if (dete_iz_niza->matrica[y][sloj] == 0)
                                break;
                        if (na_vrhu != dete_iz_niza->matrica[y][sloj - 1])
                            continue;
                    }
                    //formiranje novog deteta
                    struct cvor* nova_deca;
                    nova_deca = malloc(sizeof(cvor));
                    formiran_novi = 1;
                    nova_deca->otac = dete_iz_niza;
                    nova_deca->br_dece = 0;
                    nova_deca->redni_broj = po_redu;
                    po_redu++;
                    nova_deca->nivo = nova_deca->otac->nivo + 1;
                    nova_deca->putanja_do_resenja = 0;
                    nova_deca->resenje = 0;
                    
                    for (int q = 1; q <= n; q++)
                        for (int w = 1; w <= 4; w++)
                            nova_deca->matrica[q][w] = nova_deca->otac->matrica[q][w];

                    nova_deca->matrica[y][sloj] = na_vrhu;
                    nova_deca->matrica[x][indeks] = 0;

                    nova_deca->iz = x;
                    nova_deca->u = y;
                    for (int sloj2 = indeks - 1; sloj2 >= 1; sloj2--)
                    {
                        if (dete_iz_niza->matrica[x][sloj2] == na_vrhu && sloj <= 3 && nova_deca->matrica[y][sloj + 1] == 0)
                        {
                            nova_deca->matrica[y][sloj + 1] = na_vrhu;
                            sloj++;
                            nova_deca->matrica[x][sloj2] = 0;

                        }
                        else
                            break;
                    }

                    nova_deca->otac->deca[nova_deca->otac->br_dece++] = nova_deca;

                    if (pun(queue))
                    {
                        queue->kapacitet = po_redu;
                        queue->array = (struct cvor*)realloc(queue->array, po_redu * sizeof(cvor));
                    }
                    //ispis(nova_deca->matrica, n, 4, nova_deca->nivo,nova_deca->redni_broj,nova_deca->otac->redni_broj);
                    if (resenje(nova_deca->matrica, n, 4))
                    {
                        nova_deca->resenje = 1;
                        nova_deca->putanja_do_resenja = 1;
                        struct cvor* pom = nova_deca;
                        while (pom->otac->otac != pom->otac && pom->otac->putanja_do_resenja == 0)
                        {
                            pom->otac->putanja_do_resenja = 1;
                            pom = pom->otac;
                        }
                        pom->otac->putanja_do_resenja = 1;
                    }
                    dodaj_u_red(queue, nova_deca);
                }
            }
            //free(dete_iz_niza);
            izbaci(queue);   
        }
        else
            break;
    }
    
    koren->ukupan_broj_cvorova = po_redu;
    return koren;
}

struct cvor* predji_u_sledece_stanje(struct cvor* trenutni,int iz_bocice, int u_bocicu,int hint)
{
    if (hint == 2)
    {
        for (int i = 0; i < trenutni->br_dece; i++)
        {
            if ((trenutni->deca[i]->iz == iz_bocice) && (trenutni->deca[i]->u = u_bocicu))
                return trenutni->deca[i];
        }
        
    }
    else
    {
        for (int i = 0; i < trenutni->br_dece; i++)
        {
            if (trenutni->deca[i]->putanja_do_resenja == 1)
                return trenutni->deca[i];
        }

    }
    return trenutni;


}

void ispis_validnog(struct cvor* koren,int vrste, int p)
{
    struct cvor* pom = koren;
    
    while (pom->resenje != 1 && pom->putanja_do_resenja==1)
    {
        printf("\n------------------\n");
        ispis(pom->matrica, vrste, 4, 0, 0, 0);
        for (int i = 0; i < pom->br_dece; i++)
        {
            if (pom->deca[i]->putanja_do_resenja == 1)
            {
                pom = pom->deca[i]; 
                break;
            }
        }
        
    }
    if (pom->putanja_do_resenja == 0 && pom->resenje==0)
        printf("Nema resenja za ovaj broj poteza.");
    else {
        printf("\n------------------\n");
        if (pom->resenje == 1)
            ispis(pom->matrica, vrste, 4, 0, 0, 0);
    }
}

int main() 
{
    
    int n, k, p, seed;
    int pocetna[20][4];
    int niz[20];

    printf("------RAD SA BOCICAMA------\n\n\n");

    printf("Unesite vrednosti za ukupan broj bocica, broj praznih bocica i maksimalan broj poteza.\n");
    printf("Broj bocica: ");
    scanf("%d", &n);
    printf("Broj praznih bocica: ");
    scanf("%d", &k);
    printf("Mkasimalan broj poteza: ");
    scanf("%d", &p);
    printf("\n\n-----MENI-----\n\n");
    printf("1. Ispis formiranog stabla u preorder formatu\n");
    printf("2. Zapocni resavanje igre\n");
    printf("3. Ispis jednog validnog resenja po koracima\n");
    printf("4. Izadji iz programa\n");
    printf("Izaberite opciju iz menija: ");
    int opcija;
    scanf("%d", &opcija);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= 4; j++)
            pocetna[i][j] = 0;

    int M = 253;
    int br_boja = n - k;
    
    int boje[20] = { 0 };
    srand(time(0));
    int sledeci = rand();
    for (int i = 1; i <= br_boja; i++)
    {   
        for (int j = 1; j <= 4; j++)
        {
            while (1)
            {
                int k = 3;
                int broj = 0;
                while (k >= 0)
                {
                    //printf("sledeci je %d\n", sledeci);
                    sledeci = (sledeci * sledeci) % M;
                    int druga_cifra = sledeci % 2;
                    int prva_cifra = (sledeci / 2) % 2;
                    broj = broj + pow(2, k) * prva_cifra + pow(2, k - 1) * druga_cifra;
                    k -= 2;
                }
                broj = broj % br_boja+1;
                if (boje[broj] <4)
                {
                    //printf("broj je %d\n", broj);
                    pocetna[i][j] = broj;
                    boje[broj]++;
                    break;
                }
            }
        }
    }
    struct cvor* koren = formiraj_stablo(n, k, p, pocetna);
    
    struct cvor* trenutno_stanje;
    int resio = 0,nema_vise_poteza=0,ne_moze_dalje=0;
    while (opcija != 4)
    {
        switch (opcija)
        {
        case 1: preorder(koren, koren->ukupan_broj_cvorova, n, 4); break;
        case 2:
            if (nema_vise_poteza) { printf("Nemate vise poteza. Izaberite drugu opciju.\n"); break;}
            
            trenutno_stanje = koren;
            while (1)
            {
                printf("\n\n\n---------------------\nBroj preostalih poteza vam je %d\n", p - trenutno_stanje->nivo);
                printf("Ovo je trenutno stanje bocica:\n");
                ispis(trenutno_stanje->matrica, n, 4, trenutno_stanje->nivo, trenutno_stanje->redni_broj, trenutno_stanje->otac->redni_broj);
                if (trenutno_stanje->resenje == 1)
                {
                    printf("CESTITAMO. RESILI STE IGRU!!!!\n");
                    resio = 1;
                    break;
                }
                if (trenutno_stanje->nivo == p)
                {
                    if(trenutno_stanje->resenje==1)
                    {
                        printf("CESTITAMO. RESILI STE IGRU!!!!\n");
                        resio = 1;
                        break;
                    }
                    printf("Zao nam je. Niste uspeli da resite igru u zadatom broju poteza.\n");
                    nema_vise_poteza = 1;
                    break;
                }
                printf("Pritisnite 1 ukoliko zelite HINT ili 2 da nastavite sami: ");
                int hint;
                scanf("%d", &hint);
                if (hint == 2)
                {
                    printf("\nOdigrajte sledeci potez unosom rednog broja bocice.\n");
                    printf("Bocica IZ koje presipam: ");
                    int iz, u;
                    scanf("%d", &iz);
                    printf("Bocica U koju presipam: ");
                    scanf("%d", &u);
                    struct cvor* sledece = predji_u_sledece_stanje(trenutno_stanje, iz, u,2);
                    if (sledece->redni_broj == trenutno_stanje->redni_broj || sledece->putanja_do_resenja == 0)
                    {
                        printf("Zao nam je, ovo stanje ne vodi do resenja. Vise srece drugi put.\n");
                        ne_moze_dalje = 1;
                        break;

                    }
                    trenutno_stanje = sledece;
                }
                else
                {
                    struct cvor* sledece = predji_u_sledece_stanje(trenutno_stanje, 0, 0, 1);
                    if (sledece->redni_broj == trenutno_stanje->redni_broj || sledece->putanja_do_resenja == 0)
                    {
                        printf("Zao nam je, ovo stanje ne vodi do resenja. Vise srece drugi put.\n");
                        ne_moze_dalje = 1;
                        break;

                    }
                    trenutno_stanje = sledece;
                }

            }break;
        case 3: ispis_validnog(koren, n,p); break;
        }
        if (resio == 1 || ne_moze_dalje==1) // prekida se ukoliko je resio ili je dosao u stanje koje ne vodi do resenja.
            break;
        printf("Unesite sledecu opciju iz menija: ");
        scanf("%d", &opcija);
        
    }
    printf("\nDovidjenja.\n\n\n\n");
    return 0;
}