#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>


#define MEMORY_SIZE 4096
#define BITS_NUMBER 15
#define SOURCE_CODE_NUMBER 81
#define MAX_LINE_LENGTH 81
#define MAX_VARIABLES 4096
#define MAX_VARIABLES_NAMES 81
/**
 * Represents a single source line, including it's details
 */
/*Deffines*/

/*lines*/
typedef struct {
    char content[MAX_LINE_LENGTH];
} lineInfo;

typedef enum {
    DATA,
    CODE,
    MDEFINE
} dataType;

typedef enum {
    NONE,
    ENTRY,
    EXTERNAL,
    LABEL
} updateType;

/*FROM HERE*/
typedef enum {
    SITUATION_DEFULT,
    SITUATION_DEFINE,
    SITUATION_DATA,
    SITUATION_STRING,
    SITUATION_COMMAND,
    SITUATION_EXTERN,
    SITUATION_ENTRY
} SituationType;

typedef struct  variables {
    int variablesNumber;
    char variablesName[MAX_VARIABLES_NAMES];
    int variablesValue;
    dataType type;
    updateType typeOfUpdate;
    struct  variables* next;
}  variables;


/*
 * This struct for keeping a command and its numeric values
 */
typedef struct AddrMethodsOptions
{
    unsigned int immediate : 1;
    unsigned int direct : 1;
    unsigned int index : 1;
    unsigned int reg : 1;
} AddrMethodsOptions;

#define COMMAND_NAME_LENGTH 5
typedef struct command {
    char name[COMMAND_NAME_LENGTH];
    int opcode;
    int numberOfOperand;
    AddrMethodsOptions src; /*legal addressing mehods for the source operand*/
    AddrMethodsOptions des; /*legal addressing mehods for the destination operand*/
} command;

/** Registers - r0->r1 + not found */
typedef enum registers {
    r0 = 0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    NONE_REG = -1
} reg;


/*Bits*/
typedef enum {
    STRING,
    INTEGER,
    CHAR
} DataType;

typedef enum {
    DIRECT,
    INDIRECT,
    REGISTER
} AddressingMethod;

typedef struct Bits {
    int number;
    char bits[BITS_NUMBER];
    DataType dataType;
    AddressingMethod addressingMethod;
    char sourceCode[SOURCE_CODE_NUMBER];
    int IC;
    int instruction;
    struct Bits* next;
} Bits;


/* Macro type*/
typedef struct macro
{
    char name_of_macro[MAX_LINE_LENGTH];
    char *macro_info;
    struct macro* next_macro;
}macro;



/* LISTS FUNCS*/
int insertBitsNode(Bits** head, Bits* newNode);
variables* createVariable(int *DC, const char* word, int variablesValue, DataType type, updateType typeOfUpdate);
variables* createTempVariable(const char* word, int variablesValue, DataType type, updateType typeOfUpdate);
int insertVariable( variables** head,  variables* newVariable);
Bits* createNode(int *IC, DataType dataType, AddressingMethod addressingMethod, char sourceCode[],int instruction);
int changeBitsFromIndexToIndex(Bits* node, int start, int end, const char *newSubstring);
int commandChanger(Bits *node, command commands[]);
int bitChanger(Bits *node);
int binary4ToDecimal(char *binary);
int isASavedWord(char *word, char* macrosNames);
int changeVariablesType( variables *head, int type);
void deleteVar(variables** head, int position);
char* encoding(Bits *node, char *coded_str);
void printEncoding(Bits* head);
void printList(Bits* head);
int variablesAlreadySaved(char *str,  variables* head, int type);
void printVariableList( variables* head);


/* HELP FUNCS*/
int getRegisterIndexByString(const char* word);
int getRegisterIndexByNode(Bits* node);
int hasOneInCmdSrc(const command *cmd);
int hasOneInCmdDes(const command *cmd);
int isCommand(const char *str, const command commands[], int num_commands);
void extractBetween(const char *input, char *output, char first, char last);
int parseString(const char *input, char *part1, char *part2);
int countCommas(char* str);
int isNumber(const char *str);
char **breakString(char *input_string, const char *delimiter, int *num_elements);
void intToBinary(int n, int size, char *binary);
int isAValidLabel(char *word,int line);
int isAsmbliNumber(const char *str);
void removeHash(char *str);
int binaryToChar(char binary[], int start, int end);
char printSymbol(int value);
void freeList(Bits** head);
void freeVariableList( variables** head);
void removeSubstring(char *str, const char *sub);
int countInstruction(Bits* head);
int countEntrys(variables* varsList);
int splitString(char *str, char *before_equal, char *after_equal);
void removeSpaces(char *str);
int hasSpaceInMiddle(const char *str);
void removeDefine(char *str);

void messageIssued(const char *format, ...);


/*STAGE_ONE FUNCS*/
int stageOne(int Line,lineInfo lines,command commands[],Bits** head, variables** VarsList, variables** TempVarsList ,int **IC,int **DC, char* macrosNames);
int preStageOne(int *IC, int *DC, const char *filename, command commands[], Bits **head, variables **VarsList, variables **TempVarsList, char* macrosNames);

/*STAGE_TWO FUNCS*/
int searchAndChangeVaribles(Bits *headList,Bits *node, variables* varsList);
void stageTwo(Bits* head, command commands[], variables* varsList, variables* tempVarList);

/*TO_FILE FUNCS*/
void searchForExtern(Bits *node, variables* varsList);
void variablesEntryExternal( variables* VarsList,  variables* TempVarsList);
void writeExternalVarsToFile( variables* varsList, const char *filename);
void exportCodeFile(Bits* head, variables* varsList, const char *filename,int* IC,int* DC);
void writeVarsToFile( variables* varsList, const char *filename);
void writeToFiles(Bits* head, variables* varsList, const char *filename,int* IC,int* DC);

/*MACRO FUNCS*/
struct macro* create_macro_Node(char macro_name[MAX_LINE_LENGTH]);
void insertAtBeginning(struct macro** headRef, char macro_name[MAX_LINE_LENGTH]);
void deleteList(struct macro** headRef);
int main_macro(char* file_name, char* macrosNames);
int insertLine(macro *Macros, char *line);
int maco_deploy(FILE *input, FILE *output, char* macrosNames);
