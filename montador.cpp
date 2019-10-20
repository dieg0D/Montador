#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector> /*Amor da minha vida*/
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
vector<string> assembled_lines;        /*Linhas montadas*/
// apenas lê do asm
ifstream asmFile;
// lê e escreve o pre
fstream preFile;
// apenas escreve o obj
ofstream objFile;

int positionCount = 0, sectionTextLine = -1, sectionDataLine = -1;

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


// SUCESSO
void symbolTableCheck() {
  // percorre toda a tabela de símbolos e se tiver algum undefined, encerra com
  // erro
  for (symbol s : symbol_table) {
    cout << "[symbolTableCheck] "
         << "Símbolo " << s.name << "  " << s.value
         << (s.defined ? " T " : " F ") << endl;

    if (s.defined == false) {
      cout << "[symbolTableCheck] "
           << "ERRO SEMANTICO: Símbolo " << s.name << " não definido" << endl;

      getchar();
      exit(-1);
    }
  }
}


// TODO
// Funcao que checa se:
// 1. string tem até 50 de tamanho
// 2. não começa com número
// +
// Adicionar suporte a _ na regex

vector<string> tokeniza(string line){
  vector<string> tokens; //
  // ler a linha
  smatch sm;
  regex label_regex("(?:((?:[[:alnum:]]|\\_)+):\\s)?([A-Z]+)\\s?(-?(?:[[:alnum:]]|\\+|\\-|\\_)+)?(?:,(-?(?:[[:alnum:]]|\\+|\\-|\\_)+))?");

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
int checkLabelInTable(string label) {
  for (symbol s : symbol_table) {
    if (s.name == label) {
      if (s.defined == true) {
        return 1;
      }
    }
  }
  return 0;
}

int lineHasSymbol(string op1, string op2, int lineCount) {
  int resultado = 0;
  for (auto c : op1) {
    if (isalpha(c)) {
      if (isupper(c)) {
        resultado = 1;
      }
      break;
    }
  }
  for (auto c : op2) {
    if (isalpha(c)) {
      if (isupper(c)) {
        resultado += 2;
      }
      break;
    }
  }
  switch (resultado) {
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


// PASSO_6

int addSymbolToTable(string name, int position){
  vector<int> lista;
  lista.push_back(position);
  symbol s = {name, -1, false, lista};
  return 0;
}

// PASSO_7
int addEntryToSymbolOcurrenceList(string simbolo, int position){
  for (symbol s : symbol_table){
    if (s.name == simbolo){
      s.list.push_back(position);
      return 0;
    }
  }
  return -1;
}

int searchSTForSymbol(string simbolo, int pos){
  for (symbol s : symbol_table){
    if (s.name == simbolo){
      cout << "TA NA TABELA JA " << endl;
      if (!s.defined){// Não ta definido
        // VAI PARA PASSO_7
        addEntryToSymbolOcurrenceList(simbolo,pos);
      }
      // VAI PARA PASSO_3
      return 0;
    }else{
      cout << "NÃO TA NA TABELA AINDA " << endl;
      // VAI PRO PASSO_6
      addSymbolToTable(simbolo, pos);
    }
  }
  return 0;
}

int addOrDefineLabelinTable(string name, int pos){
  for (symbol s : symbol_table){
    if (s.name == name){
      s.value = pos;
      s.defined = true;
      return 0;
    }
  }
  vector<int> list;
  list.push_back(-1);
  symbol s = {name, pos, true, list};
  //PASSO 4
  symbol_table.push_back(s);
  return 0;
}

void singlePass(){

  string line;
  vector<string> tokens;
  int lineCount = 0, labelDefined = 0;
  while (!preFile.eof()) {
    int instrType = 0;
    getline(preFile, line);
    lineCount++;
    cout << "--------------------------------------- " << endl;
    cout << "Linha " << lineCount << " : " << line << endl;
    tokens = tokeniza(line);

    if(tokens.at(1) == "SECTION"){
      if(tokens.at(2) == "TEXT"){
        sectionTextLine = lineCount;
      }else if(tokens.at(2) == "DATA"){
        sectionDataLine = lineCount;
      }else{
        cout << "ERRO SEMANTICO: seção inválida!" << endl;
        exit(-1);
      }
      continue;
    }

    if (tokens.at(0) != "") {
      // cout << "TEM LABEL" << endl;
      labelDefined = checkLabelInTable(tokens.at(0));
      if (labelDefined) {
        cout << "LABEL JÁ DEFINIDA!" << endl;
        getchar();
        exit(-1);
      }

      addOrDefineLabelinTable(tokens.at(0), positionCount);
    }else{
      //cout << "NÃO TEM LABEL" << endl;
    }
    // PASSO 2
    // Confere se o numero de operandos está correto
    for (directive d : directive_table) {
      if (d.name == tokens.at(1)) {
        cout << "É DIRETIVA! " << d.operands << endl;
        int val = 0;
        val = tokens.at(2) == "" ? val : val + 1;
        val = tokens.at(3) == "" ? val : val + 1;
        if ((d.name == "SPACE") && (val == 0)) {
          /**
           * SPACE pode receber 0 ou 1 operandos, na tabela registramos 1
           * Para que a verificação seja correta, quando o valor for 0, mudamos
           * para 1
           */
          val = 1;
        }
        cout << tokens.at(2) << "    " << tokens.at(3) << endl;

        if (d.operands == val){
          int symbolFlag = lineHasSymbol(tokens.at(2), tokens.at(3), lineCount);

          switch (symbolFlag){
            case 0:
              // Monta a linha [?]
              positionCount++;
              break;
            case 1:
              searchSTForSymbol(tokens.at(2), positionCount + 1);
              positionCount+=2;
              break;
            case 2:
              searchSTForSymbol(tokens.at(3), positionCount + 2);
              positionCount+=2;
              break;
            case 3:
              searchSTForSymbol(tokens.at(2), positionCount + 1);
              searchSTForSymbol(tokens.at(3), positionCount + 2);
              positionCount+=3;
              break;
            default:
              break;
          }

          // PASSO_3
          // Montagem da linha
          string linhaMontada;

          linhaMontada = lineCount + ": " + tokens.at(1) + " \n";

          assembled_lines.push_back(linhaMontada);

        }else {

          cout << "ERRO : Nº DE OPERANDOS ERRADO!" << endl;
          exit(-1);
          getchar();
        }
        instrType = 1;
        break;
      }
    }
    if (!instrType) {
      for (instruction i : instruction_table) {
        if (i.name == tokens.at(1)) {
          cout << "É INSTRUÇÃO! " << i.operands << endl;
          int val = 0;
          val = tokens.at(2) == "" ? val : val + 1;
          val = tokens.at(3) == "" ? val : val + 1;

          cout << tokens.at(2) << "    " << tokens.at(3) << endl;
          if (i.operands == val) {
            lineHasSymbol(tokens.at(2), tokens.at(3), lineCount);
          } else {
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

vector<string> lineParsing(string line, ifstream *inputfile, int *line_count) {
  vector<string> words, wordsAux;
  string aux;
  unsigned int i=0, j=0, finish=0, rotCount=0;
  int size;
  cout << "Linha: " << *line_count << endl;
  while (finish == 0) {
    while (i <= line.length()) { /*Le a linha toda*/
      while (((line[i] == ' ') || (line[i] == '\t')) && (i < line.length())) { /*Remove espaços vazios*/
        i++;
      }


      if (line[i] == ';') { /*Ignora comentarios*/
        break;
      }

      if ((line[i] == '\0') && (aux == "")) { /*Ignora comentarios*/
        break;
      }

      aux = aux + line[i];

      if ((line[i+1] == ' ') || (line[i+1] == '\t') || (line[i+1] == '\n') || (i == line.length())) {
        if ((wordsAux.size() == 0) && (aux.find(":") == string::npos)) {
          wordsAux.push_back("none:");
        }
        if (aux[aux.length()-1] == '\0') {
          aux.erase(aux.length()-1, aux.length()-1);
        }
        wordsAux.push_back(aux);
        aux = "";
        j=0;
      }

      j++;
      i++;
    }
    if (wordsAux.size() == 0) {
      break;
    }
    if ((wordsAux.size() != 1) && (wordsAux[0].find(":") != string::npos)) { /*Caso só tenha a label ele junta com a outra linha*/
      finish = 1;
    }
    else {
      getline(*inputfile, line);
      transform(line.begin(), line.end(), line.begin(), ::toupper); /*Deixa toda a string maiuscula*/
      i=0;
      j=0;
      *line_count = *line_count + 1;
    }
  }
  
  if (wordsAux.size() >= 2) {
    words.push_back(wordsAux[0]);
    words.push_back(wordsAux[1]);
  }
  for (i=2;i<wordsAux.size();i++) {
    aux = wordsAux[i];
    size = wordsAux[i].length()-1;
    while ((wordsAux[i][size] != ',') && (i != wordsAux.size()-1)) {
      aux = aux + " " + wordsAux[i+1];
      i++;
      size = wordsAux[i].length()-1;
    }
    words.push_back(aux);
  }
  

  for (i=0;i<words.size();i++) { /*retira virgulas e dois pontos*/
    size = words[i].length()-1;
    if (words[i][size] == ':') {
      rotCount++;
    }
    if ((words[i][size] == ':') || (words[i][size] == ',')) {
      words[i].erase(size, size);
    }
  }
  
  if (rotCount >=2) {
    cout << "ERRO SINTATICO: Dois rotulos para mesma instrução, linha: " << *line_count << endl;
    //error = true;
    exit(-1);
  }
  return words;
}

void preProcessor(string fileName) {
  ifstream inputfile;
  string line;
  map<string, int> EQUTable;
  int flag = 0, textSectionFound = 0,lineCount = 0, data_location = -1;
  vector<string> sm;
  vector<string> data_camp;
  //smatch sm;
  //regex label_regex("\\s*(?:((?:[[:alnum:]]|\\_)+):\\s*)?(?:((?:[[:alnum:]]|\\_)+):\\s*)*([A-Z]+)\\s*(-?(?:[[:alnum:]]|\\+|\\-|\\_)+)?(?:,(-?(?:[[:alnum:]]|\\+|\\-|\\_)+))?.*");
  inputfile.open((fileName).c_str());
  int offset = fileName.length() - 3;
  fileName.replace(offset, 3, "pre");
  ofstream outfile(fileName);
  while (getline(inputfile, line)) {
    lineCount++;
    for (int i=0; i < (int) line.length(); i++){
      line[i] = toupper(line[i]);
    }
    //regex_match(line, sm, label_regex);
    sm = lineParsing(line, &inputfile, &lineCount);
    cout << "PARSEOIU" << endl;
    if(sm.size() > 0){
      cout << "TEM SM" << endl;
    }else {
      cout << "NÃO TEM SM" << endl;
      //lineCount++;
      continue;
    }
    for (string x : sm){
      cout << x << " /  ";
    }
    cout << endl;

    if(sm[1] == "SECTION"){
      if(sm[2] == "TEXT" ){
        textSectionFound++;
      }else if(sm[2] != "DATA" && sm[2] != "TEXT"){
        cout << "ERRO : SEÇÃO INVALIDA!" << endl;
        exit(-1);
      }
    }

    if(sm[1] == "SECTION"){
      if(sm[2] == "DATA"){
        if(!textSectionFound){
          // salva as parada na memoria, pq a DATA veio antes da TEXT
          data_location = lineCount;
          continue;
        }
      }
    }
    if(data_location > 0 && textSectionFound == 0){
      continue;
    }

    if (sm[1] == "EQU") {
      EQUTable.insert(std::pair<string, int>(sm[1], stoi(sm[3])));
    }
    

    if (sm[1] == "IF") {
      int hasEQU = 0; 
      for (auto it = EQUTable.begin(); it != EQUTable.end(); it++) {
        int num = it->first.find(":");
        string str = it->first;
        str = str.erase(num);
        if (str == sm[2] && it->second == 0) {
          flag = 1;
        }
  
        if(str == sm[2] ){
          hasEQU = 1;
        }
      }
      if(hasEQU == 0){
        cout << "ERRO:  IF UTILIZANDO LABEL QUE NÃO FOI DECLARADA NO EQU!" << endl;
      }else{
        hasEQU = 0;
      }
    }
    

    
    if (sm[1] == "SECTION" && sm[2] == "DATA"){
      cout << endl << "INICIO DA SECTION DATA!" << endl;
      cout << (outfile.is_open() ? "ABERTO" : "FECHADO") << endl;
      cout << "FLAG: " << flag << " / SM[1]: " << sm[1] << endl;
    }

    if (flag == 0 && sm[1] != "EQU" && sm[1] != "IF") {
      // cout << "ENTROU NO IF" << endl;
      if (sm[0] != "none") {
        outfile << sm[0] << ": ";
      }
      // cout << "TAMANHO DO SM: " << sm.size() << endl;
      int n_tokens = (int) sm.size();
      // for (int ix = 0; ix < n_tokens; ix++){
      //   cout << "SM[" << ix << "]: " << sm[ix] << endl;
      // }
      
      outfile << sm[1];
      if (n_tokens > 2){
        outfile << " " << sm[2];
      }
      if(n_tokens > 3){
       outfile << "," << sm[3];
      }
      outfile << endl;

    } else {
      // cout << "ENTROU NO ELSE" << endl;
      if (sm[1] != "IF") {
      // cout << "ENTROU NO ELSE IF" << endl;
        flag = 0;
      }
    }
      // cout << "SAIU DO IF" << endl;
  }
    

  if(textSectionFound == 0){
    cout << "ERRO : SEÇÃO TEXT FALTANDO!" << endl;
  }else if(textSectionFound > 1){
    cout << "ERRO : MAIS DE UMA SEÇÃO TEXT NO ARQUIVO!" << endl;
  }

  // SE DATA foi declarado antes do TEXT, manda bala
  if(data_location > 0){
    vector<string> sm;
    int lineCount = 0;
    string line;
    // limpa a flag de EOF
    inputfile.clear();
    // Volta pro inicio do arquivo
    inputfile.seekg(0, std::ios::beg);
    // Vai até o "SECTION DATA"
    for(int i = 0; i < data_location; i++){
      getline(inputfile, line);
      lineCount = i;
    }
    outfile << "SECTION DATA" << endl;
    // Fica lendo até o "SECTION TEXT"
    while(1){
      getline(inputfile, line);
      lineCount++;
      // CAPSLOCKA a linha
      for (int ix=0; ix < (int) line.length(); ix++){
        line[ix] = toupper(line[ix]);
      }
      cout << "PARSEANDO A LINHA: " << lineCount << endl << endl;
      sm = lineParsing(line, &inputfile, &lineCount);
      cout << "PARSEOU" << endl;
      int tokenCount = sm.size();
      cout << "SMSIZE: " << sm.size() << endl;
      if(tokenCount > 0){
        cout << "TEM SM" << endl;
        if(tokenCount > 2){
          if(sm[1] == "SECTION" && sm[2] == "TEXT"){
            break;
          }
        }
        if (sm[0] != "none") {
        outfile << sm[0] << ": ";
        }
        outfile << sm[1];
        if (tokenCount > 2){
          outfile << " " << sm[2];
        }
        if(tokenCount > 3){
        outfile << "," << sm[3];
        }
        outfile << endl;

      }else {
        //cout << "NÃO TEM SM" << endl;
        //lineCount++;
        continue;
      }
    }
  }

  
  preFile.close();
  for (string s : assembled_lines){
    cout<< "oooopa   " << s;
  }

  symbolTableCheck();

  outfile.close();
}

// Função que abre o arquivo texto que vem da linha de comando
void ioController(int argc, string fileName) {
  int offset = fileName.length() - 3;
  cout << "[ioController] "
       << "até aqui oquei" << endl;
  cout << "[ioController] "
       << "Nome do arquivo: " << fileName << endl;
  asmFile.open(fileName.c_str(), ios::in);
  fileName.replace(offset, 3, "pre");
  cout << fileName << endl;
  preFile.open(fileName.c_str(), ios::out | ios::in);
  asmFile.close();
}

int main(int argc, char *argv[]) {
  // Verifica se os argumentos foram passados
  if (argc > 1) {
    cout << "[main] "
         << "Arquivo recebido, tentado abrir..." << endl;
    ioController(argc, argv[1]);
    cout << "[main] "
         << "Inicializando a tabela de instruções..." << endl;
    instruction_table_init();
    cout << "[main] "
         << "Inicializando a tabela de diretivas..." << endl;
    directive_table_init();
    preProcessor(argv[1]);
    // singlePass();
  } else {
    cout << "[main] "
         << "ERRO SEMANTICO: O programa precisa de 1 argumento. Por favor "
            "insira um arquivo =D"
         << endl;
    return -1;
  }
  return 0;
}
