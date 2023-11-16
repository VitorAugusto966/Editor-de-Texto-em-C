#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "vtrlib.c"

#define MAX_TEXT_SIZE 10000 // Tamanho máximo do texto
#define MAX_HELP_TEXT_SIZE 1000

void inserirCaracter(char *texto, Cursor *cursor, char caracter)
{
    //printf("Cursor (x,y) = (%d, %d)", cursor->x, cursor->y);

    int tamanhoTexto = strlen(texto);
    int posicao;

    /// Verifica se o tamanho atual do texto não excede o máximo
    if (tamanhoTexto < MAX_TEXT_SIZE - 1)
    {

        if(cursor->y == 0)
        {
            for (int i = tamanhoTexto + 1; i > cursor->x; i--)
            {
                texto[i] = texto[i - 1];
            }

            /// Insere o caractere na posição do cursor
            texto[cursor->x] = caracter;

            /// Incrementa a posição do cursor x
            (cursor->x)++;

        }
        else
        {
            //printf("Cursor (x,y) = (%d, %d)", cursor->x, cursor->y);
            posicao = contarCaracteresAteCursor(cursor, texto);
            // printf(" pos = %d", posicao);
            for (int i = tamanhoTexto + 1; i > posicao; i--)
            {
                texto[i] = texto[i - 1];
            }

            /*  if(cursor->y > 1){

              /// Insere o caractere na posição do cursor
                   texto[posicao+1] = caracter;
              }
              else{

              /// Insere o caractere na posição do cursor
                           texto[posicao] = caracter;
              }*/

            /// Insere o caractere na posição do cursor
            texto[posicao] = caracter;


            /// Incrementa a posição do cursor x
            (cursor->x)++;
        }

        tamanhoTexto = strlen(texto);
        ///Indicação de fim do texto
        texto[tamanhoTexto + 1] = '\0';

        /// Atualize a tela com o novo texto
        // system("cls");
        //printf("%s", texto);

        /// Mantém o cursor na mesma coluna
        gotoxy(cursor->x, cursor->y);

    }
}

void funcInsert(char *texto, Cursor *cursor, char caracter)
{
    int tamanhoTexto = strlen(texto);
    int posicao;

    // Verifica se o tamanho atual do texto não excede o máximo
    if (tamanhoTexto < MAX_TEXT_SIZE - 1)
    {
        // Verifica se o cursor está na primeira linha
        if (cursor->y == 0)
        {
            // Se sim, sobrescreva o caractere na posição do cursor
            texto[cursor->x] = caracter;
        }
        else
        {
            // Caso contrário, calcule a posição de inserção com base na posição do cursor
            posicao = contarCaracteresAteCursor(cursor, texto);

            // Sobrescreva o caractere na posição do cursor
            texto[posicao] = caracter;
        }

        // Atualize a tela com o novo texto
        system("cls");
        printf("%s", texto);

        // Mantém o cursor na mesma coluna
        gotoxy(cursor->x, cursor->y);
    }
}

void moverCursorEsquerda(Cursor *cursor, char *texto)
{
    int tamanhoTexto;

    if (cursor->x > 0)
    {
        (cursor->x)--;
       //a printf("Cursor (x,y) = (%d, %d)", cursor->x, cursor->y);
        gotoxy(cursor->x, cursor->y);
    }
    else if(cursor->x == 0 && cursor->y > 0)
    {
        (cursor->y)--;
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        cursor->x = tamanhoTexto;
        gotoxy(cursor->x, cursor->y);
    }

    else if (cursor->y > 0)
    {
        /// Se o cursor estiver no início de uma linha, mova-o para a linha anterior
        (cursor->y)--;
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        cursor->x = tamanhoTexto;
        /*cursor->x = 0;
        while (texto[cursor->x] != '\n' && cursor->x < MAX_TEXT_SIZE)
        {
            (cursor->x)++;
        }*/
        gotoxy(cursor->x, cursor->y);
    }
}

