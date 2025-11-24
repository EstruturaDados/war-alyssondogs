/*
 * Cadastro de Territórios
 * Autor: (coloque seu nome)
 * Descrição: Exemplo didático de uso de struct para armazenar dados relacionados.
 * Requisitos atendidos:
 *  - struct Territorio com nome, cor e tropas
 *  - cadastro de 5 territórios (entrada guiada)
 *  - exibição imediata após o cadastro
 *  - usabilidade (mensagens claras), manutenibilidade (nomes claros), comentários
 *  - desempenho: execução instantânea (< 2s)
 */

#include <stdio.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_COR  10
#define NUM_TERRITORIOS 5

/* 
 * Estrutura que agrupa os dados de um território.
 * - nome: até 29 caracteres (+1 do '\0')
 * - cor:  até 9 caracteres (+1 do '\0')
 * - tropas: quantidade de tropas no território
 */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int  tropas;
} Territorio;

/*
 * Função auxiliar para ler uma linha com scanf usando "scanset".
 * Usamos " %29[^\n]" para permitir espaços (até o '\n') e evitar overflow.
 * O espaço inicial consome quebras de linha pendentes.
 */
void lerLinha(char *destino, int tamanho) {
    // tamanho aqui é apenas para documentação; o limite real é no formato do scanf
    // Ex.: para nome usamos %29[^\n] e para cor %9[^\n]
    (void)tamanho; // evita aviso de variável não usada
}

/*
 * Lê um território do usuário.
 * Usa scanf com "scanset" para strings e %d para o número de tropas.
 */
void lerTerritorio(Territorio *t, int indice) {
    printf("\n== Cadastro do Território %d ==\n", indice + 1);

    // Nome (até 29 chars, aceita espaços)
    printf("Digite o NOME do território: ");
    scanf(" %29[^\n]", t->nome);  // lê até 29 chars ou até '\n'

    // Cor (até 9 chars, aceita espaços curtos, ex.: \"verde claro\")
    printf("Digite a COR do exército: ");
    scanf(" %9[^\n]", t->cor);    // lê até 9 chars ou até '\n'

    // Tropas (inteiro)
    printf("Digite a QUANTIDADE de tropas: ");
    while (scanf("%d", &t->tropas) != 1) {
        // Limpa entrada inválida
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Valor inválido. Digite um número inteiro para as tropas: ");
    }

    // Limpa o '\n' que pode ter sobrado após o número
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*
 * Exibe todos os territórios de forma formatada.
 */
void exibirTerritorios(const Territorio vet[], int n) {
    printf("\n================= RELATÓRIO DE TERRITÓRIOS =================\n");
    for (int i = 0; i < n; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome:   %s\n", vet[i].nome);
        printf("  Cor:    %s\n", vet[i].cor);
        printf("  Tropas: %d\n", vet[i].tropas);
        printf("-------------------------------------------------------------\n");
    }
}

int main(void) {
    // Vetor para armazenar os 5 territórios
    Territorio territorios[NUM_TERRITORIOS];

    // Entrada de dados (laço for para preencher os 5)
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        lerTerritorio(&territorios[i], i);
    }

    // Exibição logo após o cadastro
    exibirTerritorios(territorios, NUM_TERRITORIOS);

    return 0;
}
