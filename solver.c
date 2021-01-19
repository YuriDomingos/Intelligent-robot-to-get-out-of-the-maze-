#include <stdio.h>
#include <stdlib.h>

/*
@Autores : Yuri Domingos (UCAN 19 -01 -2021 )
Data     : 18 - 01 -2021
Objectivo : Construir um labirinto para simular um agente baseado em aprendizagem

*/


char** maze;     // Inicializa a matriz (ou seja o cenário todo ( o labirinto)
int** visited;   // Cria a matriz cópia ( para  marcar os elementos já visitados
int rows;        // linhas
int cols;        // Colunas
int start_row;   // Acho que aprovamos INglês técnico 1 ( logo não é preciso traduzir esta linha ) Sorry
int start_col;   // mesma coisa


//-- Criamos enumerações ( constantes profissionais para o labirinto )
enum terrain {
	empty, // vazio
	wall, // parede
	goal, // objectivo
	crumb  // os pontinhos no cenário
};


//--- as funções abaixo servem para criar espaço no nosso Buffer de memória  (aloca espaço para um bloco de bytes consecutivos na memória RAM (= random access memory) do computador e devolve o endereço desse bloco)
 
void alloc_maze()     
{
	maze = malloc(rows * sizeof(char*));
	
	int i;
	for (i = 0; i < rows; ++i){
		maze[i] = malloc(cols * sizeof(char*));
	}
}


void alloc_visited()
{
	visited = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		visited[i] = malloc(cols * sizeof(int*));
	}
}




//-- Simplesmente esta função carrega o nosso labirinto ( arquivo maze.txt ) Maze quer dizer labirinto 

//-- As validações para carregar o ficheiro


void get_maze(char* file_name)
{
	char c;
	char rows_s[3\] = { '\0' };
	char cols_s[3] = { '\0' };
	int rows_i = 0;
	int cols_i = 0;
	int swap = 0;

	FILE* maze_file = fopen(file_name, "r");

	if (maze_file) {
		while ((c = getc(maze_file)) != EOF) {
			if (c == '\n') {
				break;
			} else if (c == ','){
				swap = 1;
			} else if (!swap) {
				rows_s[rows_i] = c;
				rows_i++;
			} else {
				cols_s[cols_i] = c;
				cols_i++;
			}
		}
	} else {
		printf("No such file!");
		return;
	}

	rows = atoi(rows_s);
	cols = atoi(cols_s);

	alloc_maze();
	
	int i,j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {

			if ((c = getc(maze_file)) == '\n') {
				c = getc(maze_file);
			}

			maze[i][j] = c;

			if (c =='s') {
				start_row = i;
				start_col = j;
			}
		}
	}

	fclose(maze_file);
}





//  -- Marca o cenŕio todo com as enum's , para marcação dos elementos visitados 


void init_visited()
{
	alloc_visited();

	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] == '+') {
				visited[i][j] = wall;
			} else if (maze[i][j] == 'g') {
				visited[i][j] = goal;
			} else {
				visited[i][j] = empty;
			}
		}
	}
}	


//-- Printamos o nosso labirinto zinho 

void print_maze()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}	
	printf("\n");
}


//-- A função que adiciona todos pontinho do nosso labirinto, ( ou seja quando temos passagens ele adiciona um pontinho 
void add_crumbs()
{
	int i, j;
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (maze[i][j] != 's' && visited[i][j] == crumb) {
				maze[i][j] = '.';
			}
		}
	}
}



//  A nossa tradição de S.I o velho aprofundamento ( mas devemos ter atenção que este código possui um complexidade Exponencial )  

int dfs(int row, int col)
{
	int* current = &visited[row][col];

	if (*current == goal) {
		return 1;
	}

	if (*current == empty) {
		*current =  wall; // para encontrar a solução só precisamos definir wall 

		if (dfs(row, col - 1)){
			*current = crumb;
			return 1;
		} 
		if (dfs(row + 1, col)){
			*current = crumb;
			return 1;
		} 
		if (dfs(row, col + 1)){
			*current = crumb;
			return 1;		
		} 
		if (dfs(row - 1, col)){
			*current = crumb;
			return 1;
		}
	}

	return 0;
}


// Já está tudo dito 
int main() {
	get_maze("maze.txt");
	init_visited();

	print_maze();

	if (!dfs(start_row, start_col)) {
		printf("No path to the goal could be found.\n");
	} else {
		add_crumbs();
		print_maze();
	}
	return 0;
}
