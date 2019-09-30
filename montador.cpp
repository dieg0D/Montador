#include <iostream>
#include <vector>  /*Amor da minha vida*/

using namespace std;

// Struct da tabela de instruções
struct instruction {
  string name;
  int length;
  int opcode;
  int operands;
};

// Struct da tabela de diretivas
struct directive {
  string name;
  int length;
  int operands;
};

vector<instruction> instruction_table; /*Tabela de instruções*/
vector<directive> directive_table;     /*Tabela de diretivas*/


/*Inicializa a tabela de instruções*/
void instruction_table_init() {
  instruction instruction;

  instruction.name = "ADD";
  instruction.length = 2;
  instruction.opcode = 1;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "SUB";
  instruction.length = 2;
  instruction.opcode = 2;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "MULT";
  instruction.length = 2;
  instruction.opcode = 3;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "DIV";
  instruction.length = 2;
  instruction.opcode = 4;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMP";
  instruction.length = 2;
  instruction.opcode = 5;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPN";
  instruction.length = 2;
  instruction.opcode = 6;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPP";
  instruction.length = 2;
  instruction.opcode = 7;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "JMPZ";
  instruction.length = 2;
  instruction.opcode = 8;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "COPY";
  instruction.length = 3;
  instruction.opcode = 9;
  instruction.operands = 2;
  instruction_table.push_back(instruction);

  instruction.name = "LOAD";
  instruction.length = 2;
  instruction.opcode = 10;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "STORE";
  instruction.length = 2;
  instruction.opcode = 11;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "INPUT";
  instruction.length = 2;
  instruction.opcode = 12;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "OUTPUT";
  instruction.length = 2;
  instruction.opcode = 13;
  instruction.operands = 1;
  instruction_table.push_back(instruction);

  instruction.name = "STOP";
  instruction.length = 1;
  instruction.opcode = 14;
  instruction.operands = 0;
  instruction_table.push_back(instruction);
}

/*Inicializa a tabela de diretivas*/
void directive_table_init() {
  directive directive;

  directive.name = "SECTION";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "SPACE";
  directive.length = 1;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "CONST";
  directive.length = 1;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "EQU";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

  directive.name = "IF";
  directive.length = 0;
  directive.operands = 1;
  directive_table.push_back(directive);

}

// Função que abre o arquivo texto que vem da linha de comando
void ioController(int argc, string fileName) {
  if (argc < 1) {
    cout << "até aqui oquei" << endl;
  } else {
    cout << "Erro: Por favor insira um arquivo =D" << endl;
  }
}

int main(int argc, char *argv[]) {
  ioController(argc, argv[1]);
  return 0;
}