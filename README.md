# Montador

Este repositório contém o Projeto 1 da Disciplina Software Básico de 2019/2.O trabalho consiste em implementar em C/C++ um método de tradução de uma
linguagem de montagem simples para uma representação de código objeto. O tradutor
a ser implementado será um Assembler da linguagem hipotética vista em sala de aula.

## Para Compilar

Utilize o comando `make` do Linux. O `Makefile` irá utilizar o
comando de compilação e removerá os arquivos criados no processo de compilação.

## Para Executar

Em seu Terminal rode o comando `./montador <codigo.asm>`, substituindo o `<codigo.asm>` pelo arquivo a ser montado.

## Objetivo

Fixar o funcionamento de um processo de tradução. Especificamente as etapas
de analise léxica, sintática e semântica e a etapa de geração de código objeto.

## Tarefas que precisamos fazer

#### Montador:

- [ ] Leitura
  - [ ] Receber e abrir o arquivo
  - [ ] Contador de endereços/posição
- [ ] Parser
  - [ ] Detecção das áreas de texto e _data_
  - [ ] Deteção das linhas
  - [ ] Detecção das instruções
  - [ ] Detecção dos argumentos das instruções
  - [ ] Detecção de rótulos (com ou sem enter após)
- [ ] Tabelas
  - [x] Tabela de Instruções para consulta
  - [ ] Montagem da tabela de símbolos
- [ ] Detecção de Erros
  - [ ] Detecção de erros léxicos
  - [ ] Detecção de erros sintáticos
  - [ ] Detecção de erros semânticos
  - [ ] Exibição das linhas e classificação dos erros
- [ ] Código de Máquina
  - [ ] Adicionar o _opcode_
  - [ ] Resolver as pendências dos endereços da Tabela de Símbolos
- [ ] Arquivos de Saída
  - [ ] Geração do arquivo pré-processado (_não sei se entendi do que se trata_)
  - [ ] Geração do arquivo objeto contendo o código de máquina

#### Simulador:

- [ ] Receber o programa de entrada
- [ ] Identificar as instruções de INPUT e OUTPUT, exibindo o texto pedido
- [ ] Identificar e executar as instruções
- [ ] Exibir o conteúdo do acumulador ao executar uma instrução
- [ ] Exibir o endereço e conteúdo de endereços modificados pela instrução
