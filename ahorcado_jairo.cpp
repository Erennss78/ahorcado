// Ahorcado - version avanzada (Jairo)
// Categorias, dificultad, dibujo del ahorcado, letras usadas,
// pistas, adivinar la palabra completa, puntaje y estadisticas.
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

const int NUM_CATEGORIAS = 4;
const int PALABRAS_POR_CATEGORIA = 8;

struct Categoria
{
    string nombre;
    string palabras[PALABRAS_POR_CATEGORIA];
};

struct Estadisticas
{
    int partidas;
    int ganadas;
    int perdidas;
    int puntos;
    int mejorRacha;
    int rachaActual;
};

Categoria categorias[NUM_CATEGORIAS] = {
    {"Computacion", {"procesador", "memoria", "chipset", "registro", "almacenamiento", "particion", "cluster", "motherboard"}},
    {"Animales", {"elefante", "jirafa", "cocodrilo", "mariposa", "tortuga", "pinguino", "delfin", "murcielago"}},
    {"Frutas", {"manzana", "sandia", "maracuya", "frutilla", "durazno", "mandarina", "guanabana", "aguacate"}},
    {"Paises", {"ecuador", "colombia", "argentina", "japon", "alemania", "australia", "marruecos", "canada"}}};

void dibujarAhorcado(int etapa)
{
    // etapa va de 0 (vacio) a 6 (completo)
    string cabeza = (etapa >= 1) ? "O" : " ";
    string cuerpo = (etapa >= 2) ? "|" : " ";
    string brazoIzq = (etapa >= 3) ? "/" : " ";
    string brazoDer = (etapa >= 4) ? "\\" : " ";
    string piernaIzq = (etapa >= 5) ? "/" : " ";
    string piernaDer = (etapa >= 6) ? "\\" : " ";

    cout << "  +---+" << endl;
    cout << "  |   |" << endl;
    cout << "  " << cabeza << "   |" << endl;
    cout << " " << brazoIzq << cuerpo << brazoDer << "  |" << endl;
    cout << " " << piernaIzq << " " << piernaDer << "  |" << endl;
    cout << "      |" << endl;
    cout << "=========" << endl;
}

string aMinusculas(string texto)
{
    for (int i = 0; i < (int)texto.length(); i++)
    {
        texto[i] = tolower(texto[i]);
    }
    return texto;
}

bool soloLetras(string texto)
{
    if (texto.length() == 0)
    {
        return false;
    }
    for (int i = 0; i < (int)texto.length(); i++)
    {
        if (texto[i] < 'a' || texto[i] > 'z')
        {
            return false;
        }
    }
    return true;
}

