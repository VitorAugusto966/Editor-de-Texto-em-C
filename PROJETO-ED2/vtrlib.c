#define MAX_TEXT_SIZE 100000 // Tamanho máximo do texto
#define MAX_HELP_TEXT_SIZE 1000 // Tamanho máximo do texto de ajuda

/// Estrutura para armazenar a posição X e Y do cursor
typedef struct
{
    int x;
    int y;
} Cursor;

void gotoxy(int x, int y)
{
    /// Função para mover o cursor para a posição (x, y) na tela de execução
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void exibirAjuda()
{
    system("cls"); /// Limpa a tela
    FILE *arquivoAjuda = fopen("VitorSouza_ajuda.txt", "r");
    if (arquivoAjuda == NULL)
    {
        printf("Erro ao abrir o arquivo de ajuda.\n");
        return;
    }

    char linha[MAX_HELP_TEXT_SIZE];
    while (fgets(linha, sizeof(linha), arquivoAjuda) != NULL)
    {
        printf("%s", linha);
    }

    fclose(arquivoAjuda);
}

void exibirInformacoes()
{
    system("cls"); /// Limpa a tela
    FILE *arquivoInfo = fopen("VitorSouza.txt", "r");
    if (arquivoInfo == NULL)
    {
        printf("Erro ao abrir o arquivo de informacoes.\n");
        return;
    }

    char linha[MAX_HELP_TEXT_SIZE];

    while (fgets(linha, sizeof(linha), arquivoInfo) != NULL)
    {
        printf("%s", linha);
    }

    fclose(arquivoInfo);

}

/// Exibir arquivo salvo no txt - F10
void exibirArquivoSalvo()
{
    system("cls"); /// Limpa a tela
    FILE *arquivoInfo = fopen("VitorSouza_texto.txt", "r");
    if (arquivoInfo == NULL)
    {
        printf("Erro ao abrir o arquivo de informacoes.\n");
        return;
    }

    char linha[MAX_HELP_TEXT_SIZE];

    while (fgets(linha, sizeof(linha), arquivoInfo) != NULL)
    {
        printf("%s", linha);
    }

    fclose(arquivoInfo);

}

/// Função para salvar o conteudo do vetor texto em um arquivo
void salvarTexto(char *texto)
{
    FILE *arquivo = fopen("VitorSouza_texto.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    fputs(texto, arquivo);
    fclose(arquivo);
}


int contarQuebrasDeLinha(char *texto) {
    int contador = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\n') {
            contador++;
        }
    }
    return contador;
}

/// Função que calcula a quantidade de caracteres na linha do cursor
int calcularTamanhoTexto(Cursor *cursor, char *texto) {
    int tamanho = 0;
    int linhaAtual = 0; // Variável para acompanhar a linha atual
    int i = 0;
    int tamText = strlen(texto);

    while (texto[i] != '\0' && linhaAtual < cursor->y) {
        if (texto[i] == '\n') {
            linhaAtual++;
        }
        i++;
    }

    // Contar os caracteres na linha atual até a posição do cursor
    while (texto[i] != '\n' && texto[i] != '\0') {
        tamanho++;
        i++;
    }

    //tamanho+=1;

    return tamanho;
}

/// Função que calcula a quantidade de caracteres até a linha do cursor
int qtdCaracterAteCursor(Cursor *cursor, char *texto) {
    int tamanho = 0;
    int linhaAtual = 0; // Variável para acompanhar a linha atual
    int i = 0;
    int tamText = strlen(texto);

    // Contar os caracteres até a linha do cursor
    while (texto[i] != '\0' && linhaAtual < cursor->y) {
        if (texto[i] == '\n') {
            linhaAtual++;
        }
        tamanho++;
        i++;
    }

    return tamanho + cursor->x;
}


int contarCaracteresAteCursor(Cursor *cursor, char *texto) {
    int contador = 0;
    int i = 0;
    Cursor aux;
    aux.y = i;
    aux.x = cursor->x;

    /// Contar os caracteres nas linhas anteriores
    while( i <= cursor->y){
        contador += contarCaracteresNaLinhaDoCursor(&aux, texto);
        i++;
        aux.y = i;
    }

    //contador+=1;
    /// Incrementa no contador a quantidade de linhas do texto
    contador+= cursor->y;

    ///verificar contador+= aux.y;
   // printf("Contador esta em =  %d ", contador);

    return contador;
}

int contarCaracteresNaLinhaDoCursor(Cursor *cursor, char *texto) {
    int contador = 0;
    int i = 0;

    // Encontrar a posição inicial da linha do cursor
    int posicaoInicioLinha = 0;
    while (i < cursor->y && texto[posicaoInicioLinha] != '\0') {
        if (texto[posicaoInicioLinha] == '\n') {
            i++;
        }
        posicaoInicioLinha++;
    }

    // Contar os caracteres na linha do cursor
    while (texto[posicaoInicioLinha] != '\n' && texto[posicaoInicioLinha] != '\0') {
        contador++;
        posicaoInicioLinha++;
    }

    return contador;
}



void exibirTextoSalvo(char *texto) {
    int tamanhoTexto = strlen(texto);

    printf("\n\n");
    for (int i = 0; i < tamanhoTexto; i++) {
        // Use i para mostrar a posição e texto[i] para mostrar o caractere
        printf("Posição %d: %c\n", i, texto[i]);
    }
}


