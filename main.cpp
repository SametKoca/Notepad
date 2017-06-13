#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <locale.h>

struct dugum {
int harf;
int satir;
int sutun;
int isaret;
struct dugum *sonraki;
struct dugum *onceki;
};

int Renk;

int NullKontrol(struct dugum **bas,int x,int y);
int SatirKontrol(struct dugum **bas,int y);
void SayfaSirala(int x,int y);
void SayfaYazdir(struct dugum **bas);
int SatirdakiIlkHarf(struct dugum **bas,int x,int y);

int IlkEleman(struct dugum **bas,int sutun,int satir)
{
    struct dugum *e;
    int deger = 0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == satir && e->sutun == sutun)
        {
            if(e->onceki == NULL)
                deger=1;
        }
    }

    return deger;
}


int SonElemanKontol(struct dugum **bas,struct dugum **son,int sutun,int satir)
{
    struct dugum *e;
    int deger = 0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == satir && e->sutun == sutun)
        {
            if(e->sonraki == *son)
                deger = 1;
        }
    }

    return deger;
}


int ElemandanSonraki(struct dugum **bas,int sutun,int satir)
{
    struct dugum *e;
    int sonuc=0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir > satir && e->sutun >= sutun)
            sonuc = 1;
    }

    return sonuc;
}


void Ekle2(struct dugum **bas2,struct dugum **son2,int harf)
{
    struct dugum *yeni =(struct dugum*) malloc(sizeof(struct dugum));
    yeni->harf = harf;
    yeni->satir = 0;
    yeni->sutun = 0;

    if(*bas2 == NULL)
    {
        yeni->sonraki = NULL;
        yeni->onceki = NULL;
        *bas2 = *son2 = yeni;
    }
    else
    {
        yeni->sonraki = NULL;
        yeni->onceki = *son2;
        (*son2)->sonraki = yeni;
        *son2 = yeni;
    }
}


void Ekle(struct dugum **bas,struct dugum **son,int harf,int satir,int sutun)
{
    struct dugum *e;
    struct dugum *f;


    struct dugum *yeni =(struct dugum*) malloc(sizeof(struct dugum));
    yeni->harf = harf;
    yeni->satir = satir;
    yeni->sutun = sutun;
    yeni->isaret = 0;

    if(*bas == NULL)
    {
       yeni->sonraki = NULL;
       yeni->onceki = NULL;
       *bas = *son = yeni;
    }
    else if(IlkEleman(bas,sutun,satir)==1)
    {
        for(e=*bas;e!=NULL;e=e->sonraki)
        {
            if(e->satir == satir && e->sutun == sutun)
            {
                yeni->onceki = NULL;
                yeni->sonraki = *bas;
                yeni->sonraki = e;
                e->onceki = yeni;
                *bas = yeni;

                break;
            }
        }

        for(e=*bas;e!=NULL;e=e->sonraki)
        {
            if(e!=*bas && e->satir == satir && e->sutun >= sutun)
                e->sutun = e->sutun+20;
        }

    }

    else if(ElemandanSonraki(bas,sutun,satir)==1)
    {
        yeni->onceki = NULL;
        yeni->sonraki = *bas;
        (*bas)->onceki = yeni;
        *bas = yeni;
    }

    else if(SonElemanKontol(bas,son,sutun,satir)!=1)  ///Son Eleman degilse
    {
        struct dugum *e;

        for(e=*bas;e!=NULL;e=e->sonraki)
        {
            if(e->satir == satir && e->sutun == sutun-20)
            {
                if(e == *son)
                {
                    yeni->sonraki = NULL;
                    yeni->onceki = *son;
                    (*son)->sonraki = yeni;
                    *son = yeni;
                }
                else
                {
                    struct dugum *g;

                    for(g=*bas;g!=NULL;g=g->sonraki)
                    {
                        if(g->satir == satir && g->sutun >= sutun && g != *bas)
                        {
                            g->sutun = g->sutun+20;
                        }
                    }

                    yeni->sonraki = e->sonraki;
                    e->sonraki = yeni;
                    yeni->onceki = e;
                    e->sonraki->onceki = yeni;

                }
            }
            else if(e->sutun == sutun && SatirKontrol(bas,satir)==0 && ElemandanSonraki(bas,sutun,satir)!=1)
            {
                yeni->sonraki = NULL;
                yeni->onceki = *son;
                (*son)->sonraki = yeni;
                *son = yeni;
            }
            else if(ElemandanSonraki(bas,sutun,satir)==1 && SatirKontrol(bas,satir)==0)
            {
                for(f=*bas;f!=NULL;f=f->sonraki)
                {
                    if(f->satir < satir && f->sonraki->satir >satir)
                        break;
                }

                yeni->sonraki = f->sonraki;
                f->sonraki = yeni;
                f->sonraki->onceki = yeni;
                yeni->onceki = f;

                break;
            }
        }
    }
    else
    {
        for(e=*bas;e!=NULL;e=e->sonraki)
        {
            if(e->satir == satir && e->sutun == sutun)
            {
                if(e->onceki == NULL)
                {
                    e->onceki = yeni;
                    yeni->sonraki = e;
                    *bas = yeni;
                    e->sutun = e->sutun+20;
                }
                else
                {
                    e->onceki->sonraki = yeni;
                    yeni->onceki = e->onceki;
                    e->onceki = yeni;
                    yeni->sonraki = e;
                    e->sutun = e->sutun+20;
                }
            }
            else if(e->satir == satir && e->sutun >= sutun)
                    e->sutun= e->sutun+20;
            else{}
        }
    }
}




