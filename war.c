/*
 * WAR Estruturado - Nível: Interatividade e Ataque
 * Autor: (seu nome)
 *
 * Objetivo:
 *  - Utilizar struct Territorio.
 *  - Alocar dinamicamente um vetor de Territorio.
 *  - Permitir ataques entre territórios usando rand() (dado 1 a 6).
 *  - Atualizar cor e tropas do território defensor em caso de vitória.
 *  - Usar ponteiros para acessar/modificar os dados.
 *  - Liberar a memória ao final com free().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR  10

/* Estrutura que representa um território */
typedef struct {
    char nome[MAX_NOME];  // nome do território
    char cor[MAX_COR];    // cor do exército que domina o território
    int tropas;           // quantidade de tropas
} Territorio;

/* ------------ Protótipos das funções ------------ */

Territorio* alocarMapa(int quantidade);
void cadastrarTerritorios(Territorio *mapa, int quantidade);
void exibirTerritorios(const Territorio *mapa, int quantidade);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa);

int escolherIndice(const Territorio *mapa, int quantidade, const char *mensagem);
void limparBufferEntrada(void);

/* ------------ Função principal ------------ */

int main(void) {
    int quantidade;
    Territorio *mapa = NULL;
    int opcao;

    /* Inicializa gerador de números aleatórios para os dados */
    srand((unsigned int)time(NULL));

    printf("==== WAR ESTRUTURADO - NIVEL ATAQUE ====\n\n");

    /* Pergunta quantos territórios serão cadastrados */
    printf("Informe o numero de territorios: ");
    while (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        limparBufferEntrada();
        printf("Valor invalido. Digite um numero inteiro positivo: ");
    }
    limparBufferEntrada(); // limpa \n

    /* Alocação dinâmica do vetor de Territorio */
    mapa = alocarMapa(quantidade);
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        return 1;
    }

    /* Cadastro dos territórios */
    cadastrarTerritorios(mapa, quantidade);

    /* Loop de interação com o usuário */
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada();

        switch (opcao) {
        case 1:
            exibirTerritorios(mapa, quantidade);
            break;

        case 2: {
            int idxAtacante, idxDefensor;

            exibirTerritorios(mapa, quantidade);

            /* Escolhe o território atacante */
            idxAtacante = escolherIndice(mapa, quantidade,
                                         "Digite o indice do TERRITORIO ATACANTE: ");

            /* Escolhe o território defensor */
            idxDefensor = escolherIndice(mapa, quantidade,
                                         "Digite o indice do TERRITORIO DEFENSOR: ");

            /* Validações:
               - não pode atacar ele mesmo
               - não pode atacar território da mesma cor
            */
            if (idxAtacante == idxDefensor) {
                printf("Um territorio nao pode atacar a si mesmo!\n");
                break;
            }
            if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
                printf("Nao e permitido atacar um territorio da mesma cor!\n");
                break;
            }
            if (mapa[idxAtacante].tropas < 1) {
                printf("O territorio atacante nao possui tropas suficientes!\n");
                break;
            }

            /* Realiza o ataque usando ponteiros */
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

            /* Exibe situação após o ataque */
            exibirTerritorios(mapa, quantidade);
            break;
        }

        case 0:
            printf("Saindo do jogo...\n");
            break;

        default:
            printf("Opcao invalida.\n");
            break;
        }

    } while (opcao != 0);

    /* Libera a memória alocada */
    liberarMemoria(mapa);

    return 0;
}

/* ------------ Implementação das funções ------------ */

/*
 * Aloca dinamicamente um vetor de Territorio usando calloc.
 * Retorna o ponteiro para o primeiro elemento ou NULL em caso de erro.
 */
Territorio* alocarMapa(int quantidade) {
    Territorio *mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    return mapa;
}

/*
 * Faz o cadastro dos territórios: nome, cor e tropas.
 * Usa ponteiro para acessar e modificar diretamente o vetor.
 */
