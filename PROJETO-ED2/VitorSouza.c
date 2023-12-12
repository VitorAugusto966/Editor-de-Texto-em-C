#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include "vtrlib.c"
#define MAX_TEXT_SIZE 30000 /// Tamanho máximo do texto
#define MAX_HELP_TEXT_SIZE 1000


void inserirCaracter(char *texto, Cursor *cursor, char caracter)
{
    int tamanhoTexto = strlen(texto) + 1;
    int tamanhoAnterior = strlen(texto);
    int posicao;

    /// Verifica se o tamanho atual do texto não excede o máximo
    if (tamanhoTexto < MAX_TEXT_SIZE - 1)
    {
        /// Calcula o número de colunas disponíveis na linha do cursor
        int colunasNaLinha = calcularTamanhoTexto(cursor, texto);

        /// Verifica se há espaço suficiente na linha para inserir um novo caractere
        if (colunasNaLinha < 80)
        {
            /// Caso o cursor esteja na linha 1 ele irá inserir com base na posição x do cursor
            if (cursor->y == 0)
            {
                for (int i = tamanhoTexto + 1; i > cursor->x; i--)
                {
                    texto[i] = texto[i - 1];
                }
                texto[cursor->x] = caracter;
                (cursor->x)++;
            }
            else
            {
                /// Se não ele irá procurar a posição do cursor no texto
                posicao = qtdCaracterAteCursor(cursor, texto);

                ///Dado a posição ele irá remanejar os caracteres
                for (int i = tamanhoTexto + 1; i > posicao; i--)
                {
                    texto[i] = texto[i - 1];
                }
                texto[posicao] = caracter;
                (cursor->x)++;
            }

            /// Atualiza o tamanho do texto após a inserção
            tamanhoTexto = strlen(texto);

            /// Indicação de fim do texto
            texto[tamanhoTexto + 1] = '\0';

            /// Caso não tenha sido sobrescrito algum caracter
            /// Ele irá apenas imprimir o novo caracter na linha
            if(posicao== tamanhoTexto-1)
            {
                printf("%c",caracter);
            }
            else
            {
                ///Caso tenha sobrescrito irá limpar a tela e apresentar novamente
                system("cls");
                printf("%s", texto);
            }

            /// Mantém o cursor na mesma coluna
            gotoxy(cursor->x, cursor->y);
        }

        ///Caso o numero de colunas esteja no limite máximo
        /// ele irá adicionar o novo caracter no inicio da próxima linha
        else
        {
            tamanhoTexto = strlen(texto);
            cursor->y = cursor->y+1;
            cursor->x = 0;
            texto[tamanhoTexto] = '\n';
            texto[tamanhoTexto+1] = caracter;
            system("cls");
            printf("%s", texto);
            gotoxy(cursor->x, cursor->y);
        }
    }
}


/// Protótipo da função tratarCaracterEspecial para a função insert reconhece-la
/// Serve para resolver o erro de conflito de tipo
int tratarCaracterEspecial(char tecla, char *texto, Cursor *cursor, int op, int *teclaAnterior);

///Funcionalidade da tecla insert
void funcInsert(char *texto, Cursor *cursor)
{
    int posicao;
    int insert = 0;
    int *teclaAnterior = 0;
    char tecla;

    /// Enquanto a tecla insert não for pressionada novamente
    /// Continuará o processo de sobrescrever os caracter na posição do cursor
    /// e permitindo utilizar as teclas especiais
    while (insert != 1)
    {
        tecla = _getch();

        ///Verifica se é uma tecla inserivel não especial
        if ((tecla >= 32 && tecla <= 126) && tecla != 75 && tecla != 77)
        {
            posicao = qtdCaracterAteCursor(cursor, texto) ;
            texto[posicao] = tecla;
            system("cls");
            printf("%s", texto);
            gotoxy(cursor->x, cursor->y);
        }
        ///Caso seja uma tecla especial irá chamar a função que trata os caracteres especiais
        else
        {
            insert = tratarCaracterEspecial(tecla,texto,cursor,2, teclaAnterior);
        }
    }

}

///Funcionalidade da tecla de seta esquerda
void moverCursorEsquerda(Cursor *cursor, char *texto)
{
    int tamanhoTexto;

    /// Caso o cursor não esteja no inicio da linha ele apenas decrementa
    /// em uma posição o valor do cursor->x
    if (cursor->x > 0)
    {
        (cursor->x)--;
        gotoxy(cursor->x, cursor->y);
    }

    /// Caso esteja no inicio da linha e não for a primeira linha do texto
    /// ele irá decrementar a linha em uma posição e irá pro fim da linha anterior
    else if(cursor->x == 0 && cursor->y > 0)
    {
        (cursor->y)--;
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        cursor->x = tamanhoTexto;
        gotoxy(cursor->x, cursor->y);
    }

}