void Sil(struct dugum **bas,struct dugum **son,int satir,int sutun)
{
    struct dugum *aranan;
    int sayac = 0;

    for(aranan = *bas ; aranan!=NULL ; aranan=aranan->sonraki)
        sayac++;

    for(aranan = *bas ; aranan != NULL ; aranan=aranan->sonraki)
    {
        if(aranan->satir == satir && aranan->sutun == sutun)
        {
            if(aranan == *son)
            {
                if(sayac == 1)
                    *bas = *son = NULL;

                else
                {
                    aranan->onceki->sonraki = NULL;
                    *son = (*son)->onceki;
                    sayac--;
                }
            }
            else if(aranan == *bas)
            {
                aranan->sonraki->onceki = NULL;
                *bas = (*bas) -> sonraki;
                sayac--;
            }
            else
            {
                struct dugum *f;
                struct dugum *yedek;

                if(NullKontrol(bas,sutun+20,satir)==0)
                {
                    for(f=*bas;f!=NULL;f=f->sonraki)
                    {
                        if(f->satir == satir && f->sutun == sutun-20 && SatirKontrol(bas,satir)!=70)
                        {
                            yedek = f->sonraki->sonraki;
                            f->sonraki->sonraki->onceki = f;
                            f->sonraki = yedek;
                        }
                        else if(f->satir == satir && SatirKontrol(bas,satir)==70)
                        {
                            if(f->satir == satir && f->sutun == 70)
                            {
                                f->onceki->sonraki = f->sonraki;
                                f->sonraki->onceki = f->onceki;
                            }
                        }
                    }
                }
                else
                {
                    aranan->onceki->sonraki = aranan->sonraki;
                    aranan->sonraki->onceki = aranan->onceki;
                }
                   sayac--;
                   break;
            }
        }
    }

    free(aranan);
}


void Duzenle(struct dugum **bas,int sutun,int satir)
{
    struct dugum *gerisi;

    for(gerisi = *bas; gerisi!=NULL;gerisi=gerisi->sonraki)
    {
        if(gerisi->satir == satir && gerisi->sutun > sutun)
            gerisi->sutun = gerisi->sutun-20;
    }
}


void Duzenle2(struct dugum **bas,int sutun,int satir)
{
    struct dugum *e;
    int devam=50;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == satir && e->sutun > sutun)
            e->sutun = e->sutun+20;

        if(e->sutun >= 1800)
        {
            e->satir = e->satir+20;
            e->sutun = devam;
            devam=devam+20;
        }
    }
}


void Sirala(struct dugum **bas,int x,int y)
{
    setfillstyle(1,15);
    setcolor(15);
    bar(x,y,1810,y+20);
    rectangle(x,y,1810,y+20);
}


void Sirala2(struct dugum **bas,int x,int y)
{
    setfillstyle(1,15);
    setcolor(15);
    bar(x,y,x+20,y+20);
    rectangle(x,y,x+20,y+20);
}


void Yazdir(struct dugum **bas,int x,int y)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == y && e->sutun > x)
        {
            char harf[2] = {e->harf,'\0'};
            setcolor(Renk);

            if(y < 980)
                outtextxy(e->sutun-20,e->satir,harf);
            else
                outtextxy(e->sutun-20,980,harf);
        }
    }
}


int SatirKontrol(struct dugum **bas,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->satir == y)
            deger=e->sutun;
    }

    return deger;
}


int HarfKontrol(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
        {
            deger = e->satir;
            break;
        }
    }

    return deger;
}


int NullKontrol(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
            deger=e->satir;
    }

    return deger;
}


int SonrakiSatir(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
            deger=e->sonraki->satir;
    }

    return deger;
}


int SonrakiSutun(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
            deger=e->sonraki->sutun;
    }

    return deger;
}


int SatirdakiIlkHarf(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
        {
            deger=e->sutun;
            break;
        }
    }

    return deger;
}


int SatirdakiSonHarf(struct dugum **bas,int x,int y)
{
    struct dugum *e;
    int deger = 0;

    for (e=(*bas);e!=NULL;e=e->sonraki)
    {
        if(e->sutun == x && e->satir == y)
            deger=e->sutun;
    }

    if(deger == 0)
        return 50;
    else
        return deger;
}


void UstSatiraEkle(struct dugum **bas,int sutun,int satir)
{
    struct dugum *e;
    int devam=20;
    int devam2=20;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == satir+20)
        {
            e->satir = e->satir-20;
            e->sutun = SatirdakiSonHarf(bas,sutun,satir)+devam;
            devam = devam+20;

            if(e->sutun >= 1810)
            {
                e->satir = e->satir+20;
                e->sutun = 50+devam2;
                devam2=devam2+20;
            }
        }
    }
}


void HepsiUstSatira(struct dugum **bas,int y,int Go)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir > y && e->satir <= Go)
            e->satir = e->satir-20;
    }
}


void AltSatiraEkle(struct dugum **bas,int sutun,int satir)
{
    struct dugum *e;
    int devam = 20;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir > satir)
            e->satir = e->satir+20;
    }

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->sutun > sutun && e->satir == satir)
        {
            e->satir = e->satir+20;
            e->sutun = 50+devam;
            devam=devam+20;
        }
    }
}


void SayfaSirala(int x,int y) ///x ve y parametresini kaldir
{
    setfillstyle(1,15);
    setcolor(15);
    bar(50,100,1810,1000);
    rectangle(50,100,1810,1000);
}


void SayfaYazdir(struct dugum **bas,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        char harf[2] = {e->harf,'\0'};
        setcolor(Renk);

        if(fark <= 0 && e->satir <=980)
            outtextxy(e->sutun-20,e->satir,harf);
        else
        {
            if(e->satir-fark >= 100 && e->satir-fark <= 980)
                outtextxy(e->sutun-20,e->satir-fark,harf);
            else{}
        }
    }
}


void SayfaYazdir2(struct dugum **bas,int satir,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        char harf[2] = {e->harf,'\0'};
        setcolor(Renk);

        if(e->satir > satir)
        {
            if(fark <= 0)
                outtextxy(e->sutun-20,e->satir,harf);
            else
            {
                if(e->satir-fark >= 100 && e->satir-fark <= 980)
                    outtextxy(e->sutun-20,e->satir-fark,harf);
                else{}
            }

        }
    }
}


void SayfaDuzenle(struct dugum **bas,int sutun,int satir)
{
    struct dugum *gerisi;
    int devam2=0;

    for(gerisi = *bas; gerisi!=NULL;gerisi=gerisi->sonraki)
    {
        if( gerisi->satir == satir+20)
            gerisi->satir = gerisi->satir-20;

        else if(gerisi->satir == satir+40)
        {
            gerisi->sutun = 70+devam2;
            devam2 = devam2 +20;
        }
    }

    for(gerisi = *bas; gerisi!=NULL;gerisi=gerisi->sonraki)
    {
        if( gerisi->sutun >= 1800)
        {
            gerisi->satir = gerisi->satir+20;
            gerisi->sutun = gerisi->sutun-1750;
        }
    }
}