void moverCursorDireita(Cursor *cursor, char *texto)
{

    int x = cursor->x;
    int y = cursor->y;

    //int tamanhoTexto = contarCaracteresNaLinhaDoCursor(cursor, texto);
    int tamanhoTexto = calcularTamanhoTexto(cursor, texto);
    cursor->y = y;
    cursor->x = x;
    int linhas = contarQuebrasDeLinha(texto);
    cursor->y = y;
    cursor->x = x;

    // printf("texto = %d", tamanhoTexto);
    //printf("Cursor (x,y) = (%d, %d)", cursor->x, cursor->y);

   if(cursor->x < tamanhoTexto &&cursor->y <= linhas){
     cursor->x++;
   }
   else if(cursor->x == tamanhoTexto && cursor->y < linhas){
        cursor->x = 0;
        cursor->y++;
   }

    gotoxy(cursor->x, cursor->y);
}


void rolarParaCima(Cursor *cursor, char *texto)
{
    if (cursor->y > 0)
    {
        /// Mova o cursor para a linha anterior
        (cursor->y)--;

        /// Encontre o início da linha anterior
        while (cursor->y > 0 && texto[cursor->x] != '\n')
        {
            cursor->y--;
        }

        /// Defina a posição x no início da linha anterior
        cursor->x = 0;

        gotoxy(cursor->x, cursor->y);
    }
    else
    {
        /// Estamos no topo do texto, não podem rolar mais para cima
        gotoxy(cursor->x, cursor->y);
    }
}

/// TECNICAMENTE CORRETO
void rolarParaBaixo(Cursor *cursor, char *texto)
{
    int qtdLinhas = contarQuebrasDeLinha(texto);
    int tamanhoTexto, tamanhoTextoAnterior;
    Cursor aux;

    if (cursor->y < qtdLinhas)
    {
        /// Mova o cursor para a próxima linha
        (cursor->y)++;

        aux.y = cursor->y - 1;
        aux.x = cursor->x;
        ///Calcular o tamanho do texto da proxima linha
        tamanhoTexto = calcularTamanhoTexto(cursor, texto);
        tamanhoTextoAnterior = calcularTamanhoTexto(&aux, texto);

        if(cursor->x > tamanhoTexto)
        {
            cursor->x = tamanhoTexto;
        }

        /// Encontre o início da próxima linha
        /*while (cursor->y < tamanhoTexto && texto[cursor->x] != '\n')
        {
            cursor->y++;
        }*/

        gotoxy(cursor->x, cursor->y);
    }
    else
    {
        /// Estamos no final do texto, não podemos rolar mais para baixo
        gotoxy(cursor->x, cursor->y);
    }
}


/// Função para posicionar o cursor no início da linha atual
/// TECNICAMENTE CORRETO
void funcHome(Cursor *cursor, char *texto)
{
    int posicao = cursor->x;

    /// Retroceda até encontrar o início da linha ou o início do texto
    while (posicao > 0 && texto[posicao - 1] != '\n')
    {
        posicao--;
    }

    cursor->x = posicao;
    gotoxy(cursor->x, cursor->y);
}

/// Função para posicionar o cursor no final da linha atual
/// TECNICAMENTE CORRETO
void funcEnd(Cursor *cursor, char *texto)
{
    int posicao = cursor->x;
    int tamanhoTexto = strlen(texto);

    /// Avance até encontrar o final da linha ou o final do texto
    while (posicao < tamanhoTexto && texto[posicao] != '\n')
    {
        posicao++;
    }

    cursor->x = posicao;
    gotoxy(cursor->x, cursor->y);
}

