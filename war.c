/*
 * WAR Estruturado - Nivel Final: Ataque + Missoes
 * Autor: Alysson
 *
 * Neste programa sao usados:
 *  - struct Territorio (nome, cor, tropas)
 *  - alocacao dinamica para o vetor de territorios
 *  - funcao de ataque entre territorios com rand()
 *  - sistema de missoes estrategicas sorteadas em um vetor de strings
 *  - missao armazenada com malloc (memoria dinamica)
 *  - ponteiros para acessar e modificar os dados
 *  - liberacao da memoria no final com free()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR  10
#define TAM_MISSAO 200  /* tamanho maximo para armazenar a missao sorteada */

/* Estrutura que representa um territorio */
typedef struct {
    char nome[MAX_NOME];  // nome do territorio
    char cor[MAX_COR];    // cor do exercito que domina o territorio
    int tropas;           // quantidade de tropas
} Territorio;

/* ------------ Protótipos das funcoes ------------ */

/* territorios / mapa */
Territorio* alocarMapa(int quantidade);
void cadastrarTerritorios(Territorio *mapa, int quantidade);
void exibirTerritorios(const Territorio *mapa, int quantidade);
void atacar(Territorio *atacante, Territorio *defensor);

/* missoes */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMissao(const char *missao);
int verificarMissao(char *missao, Territorio *mapa, int tamanho);

/* utilitarios */
int escolherIndice(const Territorio *mapa, int quantidade, const char *mensagem);
void limparBufferEntrada(void);
void liberarMemoria(Territorio *mapa, char *missaoJogador);

/* ------------ Funcao principal ------------ */

int main(void) {
    int quantidade;
    Territorio *mapa = NULL;
    int opcao;
    char *missaoJogador = NULL;

    /* vetor de missoes pre-definidas (vetor de strings) */
    char *listaMissoes[] = {
        "Ter pelo menos 3 territorios com 5 ou mais tropas",
        "Ter pelo menos 2 territorios com 10 ou mais tropas",
        "Ter pelo menos 20 tropas no total",
        "Ter pelo menos 1 territorio com mais de 15 tropas",
        "Ter a soma das tropas maior ou igual a 30"
    };
    int totalMissoes = (int)(sizeof(listaMissoes) / sizeof(listaMissoes[0]));

    /* Inicializa gerador de numeros aleatorios */
    srand((unsigned int)time(NULL));

    printf("==== WAR ESTRUTURADO - NIVEL FINAL (ATAQUE + MISSAO) ====\n\n");

    /* Pergunta quantos territorios serao cadastrados */
    printf("Informe o numero de territorios: ");
    while (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        limparBufferEntrada();
        printf("Valor invalido. Digite um numero inteiro positivo: ");
    }
    limparBufferEntrada(); // limpa '\n'

    /* Alocacao dinamica do vetor de Territorio */
    mapa = alocarMapa(quantidade);
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        return 1;
    }

    /* Alocacao dinamica para armazenar a missao do jogador */
    missaoJogador = (char*)malloc(TAM_MISSAO);
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para a missao.\n");
        liberarMemoria(mapa, NULL);
        return 1;
    }

    /* Cadastro dos territorios */
    cadastrarTerritorios(mapa, quantidade);

    /* Sorteia e atribui a missao ao jogador (mostra apenas uma vez) */
    atribuirMissao(missaoJogador, listaMissoes, totalMissoes);
    exibirMissao(missaoJogador);

    /* Loop de interacao com o usuario */
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

            /* Escolha do atacante */
            idxAtacante = escolherIndice(mapa, quantidade,
                                         "Digite o indice do TERRITORIO ATACANTE: ");

            /* Escolha do defensor */
            idxDefensor = escolherIndice(mapa, quantidade,
                                         "Digite o indice do TERRITORIO DEFENSOR: ");

            /* Validacoes basicas */
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

            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
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

        /* Verifica silenciosamente se a missao foi cumprida ao final do turno */
        if (opcao != 0) {
            if (verificarMissao(missaoJogador, mapa, quantidade)) {
                printf("\n===========================================\n");
                printf(" PARABENS! Voce cumpriu a sua missao!\n");
                printf(" Missao: %s\n", missaoJogador);
                printf(" Voce VENCEU o jogo.\n");
                printf("===========================================\n");
                break;  // sai do loop
            }
        }

    } while (opcao != 0);

    /* Libera toda a memoria alocada dinamicamente */
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

