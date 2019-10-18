#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>  /*Amor da minha vida*/
#include <cstdlib>
#include <cctype>

using namespace std;

// Struct da tabela de instruções
typedef struct instruction {
  string name;
  int length;
  int opcode;
  int operands;
} instruction;

// Struct da tabela de diretivas
typedef struct directive {
  string name;
  int length;
  int operands;
} directive;

// Struct da tabela de símbolos
typedef struct symbol {
  string name;
  int value;
  bool defined;
  vector<int> list;
} symbol;



vector<instruction> instruction_table; /*Tabela de instruções*/
vector<directive> directive_table;     /*Tabela de diretivas*/
vector<symbol> symbol_table;           /*Tabela de símbolos*/

// apenas lê do asm
ifstream asmFile;
// lê e escreve o pre
fstream preFile;
// apenas escreve o obj
ofstream objFile;

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

void symbolTableCheck(){
 
  // percorre toda a tabela de símbolos e se tiver algum undefined, encerra com erro
  for (symbol s : symbol_table){
      cout << "[symbolTableCheck] " << "Símbolo " << s.name << "  " << s.value << (s.defined ? " T " : " F ") << endl;

    if (s.defined == false){
      cout << "[symbolTableCheck] " << "ERRO SEMANTICO: Símbolo " << s.name << " não definido" << endl;
      getchar();
      exit(-1);
    }
  }
  
}

vector<string> tokeniza(string line){
  vector<string> tokens; //
  // ler a linha
  smatch sm;
  regex label_regex("(?:([[:alnum:]]+):\\s)?([A-Z]+)\\s?(-?(?:[[:alnum:]]|\\+|\\-)+)?(?:,(-?(?:[[:alnum:]]|\\+|\\-)+))?");
  regex_match(line, sm, label_regex);

  // pegar label
    // cout << "Label: " << sm[1] << endl;
    tokens.push_back(sm[1]);
  // pegar instruction
    // cout << "Instr: " << sm[2] << endl;
    tokens.push_back(sm[2]);
  // pegar operator1
    // cout << "OPER1: " << sm[3] << endl;
    tokens.push_back(sm[3]);
  // pegar operator2
    // cout << "OPER2: " << sm[4] << endl;
    tokens.push_back(sm[4]);
  return tokens;
}

// APENAS PARA LABEL
int checkLabelInTable(string label){
  for (symbol s : symbol_table){
    if (s.name == label){
      if (s.defined == true){
        return 1;
      }
    }
  }
  return 0;
}

int lineHasSymbol(string op1, string op2, int lineCount){
  int resultado = 0;
  for (auto c : op1){
    if(isalpha(c)){
      if(isupper(c)){
        resultado = 1;
      }
      break;
    }
  }
  for (auto c : op2){
    if(isalpha(c)){
      if(isupper(c)){
        resultado += 2;
      }
      break;
    }
  }
  switch (resultado){
    case 0:
      cout << "SEM SIMBOLOS NA LINHA " << lineCount << endl;
      break;
    case 1:
      cout << "OP1 É SIMBOLO " << lineCount << endl;
      break;
    case 2:
      cout << "OP2 É SIMBOLO " << lineCount << endl;
      break;
    case 3:
      cout << "OS DOIS OP SÃO SIMBOLOS " << lineCount << endl;
      break;
    default:
      break;
  }
  return resultado;
}

void singlePass(){
  string line;
  vector<string> tokens;
  int lineCount = 0, labelDefined = 0;
  while(!preFile.eof()){
    int instrType = 0;
    getline(preFile, line);
    lineCount++;
    cout << "--------------------------------------- " << endl;
    cout << "Linha " << lineCount << " : " << line << endl;
    tokens = tokeniza(line);

    if (tokens.at(0) != ""){
      //cout << "TEM LABEL" << endl;
      labelDefined = checkLabelInTable(tokens.at(0));
      if (labelDefined){
        cout << "LABEL JÁ DEFINIDA!" << endl;
        getchar();
        exit(-1);
      }

      vector<int> list;
      list.push_back(-1);
      symbol s = {tokens.at(0), lineCount, true, list};
      //PASSO 4
      symbol_table.push_back(s);
    }else{
      //cout << "NÃO TEM LABEL" << endl;
    }
    //PASSO 2
    // Confere se o numero de operandos está correto
    for (directive d : directive_table){
      if (d.name == tokens.at(1)){
        cout << "É DIRETIVA! " << d.operands << endl;
        int val=0;
        val = tokens.at(2) == "" ? val : val+1;
        val = tokens.at(3) == "" ? val : val+1;
        if ((d.name == "SPACE")&&(val == 0)){
          /**
           * SPACE pode receber 0 ou 1 operandos, na tabela registramos 1
           * Para que a verificação seja correta, quando o valor for 0, mudamos para 1
           */ 
          val = 1;
        }
        cout << tokens.at(2) << "    " << tokens.at(3) << endl;
        if (d.operands == val){
          lineHasSymbol(tokens.at(2), tokens.at(3), lineCount);
        }else {
          cout << "ERRO : Nº DE OPERANDOS ERRADO!" << endl;
          getchar();
          exit(-1);
        }
        instrType = 1;
        break;
      }
    }
    if (!instrType){
      for(instruction i : instruction_table){
        if (i.name == tokens.at(1)){
          cout << "É INSTRUÇÃO! " << i.operands << endl;
            int val=0;
            val = tokens.at(2) == "" ? val : val+1;
            val = tokens.at(3) == "" ? val : val+1;
            
            cout << tokens.at(2) << "    " << tokens.at(3) << endl;
            if (i.operands == val){
              lineHasSymbol(tokens.at(2), tokens.at(3), lineCount);
            }else {
              cout << "ERRO : Nº DE OPERANDOS ERRADO!" << endl;
              getchar();
              exit(-1);
            }
          break;
        }
      }
    }



  }
  preFile.close();
  symbolTableCheck();
}

// Função que abre o arquivo texto que vem da linha de comando
void ioController(int argc, string fileName) {
    int offset = fileName.length() - 3;
    cout << "[ioController] " << "até aqui oquei" << endl;
    cout << "[ioController] " << "Nome do arquivo: " << fileName << endl;
    asmFile.open(fileName.c_str(), ios::in);
    fileName.replace(offset, 3, "pre");
    cout << fileName << endl;
    preFile.open(fileName.c_str(), ios::out | ios::in);
    asmFile.close();        
}

int main(int argc, char *argv[]) {
  //Verifica se os argumentos foram passados
  if (argc > 1) {
    cout << "[main] " <<"Arquivo recebido, tentado abrir..." << endl;
    ioController(argc, argv[1]);
    cout << "[main] " <<"Inicializando a tabela de instruções..." << endl;
    instruction_table_init();
    cout << "[main] " <<"Inicializando a tabela de diretivas..." << endl;
    directive_table_init();

    singlePass();
  } else {
    cout << "[main] " << "ERRO SEMANTICO: O programa precisa de 1 argumento. Por favor insira um arquivo =D" << endl;
    return -1;
  }
  return 0;
}