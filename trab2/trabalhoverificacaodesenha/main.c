#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_SENHA 50

// Função para ler senha mascarada
void lerSenha(char *senha, int max) {
    struct termios oldt, newt;
    int i = 0;
    char c;

    // Desabilita echo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        c = getchar();
        if (c == '\n') { // Enter
            senha[i] = '\0';
            break;
        } else if (c == 127 || c == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (i < max - 1) {
            senha[i++] = c;
            printf("*");
        }
    }

    printf("\n");
    // Restaura terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main() {
    char senhaCorreta[MAX_SENHA];
    char senhaUsuario[MAX_SENHA];
    FILE *arquivo;

    arquivo = fopen("senha.txt", "r");
    if (arquivo == NULL) {
        printf("Erro: arquivo 'senha.txt' nao encontrado!\n");
        return 1;
    }

    if (fgets(senhaCorreta, sizeof(senhaCorreta), arquivo) == NULL) {
        printf("Erro ao ler a senha do arquivo.\n");
        fclose(arquivo);
        return 1;
    }
    fclose(arquivo);
    senhaCorreta[strcspn(senhaCorreta, "\r\n")] = '\0';

    printf("Digite a senha: ");
    lerSenha(senhaUsuario, MAX_SENHA);

    if (strcmp(senhaUsuario, senhaCorreta) == 0) {
        printf("Senha correta! Acesso permitido.\n");
    } else {
        printf("Senha incorreta! Acesso negado.\n");
    }

    return 0;
}