/// TECNICAMENTE CORRETO
void teclaEnter(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y; ///salvar a posição Y do cursor antes da varredura

    // Encontre a posição do cursor->y-1 no texto
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

    // Desloque o texto subsequente uma posição para frente
    for (int i = tamanhoTexto; i >= posicaoInsercao; i--)
    {
        texto[i + 1] = texto[i];
    }

    // Insira a quebra de linha na posição correta
    texto[posicaoInsercao] = '\n';

    // Atualize o cursor para a nova linha
    cursor->y = y;
    cursor->y++;
    cursor->x = 0;

    /// Atualize a tela com o novo texto
    system("cls");
    printf("%s", texto);
    exibirTextoSalvo(texto);
    gotoxy(cursor->x, cursor->y);
}

/// TODO ERRRADO
void deletarCaracter(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y;

    // Encontre a posição do cursor->y-1 no texto
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

    // Move todos os caracteres após a posição de remoção uma posição para trás
    for (int i = posicaoRemocao; i < tamanhoTexto; i++)
    {
        texto[i] = texto[i + 1];
    }

    // Atualize a tela com o novo texto
    cursor->y = y;
    system("cls");
    printf("%s", texto);

    // Mantém o cursor na mesma coluna
    gotoxy(cursor->x, cursor->y);
}



void backspace(char *texto, Cursor *cursor)
{
    int tamanhoTexto = strlen(texto);
    int y = cursor->y;

    // Encontre a posição do cursor->y-1 no texto
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

    // Calcule a posição para remoção
    int posicaoRemocao = posicaoQuebraAnterior + cursor->x;

    // Move todos os caracteres após a posição de remoção uma posição para trás
    for (int i = posicaoRemocao; i < tamanhoTexto; i++)
    {
        texto[i] = texto[i + 1];
    }

    // Atualize a tela com o novo texto
    cursor->y = y;
    cursor->x = cursor->x-1;
    system("cls");
    printf("%s", texto);

    // Mantém o cursor na mesma coluna
    gotoxy(cursor->x, cursor->y);
}


void main()
{
    char tecla;
    char texto[MAX_TEXT_SIZE]; /// Estrutura de dados para armazenar o texto
    Cursor cursor = {0, 0}; /// Inicializa o cursor no ponto (0, 0)
    memset(texto, 0, sizeof(texto));
    while (1)
    {
        tecla = _getch();
        if ((tecla >= 32 && tecla <= 126) && tecla != 75 && tecla != 77)
        {
            inserirCaracter(texto, &cursor, tecla);
            system("cls");
            printf("%s", texto);
        }

        switch (tecla)
        {
        case 0:
            /// Verifique se é uma tecla especial (F1, F2, setas, etc.)
            tecla = _getch(); /// Obtenha o segundo caractere da sequência
            switch(tecla)
            {
            case 59: /// F1
                exibirAjuda();
                break;
            case 60: /// F2
                salvarTexto(texto);
                break;
            case 68: /// F10
                exibirArquivoSalvo();
                break;
            case 82:
                funcInsert(texto,&cursor,tecla);
            }
            break;
        case 13: // Tecla Enter
            teclaEnter(texto, &cursor);
            break;

        case 27: // Tecla Esc
            return 0; // Saia do programa
        case -32:
            tecla = _getch();
            switch(tecla)
            {
            case 71: // Home
                funcHome(&cursor, texto);
                break;
            case 72: // Tecla de seta para cima (ASCII)
                rolarParaCima(&cursor, texto);
                break;

            case 80: // Tecla de seta para baixo (ASCII)
                rolarParaBaixo(&cursor, texto);
                break;
            case 79: // End
                funcEnd(&cursor, texto);
                break;
            case 75:
                moverCursorEsquerda(&cursor, texto);
                break;
            case 77: // Seta para a direita
                moverCursorDireita(&cursor, texto);
                break;
            case 83: // Tecla Delete
                deletarCaracter(texto, &cursor);
                break;
            case -122: ///f12
                exibirInformacoes();
                break;
            }
            break;

        case 8: // Tecla Backspace
            backspace(texto, &cursor);
            break;
        }
    }

}
