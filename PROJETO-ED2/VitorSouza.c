#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "vtrlib.c"
#define MAX_TEXT_SIZE 100000 // Tamanho máximo do texto
#define MAX_HELP_TEXT_SIZE 1000


///falta pgup e pgDown
/// falta insert
/// falta esc
/// falta validar tamanho lateral do inserir

/// ta bugado
void inserirCaracter(char *texto, Cursor *cursor, char caracter)
{
    int tamanhoTexto = strlen(texto) + 1;
    int tamanhoAnterior = strlen(texto);
    int posicao;

    //  CONSOLE_SCREEN_BUFFER_INFO csbi;
    // GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    // int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Verifica se o tamanho atual do texto não excede o máximo
    if (tamanhoTexto < MAX_TEXT_SIZE - 1)
    {
        // Calcula o número de colunas disponíveis na linha do cursor
        // int colunasNaLinha = contarCaracteresNaLinhaDoCursor(cursor, texto);
        int colunasNaLinha = calcularTamanhoTexto(cursor, texto);
        // Verifica se há espaço suficiente na linha para inserir um novo caractere
        if (colunasNaLinha < 80)
        {
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

                posicao = qtdCaracterAteCursor(cursor, texto);
                // printf("valor da posicao eh %d ", posicao);
                for (int i = tamanhoTexto + 1; i > posicao; i--)
                {
                    texto[i] = texto[i - 1];
                }
                texto[posicao] = caracter;
                (cursor->x)++;
            }

            // Atualiza o tamanho do texto após a inserção
            tamanhoTexto = strlen(texto);

            // Indicação de fim do texto
            texto[tamanhoTexto + 1] = '\0';

            // Atualiza a tela com o novo texto
            //system("cls");

            if(posicao== tamanhoTexto-1)
            {
                printf("%c",caracter);
            }
            else
            {
                system("cls");
                printf("%s", texto);
            }

            // Mantém o cursor na mesma coluna
            gotoxy(cursor->x, cursor->y);
        }
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
//int tratarCaracterEspecial(char *texto, Cursor *cursor, int op);


void funcInsert(char *texto, Cursor *cursor)
{
    int posicao;
    int insert = 0;
    int *teclaAnterior = 0;
    char tecla;


    while (insert != 1)
    {
        tecla = _getch();
        if ((tecla >= 32 && tecla <= 126) && tecla != 75 && tecla != 77)
        {
            posicao = qtdCaracterAteCursor(cursor, texto) ;
            texto[posicao] = tecla;
            system("cls");
            printf("%s", texto);
            gotoxy(cursor->x, cursor->y);
        }
        else
        {
            /*  tecla = _getch();
             if(tecla == 82){
             insert = 1;
            }*/
            insert = tratarCaracterEspecial(tecla,texto,cursor,2, teclaAnterior);
            //insert = tratarCaracterEspecial(texto,cursor,2);
        }
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

    if(cursor->x < tamanhoTexto &&cursor->y <= linhas)
    {
        cursor->x++;
    }
    else if(cursor->x == tamanhoTexto && cursor->y < linhas)
    {
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
    /*int posicao = cursor->x;


    /// Retroceda até encontrar o início da linha ou o início do texto
    while (posicao > 0 && texto[posicao - 1] != '\n')
    {
        posicao--;
    }

    cursor->x = posicao;*/
    cursor->x = 0;
    gotoxy(cursor->x, cursor->y);
}

/// Função para posicionar o cursor no final da linha atual
/// TECNICAMENTE CORRETO
void funcEnd(Cursor *cursor, char *texto)
{
    // int posicao = cursor->x;
    //int tamanhoTexto = strlen(texto);
    int posicao = contarCaracteresNaLinhaDoCursor(cursor,texto);
    // int posicao = calcularTamanhoTexto(cursor, texto) - 1;

    /// Avance até encontrar o final da linha ou o final do texto
    /*while (posicao < tamanhoTexto && texto[posicao] != '\n')
    {
        posicao++;
    }*/

    cursor->x = posicao+1;
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
    //exibirTextoSalvo(texto);
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

}

void funcPgDown(Cursor *cursor, char *texto)
{
    int qtd = contarQuebrasDeLinha(texto);
    int tamanhoLinha;

    if((cursor->y + 30) <= qtd)
    {
        cursor->y = qtd-1;
        tamanhoLinha = contarCaracteresNaLinhaDoCursor(cursor, texto);
        cursor->y = qtd - 1;
        cursor->x = tamanhoLinha-1;
        gotoxy(cursor->x, cursor->y);
    }
    else
    {
        /*cursor->y+=30;
        tamanhoLinha = contarCaracteresAteCursor(cursor, texto);
        cursor->y+=30;
        cursor->x = tamanhoLinha;
        gotoxy(cursor->x, cursor->y);*/

        if (cursor->y < qtd - 1)
        {
            cursor->y += 30;
        }
        else
        {
            cursor->y = qtd - 1;
        }
        tamanhoLinha = contarCaracteresNaLinhaDoCursor(cursor, texto);
        cursor->x = tamanhoLinha;
        gotoxy(cursor->x, cursor->y);
    }
}

void funcPgUp(Cursor *cursor, char *texto)
{
    // Verifica se o cursor já está acima de 30 linhas
    if (cursor->y >= 30)
    {
        cursor->y -= 30;
    }
    else
    {
        // Se o cursor estiver a menos de 30 linhas, vai para a linha 0
        cursor->y = 0;
    }

    int tamanhoLinha = contarCaracteresAteCursor(cursor, texto);
    cursor->x = tamanhoLinha;
    gotoxy(cursor->x, cursor->y);
}

int tratarCaracterEspecial(char tecla, char *texto, Cursor *cursor, int op, int *anterior)
{
    int opc = 0;
    int esc;
    switch (tecla)
    {
    case 0:
        // Verifique se é uma tecla especial (F1, F2, setas, etc.)
        tecla = _getch(); // Obtenha o segundo caractere da sequência
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
            exibirArquivoSalvo();
            *anterior = 0;
            break;
        }
        break;
    case 13: // Enter
        teclaEnter(texto, cursor);
        *anterior = 0;
        break;
    case 27: // Esc
        if(*anterior == 1)
        {
            system("cls");
            printf("%s", texto);
        }
        else
        {


            do
            {
                printf("\nDeseja salvar o texto antes de sair da aplicação? (1/Sim, 2/Não): ");
                scanf("%d", &esc);
            }
            while (esc != 1 && esc != 2);

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
            opc = 1; // Retorna 1 quando a tecla de inserção é pressionada
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

/*int tratarCaracterEspecial(char *texto, Cursor *cursor, int op) {

    int resultado = 0;

    if (GetAsyncKeyState(VK_F1) & 0x8001) {
        exibirAjuda();
    } else if (GetAsyncKeyState(VK_F2) & 0x8001) {
         salvarTexto(texto);
    } else if (GetAsyncKeyState(VK_F10) & 0x8001) {
            exibirArquivoSalvo();
    } else if (GetAsyncKeyState(VK_LEFT) & 0x8001) {
       moverCursorEsquerda(cursor, texto);
    } else if (GetAsyncKeyState(VK_RIGHT) & 0x8001) {
       moverCursorDireita(cursor, texto);
    }
    else if(GetAsyncKeyState(VK_UP)& 0x8001){

    }
    else if(GetAsyncKeyState(VK_DOWN)& 0x8001){

    }
    else if(GetAsyncKeyState(VK_HOME)& 0x8001){

    }
    else if(GetAsyncKeyState(VK_END)& 0x8001){

    }
    else if(GetAsyncKeyState(VK_RETURN)& 0x8001){
        teclaEnter(texto, cursor);
    }
    else if (GetAsyncKeyState(VK_INSERT) & 0x8001 && op != 2) {
        funcInsert(texto, cursor);
        resultado = 1;
    } else if (GetAsyncKeyState(VK_DELETE) & 0x8001) {
        deletarCaracter(texto,cursor);
    }
    else if(GetAsyncKeyState(VK_BACK)){
        backspace(texto,cursor);
    }
    else if(GetAsyncKeyState(VK_F12)& 0x8001){
        exibirInformacoes();
    }
    else if(GetAsyncKeyState(VK_ESCAPE)& 0x8001)
    {
        resultado = 2;
    }
    return resultado;
}*/


void main()
{
    char tecla;
    char texto[MAX_TEXT_SIZE];
    Cursor cursor = {0, 0};
    memset(texto, 0, sizeof(texto));
    int sair  = 0;
    int teclaAnterior = 0;

    while (sair != 2)
    {
        tecla = _getch();

        // Verifica se é uma tecla de inserção
        if ((tecla >= 32 && tecla <= 126) && tecla != 75 && tecla != 77)
        {
            inserirCaracter(texto, &cursor, tecla);
        }
        else
        {
            sair = tratarCaracterEspecial(tecla,texto, &cursor, 0, &teclaAnterior);
        }
    }
}


