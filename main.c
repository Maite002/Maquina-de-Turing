#include <stdlib.h>
#include <stdio.h>
#define R 82
#define L 76
#define P 80

// Defini��o de uma estrutura "tupla", que representa os elementos da tabela de transi��o da m�quina de Turing.
typedef struct{
    int estado;  // Estado de transi��o.
    char alfa;   // S�mbolo do alfabeto lido.
    char comando; // Comando a ser executado (Direita, Esquerda e Parar).
} tupla;

// Fun��o parser respons�vel por ler a configura��o da m�quina de um arquivo e construir a tabela de transi��o.
tupla** parser(short *alf){
    FILE *arq;
    int i = 0;
    int j = 0;
    tupla **aux;
    int T_alfabeto = 3; // Tamanho do alfabeto padr�o (3 no exemplo).
    int T_estados = 0;  // N�mero de estados.
    char c;

    int T;

    // Abre um arquivo de texto para leitura
    arq = fopen("config.txt", "r");
    if (arq == NULL) // C�digo para caso exista algum erro na abertura do arquivo
    {
        printf("Problemas na abertura do arquivo\n");
        exit(-1);
    }

    // L� o n�mero de estados e o tamanho do alfabeto a partir do arquivo.
    fscanf(arq, "%d %d\n", &T_estados, &T_alfabeto);
    printf("%d %d\n", T_alfabeto, T_estados);
    printf("\n");

    // L� os s�mbolos do alfabeto e os mapeia para �ndices em um vetor.
    do{
        fscanf(arq, "%c ", &c);
        printf("%7c%9c", c, ' ');
        alf[(int)c] = i;
        i++;
        if (i > T_alfabeto){
            printf("BRANCO DE FITA N�O ENCONTRADO\n");
            exit(-1);
        }
    } while (c != '~');

    // Aloca��o de mem�ria para a tabela de transi��o.
    aux = malloc(sizeof(tupla *) * T_estados);
    if (aux == NULL){
        fprintf(stderr, "out of memory\n");
        exit(-1);
    }

    for (i = 0; i < T_estados; i++){
        aux[i] = malloc(T_alfabeto * sizeof(tupla));
        if (aux[i] == NULL){
            fprintf(stderr, "out of memory\n");
            exit(-1);
        }
    }

    i = 0;
    printf("\n");

    // L� os elementos da tabela de transi��o do arquivo.
    while (!feof(arq)){
        fscanf(arq, "%d,%c,%c\n", &aux[j][i].estado, &aux[j][i].alfa, &aux[j][i].comando);
        printf("%2d,%2c,%2d\t", aux[j][i].estado, aux[j][i].alfa, aux[j][i].comando);

        i++;
        if (i == T_alfabeto){
            printf("\n");
            i = 0;
            j++;
        }
    }

    fclose(arq);
    return aux;
}

// Fun��o principal
int main(){
    int cabecote;
    int estAtual;
    int alfaAtual;
    int comandoAtual;
    char entrada[255];

    short alfabeto[127];
    tupla **tuplas = NULL;
    tuplas = parser(alfabeto);

    // Verifica se houve erro na leitura do arquivo de configura��o
    if (tuplas == 0){
        printf("ERRO\n");
        exit(1);
    }

    printf("INICIO: %s\n", entrada);

    // Loop para processar entradas at� o final do arquivo
    while (scanf("%s", &entrada) != EOF){
        cabecote = 0;
        estAtual = 0;
        alfaAtual = 0;
        comandoAtual = 0;

        // Loop principal da m�quina de Turing
        do{
            alfaAtual = entrada[cabecote];
            comandoAtual = tuplas[estAtual][alfabeto[alfaAtual]].comando;
            entrada[cabecote] = tuplas[estAtual][alfabeto[alfaAtual]].alfa;

            // Execu��o dos comandos da m�quina de Turing
            switch (tuplas[estAtual][alfabeto[alfaAtual]].comando){
            case (R):
                cabecote++;
                break;
            case (L):
                cabecote--;
                break;
            case (P):
                comandoAtual = P;
                printf("RECONHECIDO ");
                break;
            default:
                comandoAtual = P;
                printf("ERROR");
                break;
            }
            estAtual = tuplas[estAtual][alfabeto[alfaAtual]].estado;

            printf("%s\n", entrada);
        } while (comandoAtual != P);
    }

    // Libera a mem�ria alocada para a tabela de transi��o
    free(tuplas);
    return 0;
}