void SonSutunDuzenle(struct dugum **bas)
{
    struct dugum *e;
    struct dugum *f;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->sutun > 1810)
        {
            for(f=*bas;f!=NULL;f=f->sonraki)
            {
                if(f->satir == e->satir+20)
                    f->sutun = f->sutun+20;
            }

        e->satir = e->satir+20;
        e->sutun = 70;
        }
    }

    SayfaSirala(0,0);
    SayfaYazdir(bas,0);
}


void AltSatiriYazdir(struct dugum **bas,int sutun,int satir,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        char harf[2] = {e->harf,'\0'};
        setcolor(Renk);

        if(e->satir-fark>=100)
            outtextxy(e->sutun-20,e->satir-fark,harf);
    }
}


void AlaniSil(struct dugum **bas,int x,int y,int sonx,int sony,int renk,int fark) ///Silinebilir
{
    setfillstyle(1,renk);
    setcolor(15);

    if(fark<=0)
        fark=0;

    bar(x,y-fark,sonx,sony-fark);
    rectangle(x,y-fark,sonx,sony-fark);
}


void TaraliYaz(struct dugum **bas,int x,int y,int sonx,int sony,int fark)
{
    struct dugum *e;

    if(fark<=0)
        fark=0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir >= y && e->satir < sony && e->sutun > x && e->sutun <= sonx)
        {
            char harf[2] = {e->harf,'\0'};

            if(e->isaret == 0)
            {
                AlaniSil(bas,x,y,sonx,sony,9,fark);
                setcolor(Renk);
                setbkcolor(9);
                e->isaret = 1;

                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(x,y-fark,harf);
            }
            else
            {
                AlaniSil(bas,x,y,sonx,sony,15,fark);
                setcolor(Renk);
                setbkcolor(15);
                e->isaret = 0;

                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(x,y-fark,harf);
            }
        }
    }
}


void YatayTarali2(struct dugum **bas,int x,int y,int sonx,int sony,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == y && e->sutun >= x)
            TaraliYaz(bas,e->sutun,e->satir,e->sutun+20,e->satir+20,fark);

        if(e->satir == sony && e->sutun <= sonx)
            TaraliYaz(bas,e->sutun-20,e->satir,e->sutun,e->satir+20,fark);

        if(e->satir == y && x == 50)
            TaraliYaz(bas,50,e->satir,70,e->satir+20,fark);
    }
}


void YatayTarali3(struct dugum **bas,int x,int y,int sonx,int sony,int Tx,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == y && e->sutun >= x)
            TaraliYaz(bas,e->sutun,e->satir,e->sutun+20,e->satir+20,fark);

        if(e->satir == sony && e->sutun <= x)
            TaraliYaz(bas,e->sutun-20,e->satir,e->sutun,e->satir+20,fark);

        if(e->satir == y && x == 50)
            TaraliYaz(bas,50,e->satir,70,e->satir+20,fark);
    }
}


void YatayTarali(struct dugum **bas,int x,int y,int sonx,int sony,int fark)
{
    struct dugum *e;

    if(fark<=0)
        fark=0;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir >= y && e->satir < e->satir+20 && e->sutun > x )
        {
            char harf[2] = {e->harf,'\0'};

            if(e->isaret == 0)
            {
                setcolor(Renk);
                setbkcolor(9);

                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(e->sutun-20,y-fark,harf);

                e->isaret = 1;
            }
            else
            {
                setcolor(Renk);
                setbkcolor(15);

                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(e->sutun-20,y-fark,harf);

                e->isaret = 0;
            }
        }
        if(e->satir >= sony && e->satir < sony+20 && e->sutun <=sonx)
        {
            char harf[2] = {e->harf,'\0'};

            if(e->isaret == 0)
            {
                setcolor(Renk);
                setbkcolor(9);
                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(e->sutun-20,sony-fark,harf);

                e->isaret = 1;
            }
            else
            {
                setcolor(Renk);
                setbkcolor(15);

                if(y-fark >=100 && y-fark <= 980)
                    outtextxy(e->sutun-20,sony-fark,harf);

                e->isaret = 0;
            }
        }


    }
}

void YatayAlaniSil(struct dugum **bas,int x,int y,int sonx,int sony,int renk)
{
    struct dugum *e;
    setfillstyle(1,renk);
    setcolor(15);

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        if(e->satir == y && e->sutun > x)
        {
            setfillstyle(1,renk);
            bar(e->sutun-20,e->satir,e->sutun,e->satir+20);
            rectangle(e->sutun-20,e->satir,e->sutun,e->satir+20);
        }
        if(e->satir == sony && e->sutun <= sonx)
        {
            setfillstyle(1,renk);
            bar(e->sutun-20,e->satir,e->sutun,e->satir+20);
            rectangle(e->sutun-20,e->satir,e->sutun,e->satir+20);
        }
    }
}


void OzelYazdir(struct dugum **bas,int fark)
{
    struct dugum *e;

    for(e=*bas;e!=NULL;e=e->sonraki)
    {
        char harf[2] = {e->harf,'\0'};
        setcolor(Renk);
        outtextxy(e->sutun,e->satir+fark,harf);
    }
}


void SayfaSirala2()
{
    setfillstyle(1,15);
    setcolor(15);
    bar(50,100,1810,120);
    rectangle(50,100,1810,120);
}


void YazdirDizi(char Yer[],int x,int toplam)
{
    struct dugum *bas3=NULL;
    struct dugum *son3=NULL;
    int devam=50;
    int i;

    for(i=0;i<toplam;i++)
    {
        struct dugum *yeni =(struct dugum*) malloc(sizeof(struct dugum));
        yeni->harf = Yer[i];

        if(bas3 == NULL)
        {
            yeni->sonraki = NULL;
            yeni->onceki = NULL;
            bas3 = son3 = yeni;
        }
        else
        {
            yeni->sonraki = NULL;
            yeni->onceki = son3;
            (son3)->sonraki = yeni;
            son3 = yeni;
        }
    }

    struct dugum *e;

    for(e=bas3;e!=NULL;e=e->sonraki)
    {
        char Tekharf[2] = {e->harf,'\0'};
        setcolor(Renk);
        outtextxy(devam,100,Tekharf);
        devam=devam+20;
    }

}


