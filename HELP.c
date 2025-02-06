#include "main.h"

/*
    Retrieves the index of a register based on the given word.
    @param word: The register name or "NONE_REG".
    @return: The index of the register or NONE_REG if not found, -1 if invalid.
*/
int getRegisterIndexByString(const char* word) {
    if (strcmp(word, "r0") == 0) {
        return r0;
    } else if (strcmp(word, "r1") == 0) {
        return r1;
    } else if (strcmp(word, "r2") == 0) {
        return r2;
    } else if (strcmp(word, "r3") == 0) {
        return r3;
    } else if (strcmp(word, "r4") == 0) {
        return r4;
    } else if (strcmp(word, "r5") == 0) {
        return r5;
    } else if (strcmp(word, "r6") == 0) {
        return r6;
    } else if (strcmp(word, "r7") == 0) {
        return r7;
    } else if (strcmp(word, "NONE_REG") == 0) {
        return NONE_REG;
    } else {
        return -1;
    }
}
int getRegisterIndexByNode(Bits* node) {
    if (node == NULL) {
        return 0; 
    }
    if (strcmp(node->sourceCode, "r0") == 0) {
        return r0;
    } else if (strcmp(node->sourceCode, "r1") == 0) {
        return r1;
    } else if (strcmp(node->sourceCode, "r2") == 0) {
        return r2;
    } else if (strcmp(node->sourceCode, "r3") == 0) {
        return r3;
    } else if (strcmp(node->sourceCode, "r4") == 0) {
        return r4;
    } else if (strcmp(node->sourceCode, "r5") == 0) {
        return r5;
    } else if (strcmp(node->sourceCode, "r6") == 0) {
        return r6;
    } else if (strcmp(node->sourceCode, "r7") == 0) {
        return r7;
    } else if (strcmp(node->sourceCode, "NONE_REG") == 0) {
        return NONE_REG;
    } else {
        return -1;
    }
}
/*
    Checks if any of the source fields in the given command structure have a value of 1.
    @param cmd: Pointer to the command structure.
    @return: 1 if any source field has a value of 1, otherwise returns 0.
*/
int hasOneInCmdSrc(const command *cmd) {
    /* Check if any of the fields in src have a value of 1*/
    if (cmd->src.immediate || cmd->src.direct || cmd->src.index || cmd->src.reg)
    {
        return 1;
    }
    else
        return 0;
}
/*
    Checks if any of the destination fields in the given command structure have a value of 1.
    @param cmd: Pointer to the command structure.
    @return: 1 if any destination field has a value of 1, otherwise returns 0.
*/
int hasOneInCmdDes(const command *cmd) {
    /* Check if any of the fields in des have a value of 1*/
    if (cmd->des.immediate || cmd->des.direct || cmd->des.index || cmd->des.reg)
        return 1;
    else
        return 0;
}
/*
    Checks if the given string matches any command name in the specified array of commands.
    @param str: The string to be checked.
    @param commands: Array of command structures.
    @param num_commands: The number of commands in the array.
    @return: The opcode of the matching command, or -1 if not found.
*/
int isCommand(const char *str, const command commands[], int num_commands)
{
    int i;
    for (i = 0; i < num_commands; ++i)
    {
        if (strcmp(str, commands[i].name) == 0) {
            return commands[i].opcode;
        }
    }
    return -1;
}
/*
   Extracts the substring between the first occurrence of 'first' and the first occurrence of 'last' characters.
   @param input: The input string.
   @param output: The extracted substring will be stored here.
   @param first: The starting character to search for.
   @param last: The ending character to search for.
*/
void extractBetween(const char *input, char *output, char first, char last) {
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == first) {
            i++;
            while (input[i] != last && input[i] != '\0') {
                output[j++] = input[i++];
            }
            output[j] = '\0';
            return;
        }
    }
    output[0] = '\0';
}
/*
   Parses the input string into two parts separated by '[' and ']'.
   @param input: The input string to be parsed.
   @param part1: The first part of the string will be stored here.
   @param part2: The second part of the string will be stored here.
   @return: Returns 1 if successfully parsed, 0 otherwise.
*/
int parseString(const char *input, char *part1, char *part2) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '[') {
            strncpy(part1, input, i);
            part1[i] = '\0';
            extractBetween(input + i, part2, '[', ']');
            return 1;
        }
    }
    return 0;
}
/*
   Counts the number of commas in the given string.
   @param str: The input string.
   @return: The number of commas found in the string.
*/
int countCommas(char* str) {
    int count = 0;
    while (*str) {
        if (*str == ',') {
            count++;
        }
        str++;
    }
    return count;
}
/*
   Checks if the given string represents a number.
   @param str: The input string to be checked.
   @return: Returns 1 if the string is a number, 0 otherwise.
*/
int isNumber(const char *str) {
    while (*str) {
        if ((*str < '0' || *str > '9') && *str != '-') {
            return 0;
        }
        str++;
    }
    return 1; 
}
/*
   Splits the input string into an array of strings using the specified delimiter.
   @param input_string: The string to be split.
   @param delimiter: The delimiter used to separate the string.
   @param num_elements: Pointer to an integer to store the number of elements in the resulting array.
   @return: An array of strings containing the broken parts of the input string.
            Memory is dynamically allocated for the array and its elements.
            Caller is responsible for freeing the memory.
*/
char **breakString(char *input_string, const char *delimiter, int *num_elements) {
    char **broken_strings = NULL;
    char *token;
    int num_strings = 0;
    token = strtok(input_string, delimiter);

    while (token != NULL) {
        broken_strings = realloc(broken_strings, (num_strings + 1) * sizeof(char *));
        if (broken_strings == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        broken_strings[num_strings] = calloc(strlen(token) + 1, sizeof(char));
        if (broken_strings[num_strings] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(broken_strings[num_strings], token); 

        num_strings++;
        token = strtok(NULL, delimiter);
    }

    *num_elements = num_strings;
    return broken_strings;
}
/*
   Converts an integer to a binary representation.
   @param n: The integer to be converted.
   @param size: The size of the binary representation.
   @param binary: Pointer to the character array to store the binary representation.
*/
void intToBinary(int n, int size, char *binary) {
    int i;
    for (i = 0; i < size; i++) {
        binary[i] = (n & (1 << (size - 1 - i))) ? '1' : '0';
    }
    binary[size] = '\0'; 
}
/*
    Checks if the given word is a saved word in the assembly language.
    @param word: The word to be checked.
    @return: Returns 1 if the word is a saved word, 0 otherwise.
*/
int isASavedWord(char *word, char* macrosNames) {
    char *saved_words[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt", "r1", "r2", "r3", "r4", "r5", "r6", "r7",".entry",".extern","mcr","endmcr",".data",".string"};
    char* token;
    int num_saved_words = sizeof(saved_words) / sizeof(saved_words[0]);
    int i;
    for (i = 0; i < num_saved_words; i++) {
        if (strcmp(word, saved_words[i]) == 0) {
            return 1; 
        }
    }
    token = strtok(macrosNames, " ");
    while (token != NULL) {
        if (strcmp(word, token) == 0) {
            printf("----------------------------%s-------------------------------\n", token);
            return 1; 
        }
        token = strtok(NULL, ",");
    }


    return 0;
}
/*
   Checks if the given word is a valid label.
   @param word: The word to be checked.
   @param line: The line number where the word appears.
   @return: Returns 1 if the word is a valid label, 0 otherwise.
*/
int isAValidLabel(char *word,int line)
{
    if(strlen(word) > 31)
    {
        messageIssued("Problem in line %d - Label is too long\n", line);
        return 0;
    }
    else if (!isalpha(word[0]))
    {
        messageIssued("Problem in line %d - First char at label is not a char\n", line);
        return 0;
    }
    return 1;
}
/*
   Writes a formatted message to the error log file "errors.log".
   @param format: The format string for the message.
   @param ...: Additional arguments for the format string.
*/
void messageIssued(const char *format, ...) {
    FILE *error_file = fopen("errors.log", "a");
    va_list args;
    if (error_file == NULL) {
        printf("Error opening error log file errors.log.\n");
        return;
    }

    va_start(args, format);
    vprintf(format, args); 
    vfprintf(error_file, format, args); 
    va_end(args);

    if (ferror(error_file)) {
        printf("Error writing to error log file errors.log.\n");
    }

    fclose(error_file);
}
/*
   Removes the ".define" directive from the given string.
   @param str: The string to remove the directive from.
*/
void removeDefine(char *str) {
    char *ptr = strstr(str, ".define");
    if (ptr != NULL) {
        memmove(ptr, ptr + strlen(".define"), strlen(ptr + strlen(".define")) + 1);
    }
}
/*
   Checks if the given string has a space character in the middle (surrounded by non-space characters).
   @param str: The string to be checked.
   @return: Returns 1 if there is a space in the middle, 0 otherwise.
*/
int hasSpaceInMiddle(const char *str) {
    int i = 0;
    while (isspace(str[i])) {
        i++;
    }
    while (str[i] != '\0') {
        if (isspace(str[i]) && str[i+1] != '\0' && !isspace(str[i+1])) {
            return 1;
        }
        i++;
    }
    return 0;
}
/*
   Removes all space characters from the given string.
   @param str: The string to remove spaces from.
*/
void removeSpaces(char *str) {
    char *src = str, *dst = str;
    while (*src) {
        if (!isspace(*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}
/*
   Splits the given string into two parts before and after the first occurrence of '=' character.
   @param str: The string to be split.
   @param before_equal: The part of the string before '=' will be stored here.
   @param after_equal: The part of the string after '=' will be stored here.
   @return: Returns 1 if the string was successfully split, 0 otherwise.
*/
int splitString(char *str, char *before_equal, char *after_equal) {
    const char *equal_ptr;
    equal_ptr = strchr(str, '=');
    if (equal_ptr != NULL) {
        int before_len = equal_ptr - str;
        strncpy(before_equal, str, before_len);
        before_equal[before_len] = '\0';
        removeSpaces(before_equal);
        strcpy(after_equal, equal_ptr + 1);
        return 1;
    } else {
        strcpy(before_equal, str);
        after_equal[0] = '\0';
        return 0;
    }
}
/*
   Checks if the given string represents a valid assembly number within the range [-2047, 2047].
   @param str: The string to be checked.
   @return: Returns 1 if the string is a valid assembly number, 0 otherwise.
*/
int isAsmbliNumber(const char *str)
{
    int i = 0;
    int number;
    if (str == NULL || *str == '\0') 
        return 0;

    if (str[i] == '-' || str[i] == '+')
        i++;

    while (str[i]) {
        if (!isdigit(str[i])) {
            return 0;
        }
        
        if (str[i] == '.') {
            return 0;
        }
        
        if (str[i] == 'e' || str[i] == 'E') {
            return 0;
        }
        i++;
    }
    number = atoi(str);
    if (number <= -2047 || number >= 2048) {
        return 0;
    }
    return 1;
}
/*
   Removes all '#' characters from the given string.
   @param str: The string to remove '#' characters from.
*/
void removeHash(char *str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != '#') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}
/*
   Converts a binary string to its equivalent integer value within the specified range.
   @param binary: The binary string to be converted.
   @param start: The starting index of the range within the binary string.
   @param end: The ending index of the range within the binary string.
   @return: Returns the integer value represented by the binary string within the specified range.
            Returns -1 if the input is invalid or contains non-binary characters.
*/
int binaryToChar(char binary[], int start, int end) {
    int c = 0;
    int i;
    
    if (start < 0 || end < 0 || end <= start) {
        messageIssued("Invalid input\n");
        return -1; 
    }

    for (i = start; i <= end; i++) {
        if (binary[i] != '0' && binary[i] != '1') {
            messageIssued("Invalid binary string\n");
            return -1; 
        }

        if (binary[i] == '1') {
            c |= (1 << (end - i));
        }
    }
    return c;
}
/*
   Prints a symbol based on the given integer value.
   @param value: The integer value representing the symbol index.
   @return: Returns the symbol corresponding to the value.
            Returns '?' if the value is out of range.
*/
char printSymbol(int value) {
    const char symbols[] = "*#%!?";
    return (value >= 0 && value < 5) ? symbols[value] : '?';
}
/*
   Frees the memory allocated for a linked list of Bits nodes.
   @param head: Pointer to the head of the linked list.
*/
void freeList(Bits** head) {
    Bits* current;
    Bits* next;
    current = *head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
/*
   Frees the memory allocated for a linked list of variables nodes.
   @param head: Pointer to the head of the linked list.
*/
void freeVariableList( variables** head) {
     variables* current;
     variables* next;
    current = *head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
/*
   Removes all occurrences of a substring from the given string.
   @param str: The string to remove the substring from.
   @param sub: The substring to be removed.
*/
void removeSubstring(char *str, const char *sub) {
    int len = strlen(sub);
    while ((str = strstr(str, sub))) {
        memmove(str, str + len, strlen(str + len) + 1);
    }
}
/*
   Counts the number of instructions in a linked list of Bits nodes.
   @param head: Pointer to the head of the linked list.
   @return: Returns the count of instructions in the linked list.
*/
int countInstruction(Bits* head)
{
    int counter = 0;
    while (head != NULL){
        if(head->instruction == 1){
            counter++;
        }
        head = head->next;
    }
    return counter;
}
/*
   Counts the number of entries in a linked list of variables nodes.
   @param varsList: Pointer to the head of the linked list.
   @return: Returns the count of entries in the linked list.
*/
int countEntrys(variables* varsList)
{
    int counter = 0;
    while (varsList != NULL){
        if(varsList->typeOfUpdate == 1){
            counter++;
        }
        varsList = varsList->next;
    }
    return counter;
}
