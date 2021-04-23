//
//  main.cpp
//  PPMLoader
//
//  Created by Márcio Sarroglia Pinho on 05/04/21.
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

using namespace std;
class RGB {
public:
    int r, g, b;
};

RGB Imagem[1000][1000];

RGB freq[50];

const int limiar = 7;

unsigned int Largura, Altura;


// Utilize esta matriz 3D para realizar a contagem das cores
// Lembre-se de zerar a matriz antes de iniciar.
unsigned int ContadoresRGB[255][255][255];//preto(0,0,0)


void SalvaPPM(const char* nome)
{
    int x, y;

    ofstream arquivo;

    arquivo.open(nome, ios::out);


    arquivo << "P3" << endl;
    arquivo << Largura << " " << Altura << endl;
    arquivo << 255 << endl;
    for (y = Altura - 1; y >= 0; y--)
    {
        for (x = 0; x < Largura; x++)
        {
            arquivo << " " << Imagem[y][x].r << " " << Imagem[y][x].g << " " << Imagem[y][x].b;

        }
        arquivo << endl;
    }
}

void CarregaPPM(const char* nome)
{
    int x, y;

    string dummy;

    ifstream arquivo;

    arquivo.open(nome, ios::in);
    if (!arquivo)
    {
        cout << "Arquivo inexistente." << endl;
        exit(1);
    }


    arquivo >> dummy;

    if (dummy != "P3")
    {
        cout << "Formato Invalido" << endl;
        exit(1);
    }
    arquivo >> Largura;
    if ((Largura < 0) || (Largura > 900))
    {
        cout << "Largura Invalida" << endl;
        exit(2);
    }
    arquivo >> Altura;
    if ((Altura < 0) || (Altura > 900))
    {
        cout << "Altura Invalida" << endl;
        exit(3);
    }

    arquivo >> dummy;
    if (dummy != "255")
    {
        cout << "Formato Invalido" << endl;
        exit(4);
    }
    for (y = Altura - 1; y >= 0; y--)
    {
        for (x = 0; x < Largura; x++)
        {
            arquivo >> Imagem[y][x].r >> Imagem[y][x].g >> Imagem[y][x].b;
        }
    }
}

void zeraRGB() {
    for (int i = 0; i < 255; i++)
    {
        for (int j = 0; j < 255; j++)
        {
            for (int k = 0; k < 255; k++)
            {
                ContadoresRGB[i][j][k] = 0;
            }
        }
    }
}

void contarCores() {
    for (int y = 0; y < Altura; y++)
    {
        for (int x = 0; x < Largura; x++)
        {
            RGB cor = Imagem[y][x];
            ContadoresRGB[cor.r][cor.g][cor.b]++;
        }
    }
}
int calculaDistancia(RGB c1,RGB c2) {
    return sqrt(pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2));
}


void buscarCoresFreq() {
    int ultimoInserido = 0;
    for (int i = 0; i < 50; i++)
    {
        int maiorValor = 0;
        int mr = 0;
        int mg = 0;
        int mb = 0;
        for (int r = 0; r < 255; r++)
        {
            for (int g = 0; g < 255; g++)
            {
                for (int b = 0; b < 255; b++)
                {
                    if (ContadoresRGB[r][g][b] > maiorValor) {
                        mr = r;
                        mg = g;
                        mb = b;
                        maiorValor = ContadoresRGB[r][g][b];
                    }
                }
            }
        }
        ContadoresRGB[mr][mg][mb] = 0;
        RGB c;
        c.r = mr;
        c.g = mg;
        c.b = mb;
        bool insere = true;
        for (int j = 0; j < i; j++)
        {
            int dis = calculaDistancia(freq[j], c);
            if (dis <= limiar) {
                insere = false;
                break;
            }
        }

        if (insere) {
            freq[i].r = mr;
            freq[i].g = mg;
            freq[i].b = mb;
        }
        else {
            cout << "cor parecida" << endl;
            i--;
        }
    }
}


RGB ProcuraCorProxima(RGB cor) {
    //cor = cor atual
    //freq = mais frequente
    int dist = numeric_limits<int>::max();
    int index = 0;
    for (int i = 0; i < 50; i++)
    {
        int dis = calculaDistancia(cor,freq[i]);
        if (dis < dist) {
            dist = dis;
            index = i;
        }
    }
    return freq[index];
}

void quantificarImagem() {
    for (int y = 0; y < Altura; y++)
    {
        for (int x = 0; x < Largura; x++)
        {
            RGB cor = Imagem[y][x]; 
            RGB corProx = ProcuraCorProxima(cor);
            Imagem[y][x] = corProx;
        }
    }
}

int main(int argc, const char* argv[]) {
    cout << "zerando rgb" << endl;
    zeraRGB();
    CarregaPPM("Colegio.ppm");
    cout << "Contar Cores" << endl;
    contarCores();
    cout << "buscar Cores Freq" << endl;
    buscarCoresFreq();
    cout << "quantificar Imagem" << endl;
    quantificarImagem();
    SalvaPPM("Copia-Colegio.ppm");
    return 0;
}