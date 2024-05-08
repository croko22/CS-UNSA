#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// Función para determinar si un caracter es un separador
bool esSeparador(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

// Función para determinar si un caracter es un símbolo especial
bool esSimboloEspecial(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '(' || c == ')');
}

int main()
{
    // Abrir el archivo de código fuente
    ifstream archivo("test.txt");
    if (!archivo)
    {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    // Variables para almacenar la palabra, número y símbolo actual
    string palabra = "";
    string numero = "";
    char simbolo;

    // Leer el archivo caracter por caracter
    char caracter;
    while (archivo.get(caracter))
    {
        // Si el caracter es un separador
        if (esSeparador(caracter))
        {
            // Si hay una palabra acumulada, imprimir que es una palabra
            if (!palabra.empty())
            {
                cout << "palabra = " << palabra << endl;
                palabra = ""; // Limpiar la palabra actual
            }
            // Si hay un número acumulado, imprimir que es un número
            if (!numero.empty())
            {
                cout << "numero = " << numero << endl;
                numero = ""; // Limpiar el número actual
            }
        }
        // Si el caracter es un símbolo especial
        else if (esSimboloEspecial(caracter))
        {
            cout << "simbolo = " << caracter << endl;
        }
        // Si el caracter es una letra
        else if (isalpha(caracter))
        {
            palabra += caracter; // Agregar el caracter a la palabra actual
        }
        // Si el caracter es un dígito
        else if (isdigit(caracter))
        {
            numero += caracter; // Agregar el caracter al número actual
        }
    }

    // Cerrar el archivo
    archivo.close();

    return 0;
}
