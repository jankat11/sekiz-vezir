#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

string tahta[8][8];
char yatay_aks[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
int düşey_aks[8] = {8, 7, 6, 5, 4, 3, 2, 1};

typedef struct
{
    bool yeri[8][8];
}
taş;

const int vezir_sayısı = 8;
taş vezir[vezir_sayısı];


void taslari_sor_yerlestir(void);
void taşları_yatay_dağıt(void);
void tahtayı_yerleştir(void);
void tahtayı_görüntüle(void);
void aşağı(int vezirNO);
void yukarı(int vezirNO);



bool yatay_kontrol(int vezirNO);
bool çapraz_kontrol(int vezirNO);
bool kare_dolu(int i, int j);



int main(void)
{
    //vezir yerlerini kullanıcıdan al ve tahtaya yerleştir. sonra tahtayı görsel olarak print et.
    printf("\n");
    taslari_sor_yerlestir();
    printf("\n");
    printf("\n");
    printf("ilk yerleşim:\n");
    tahtayı_görüntüle();

    //vezirleri yatay eksende her sütuna bir tane gelecek şekide dağıt.
    taşları_yatay_dağıt();

    //tüm vezirler çakışmadan sıralanana kadar döngüye sok.
    bool taşlar_çakışıyor = true;
    while(taşlar_çakışıyor)
    {
        int sayaç = 0;
        for (int i = 0; i < vezir_sayısı; i++)
        {
            int hamle = 0;
            while (!çapraz_kontrol(i) || !yatay_kontrol(i))
            {
                yukarı(i);
                hamle++;
                sayaç = hamle;
                if (hamle == 8)
                {
                    aşağı(0);
                    break;
                }
            }
            if (hamle == 8)
            {
                break;
            }
        }
        if(sayaç < 8)
        {
        taşlar_çakışıyor = false;
        }
    }
    //vezirlerin son dizilmiş halini print et.
    printf("\n");
    printf("son yerleşim:\n");
    tahtayı_görüntüle();
}

//kullanıcıdan taş yerlerini alır ve görsele uyumlu yerleştirir.
void taslari_sor_yerlestir(void)
{
     tahtayı_yerleştir();
     int i = 0, j = 1;
     string yer;
     printf("taşların yerini giriniz: (örn: e4 veya E4)\n");
     do
     {
     yer = get_string("%i. vezir: ", j);

     //alınan noktanın koordinatı sol üstten 00 değil sol alttan A1 olarak başladığı için sayıları görsele göre düzenle.
     while(strlen(yer) != 2 || (int)toupper(yer[0]) % 65 >= 8 || (int)toupper(yer[0]) %65 < 0 || (int)(yer[1] - 48) <= 0 ||
           (int)(yer[1] - 48) > 8 || kare_dolu(düşey_aks[(int)(yer[1] - 48)], (int)toupper(yer[0]) % 65 ))
     {
         printf("hatalı giriş! tekrar giriniz:\n");
         yer = get_string("%i. vezir: ", j);
     }
     vezir[i].yeri[düşey_aks[(int)(yer[1] - 48)]][(int)toupper(yer[0]) % 65] = true;
     tahtayı_yerleştir();
     i++;
     j++;
     }
     while(i < vezir_sayısı);
}

//taşları her sütuna bir tane gelecek şekilde dizer. satır yüksekliğini değiştirmez.
void taşları_yatay_dağıt(void)
{
    for (int k = 0; k < vezir_sayısı; k++)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (vezir[k].yeri[i][j] == true)
                {
                    vezir[k].yeri[i][j] = false;
                    vezir[k].yeri[i][k] = true;
                    tahtayı_yerleştir();
                }
            }
        }
    }
}

//hamle veya dizilimden sonra tahtaya hangi taşın nerede olduğunu kaydeder.
void tahtayı_yerleştir(void)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            //buraya vezirleri yerleştir.
            for (int k = 0; k < 8; k++)
            {
                if (vezir[k].yeri[i][j] == true)
                {   //vezir varsa dolu kare
                    tahta[i][j] = "_O|";
                    break;
                }
                else
                {  //vezir yoksa boş kare
                   tahta[i][j] = "__|";
                }
            }
        }
    }
}

//tahtanın o anki halini komut satırında görsel olarak görüntülemek için.
void tahtayı_görüntüle(void)
{
    printf("   ");
    for (int i = 0; i < 8; i++)
        {
            printf("__ " );
        }
        printf("\n");
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (j == 0)
                {
                    printf("%i |", düşey_aks[i]);
                }
                printf("%s", tahta[i][j]);
            }
            printf("\n");
        }
        printf("   ");
        for (int i = 0; i < 8; i++)
        {
            printf(" %c ", yatay_aks[i]);
        }
    printf("\n");
    printf("\n");
}

//koordinatları girilen karede taş olup olmadığını kontrol eder.
bool kare_dolu(int i, int j)
{
    if(strcmp(tahta[i][j], "_O|") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//numarası girilen taş aşağı yönde bir hamle gider. eğer en aşağıdaysa yukarıdan çıkar.
void aşağı(int vezirNO)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true && i != 7 && !kare_dolu(i + 1, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[i + 1][j] = true;
                tahtayı_yerleştir();
                return;
            }
            else if (vezir[vezirNO].yeri[i][j] == true && i == 7 && !kare_dolu(0, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[0][j] = true;
                tahtayı_yerleştir();
                return;
            }
        }
    }
}

//numarası girilen taş yukarı yönde bir hamle gider. eğer en yukarıdaysa aşağıdan çıkar.
void yukarı(int vezirNO)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true && i != 0 && !kare_dolu(i - 1, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[i - 1][j] = true;
                tahtayı_yerleştir();
                return;
            }
            else if (vezir[vezirNO].yeri[i][j] == true && i == 0 && !kare_dolu(7, j))
            {
                vezir[vezirNO].yeri[i][j] = false;
                vezir[vezirNO].yeri[7][j] = true;
                tahtayı_yerleştir();
                return;
            }
        }
    }
}

// taşın satırında solda kalan kısım için çakışan başka taşı kontrol eder.
bool yatay_kontrol(int vezirNO)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true)
            {
                int m = 0;
                for (int k = 0; k < j + 1; k++)
                {
                    if (kare_dolu(i, k))
                    {
                        m++;
                    }
                }
                if(m == 1)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}

// taşın sol tarafı için çapraz yollarında çakışan başka taşı kontrol eder.
bool çapraz_kontrol(int vezirNO)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(vezir[vezirNO].yeri[i][j] == true)
            {
                int m = 0;
                for (int k = j, n = i; k >= 0 && n < 8; k--, n++)
                {
                    if (kare_dolu(n, k))
                    {
                        m++;
                    }
                }
                for (int k = j, n = i; k >= 0 && n >= 0; k--, n--)
                {
                    if (kare_dolu(n, k))
                    {
                        m++;
                    }
                }
                if(m == 2)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return false;
}