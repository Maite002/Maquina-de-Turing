#include <stdlib.h>
#include <stdio.h>
#define R 82
#define L 76
#define P 80

// Definição de uma estrutura "tupla", que representa os elementos da tabela de transição da máquina de Turing.
typedef struct{
    int estado;  // Estado de transição.
    char alfa;   // Símbolo do alfabeto lido.
    char comando; // Comando a ser executado (Direita, Esquerda e Parar).
} tupla;

// Função parser responsável por ler a configuração da máquina de um arquivo e construir a tabela de transição.
tupla** parser(short *alf){
    FILE *arq;
    int i = 0;
    int j = 0;
    tupla **aux;
    int T_alfabeto = 3; // Tamanho do alfabeto padrão (3 no exemplo).
    int T_estados = 0;  // Número de estados.
    char c;

    int T;

    // Abre um arquivo de texto para leitura
    arq = fopen("config.txt", "r");
    if (arq == NULL) // Código para caso exista algum erro na abertura do arquivo
    {
        printf("Problemas na abertura do arquivo\n");
        exit(-1);
    }

    // Lê o número de estados e o tamanho do alfabeto a partir do arquivo.
    fscanf(arq, "%d %d\n", &T_estados, &T_alfabeto);
    printf("%d %d\n", T_alfabeto, T_estados);
    printf("\n");

    // Lê os símbolos do alfabeto e os mapeia para índices em um vetor.
    do{
        fscanf(arq, "%c ", &c);
        printf("%7c%9c", c, ' ');
        alf[(int)c] = i;
        i++;
        if (i > T_alfabeto){
            printf("BRANCO DE FITA NÃO ENCONTRADO\n");
            exit(-1);
        }
    } while (c != '~');

    // Alocação de memória para a tabela de transição.
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

    // Lê os elementos da tabela de transição do arquivo.
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

// Função principal
int main(){
    int cabecote;
    int estAtual;
    int alfaAtual;
    int comandoAtual;
    char entrada[255];

    short alfabeto[127];
    tupla **tuplas = NULL;
    tuplas = parser(alfabeto);

    // Verifica se houve erro na leitura do arquivo de configuração
    if (tuplas == 0){
        printf("ERRO\n");
        exit(1);
    }

    printf("INICIO: %s\n", entrada);

    // Loop para processar entradas até o final do arquivo
    while (scanf("%s", &entrada) != EOF){
        cabecote = 0;
        estAtual = 0;
        alfaAtual = 0;
        comandoAtual = 0;

        // Loop principal da máquina de Turing
        do{
            alfaAtual = entrada[cabecote];
            comandoAtual = tuplas[estAtual][alfabeto[alfaAtual]].comando;
            entrada[cabecote] = tuplas[estAtual][alfabeto[alfaAtual]].alfa;

            // Execução dos comandos da máquina de Turing
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

    // Libera a memória alocada para a tabela de transição
    free(tuplas);
    return 0;
}