/* ------------ Implementacao das funcoes ------------ */

/* Aloca dinamicamente um vetor de Territorio usando calloc */
Territorio* alocarMapa(int quantidade) {
    Territorio *mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    return mapa;
}

/* Faz o cadastro dos territorios: nome, cor e tropas */
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

/* Exibe todos os territorios cadastrados */
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

/* Funcao de ataque entre dois territorios (usa rand() para dados de 1 a 6) */
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante, dadoDefensor;

    printf("\n=== FASE DE ATAQUE ===\n");
    printf("Atacante: %s (%s) - Tropas: %d\n",
           atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (%s) - Tropas: %d\n",
           defensor->nome, defensor->cor, defensor->tropas);

    dadoAtacante = (rand() % 6) + 1;
    dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: Ataque BEM-SUCEDIDO! O atacante CONQUISTOU o territorio.\n");

        /* Transfere a cor e metade das tropas do atacante para o defensor */
        int tropasMovidas;

        if (atacante->tropas < 2) {
            tropasMovidas = 1;
        } else {
            tropasMovidas = atacante->tropas / 2;
        }

        atacante->tropas -= tropasMovidas;
        if (atacante->tropas < 1) {
            atacante->tropas = 1;  // garante pelo menos 1 tropa
        }

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = tropasMovidas;

    } else {
        printf("Resultado: Ataque FRACASSADO! O defensor resistiu.\n");
        if (atacante->tropas > 0) {
            atacante->tropas--;
        }
    }
}

/* Sorteia uma missao do vetor e copia para 'destino' usando strcpy */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/* Exibe a missao do jogador (somente no inicio do jogo) */
void exibirMissao(const char *missao) {
    printf("\n=== SUA MISSAO ESTRATEGICA ===\n");
    printf("%s\n", missao);
    printf("(Ela sera verificada automaticamente ao longo do jogo.)\n\n");
}

/*
 * Verifica se a missao foi cumprida.
 * Aqui usamos uma logica simples, baseada no texto da missao:
 * contamos tropas e territorios e comparamos com o que a missao pede.
 */
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    int i;
    int totalTropas = 0;
    int territCom5ouMais = 0;
    int territCom10ouMais = 0;
    int territComMaisDe15 = 0;

    for (i = 0; i < tamanho; i++) {
        totalTropas += mapa[i].tropas;

        if (mapa[i].tropas >= 5) {
            territCom5ouMais++;
        }
        if (mapa[i].tropas >= 10) {
            territCom10ouMais++;
        }
        if (mapa[i].tropas > 15) {
            territComMaisDe15++;
        }
    }

    /* Checamos qual missao e pelo texto (simples, mas suficiente para o exercicio) */
    if (strstr(missao, "3 territorios com 5 ou mais tropas") != NULL) {
        if (territCom5ouMais >= 3) {
            return 1;
        }
    } else if (strstr(missao, "2 territorios com 10 ou mais tropas") != NULL) {
        if (territCom10ouMais >= 2) {
            return 1;
        }
    } else if (strstr(missao, "20 tropas no total") != NULL) {
        if (totalTropas >= 20) {
            return 1;
        }
    } else if (strstr(missao, "1 territorio com mais de 15 tropas") != NULL) {
        if (territComMaisDe15 >= 1) {
            return 1;
        }
    } else if (strstr(missao, "tropas maior ou igual a 30") != NULL) {
        if (totalTropas >= 30) {
            return 1;
        }
    }

    return 0;  // ainda nao cumpriu
}

/* Escolhe um indice valido de territorio */
int escolherIndice(const Territorio *mapa, int quantidade, const char *mensagem) {
    int indice;
    (void)mapa; // nao usamos diretamente aqui, mas poderia ser usado para mais validacoes

    printf("\n%s", mensagem);
    while (scanf("%d", &indice) != 1 || indice < 0 || indice >= quantidade) {
        limparBufferEntrada();
        printf("Indice invalido. Digite um valor entre 0 e %d: ", quantidade - 1);
    }
    limparBufferEntrada();
    return indice;
}

/* Limpa o buffer de entrada (stdin) */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres restantes */
    }
}

/* Libera toda a memoria dinamica usada no programa */
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);
    if (missaoJogador != NULL) {
        free(missaoJogador);
    }
}