///Funcionalidade da tecla de seta direita
void moverCursorDireita(Cursor *cursor, char *texto)
{
    int x = cursor->x;
    int y = cursor->y;
    int tamanhoLinha = calcularTamanhoTexto(cursor, texto);
    int qtdLinhas = contarQuebrasDeLinha(texto);

    /// Recebe as posições originais do cursor que foram alteradas na função de calcular
    /// o tamanho do texto
    cursor->y = y;
    cursor->x = x;

    /// Se a posição do cursor for menor que a quantidade de caracteres que tem na linha
    /// e a linha atual for menor ou igual a quantidade de linhas no texto
    if(cursor->x < tamanhoLinha && cursor->y <= qtdLinhas)
    {
        /// ele adiciona uma posição ao cursor->x que fará o cursor ir para a direita
        cursor->x++;
    }

    /// Caso o cursor esteja no fim da linha e a linha que o cursor estiver for menor
    /// que a quantidade de linhas disponiveis
    else if(cursor->x == tamanhoLinha && cursor->y < qtdLinhas)
    {
        /// o cursor-> x é atualizado pro inicio da linha
        /// e o cursor->y é atualizado para a proxima linha
        cursor->x = 0;
        cursor->y++;
    }

    ///Atualiza o cursor pra posição definida
    gotoxy(cursor->x, cursor->y);
}

///Funcionalidade da tecla de seta para cima
void rolarParaCima(Cursor *cursor, char *texto)
{
    int qtdLinhas = contarQuebrasDeLinha(texto);
    int tamanhoTexto, tamanhoTextoAnterior;
    Cursor aux;

    if (cursor->y > 0)
    {
        /// Decrementa o valor do cursor para a linha anterior
        (cursor->y)--;

        /// Cursor auxiliar para calcular o tamanho da linha seguinte
        aux.y = cursor->y + 1;
        aux.x = cursor->x;

        /// Calcular o tamanho do texto da linha atual
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        /// Calcular o tamanho do texto da linha seguinte
        tamanhoTextoAnterior = calcularTamanhoTexto(&aux, texto);

        /// Se o valor atual do cursor na posição x for maior que a quantidade de caracteres da linha anterior
        if (cursor->x > tamanhoTexto)
        {
            /// Cursor na posição x irá receber a quantidade de caracteres da linha anterior
            cursor->x = tamanhoTexto;
        }

        /// Atualiza o cursor para a devida posição
        gotoxy(cursor->x, cursor->y);
    }
    else
    {
        /// Cursor na posiçao y esta no início do texto, não podemos rolar mais para cima
        gotoxy(cursor->x, cursor->y);
    }
}


///Funcionalidade da tecla de seta para baixo
void rolarParaBaixo(Cursor *cursor, char *texto)
{
    int qtdLinhas = contarQuebrasDeLinha(texto);
    int tamanhoTexto, tamanhoTextoAnterior;
    Cursor aux;

    /// Caso a linha que o cursor estiver for menor que a quantidade de linhas que o texto possui
    if (cursor->y < qtdLinhas)
    {
        ///Incrementa o valor do cursor para a próxima linha
        (cursor->y)++;

        ///Cursor auxiliar para o calculo do tamanho da linha anterior
        aux.y = cursor->y - 1;
        aux.x = cursor->x;

        ///Calcular o tamanho do texto da proxima linha
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        ///Calcula o tamanho do texto da linha anterior
        tamanhoTextoAnterior = calcularTamanhoTexto(&aux, texto);

        ///Se o valor atual do cursor na posição x for maior que a quantidade de caracteres da proxima linha
        if(cursor->x > tamanhoTexto)
        {
            ///cursor na posição x irá receber a quantidade de caracteres da proxima linha
            cursor->x = tamanhoTexto;
        }

        ///Atualiza o cursor pra devida posição
        gotoxy(cursor->x, cursor->y);
    }

    /// Caso esteja no final do texto, não podemos rolar mais para baixo
    else
    {
        ///Mantem o cursor na posição atual
        gotoxy(cursor->x, cursor->y);
    }
}


/// Função para posicionar o cursor no início da linha atual
void funcHome(Cursor *cursor, char *texto)
{
    ///define o cursor x para posição 0 e mantem o y na linha atual
    cursor->x = 0;
    gotoxy(cursor->x, cursor->y);
}

/// Função para posicionar o cursor no final da linha atual
void funcEnd(Cursor *cursor, char *texto)
{
    /// recebe a quantidade de caracteres presente na linha atual
    int posicao = contarCaracteresNaLinhaDoCursor(cursor,texto);

    ///cursor recebe a quantidade de caracteres na linha + 1 para poder ir pro fim da linha;
    cursor->x = posicao+1;

    ///Atualiza o cursor para a posição
    gotoxy(cursor->x, cursor->y);
}

