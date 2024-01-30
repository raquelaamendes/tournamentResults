#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura do tipo jogador para guardar todos os dados necessários
typedef struct {
	char nome[50];
	int pontosJogo;
	int pontosTotal;
	int vitorias;
}Jogador;

Jogador * CriarListaJogadores(int *numJogadores){
	int i=0;	
	char nomeJogador[50];
	Jogador *jogadores = NULL;
	
	fgets(nomeJogador, sizeof(nomeJogador), stdin);
	while(strcmp(nomeJogador, "\n") != 0){
		jogadores = realloc(jogadores, (i + 1) * sizeof(Jogador));
		nomeJogador[strcspn(nomeJogador, "\n")] = '\0';
		strcpy(jogadores[i].nome, nomeJogador);
		jogadores[i].pontosJogo=0;
		jogadores[i].pontosTotal=0;
		jogadores[i].vitorias=0;
		i++;
		fgets(nomeJogador, sizeof(nomeJogador), stdin);
	}
	*numJogadores=i;
	return jogadores;
}

int DescobrePontos(char *linha){
	int pontos, index;
	char stringpontos[3];
	char *posicaodivisor;
	
	posicaodivisor = strchr(linha, ':');
	index = (int)(posicaodivisor-linha);
	index++;
	
	strcpy(stringpontos, linha + index);
	pontos = atoi(stringpontos);
	
	return pontos;
	
}


void AnalisarLinha(Jogador *jogadores, char*linha, int lim, int*maiorpontos){
	int pontos, index;
	char *posicaodivisor;
	char nome[50];
	
	posicaodivisor = strchr(linha, ':');
	if(posicaodivisor != NULL){

        index = (int)(posicaodivisor-linha);


        strncpy(nome, linha, index);
        nome[index] = '\0';

        pontos = DescobrePontos(linha);
        if(pontos > *maiorpontos){
            *maiorpontos=pontos;
        }

        for(int i=0; i<lim;i++){
            if(strcmp(nome, jogadores[i].nome) == 0){
                jogadores[i].pontosJogo=pontos;
                jogadores[i].pontosTotal+=pontos;
            }
        }
	}	
}

void atribuirVitoria(Jogador *jogadores, int lim, int maiorpontos){
	for(int i=0;i<lim;i++){
		if(jogadores[i].pontosJogo == maiorpontos){
			jogadores[i].vitorias += 1;
		}
	}
}

void ResetPontosJogo(Jogador *jogadores, int lim){
	for(int i=0;i<lim;i++){
		jogadores[i].pontosJogo = 0;
	}
}

void AnalisaJogo(int *numJogadores, Jogador *jogadores){
	char linha[100];
	int maiorpontos=0;
	
	fgets(linha, sizeof(linha), stdin);
	while(strcmp(linha, "\n") != 0){
		AnalisarLinha(jogadores, linha, *numJogadores, &maiorpontos);
		if(feof(stdin) != 0)
		{
            break;
		}
		fgets(linha, sizeof(linha), stdin);
	}
	if(maiorpontos>1){
		atribuirVitoria(jogadores, *numJogadores, maiorpontos);
	}
	ResetPontosJogo(jogadores, *numJogadores);
}

int getmaxVitorias(Jogador *jogadores, int lim){
	int max=0;
	for(int i=0; i<lim; i++){
		if(jogadores[i].vitorias > max){
			max = jogadores[i].vitorias;
		}
	}
	return max;
}

int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void empatePorPontos(Jogador *jogadores, int *indexjogadoresempatadosporpontos, int contadorEmpatesPontos, int maxPontos, int *jogadoresMostrados){
	char *nomes[contadorEmpatesPontos];
	
	
	for (int i=0; i < contadorEmpatesPontos; i++){
		nomes[i] = (char *)malloc(50 * sizeof(char)); //Alocar memoria para o nome
		strcpy(nomes[i], (jogadores[indexjogadoresempatadosporpontos[i]].nome));
	}
	
	
	qsort(nomes, contadorEmpatesPontos, sizeof(nomes[0]), compare);
	
	
	for (int i=0; i < contadorEmpatesPontos; i++){
		for(int j=0; j < contadorEmpatesPontos; j++){
			if( strcmp(nomes[i], jogadores[indexjogadoresempatadosporpontos[j]].nome) == 0 ){
				printf("%d. %s - %d pontos, %d vitorias.\n",(*jogadoresMostrados) + 1, jogadores[indexjogadoresempatadosporpontos[j]].nome, jogadores[indexjogadoresempatadosporpontos[j]].pontosTotal, jogadores[indexjogadoresempatadosporpontos[j]].vitorias);
				(*jogadoresMostrados) += 1;
			}
		}
	}
	
	for (int i = 0; i < contadorEmpatesPontos; i++) { //Libertar memoria dos nomes
        free(nomes[i]);
    }
	
}