void BilgiSirala(int left,int top,int right,int bottom)
{
    setfillstyle(1,15);
    setcolor(15);
    bar(left,top,right,bottom);
    rectangle(left,top,right,bottom);
}


void SiralaSayi(int Go,int fark)
{
    int i;
    char a;
    char as[4];
    setfillstyle(1,15);
    setcolor(15);
    bar(0,100,45,1000);
    rectangle(0,100,45,1000);

    for(i=0;i <= (Go-100)/20 ; i++)
    {
        setcolor(Renk);
        setbkcolor(15);
        itoa(i+1,as,10);

        if(fark <0 && i <= 45)
            outtextxy(15,100+(20*i),as);
        else
        {
            if(100+(20*i)-fark>=100 && 100+(20*i)-fark <=980)
                outtextxy(15,100+(20*i)-fark,as);
            else{}
        }
    }
}


int main()
{
    int AnaEkran = initwindow(getmaxwidth(),getmaxheight(),"Notepad");
    struct dugum *bas = NULL;
    struct dugum *son = NULL;
    struct dugum *bas2 = NULL;
    struct dugum *son2 = NULL;

    char c;
    char h;
    char OkumaYeri[100];
    char OkumaYeri2[100];

    int x = 50;
    int y = 100;
    int indexy = 100;
    int KeyDeger = 0;
    int Tx;
    int Ty;
    int Go = 0;
    int fark = 100+Go-980;
    int fark2;
    int Td=0;
    int satir2;
    int sutun2;
    int Oku;
    int Wx=50;
    int Wy=100;
    int cikis=0;
    int yer[100];
    int i=0;
    int j=0;
    int OkuGo;
    int k;
    int okux=70;
    int okuy=100;
    int DuzenleTx;
    int Bilgi;
    int y2;

    FILE *dosya;

    setlocale(LC_ALL,"Turkish");
    floodfill(0,0,WHITE);

    Renk = 0;

    setcolor(Renk);
    setbkcolor(15);

    line(0,99,getmaxx(),99);
    line(48,99,48,getmaxy());
    line(0,1001,getmaxx(),1001);
    line(1811,99,1811,getmaxy());


    setcolor(4);
    outtextxy(0,0,"");
    settextstyle(10,12,0);

    outtextxy(100,10,"AC => F2");
    outtextxy(500,10,"KAYDET => F3");
    outtextxy(1000,10,"SEC => F1");
    outtextxy(100,60,"KES => CTRL+X");
    outtextxy(500,60,"KOPYALA => CTRL+C");
    outtextxy(1000,60,"YAPISTIR => CTRL+V");
    outtextxy(1500,40,"*SAMET KOCA");
    settextstyle(10,12,1);
    outtextxy(1130,1020,"**SECIM ACIK IKEN HARF YAZAMAZSINIZ YADA SILEMEZSINIZ.");
    settextstyle(10,12,1);
    outtextxy(1825,150,"SECIM");
    outtextxy(1830,280,"RENK");
    setcolor(Renk);
    settextstyle(8,12,2);
    setcolor(4);
    outtextxy(1825,180,"KAPALI");
    setcolor(Renk);
    outtextxy(1831,310,"SIYAH");

    outtextxy(15,100,"1");

    while(1)
    {
        if(y<=980)
            y2=y;
        else
            y2=980;
        while(!kbhit())  ///Imlec Fonksiyonu
        {
            setcolor(Renk);
            line(x,indexy,x,indexy+20);
            delay(100);
            setcolor(15);
            line(x,indexy,x,indexy+20);
            setcolor(Renk);
            delay(100);
        }

        if(fark <= 0)
            fark=0;

        setbkcolor(15);

        c = (char) getch();
        char cs[2] = {c,'\0'};

        if (c != 0)
        {
            if(c == 8 && KeyDeger%2 != 1)
            {
                if(x<=50 && y<=100)
                {
                    x=50;
                    y=100;
                    indexy=100;
                }
                else if(x<=50)
                {
                    y=y-20;
                    indexy=indexy-20;
                    Go=Go-20;

                    if(son != NULL && son->satir==y)
                        x=son->sutun;

                    else if (y>=100 && SatirKontrol(&bas,y)==0)
                    {
                        if(SatirKontrol(&bas,y+20)!=0)
                        {
                            HepsiUstSatira(&bas,y,140+Go);
                            SayfaSirala(x,y);
                            fark = 100+Go-980;
                            SayfaYazdir(&bas,fark+Td);
                        }
                        else if(SatirKontrol(&bas,y+20)==0)
                        {
                            HepsiUstSatira(&bas,y,140+Go);
                            SayfaSirala(x,y);
                            fark = 100+Go-980;
                            SayfaYazdir(&bas,fark+Td);
                        }

                        x=50;
                    }
                    else if (y>=100 && SatirKontrol(&bas,y)!=0)
                    {
                        x=SatirKontrol(&bas,y);

                        if(SatirKontrol(&bas,y+20)!=0)
                        {
                            fark = 100+Go-980;
                            UstSatiraEkle(&bas,x,y);
                            SayfaSirala(x,y);
                            SayfaYazdir(&bas,fark+Td);

                            if(SatirKontrol(&bas,y+20)==0)
                            {
                                HepsiUstSatira(&bas,y,140+Go);
                                SayfaSirala(x,y);
                                SayfaYazdir(&bas,fark+Td);
                            }

                        }
                        else
                        {
                            HepsiUstSatira(&bas,y,140+Go);
                            SayfaSirala(x,y);
                            SayfaYazdir(&bas,fark+Td);
                        }

                    SayfaSirala(x,y);
                    SayfaYazdir(&bas,fark+Td);
                    }
                        else
                            x=50; SayfaDuzenle(&bas,x,y);
                }
                else
                {
                    setfillstyle(1,15);
                    setcolor(15);
                    bar(x-20,y,x,y+20);
                    rectangle(x-20,y,x,y+20);
                    x=x-20;
                    setcolor(Renk);
                    Sil(&bas,&son,y,x+20);
                    Duzenle(&bas,x,y);
                    Sirala(&bas,x,indexy);
                    Yazdir(&bas,x,y);
                }
            }
            else if(c == 3)
            {
                struct dugum *c;
                bas2 = NULL;
                son2 = NULL;

                for(c=bas;c!=NULL;c=c->sonraki)
                {
                    if(c->isaret == 1)
                    {
                        Ekle2(&bas2,&son2,c->harf);
                        c->isaret = 0;
                    }
                }

            fark2=fark;

            if(fark2<=0)
                fark2=0;

            SayfaSirala(x,y);
            setbkcolor(15);
            SayfaYazdir(&bas,fark2+Td);
            BilgiSirala(1825,180,1920,260);
            setcolor(4);
            outtextxy(1825,180,"KAPALI");
            setcolor(Renk);
            KeyDeger++;
            }
            else if(c == 22)
            {
                struct dugum *v;
                satir2=y;
                sutun2=x;

                for(v=bas2;v!=NULL;v=v->sonraki)
                {
                    sutun2=sutun2+20;

                    if(sutun2 >= 1800)
                    {
                        sutun2=50;
                        satir2=satir2+20;
                    }

                Ekle(&bas,&son,v->harf,satir2,sutun2);
                }

            fark2=fark;

            if(fark2<=0)
                fark2=0;

            SayfaSirala(x,y);
            setbkcolor(15);
            SayfaYazdir(&bas,fark2+Td);
            }
            else if(c == 24)
            {
                struct dugum *c;
                bas2 = NULL;
                son2 = NULL;
                DuzenleTx = Tx;

                for(c=bas;c!=NULL;c=c->sonraki)
                {
                    if(c->isaret == 1)
                        Ekle2(&bas2,&son2,c->harf);
                }

            for(c=bas;c!=NULL;c=c->sonraki)
            {
                if(c->isaret == 1)
                {
                    Sil(&bas,&son,c->satir,c->sutun);

                    if(x<DuzenleTx)
                        Duzenle(&bas,x,y);
                    else
                    {
                        Duzenle(&bas,DuzenleTx,y);
                        x=DuzenleTx;
                    }
                }
            }

        fark2=fark;

        if(fark2<=0)
            fark2=0;

        SayfaSirala(x,y);
        setbkcolor(15);
        SayfaYazdir(&bas,fark2+Td);
        KeyDeger++;
        }
        else if(c == 13 && KeyDeger%2 != 1)
        {
            Go=Go+20;

            if(indexy >= 980)
            {
                fark = 100+Go-980;  ///Duzenlenecek

                AltSatiraEkle(&bas,x,y);
                SayfaSirala(x,y);
                AltSatiriYazdir(&bas,x,y,fark);
            }
            else
            {
                AltSatiraEkle(&bas,x,y);
                SayfaSirala(x,y);
                fark2=fark;

                if(fark2<=0)
                    fark2=0;

                SayfaYazdir(&bas,fark2+Td);
                fark = 100+Go-980;

                if(fark==0)
                    Td=0;

            }

            y=y+20;

            if(indexy < 980)
                indexy=indexy+20;

            x=50;
        }
        else if(c == 27){}
        else
        {
            if(KeyDeger%2 != 1)
            {
                struct dugum *e;

                if(x>=1800)
                {
                    x=50;
                    y=y+20;
                    indexy=indexy+20;
                    Go=Go+20;
                }

                Sirala2(&bas,x,indexy);
                setcolor(Renk);
                setbkcolor(15);
                outtextxy(x,indexy,cs);
                x=x+20;
                Ekle(&bas,&son,c,y,x);

                for(e=bas;e!=NULL;e=e->sonraki)
                {
                    if(e->sutun > 1810)
                        SonSutunDuzenle(&bas);
                }

                Sirala2(&bas,x,indexy);
                Sirala(&bas,x,indexy);
                Yazdir(&bas,x,y);
            }
        }
    }
    else
    {
        c = (char) getch();

        switch (c)
        {
            case KEY_PGUP:

            Renk++;

            if(Renk < 0)
                Renk = 14-Renk;

            if(Renk == 15)
                Renk = 0;

            if(Renk == 9)
                Renk=10;

            fark2=fark;

            if(fark2<=0)
                fark2=0;

            SayfaSirala(x,y);
            setbkcolor(15);
            SayfaYazdir(&bas,fark2+Td);
            BilgiSirala(1820,310,1920,360);
            setcolor(Renk);

            if(Renk==0)
                outtextxy(1831,310,"SIYAH");
            if(Renk==1)
                outtextxy(1837,310,"MAVI");
            if(Renk==2)
                outtextxy(1832,310,"YESIL");
            if(Renk==3)
                outtextxy(1837,310,"CYAN");
            if(Renk==4)
                outtextxy(1820,310,"KIRMIZI");
            if(Renk==5)
                outtextxy(1820,310,"MACENTA");
            if(Renk==6)
            {
                outtextxy(1830,310,"KAHVE");
                outtextxy(1830,330,"RENGI");
            }

            if(Renk==7)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1845,330,"GRI");
            }

            if(Renk==8)
            {
                outtextxy(1838,310,"KOYU");
                outtextxy(1845,330,"GRI");
            }

            if(Renk==10)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1833,330,"YESIL");
            }

            if(Renk==11)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1837,330,"CYAN");
            }

            if(Renk==12)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1820,330,"KIRMIZI");
            }

            if(Renk==13)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1820,330,"MAGENTA");
            }

            if(Renk==14)
                outtextxy(1837,310,"SARI");

                break;

            case KEY_PGDN:

            Renk--;

            if(Renk < 0)
                Renk = 14-Renk;

            if(Renk == 15)
                Renk = 14;

            if(Renk == 9)
                Renk=8;

            fark2=fark;

            if(fark2<=0)
                fark2=0;

            SayfaSirala(x,y);
            setbkcolor(15);
            SayfaYazdir(&bas,fark2+Td);
            BilgiSirala(1820,310,1920,360);
            setcolor(Renk);

            if(Renk==0)
                outtextxy(1831,310,"SIYAH");
            if(Renk==1)
                outtextxy(1837,310,"MAVI");
            if(Renk==2)
                outtextxy(1832,310,"YESIL");
            if(Renk==3)
                outtextxy(1837,310,"CYAN");
            if(Renk==4)
                outtextxy(1820,310,"KIRMIZI");
            if(Renk==5)
                outtextxy(1820,310,"MACENTA");
            if(Renk==6)
            {
                outtextxy(1830,310,"KAHVE");
                outtextxy(1830,330,"RENGI");
            }

            if(Renk==7)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1845,330,"GRI");
            }

            if(Renk==8)
            {
                outtextxy(1838,310,"KOYU");
                outtextxy(1845,330,"GRI");
            }

            if(Renk==10)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1833,330,"YESIL");
            }

            if(Renk==11)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1837,330,"CYAN");
            }

            if(Renk==12)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1820,330,"KIRMIZI");
            }

            if(Renk==13)
            {
                outtextxy(1837,310,"ACIK");
                outtextxy(1820,330,"MAGENTA");
            }

            if(Renk==14)
                outtextxy(1837,310,"SARI");

                break;

            case KEY_F3:   ///KAYDET

            if(getcurrentwindow()!=1)
                Oku = initwindow(1400,200,"KAYDET",225,400,false,false);

            floodfill(0,0,WHITE);
            setcolor(4);
            rectangle(49,99,1350,121);
            setbkcolor(15);
            outtextxy(350,150,"KAYDET = ENTER");
            outtextxy(850,150,"CIKIS = ESC");
            cikis = 0;
            OkuGo = 50;
            Wx=50;

            for(j=0;j<100;j++) ///Duzenlenebilir
                OkumaYeri[j]=NULL;

            setbkcolor(15);
            outtextxy(50,50,"KAYDETMEK ISTEDIGINIZ YERI VE DOSYA ADINI YAZIN : ");
            outtextxy(650,50,"(C:\\\\...\\\\...\\\\test.txt  GIBI)");
            setcolor(Renk);

            while(1)
            {
                while(!kbhit())  ///Imlec Fonksiyonu
                {
                    setcolor(Renk);
                    line(Wx,Wy,Wx,Wy+20);
                    delay(100);
                    setcolor(15);
                    line(Wx,Wy,Wx,Wy+20);
                    setcolor(Renk);
                    delay(100);
                }

                if(cikis != 1)
                    h = (char) getch();

                char hs[2] = {h,'\0'};

                if(cikis!=1 && h != 0 )
                {
                    if(h == 8)
                    {
                        if(Wx>50)
                        {
                            Wx=Wx-20;
                            OkuGo=OkuGo-20;
                        }

                        if(OkumaYeri[((Wx-50)/20)+1] == NULL)
                        {
                            i--;
                            Sirala2(&bas,Wx,Wy);
                            OkumaYeri[((Wx-50)/20)]=NULL;
                        }
                        else if(OkumaYeri[((Wx-50)/20)-1] == NULL)
                        {
                            for(j=0;j<=i;j++)
                            {
                                if(j > (Wx-50)/20 )
                                {
                                    OkumaYeri[j-1]=OkumaYeri[j];

                                    if(j == i)
                                        OkumaYeri[i]=NULL;
                                }
                            }

                            SayfaSirala2();
                            YazdirDizi(OkumaYeri,100,i);
                        }
                        else
                        {
                            if(OkumaYeri[((Wx-50)/20)-1] != NULL)
                            {
                                i--;

                                for(j=0;j<=i;j++)
                                {
                                    if(j > (Wx-50)/20 )
                                    {
                                        OkumaYeri[j-1]=OkumaYeri[j];

                                        if(j == i)
                                            OkumaYeri[i]=NULL;
                                    }
                                }

                                SayfaSirala2();
                                YazdirDizi(OkumaYeri,100,i);
                            }
                        }
                    }
                    else if(h == 13)
                        cikis = 1;

                    else if(h == 27)
                    {
                        for(j=0;j<100;j++) ///Duzenlenebilir
                        OkumaYeri[j]=NULL;
                        cikis = 1;
                    }
                    else
                    {
                        if(OkumaYeri[((Wx-50)/20)+1] != NULL)
                        {
                            k=0;

                            for(j=0;j<=i;j++)
                            {
                                if(j >= (Wx-50)/20)
                                {
                                    OkumaYeri2[k]=OkumaYeri[j];
                                    k++;
                                }
                            }

                            OkumaYeri[((Wx-50)/20)]= h;

                            for(j=1;j<k;j++)
                                OkumaYeri[((Wx-50)/20)+j] = OkumaYeri2[j-1];

                            SayfaSirala2();
                            YazdirDizi(OkumaYeri,100,i+1);
                            i++;
                            Wx=Wx+20;
                            OkuGo=OkuGo+20;
                        }
                        else
                        {
                            setcolor(Renk);
                            setbkcolor(15);
                            outtextxy(Wx,Wy,hs);
                            OkumaYeri[(Wx-50)/20]=h;
                            yer[i]=h;
                            i++;
                            Wx=Wx+20;
                            OkuGo=OkuGo+20;
                        }
                    }
                }
                else
                {
                    if(cikis != 1)
                        h = (char) getch();

                    switch(h)
                    {
                        case KEY_RIGHT:

                        if(OkumaYeri[((Wx-50)/20)] != NULL || OkumaYeri[((Wx-50)/20)+1] != NULL )
                            Wx=Wx+20;

                            break;

                        case KEY_LEFT:

                        if(Wx>50)
                            Wx=Wx-20;

                            break;
                    }
                }

                if(cikis == 1)
                    break;
            }

            struct dugum *e;
            closegraph(Oku);
            setcurrentwindow(AnaEkran);

            dosya = fopen(OkumaYeri,"w");

            if(dosya != NULL)
            {
                for(e=bas;e!=NULL;e=e->sonraki)
                {
                    fprintf(dosya,"%c",e->harf);

                    if(e->sonraki != NULL && e->satir < e->sonraki->satir)
                        fputs("\n",dosya);
                }

                setcolor(Renk);
                SayfaYazdir(&bas,0);
                x=okux-20;
                y=okuy;

                Bilgi = initwindow(420,100,"BILGI",600,450,false,false);
                floodfill(0,0,WHITE);
                setcolor(4);
                setbkcolor(15);
                settextstyle(10,12,0);
                outtextxy(410,65,"KAYDETME BASARILI");
                delay(1000);
                settextstyle(8,12,2);
                closegraph(Bilgi);
                setcurrentwindow(AnaEkran);
                setcolor(Renk);
            }
            else
                printf("DOSYA YOK!\n");

            fclose(dosya);

                break;

            case KEY_F2 :    ///OKU

            if(getcurrentwindow()!=1)
                Oku = initwindow(1400,200,"OKU",225,400,false,false);

            floodfill(0,0,WHITE);
            setcolor(4);
            rectangle(49,99,1350,121);
            cikis = 0;
            OkuGo = 50;
            Wx=50;

            for(j=0;j<100;j++) ///Duzenlenebilir
                OkumaYeri[j]=NULL;

            setbkcolor(15);
            outtextxy(50,50,"OKUMAK ISTEDIGINIZ DOSYAYI YERI ILE BIRLIKTE YAZIN : ");
            outtextxy(650,50,"(C:\\\\...\\\\...\\\\test.txt  GIBI)");
            outtextxy(350,150,"OKU = ENTER");
            outtextxy(850,150,"CIKIS = ESC");
            setcolor(Renk);

            while(1)
                {
                    while(!kbhit())  ///Imlec Fonksiyonu
                    {
                        setcolor(Renk);
                        line(Wx,Wy,Wx,Wy+20);
                        delay(100);
                        setcolor(15);
                        line(Wx,Wy,Wx,Wy+20);
                        setcolor(Renk);
                        delay(100);
                    }

                    if(cikis != 1)
                        h = (char) getch();

                    char hs[2] = {h,'\0'};

                    if(cikis!=1 && h != 0 )
                    {
                        if(h == 8)
                        {
                            if(Wx>50)
                            {
                                Wx=Wx-20;
                                OkuGo=OkuGo-20;
                            }

                            if(OkumaYeri[((Wx-50)/20)+1] == NULL)
                            {
                                i--;
                                Sirala2(&bas,Wx,Wy);
                                OkumaYeri[((Wx-50)/20)]=NULL;
                            }
                            else if(OkumaYeri[((Wx-50)/20)-1] == NULL)
                            {
                                for(j=0;j<=i;j++)
                                {
                                    if(j > (Wx-50)/20 )
                                    {
                                        OkumaYeri[j-1]=OkumaYeri[j];

                                        if(j == i)
                                            OkumaYeri[i]=NULL;
                                    }
                                }

                                SayfaSirala2();
                                YazdirDizi(OkumaYeri,100,i);
                            }
                            else
                            {
                                if(OkumaYeri[((Wx-50)/20)-1] != NULL)
                                {
                                    i--;

                                    for(j=0;j<=i;j++)
                                    {
                                        if(j > (Wx-50)/20 )
                                        {
                                            OkumaYeri[j-1]=OkumaYeri[j];

                                            if(j == i)
                                                OkumaYeri[i]=NULL;
                                        }

                                    }

                                    SayfaSirala2();
                                    YazdirDizi(OkumaYeri,100,i);
                                }
                            }
                        }
                        else if(h == 13)
                            cikis = 1;

                        else if(h == 27)
                        {
                            for(j=0;j<100;j++) ///Duzenlenebilir
                                OkumaYeri[j]=NULL;

                            cikis = 1;
                        }
                        else
                        {
                            if(OkumaYeri[((Wx-50)/20)+1] != NULL)
                            {
                                k=0;

                                for(j=0;j<=i;j++)
                                {
                                    if(j >= (Wx-50)/20)
                                    {
                                        OkumaYeri2[k]=OkumaYeri[j];
                                        k++;
                                    }
                                }

                                OkumaYeri[((Wx-50)/20)]= h;

                                for(j=1;j<k;j++)
                                    OkumaYeri[((Wx-50)/20)+j] = OkumaYeri2[j-1];

                                SayfaSirala2();
                                YazdirDizi(OkumaYeri,100,i+1);
                                i++;
                                Wx=Wx+20;
                                OkuGo=OkuGo+20;
                            }
                            else
                            {
                                setcolor(Renk);
                                setbkcolor(15);
                                outtextxy(Wx,Wy,hs);
                                OkumaYeri[(Wx-50)/20]=h;
                                yer[i]=h;
                                i++;
                                Wx=Wx+20;
                                OkuGo=OkuGo+20;
                            }
                        }
                    }
                    else
                    {
                        if(cikis != 1)
                            h = (char) getch();

                        switch(h)
                        {
                            case KEY_RIGHT:

                            if(OkumaYeri[((Wx-50)/20)] != NULL || OkumaYeri[((Wx-50)/20)+1] != NULL )
                                Wx=Wx+20;

                               break;

                            case KEY_LEFT:

                            if(Wx>50)
                                Wx=Wx-20;

                               break;
                        }

                    }

                    if(cikis == 1)
                        break;

                }

                closegraph(Oku);
                setcurrentwindow(AnaEkran);
                char kelime[5];
                dosya = fopen(OkumaYeri,"r");

                if(dosya != NULL)
                {
                    bas = NULL;
                    son = NULL;
                    while(!feof(dosya))
                    {
                        fscanf(dosya,"%c",&kelime[i]);

                        if(feof(dosya))
                            break;

                        if(okux >= 1800)
                            okuy=okuy+20;

                        Ekle(&bas,&son,kelime[i],okuy,okux);
                        okux = okux+20;
                    }

                    setcolor(Renk);
                    SayfaYazdir(&bas,0);
                    x=okux-20;
                    y=okuy;
                }
                else
                    printf("DOSYA YOK!\n");

                fclose(dosya);

                   break;

                case KEY_F1 :   ///SECİM

                KeyDeger++;

                if(KeyDeger%2 == 1)
                {
                    Tx=x;
                    Ty=y;
                    BilgiSirala(1825,180,1920,260);
                    setcolor(4);
                    outtextxy(1835,180,"ACIK");
                    setcolor(Renk);
                }
                else
                {
                    struct dugum *c;

                    bas2=NULL;
                    son2=NULL;
                    BilgiSirala(1825,180,1920,260);
                    setcolor(4);
                    outtextxy(1825,180,"KAPALI");
                    setcolor(Renk);

                    for(c=bas;c!=NULL;c=c->sonraki)
                    {
                        if(c->isaret == 1)
                            c->isaret = 0;
                    }

                    fark2=fark;

                    if(fark2<=0)
                        fark2=0;

                    SayfaSirala(x,y);
                    setbkcolor(15);
                    SayfaYazdir(&bas,fark2+Td);
                }

                   break;

                case KEY_UP:

                if(KeyDeger%2 == 1 && SatirKontrol(&bas,y-20)!=0)  ///Üst satirda harf olup olmadığını kontrol ediyor
                {
                    if(Ty < y)
                        YatayTarali2(&bas,x,y-20,x,y,fark+Td);
                    else
                        YatayTarali2(&bas,x,y-20,x,y,fark+Td);
                }

                if(indexy == 100)
                {
                    if(Td <= -1*fark)
                        Td=-1*fark;

                    if(Td > -1*fark)
                    {
                        Td=Td-20;
                        SayfaSirala(x,y);
                        SayfaYazdir(&bas,fark+Td);
                    }

                }

                if(y<=100)
                {
                    y=100;
                    indexy=100;
                }
                else if(SatirKontrol(&bas,y-20)==0)
                {
                    y=y-20;

                    if(indexy > 100)
                        indexy=indexy-20;

                    x=50;
                }
                else if(SatirKontrol(&bas,y-20)>= x)     ///Üst satirdaki son sutun x den büyük eşitse
                {
                    y=y-20;

                    if(indexy >= 100)
                        indexy=indexy-20;

                }
                else if(SatirKontrol(&bas,y-20)< x)      ///Üst satidaki son sutun x den kücükse
                {
                    y=y-20;

                    if(indexy > 100)
                        indexy=indexy-20;

                    x=SatirKontrol(&bas,y);
                }


                   break;

                case KEY_DOWN:

                if(KeyDeger%2 == 1 && SatirKontrol(&bas,y+20)!=0)
                {
                    if(Ty > y)
                        YatayTarali3(&bas,x,y,x,y+20,Tx,fark+Td);
                    else
                        YatayTarali3(&bas,x,y,x,y+20,Tx,fark+Td);

                }
                if(indexy == 980)
                {
                    if(Td > 0)
                        Td=0;

                    if(Td <= -20)
                    {
                        Td=Td+20;
                        SayfaSirala(x,y);
                        SayfaYazdir(&bas,fark+Td);
                    }
                }

                if(bas == NULL) {}
                else if(NullKontrol(&bas,x+20,y)==0 && son->satir == y && son->sutun == x){}
                else if(y < 100+Go && SatirKontrol(&bas,y+20)==0)
                {
                    y=y+20;

                    if(indexy < 980)
                        indexy=indexy+20;

                    x=50;
                }
                else if(NullKontrol(&bas,x+20,y)==0 && (son->satir != y || son->sutun != x))
                {
                    if(SatirKontrol(&bas,y+20)==0)
                    {
                        y=y+20;

                        if(indexy < 980)
                            indexy=indexy+20;

                        x=50;
                    }
                    else if(SatirKontrol(&bas,y+20)>= x)
                    {
                        y=y+20;

                        if(indexy < 980)
                            indexy=indexy+20;
                    }
                    else if(SatirKontrol(&bas,y+20)< x)
                    {
                        y=y+20;

                        if(indexy < 980)
                            indexy=indexy+20;

                        x=SatirKontrol(&bas,y);
                    }
                    else
                    {
                        y=y+20;

                        if(indexy < 980)
                            indexy=indexy+20;

                        x=SatirKontrol(&bas,y);
                    }
                }
                else if (SatirKontrol(&bas,y+20)!=0)
                {
                y=y+20;

                if(indexy < 980)
                    indexy=indexy+20;
                }

                   break;

                case KEY_LEFT:

                if(KeyDeger%2 == 1)
                {
                    if(Tx < x)
                        TaraliYaz(&bas,x-20,y,x,y+20,fark+Td);

                    else
                        TaraliYaz(&bas,x-20,y,x,y+20,fark+Td);

                }
                if(x<=50 && y<=100)
                {
                    x=50;
                    y=100;
                    indexy=100;
                }
                else if(x<=50)
                {
                    y=y-20;
                    indexy=indexy-20;

                    if(SatirKontrol(&bas,y)==0)
                        x=50;
                    else
                        x=SatirKontrol(&bas,y);
                }
                else
                    x=x-20;

                   break;

                case KEY_RIGHT:

                if(fark<=0)
                    fark=0;

                if(KeyDeger%2 == 1)
                {
                    if(NullKontrol(&bas,x+20,y)!=0)
                    {
                        if(Tx > x)
                        {
                            AlaniSil(&bas,x,y,x+20,y+20,15,fark+Td);
                            TaraliYaz(&bas,x,y,x+20,y+20,fark+Td);
                        }
                        else
                        {
                            AlaniSil(&bas,x,y,x+20,y+20,9,fark+Td);
                            TaraliYaz(&bas,x,y,x+20,y+20,fark+Td);
                        }
                    }
                }
                if(bas == NULL ) {}
                else if(x>=1800 && NullKontrol(&bas,50,y+20)==0) {}
                else if(x>=1800 && NullKontrol(&bas,50,y+20)!=0)
                {
                    x=50;
                    y=y+20;

                    if(indexy < 980)
                        indexy=indexy+20;
                }
                else if(x<=1800)
                {
                    if(NullKontrol(&bas,x+20,y)==0 && son->satir == y && son->sutun == x){}
                    else if(NullKontrol(&bas,x+20,y)==0 && (son->satir != y || son->sutun != x))
                    {
                        if(SatirKontrol(&bas,y+20)==0)
                        {
                            y=y+20;

                            if(indexy < 980)
                                indexy=indexy+20;

                            x=50;
                        }
                        else
                        {
                            y=y+20;

                            if(indexy < 980)
                                indexy=indexy+20;

                            x=50;
                        }
                    }
                    else
                        x=x+20;
                }

                   break;

                default:
                   break;
            }
        }

       /// if(fark<=0)
        ///    indexy=(100+Go);

        fark2=fark;

        if(fark2<=0)
            fark2=0;

        SiralaSayi(100+Go,fark+Td);

        if(x >= 1790)
            line(1811,99,1811,getmaxy());




    }

    closegraph();
    return 0;
}