int leerOpcion(int minimo, int maximo)
{
    int opcion;
    while (true)
    {
        cout << "Opcion: ";
        if (cin >> opcion && opcion >= minimo && opcion <= maximo)
        {
            return opcion;
        }
        if (cin.eof())
        {
            exit(0);
        }
        cout << "Opcion invalida. Ingresa un numero entre " << minimo << " y " << maximo << "." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// Descubre todas las apariciones de una letra. Devuelve cuantas encontro.
int descubrirLetra(string palabra, string &oculta, char letra)
{
    int encontradas = 0;
    for (int i = 0; i < (int)palabra.length(); i++)
    {
        if (palabra[i] == letra && oculta[i] == '_')
        {
            oculta[i] = letra;
            encontradas++;
        }
    }
    return encontradas;
}

void mostrarEspaciada(string texto)
{
    for (int i = 0; i < (int)texto.length(); i++)
    {
        cout << texto[i] << " ";
    }
    cout << endl;
}

// Juega una partida. Devuelve los puntos ganados (0 si pierde).
int jugarPartida(int categoria, int maxErrores)
{
    string palabra = categorias[categoria].palabras[rand() % PALABRAS_POR_CATEGORIA];
    string oculta(palabra.length(), '_');
    string usadas = "";
    int errores = 0;
    int pistasUsadas = 0;

    while (errores < maxErrores && oculta != palabra)
    {
        cout << "\n----------------------------------------" << endl;
        cout << "Categoria: " << categorias[categoria].nombre
             << "   (" << palabra.length() << " letras)" << endl;
        dibujarAhorcado(errores * 6 / maxErrores);
        cout << "Palabra:  ";
        mostrarEspaciada(oculta);
        cout << "Usadas:   " << (usadas == "" ? "-" : usadas) << endl;
        cout << "Vidas:    " << maxErrores - errores << " de " << maxErrores << endl;
        cout << "\nEscribe una letra, la palabra completa, o ? para una pista: ";

        string entrada;
        if (!(cin >> entrada))
        {
            exit(0);
        }
        entrada = aMinusculas(entrada);

        if (entrada == "?")
        {
            if (maxErrores - errores <= 1)
            {
                cout << ">> No puedes pedir pista con tu ultima vida." << endl;
                continue;
            }
            // Busca una letra oculta al azar y la descubre
            int pos;
            do
            {
                pos = rand() % palabra.length();
            } while (oculta[pos] != '_');

            char letraPista = palabra[pos];
            descubrirLetra(palabra, oculta, letraPista);
            usadas += letraPista;
            usadas += ' ';
            errores++;
            pistasUsadas++;
            cout << ">> Pista: la palabra tiene la letra '" << letraPista << "' (te cuesta 1 vida)." << endl;
            continue;
        }

        if (!soloLetras(entrada))
        {
            cout << ">> Solo se permiten letras de la a a la z (sin tildes ni n con virgulilla)." << endl;
            continue;
        }

        if (entrada.length() > 1)
        {
            // Intento de adivinar la palabra completa
            if (entrada == palabra)
            {
                oculta = palabra;
            }
            else
            {
                cout << ">> \"" << entrada << "\" no es la palabra. Pierdes 2 vidas." << endl;
                errores += 2;
            }
            continue;
        }

        char letra = entrada[0];
        if (usadas.find(letra) != string::npos)
        {
            cout << ">> Ya usaste la letra '" << letra << "'. Prueba otra." << endl;
            continue;
        }

        usadas += letra;
        usadas += ' ';

        int encontradas = descubrirLetra(palabra, oculta, letra);
        if (encontradas > 0)
        {
            cout << ">> Bien! La letra '" << letra << "' aparece " << encontradas << " vez/veces." << endl;
        }
        else
        {
            cout << ">> La letra '" << letra << "' no esta." << endl;
            errores++;
        }
    }

    cout << "\n========================================" << endl;
    if (oculta == palabra)
    {
        int puntos = (int)palabra.length() * 10 +(maxErrores - errores) * 5 - pistasUsadas * 5;
        if (puntos < 0)
        {
            puntos = 0;
        }
        dibujarAhorcado(errores * 6 / maxErrores);
        cout << "GANASTE! La palabra era: " << palabra << endl;
        cout << "Puntos de esta partida: " << puntos << endl;
        return puntos;
    }
    else
    {
        dibujarAhorcado(6);
        cout << "PERDISTE. La palabra era: " << palabra << endl;
        return 0;
    }
}

void mostrarEstadisticas(Estadisticas e)
{
    cout << "\n=== ESTADISTICAS ===" << endl;
    cout << "Partidas jugadas: " << e.partidas << endl;
    cout << "Ganadas:          " << e.ganadas << endl;
    cout << "Perdidas:         " << e.perdidas << endl;
    if (e.partidas > 0)
    {
        cout << "Porcentaje:       " << e.ganadas * 100 / e.partidas << "%" << endl;
    }
    cout << "Puntos totales:   " << e.puntos << endl;
    cout << "Mejor racha:      " << e.mejorRacha << endl;
}

int main()
{
    srand(time(0));
    Estadisticas stats = {0, 0, 0, 0, 0, 0};
    int opcion;

    do
    {
        cout << "\n########################################" << endl;
        cout << "#        JUEGO DEL AHORCADO            #" << endl;
        cout << "########################################" << endl;
        cout << "1. Jugar" << endl;
        cout << "2. Ver estadisticas" << endl;
        cout << "3. Como se juega" << endl;
        cout << "4. Salir" << endl;
        opcion = leerOpcion(1, 4);

        if (opcion == 1)
        {
            cout << "\nElige una categoria:" << endl;
            for (int i = 0; i < NUM_CATEGORIAS; i++)
            {
                cout << i + 1 << ". " << categorias[i].nombre << endl;
            }
            cout << NUM_CATEGORIAS + 1 << ". Al azar" << endl;
            int cat = leerOpcion(1, NUM_CATEGORIAS + 1) - 1;
            if (cat == NUM_CATEGORIAS)
            {
                cat = rand() % NUM_CATEGORIAS;
            }

            cout << "\nElige la dificultad:" << endl;
            cout << "1. Facil   (8 vidas)" << endl;
            cout << "2. Normal  (6 vidas)" << endl;
            cout << "3. Dificil (4 vidas)" << endl;
            int dif = leerOpcion(1, 3);
            int maxErrores = (dif == 1) ? 8 : (dif == 2) ? 6 : 4;

            int puntos = jugarPartida(cat, maxErrores);
            stats.partidas++;
            if (puntos > 0)
            {
                stats.ganadas++;
                stats.puntos += puntos;
                stats.rachaActual++;
                if (stats.rachaActual > stats.mejorRacha)
                {
                    stats.mejorRacha = stats.rachaActual;
                }
            }
            else
            {
                stats.perdidas++;
                stats.rachaActual = 0;
            }
            cout << "Racha actual: " << stats.rachaActual << "   Puntos totales: " << stats.puntos << endl;
        }
        else if (opcion == 2)
        {
            mostrarEstadisticas(stats);
        }
        else if (opcion == 3)
        {
            cout << "\n=== COMO SE JUEGA ===" << endl;
            cout << "- Adivina la palabra secreta letra por letra." << endl;
            cout << "- Cada letra equivocada te quita 1 vida." << endl;
            cout << "- Puedes escribir la palabra completa: si fallas, pierdes 2 vidas." << endl;
            cout << "- Escribe ? para revelar una letra (cuesta 1 vida)." << endl;
            cout << "- Puntos: 10 por letra + 5 por vida sobrante - 5 por pista." << endl;
        }
    } while (opcion != 4);

    mostrarEstadisticas(stats);
    cout << "\nGracias por jugar!" << endl;
    return 0;
}
