#include<stdio.h>
#include<stdlib.h>

// Obtener la cantidad de filas del laberinto
int filas(FILE* file) {
	int rows = 0;

	int caracter;
	int leer = 1;
	while (leer) {
		caracter = fgetc(file);
		if (caracter == EOF) {
			rows = rows + 1;
			leer = 0;
			rewind(file);
		}
		else if (caracter == '\n') {
			rows = rows + 1;
		}
	}

	return rows;
}

// Obtener la cantidad de columnas del laberinto
int columnas(FILE* file) {
	int cols = 0;

	int caracter;
	int leer = 1;
	while (leer) {
		caracter = fgetc(file);
		if (caracter == EOF || caracter == '\n') {
			leer = 0;
			rewind(file);
		}
		else if (caracter != 32) {
			cols = cols + 1;
		}
	}

	return cols;
}

// Crear la matriz con el laberinto dado
int** crearMatriz(int rows, int cols) {
	int** maze = (int**)calloc(rows, sizeof(int*));
	int row;
	for (row = 0; row < rows; row++) {
		maze[row] = (int*)calloc(cols, sizeof(int));
	}
	return maze;
}

void fillMatriz(int** matrix, int rows, int cols, int value) {
	int x;
	int y;

	for (x = 0; x < rows; x++) {
		for (y = 0; y < cols; y++) {
			matrix[x][y] = value;
		}
	}
}

// Completar la matriz creada con la info del .txt
void fillLaberinto(int** maze, int rows, int cols, FILE* file) {
	int x = 0;
	int y = 0;

	int caracter;
	int leer= 1;
	while (leer) {
		caracter = fgetc(file);

		if (caracter == EOF) {
			leer= 0;
			rewind(file);
		}
		else if (caracter == '\n') {
			y = 0;
			x = x + 1;
		}
		else if (caracter != 32) {
			maze[x][y] = caracter == 48 ? 0 : caracter == 49 ? 1 : 2;
			y = y + 1;
		}
	}
}

// Buscar la solución (2)
void findSolution(int** maze, int rows, int cols, int entryX, int entryY) {
	// Saber por donde ya pasó
	int** visitado = crearMatriz(rows, cols);
	fillMatriz(visitado, rows, cols, 0);

	// Buscar hasta encontrar la solución
	int x = entryX;
	int y = entryY;
	visitado[x][y] = 1;

	int searching = 1;
	while (searching) {
		if (maze[x][y] == 2) {
			printf("La solución se encuentra en las coordenadas: %d x %d\n", x, y);
			searching = 0;
			continue;
		}


		int nextX;
		int nextY;

		// Buscar a la derecha
		nextX = x;
		nextY = y + 1;
		if (nextY < cols && (maze[nextX][nextY] == 1 || maze[nextX][nextY] == 2) && visitado[nextX][nextY] == 0) {
			x = nextX;
			y = nextY;
			continue;
		}

		// Buscar abajo
		nextX = x + 1;
		nextY = y;
		if (nextX < rows && (maze[nextX][nextY] == 1 || maze[nextX][nextY] == 2) && visitado[nextX][nextY] == 0) {
			x = nextX;
			y = nextY;
			continue;
		}

		// Buscar a la izquierda
		nextX = x;
		nextY = y - 1;
		if (nextY >= 0 && (maze[nextX][nextY] == 1 || maze[nextX][nextY] == 2) && visitado[nextX][nextY] == 0) {
			x = nextX;
			y = nextY;
			continue;
		}

		// Buscar arriba
		nextX = x - 1;
		nextY = y;
		if (nextX >= 0 && (maze[nextX][nextY] == 1 || maze[nextX][nextY] == 2) && visitado[nextX][nextY] == 0) {
			x = nextX;
			y = nextY;
			continue;
		}

		printf("No hay solución en este camino\n");
		searching = 0;
	}
}

//Imprimir soluciones
void findSolutions(int** maze, int rows, int cols) {
	//Buscar la entrada
	int x;
	int y;

	// Buscar en las esquinas
	for (x = 0; x < rows; x++) {
		// Buscar arriba y abajo
		if (x == 0 || x == rows - 1) {
			for (y = 0; y < cols; y++) {
				// Buscar lados
				if (x == 0 || x == rows - 1 || y == 0 || y == cols - 1) {
					int isEntry = maze[x][y] == 1;
					if (isEntry) {
						printf("\nLa posición %d x %d es la entrada\n", x, y);

						// Encontrar la solución
						findSolution(maze, rows, cols, x, y);
					}
				}
			}
		}
	}
}

// Leer el archivo .txt
void solveMaze(const char* nombre) {

	FILE* file = fopen(nombre, "r");

	// Verificar si se leyó un archivo válido
	if (file == NULL) {
		printf("Archivo inválido\n");
		exit(1);
	}
	printf("Calculando el tamaño del laberinto\n");
	const int rows = filas(file);
	int cols = columnas(file);
	printf("El laberinto es de tamaño %d x %d\n\n", rows, cols);

	// Crear la matriz, completarla y recorrerla para encontrar la solución.
	int** maze = crearMatriz(rows, cols);
	printf("Laberinto guardado.\n");
	fillLaberinto(maze, rows, cols, file);

	// Buscar soluciones
	printf("Buscando soluciones\n");
	findSolutions(maze, rows, cols);
	fclose(file);
}

// Main
int main() {
	printf("Proyecto: Resolver el laberinto\n\n");

	// Calls the solve maze function because is a fucking bad practice to do everything on the main
	solveMaze("laberinto.txt");
}


