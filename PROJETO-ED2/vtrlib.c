///Arquivo para salvar funções auxiliares e de leitura ou de salvamento de arquivo.

#define MAX_TEXT_SIZE 50000 /// Tamanho máximo do texto
#define MAX_HELP_TEXT_SIZE 1000 /// Tamanho máximo do texto de ajuda

/// Estrutura para armazenar a posição X e Y do cursor
typedef struct
{
    int x; /// representa a coluna
    int y; /// representa a linha
} Cursor;

/// Função para mover o cursor para a posição (x, y) na tela de execução
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

///Função ao pressionar a tecla f1
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

///Função da tecla F12
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

/// Exibir arquivo salvo no arquivo .txt - F10
void exibirArquivoSalvo(char *texto, Cursor *cursor)
{
    FILE *arquivoInfo = fopen("VitorSouza_texto.txt", "r");
    system("cls"); // Limpa a tela
    if (arquivoInfo == NULL)
    {
        perror("Erro ao abrir o arquivo de informacoes");
        return;
    }

    if(strlen(texto) > 0){
        memset(texto, 0, sizeof(texto));
       // strcpy(texto, "");
    }

    char linha[MAX_TEXT_SIZE];

    while (fgets(linha, sizeof(linha), arquivoInfo) != NULL)
    {
        /// Verifica se há espaço suficiente no buffer antes de concatenar
        if ((strlen(texto) + strlen(linha)) < MAX_TEXT_SIZE - 1)
        {
            /// Concatena a linha ao texto existente
           strcat(texto, linha);
        }
    }

    cursor->x = 0;
    cursor->y = 0;
    printf("%s", texto);
    gotoxy(cursor->x, cursor->y);
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

///Função auxiliar que calcula a quantidade de linhas do texto
int contarQuebrasDeLinha(char *texto) {
    int contador = 0;
    int tamanho = strlen(texto);
    for (int i = 0; i < tamanho; i++) {
        if (texto[i] == '\n') {
            contador++;
        }
    }


    return contador;
}

/// Função que calcula a quantidade de caracteres na linha do cursor
int calcularTamanhoTexto(Cursor *cursor, char *texto) {
    int tamanho = 0;
    int linhaAtual = 0; /// Variável para acompanhar a linha atual
    int i = 0;
    int tamText = strlen(texto);

    while (texto[i] != '\0' && linhaAtual < cursor->y) {
        if (texto[i] == '\n') {
            linhaAtual++;
        }
        i++;
    }

    /// Contar os caracteres na linha atual até a posição do cursor
    while (texto[i] != '\n' && texto[i] != '\0') {
        tamanho++;
        i++;
    }

    return tamanho;
}

/// Função que calcula a quantidade de caracteres até a linha do cursor
int qtdCaracterAteCursor(Cursor *cursor, char *texto) {
    int tamanho = 0;
    int linhaAtual = 0; /// Variável para acompanhar a linha atual
    int i = 0;
    int tamText = strlen(texto);

    /// Contar os caracteres até a linha do cursor
    while (texto[i] != '\0' && linhaAtual < cursor->y) {
        if (texto[i] == '\n') {
            linhaAtual++;
        }
        tamanho++;
        i++;
    }

    return tamanho + cursor->x;
}

///Função auxiliar para os caracteres na linha do cursor
int contarCaracteresNaLinhaDoCursor(Cursor *cursor, char *texto) {
    int contador = 0;
    int i = 0;

    /// Encontrar a posição inicial da linha do cursor
    int posicaoInicioLinha = 0;
    while (i < cursor->y && texto[posicaoInicioLinha] != '\0') {
        if (texto[posicaoInicioLinha] == '\n') {
            i++;
        }
        posicaoInicioLinha++;
    }

    /// Contar os caracteres na linha do cursor
    while (texto[posicaoInicioLinha] != '\n' && texto[posicaoInicioLinha] != '\0') {
        contador++;
        posicaoInicioLinha++;
    }

    return contador;
}

///Função auxiliar que irá contar a quantidade de caracteres do inicio até a posição cursor
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

    /// Incrementa no contador a quantidade de linhas do texto
    contador+= cursor->y;

    return contador;
}




