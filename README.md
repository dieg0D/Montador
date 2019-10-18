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

## Fluxo do montador

```
  Início
  PASSO_1 // Leitura da linha
    Lê uma linha do arquivo
    Se (Fim do Arquivo)
      Vai para PASSO_5
    Senão
      Analisa a label
      Se (existe label na linha)
        Vai para PASSO_4
      Senão
        Vai para PASSO_2

  PASSO_2 // Análise da linha
    Analisa a Linha
    Se (Linha não contém símbolo)
      Vai para PASSO_3
    Senão //Linha contém símbolo
      Busca o símbolo na TS
      Se (Símbolo não está na TS)
        Vai para PASSO_6
      Senão //Símbolo na TS
        Se (Símbolo definido)
          Vai para PASSO_3
        Senão //Símbolo não definido
          Vai para PASSO_7

  PASSO_3 // Montagem da linha
    "Monta" a linha
    Carrega na memória
    Printa LC, fonte e código objeto ?
    Vai para PASSO_1

  PASSO_4 // Inserção da label na TS
    Busca na TS
    Entra com nome da label, LC(posição/endereço/valor) e Tipo=D(defined=true) na TS
    Vai para PASSO_2

  PASSO_5 // Análise da TS para encerrar o programa
    Analisa a tabela de símbolos
    Se todos os símbolos estão definidos
      Encerra o programa com sucesso
      Vai para SUCESSO
    Senão
      Encerra o programa com Erro: "Símbolos não definidos!"

  PASSO_6 // Inserção de novo símbolo na TS
    Inserir nome do símbolo, ponteiro e tipo=U(defined=false)
    Vai para PASSO_3

  PASSO_7 // Tratamento de Símbolo não definido
    Copia ponteiro do símbolo na TS para instrução sendo montada
    Insere na TS o LC(posição/endereço/valor) para apontar para a instrução (adiciona a posição na lista de ocorrência do símbolo)
    Vai para PASSO_3

  SUCESSO // Aplicação dos valores da TS
    Para cada símbolo na TS
      Para cada elemento na lista de ocorrências
        Insere o valor do Símbolo no local indicado pelo elemento
    Encerra o programa
```

Erros a serem identificados

- [ ] declarações e rótulos ausentes;
- [ ] declarações e rótulos repetidos;
- [ ] pulo para rótulos inválidos;
- [ ] pulo para seção errada;
- [ ] diretivas inválidas;
- [ ] instruções inválidas;
- [ ] diretivas ou instruções na seção errada;
- [ ] divisão por zero (para constante);
- [ ] instruções com a quantidade de operando inválida;
- [ ] instruções com o tipo de operando inválido;
- [ ] tokens inválidos;
- [ ] dois rótulos na mesma linha;
- [ ] seção TEXT faltante;
- [ ] seção inválida;
- [ ] tipo de argumento inválido;
- [ ] modificação de um valor constante;
- [ ] acessar posição não reservada pelo SPACE (exemplo accesar SPACE+4,
      sendo que somente foi reservada uma posição)