/// TECNICAMENTE CORRETO
void teclaEnter(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y; ///salvar a posição Y do cursor antes da varredura

    /// Encontre a posição do cursor->y-1 no texto
    int posicaoQuebraAnterior = -1;
    for (int i = 0; i < tamanhoTexto; i++)
    {
        if (texto[i] == '\n')
        {
            posicaoQuebraAnterior = i;
            if (cursor->y == 0)
            {
                break; /// Se cursor->y for 0, não é necessário continuar procurando
            }
            cursor->y--;
        }
    }

    /// Calcule a posição para inserir a quebra de linha com base em cursor->x
    int posicaoInsercao = posicaoQuebraAnterior + cursor->x + 1;

    /// Desloca o texto subsequente uma posição para frente
    for (int i = tamanhoTexto; i >= posicaoInsercao; i--)
    {
        texto[i + 1] = texto[i];
    }

    /// Insira a quebra de linha na posição correta
    texto[posicaoInsercao] = '\n';

    /// Atualize o cursor para a nova linha
    cursor->y = y;
    cursor->y++;
    cursor->x = 0;

    /// Atualize a tela com o novo texto
    system("cls");
    printf("%s", texto);

    ///Atualiza o cursor para posição correta
    gotoxy(cursor->x, cursor->y);
}


void deletarCaracter(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y;

    /// Encontre a posição do cursor->y-1 no texto
    int posicaoQuebraAnterior = -1;
    for (int i = 0; i < tamanhoTexto; i++)
    {
        if (texto[i] == '\n')
        {
            posicaoQuebraAnterior = i;
            if (cursor->y == 0)
            {
                break; /// Se cursor->y for 0, não é necessário continuar procurando
            }
            cursor->y--;
        }
    }

    /// Calcule a posição
    int posicaoRemocao = posicaoQuebraAnterior + cursor->x+1;

    /// Move todos os caracteres após a posição de remoção uma posição para trás
    for (int i = posicaoRemocao; i < tamanhoTexto; i++)
    {
        texto[i] = texto[i + 1];
    }

    /// Atualize a tela com o novo texto
    cursor->y = y;
    system("cls");
    printf("%s", texto);

    /// Atualiza o cursor na tela
    gotoxy(cursor->x, cursor->y);
}



void backspace(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y;

    /// Encontre a posição do cursor->y-1 no texto
    int posicaoQuebraAnterior = -1;

    if(tamanhoTexto > 0)
    {
        for (int i = 0; i < tamanhoTexto; i++)
        {
            if (texto[i] == '\n')
            {
                posicaoQuebraAnterior = i;
                if (cursor->y == 0)
                {
                    break; /// Se cursor->y for 0, não é necessário continuar procurando
                }
                cursor->y--;
            }
        }

        /// Calcule a posição para remoção
        int posicaoRemocao = posicaoQuebraAnterior + cursor->x;

        /// Move todos os caracteres após a posição de remoção uma posição para trás
        for (int i = posicaoRemocao; i < tamanhoTexto; i++)
        {
            texto[i] = texto[i + 1];
        }

        /// Atualize a tela com o novo texto
        cursor->y = y;
        cursor->x = cursor->x-1;
        system("cls");
        printf("%s", texto);

        /// Atualiza o cursor na tela
        gotoxy(cursor->x, cursor->y);
    }

}

///Função PgDown, descer a página
void funcPgDown(Cursor *cursor, char *texto)
{
    ///Salva a quantidade de linhas
    int qtd = contarQuebrasDeLinha(texto);
    int tamanhoLinha;

    //printf("%d", qtd);
    /// Se a linha atual + 30 que é o valor esticamente definido para o tamanho de cada página
    /// for menor ou igual a quantidade de linhas no texto
    if((cursor->y + 30) <= qtd)
    {
        ///Cursor y recebe quantidade - 1 porque a linha inicia em 0
        cursor->y = qtd-1;

        ///chama a função para saber quantos caracter tem na linha para poder atualizar o cursor x
        tamanhoLinha = contarCaracteresNaLinhaDoCursor(cursor, texto);

        ///atualiza novamente o cursor y e o cursor x
        cursor->y = qtd - 1;

        ///Atualize o cursor x caso contenha mais caracteres que a linha anterior
        if(cursor->x > tamanhoLinha)
        {
            ///Atualiza o cursor x
            cursor->x = tamanhoLinha;
        }

        ///Atualiza o cursor na tela
        gotoxy(cursor->x, cursor->y);
    }

    ///Caso a soma seja maior que a quantidade de linhas no texto
    else
    {
        ///Caso o cursor y for menor que a quantidade de linhas
        cursor->y = qtd;

        ///chama a função para saber quantos caracter tem na linha para poder atualizar o cursor x
        tamanhoLinha = contarCaracteresNaLinhaDoCursor(cursor, texto);


        ///Atualiza o cursor na tela
        gotoxy(cursor->x, cursor->y);
    }
}

