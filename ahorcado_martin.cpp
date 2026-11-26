// Ahorcado - version sencilla (Martin)
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    string palabras[10] = {"computadora", "teclado", "memoria", "pantalla", "raton",
                           "procesador", "disco", "programa", "impresora", "internet"};

    srand(time(0));
    string palabra = palabras[rand() % 10];
    string oculta(palabra.length(), '_');
    int intentos = 6;

    cout << "=== JUEGO DEL AHORCADO ===" << endl;

    while (intentos > 0 && oculta != palabra)
    {
        cout << "\nPalabra: " << oculta << endl;
        cout << "Intentos restantes: " << intentos << endl;
        cout << "Ingresa una letra: ";

        char letra;
        cin >> letra;

        bool acierto = false;
        for (int i = 0; i < palabra.length(); i++)
        {
            if (palabra[i] == letra)
            {
                oculta[i] = letra;
                acierto = true;
            }
        }

        if (acierto)
        {
            cout << "Bien! La letra esta en la palabra." << endl;
        }
        else
        {
            cout << "La letra no esta." << endl;
            intentos--;
        }
    }

    if (oculta == palabra)
    {
        cout << "\nGANASTE! La palabra era: " << palabra << endl;
    }
    else
    {
        cout << "\nPERDISTE. La palabra era: " << palabra << endl;
    }

    return 0;
}