void empatePorVitorias(Jogador *jogadores, int *indexjogadoresempatadosporvitorias, int contador, int maxVitorias, int *jogadoresMostrados){
	int maxPontos = 0, contadorEmpatesPontos=0;
	int *indexjogadoresempatadosporpontos;
	
	indexjogadoresempatadosporpontos = (int *)malloc(contador * sizeof(int));
	
	for(int j=0; j<contador; j++){ //Vai buscar o maxPontos nos jogadores empatados
		if(jogadores[indexjogadoresempatadosporvitorias[j]].pontosTotal > maxPontos){ 
			maxPontos = jogadores[indexjogadoresempatadosporvitorias[j]].pontosTotal;
		}
	}
	while(maxPontos >= 0){ //percorre os Pontos do Max ate ao 0
		for(int a=0; a<contador; a++){ //Contador para o nr de jogadores com os mesmos pontos para despistar empates	
			if(jogadores[indexjogadoresempatadosporvitorias[a]].pontosTotal == maxPontos){
				indexjogadoresempatadosporpontos[contadorEmpatesPontos] = indexjogadoresempatadosporvitorias[a];
				contadorEmpatesPontos++;
			}
		}
		if(contadorEmpatesPontos>1){
			empatePorPontos(jogadores, indexjogadoresempatadosporpontos, contadorEmpatesPontos, maxPontos, jogadoresMostrados);
		}else if(contadorEmpatesPontos == 1){
			printf("%d. %s - %d pontos, %d vitorias.\n",(*jogadoresMostrados) + 1, jogadores[indexjogadoresempatadosporpontos[0]].nome, jogadores[indexjogadoresempatadosporpontos[0]].pontosTotal, jogadores[indexjogadoresempatadosporpontos[0]].vitorias);
			(*jogadoresMostrados) += 1;
		}
	
		maxPontos--;
		contadorEmpatesPontos=0;
	}
	free(indexjogadoresempatadosporpontos);
	jogadoresMostrados++;
}

void MostrarResultados(Jogador *jogadores, int lim){
	int maxVitorias=0, jogadoresMostrados=0, contador;
	int *indexjogadoresempatadosporvitorias;
	
	
	indexjogadoresempatadosporvitorias = (int *)malloc(lim * sizeof(int));
	
	if(indexjogadoresempatadosporvitorias == NULL) {
		printf("Nao foi possivel mostrar os resultados\n");
		
	} else {
		for (int i = 0; i < lim; i++) { //Inicializar o vector indexjogadoresempatadosporvitorias a -1.
            indexjogadoresempatadosporvitorias[i] = -1;
        }
		maxVitorias = getmaxVitorias(jogadores, lim);
		while(jogadoresMostrados<lim){
			contador = 0;
			for(int i=0; i<lim; i++){
				if(jogadores[i].vitorias == maxVitorias){ //Contador para o nr de jogadores com os mesmo nr de vitorias para despistar empates já colocando os respetivos indexes
					indexjogadoresempatadosporvitorias[contador] = i;
					contador++;
				}
			}
			
			if(contador > 1){ //Se houver empate no numero de vitorias
				empatePorVitorias(jogadores, indexjogadoresempatadosporvitorias, contador, maxVitorias, &jogadoresMostrados);
				
			}else{ //Não há empate no numero de vitorias
				printf("%d. %s - %d pontos, %d vitorias.\n",jogadoresMostrados+1, jogadores[indexjogadoresempatadosporvitorias[0]].nome, jogadores[indexjogadoresempatadosporvitorias[0]].pontosTotal, jogadores[indexjogadoresempatadosporvitorias[0]].vitorias);
				jogadoresMostrados++;
			}
			maxVitorias--;
			
			
		}
	
		free(indexjogadoresempatadosporvitorias);
	}
	
	
	
}

void LibertarJogadores(Jogador *jogadores){
	free(jogadores);
}

int main()
{
	int numJogadores;
	Jogador *jogadores;
	
	jogadores = CriarListaJogadores(&numJogadores);
	while(feof(stdin) == 0){
		AnalisaJogo(&numJogadores, jogadores);
	}
	MostrarResultados(jogadores, numJogadores);
	LibertarJogadores(jogadores);
	
    return 0;
}