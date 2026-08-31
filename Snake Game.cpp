#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;   

// Numeros asociados a las flechas del teclado 
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESC 27

int xc = 30, yc = 15; 
int cuerpo[200][2];
int n = 1; 
int tam = 4;
int x = 10;
int y = 12;
int dir = 3;
int velocidad = 150;
int score = 0;
int h = 1;
char tecla = ' ';

// Funcion para dar posicion en pantalla (X, Y)
void gotoxy(int x, int y) {
    HANDLE hCon;
    COORD dwPos;
    dwPos.X = x; 
    dwPos.Y = y;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hCon, dwPos);
}

// Dibujar el tablero de juego
void pintar_tablero() {
    for (int i = 2; i < 78; i++) {
        gotoxy(i, 3); printf("%c", 205); 
        gotoxy(i, 23); printf("%c", 205); 
    } 
    for (int i = 4; i < 23; i++) {
        gotoxy(2, i); printf("%c", 186); 
        gotoxy(77, i); printf("%c", 186); 
    } 
    gotoxy(2, 3); printf("%c", 201);
    gotoxy(2, 23); printf("%c", 200);
    gotoxy(77, 3); printf("%c", 187);
    gotoxy(77, 23); printf("%c", 188);
}

// Guardar posicion del cuerpo
void guardar_posicion() {
    cuerpo[n][0] = x;
    cuerpo[n][1] = y;
    n++;
    if (n == tam) n = 1;
}

// Dibujar cuerpo de la vibora
void pintar_cuerpo() {
    for (int i = 1; i < tam; i++) {
        gotoxy(cuerpo[i][0], cuerpo[i][1]);
        printf("*");
    }
}

// Borrar rastro de la cola
void borrar_cuerpo() {
    gotoxy(cuerpo[n][0], cuerpo[n][1]);
    printf(" ");
}

// Dibujar la comida en pantalla con un caracter compatible ('O')
void pintar_comida() {
    gotoxy(xc, yc); 
    printf("O"); 
}

// Leer teclas ingresadas
void controles() {
    if (kbhit()) {
        tecla = getch();
        switch (tecla) {
            case ARRIBA: 
                if (dir != 2) dir = 1;
                break; 
            case ABAJO: 
                if (dir != 1) dir = 2;
                break;
            case DERECHA: 
                if (dir != 4) dir = 3;
                break; 
            case IZQUIERDA: 
                if (dir != 3) dir = 4;
                break;
        }
    }
}

// Aumentar velocidad segun puntaje
void cambiar_velocidad() {
    if (score == h * 20 && velocidad > 30) {
        velocidad -= 10;
        h++;
    }
} 

// Detectar si la vibora come
void comida() {
    if (x == xc && y == yc) {
        // Generar nueva posicion dentro de los bordes del tablero
        xc = (rand() % 73) + 3;
        yc = (rand() % 18) + 4;
        
        tam++;
        score += 10;
        cambiar_velocidad();
    }
}

// Detectar colision con limites o cuerpo
bool game_over() {
    if (y <= 3 || y >= 23 || x <= 2 || x >= 77)
        return false;
    for (int j = tam - 1; j > 0; j--) {
        if (cuerpo[j][0] == x && cuerpo[j][1] == y)
            return false;
    }
    return true;
}

// Imprimir puntuacion en pantalla
void puntos() {
    gotoxy(3, 1); 
    printf("Puntuacion: %d ", score);
}

int main() {
    // Inicializar la semilla para posiciones aleatorias reales
    srand(time(NULL));

    // Ocultar el cursor de la consola
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 2;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);

    pintar_tablero();   

    while (tecla != ESC && game_over()) {
        borrar_cuerpo();
        guardar_posicion();
        pintar_cuerpo();
        
        pintar_comida(); 
        comida();
        puntos();
        
        controles(); 
        
        if (dir == 1) y--;
        if (dir == 2) y++;
        if (dir == 3) x++;
        if (dir == 4) x--;
        
        Sleep(velocidad);
    }

    gotoxy(35, 12);
    printf("GAME OVER");
    gotoxy(1, 24);
    system("pause");
    return 0;
}