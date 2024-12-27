# Editor de Texto

## Descrição
Este projeto é um editor de texto simples desenvolvido em **C**, com diversas funcionalidades acessíveis através de teclas específicas. O objetivo é oferecer uma experiência básica de edição de texto em terminal, explorando o uso de manipulação de arquivos, entrada de teclado e navegação no conteúdo.

---

## Funcionalidades
O editor oferece as seguintes funcionalidades:

### **Teclas de Navegação e Edição**
- **Letras, números e símbolos**: Podem ser inseridos diretamente no texto.
- **Setas**:
  - **↑ ↓ → ←**: Navegam entre linhas e caracteres no texto.
- **Insert**: Ativa o modo de sobrescrita, permitindo substituir o conteúdo existente no cursor atual.
- **Delete**: Remove o caractere na posição atual do cursor.
- **Enter**: Move o cursor para o início da próxima linha.
- **Page Up/Page Down**:
  - **Page Up**: Avança para a página acima no conteúdo.
  - **Page Down**: Avança para a página abaixo no conteúdo.
- **Home/End**:
  - **Home**: Posiciona o cursor no início da linha atual.
  - **End**: Posiciona o cursor no final da linha atual.

### **Teclas de Função**
- **F1**: Exibe uma mensagem de ajuda com instruções sobre como usar o editor.
- **F2**: Salva o conteúdo atual em um arquivo.
- **F10**: Abre um arquivo para visualização.
- **F12**: Exibe informações sobre o aluno responsável pelo projeto.
- **ESC**: Encerra a aplicação e fecha o editor.

---

## Tecnologias Utilizadas
- **Linguagem**: C
- **Bibliotecas**:
  - `stdio.h` e `stdlib.h` para manipulação de entrada/saída e memória.
  - `conio.h`.
  - `string.h` para manipulação de strings.
  - `file.h` para leitura e escrita em arquivos.

---