void cadastrarTerritorios(Territorio *mapa, int quantidade) {
    int i;
    char buffer[128];

    printf("\n=== CADASTRO DOS TERRITORIOS ===\n");

    for (i = 0; i < quantidade; i++) {
        printf("\nTerritorio %d:\n", i);

        printf("Nome: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            buffer[0] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';  // remove '\n'
        strncpy(mapa[i].nome, buffer, MAX_NOME - 1);
        mapa[i].nome[MAX_NOME - 1] = '\0';

        printf("Cor do exercito: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            buffer[0] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        strncpy(mapa[i].cor, buffer, MAX_COR - 1);
        mapa[i].cor[MAX_COR - 1] = '\0';

        printf("Quantidade de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            limparBufferEntrada();
            printf("Valor invalido. Digite um numero inteiro >= 0: ");
        }
        limparBufferEntrada();
    }
}

/*
 * Exibe todos os territórios cadastrados.
 * Parâmetro 'mapa' é const, pois aqui só lemos os dados.
 */
void exibirTerritorios(const Territorio *mapa, int quantidade) {
    int i;

    printf("\n=== ESTADO ATUAL DOS TERRITORIOS ===\n");
    printf("Indice | %-28s | %-8s | Tropas\n", "Nome", "Cor");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < quantidade; i++) {
        printf("%5d | %-28s | %-8s | %6d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/*
 * Função de ataque entre dois territórios.
 * Usa rand() para gerar um valor de 1 a 6 para atacante e defensor.
 * Regras simples:
 *  - Se dadoAtacante > dadoDefensor:
 *      * defensor muda de cor para a cor do atacante.
 *      * metade das tropas do atacante se move para o defensor.
 *  - Senao:
 *      * atacante perde uma tropa (se tiver pelo menos 1).
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante, dadoDefensor;

    printf("\n=== FASE DE ATAQUE ===\n");
    printf("Atacante: %s (%s) - Tropas: %d\n",
           atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) - Tropas: %d\n",
           defensor->nome, defensor->cor, defensor->tropas);

    /* Simula rolagem de dados (1 a 6) */
    dadoAtacante = (rand() % 6) + 1;
    dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: Ataque BEM-SUCEDIDO! O atacante CONQUISTOU o territorio.\n");

        /* Transfere a cor e metade das tropas do atacante para o defensor */
        int tropasMovidas;

        if (atacante->tropas < 2) {
            /* Regra de segurança: precisa ter pelo menos 2 tropas para mover */
            tropasMovidas = 1;
        } else {
            tropasMovidas = atacante->tropas / 2;  // metade das tropas
        }

        atacante->tropas -= tropasMovidas;
        if (atacante->tropas < 1) {
            atacante->tropas = 1; // garante pelo menos 1 tropa no território de origem
        }

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasMovidas;

    } else {
        printf("Resultado: Ataque FRACASSADO! O defensor resistiu.\n");

        /* Atacante perde uma tropa, se tiver */
        if (atacante->tropas > 0) {
            atacante->tropas--;
        }
    }
}

/*
 * Libera a memória alocada para o vetor de territórios.
 */
void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

/*
 * Função auxiliar para escolher um índice de território válido.
 */
int escolherIndice(const Territorio *mapa, int quantidade, const char *mensagem) {
    int indice;

    (void)mapa; // não usamos diretamente aqui, mas poderíamos validar mais coisas
    printf("\n%s", mensagem);
    while (scanf("%d", &indice) != 1 || indice < 0 || indice >= quantidade) {
        limparBufferEntrada();
        printf("Indice invalido. Digite um valor entre 0 e %d: ", quantidade - 1);
    }
    limparBufferEntrada();
    return indice;
}

/*
 * Limpa o buffer de entrada (stdin),
 * usado para evitar problemas de leitura com scanf/fgets.
 */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres restantes */
    }
}