///Função PgUP, subir a página
void funcPgUp(Cursor *cursor, char *texto)
{
    /// Verifica se o cursor já está acima de 30 linhas
    if (cursor->y >= 30)
    {
        cursor->y -= 30;
    }
    else
    {
        /// Se o cursor estiver a menos de 30 linhas, vai para a linha 0
        cursor->y = 0;
    }

    ///Conta quantos caracteres tem naquela linha do cursor e atualiza o cursor x
    int tamanhoLinha = contarCaracteresAteCursor(cursor, texto);

    ///Atualize o cursor x caso contenha mais caracteres que a linha anterior
    if(cursor->x > tamanhoLinha)
    {
        cursor->x = tamanhoLinha;
    }

    ///Atualiza o cursor na tela
    gotoxy(cursor->x, cursor->y);
}

///Função para tratar todas as teclas especiais
///Chama a respectiva função da tecla pressionada
int tratarCaracterEspecial(char tecla, char *texto, Cursor *cursor, int op, int *anterior)
{
    int opc = 0;
    int esc;
    switch (tecla)
    {
    case 0:
        /// Verifique se é uma tecla especial (F1, F2, setas, etc.)
        tecla = _getch(); // Salva o segundo caractere da sequência
        switch (tecla)
        {
        case 59: // F1
            exibirAjuda();
            *anterior = 1;
            break;
        case 60: // F2
            salvarTexto(texto);
            *anterior = 0;
            break;
        case 68: // F10
            exibirArquivoSalvo(texto);
            *anterior = 0;
            break;
        }
        break;
    case 13: // Enter
        teclaEnter(texto, cursor);
        *anterior = 0;
        break;
    case 27: // Esc

        ///Caso a tecla anterior seja f1 ou f12 irá chamar a tela de inserção do texto
        if(*anterior == 1)
        {
            system("cls");
            printf("%s", texto);
        }
        /// Se não chama a função de  verificação de salvamento
        else
        {
            ///Ao pressionar esc pergunta se o usuario quer salvar ou não o texto
            do
            {
                printf("\nDeseja salvar o texto antes de sair da aplicação? (1/Sim, 2/Não): ");
                scanf("%d", &esc);
            }
            while (esc != 1 && esc != 2); /// while para verificar e validar se o usuário digitou 1 ou 2
            /// caso não digite, o white se repete

            ///Se ele desejar salvar o texto irá chamar a função para salvar o texto
            if(esc == 1)
            {
                salvarTexto(texto);
            }
            opc = 2;


        }
        break;
    case -32:
        tecla = _getch();
        switch (tecla)
        {
        case 71: // Home
            funcHome(cursor, texto);
            *anterior = 0;
            break;
        case 72: // Seta para cima
            rolarParaCima(cursor, texto);
            *anterior = 0;
            break;
        case 73:
            funcPgUp(cursor,texto);
            *anterior = 0;
            break;
        case 80: // Seta para baixo
            rolarParaBaixo(cursor, texto);
            *anterior = 0;
            break;
        case 81:
            funcPgDown(cursor,texto);
            *anterior = 0;
            break;
        case 79: // End
            funcEnd(cursor, texto);
            *anterior = 0;
            break;
        case 75: // Seta para a esquerda
            moverCursorEsquerda(cursor, texto);
            *anterior = 0;
            break;
        case 77: // Seta para a direita
            moverCursorDireita(cursor, texto);
            *anterior = 0;
            break;
        case 82:
            if (op != 2)
            {
                funcInsert(texto, cursor);
            }
            *anterior = 0;
            opc = 1; /// Retorna 1 quando a tecla de inserção é pressionada
            break;
        case 83: // Delete
            deletarCaracter(texto, cursor);
            *anterior = 0;
            break;
        case -122: // F12
            exibirInformacoes();
            *anterior = 1;
            break;
        }
        break;
    case 8: // Backspace
        backspace(texto, cursor);
        *anterior = 0;
        break;
    }
    return opc;
}

void main()
{
    SetConsoleTitle("Editor de texto");

    char tecla;
    char texto[MAX_TEXT_SIZE];
    Cursor cursor = {0, 0};
    memset(texto, 0, sizeof(texto));
    int sair  = 0;
    int teclaAnterior = 0;

    while (sair != 2)
    {
        tecla = _getch();

        /// Verifica se é uma tecla de inserção
        if ((tecla >= 32 && tecla <= 126) && tecla != 75 && tecla != 77)
        {
            inserirCaracter(texto, &cursor, tecla);
        }

        ///Caso seja uma tecla especial chama a devida função
        else
        {
            sair = tratarCaracterEspecial(tecla,texto, &cursor, 0, &teclaAnterior);
        }
    }
}


