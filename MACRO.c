#include "main.h"

/*
    Creates a new macro node with the given macro name.
    @param macro_name: The name of the macro to be assigned to the node.
    @return: Returns a pointer to the newly created macro node.
*/
struct macro* create_macro_Node(char macro_name[MAX_LINE_LENGTH]) {
    struct macro* macro_node = (struct macro*)malloc(sizeof(struct macro));
    if (macro_node == NULL) {
        printf("Memory allocation failed in macro.\n");
    }
    strcpy(macro_node->name_of_macro, macro_name); /* copy the name of the macro*/
    macro_node->next_macro = NULL;
    macro_node->macro_info = NULL;
    return macro_node;
}
/*
    Inserts a new macro node at the beginning of the macro linked list.
    @param headRef: Pointer to the pointer to the head of the macro linked list.
    @param macro_name: The name of the macro to be inserted.
*/
void insertAtBeginning(struct macro** headRef, char macro_name[MAX_LINE_LENGTH]) {
    struct macro* macro_node = create_macro_Node(macro_name);
    macro_node->next_macro = *headRef;
    *headRef = macro_node;
}
/*
    Deletes the entire macro linked list.
    @param headRef: Pointer to the pointer to the head of the macro linked list.
*/
void deleteList(struct macro** headRef) {
    struct macro* current = *headRef;
    struct macro* next;
    while (current != NULL) {
        next = current->next_macro;
        free(current);
        current = next;
    }
    *headRef = NULL; 
}
/*
    Main function for processing macros in a source file.
    @param file_name: The name of the source file containing macros.
    @return: Returns 1 upon successful completion.
*/
int main_macro(char* file_name, char* macrosNames){
    int pass = 0;
    FILE *input;
    FILE *output;
    /* reading the the file with the macro sings*/
    input = fopen(file_name, "r");
    if (!input){
        printf("Error Macro: can not open the sorce file");
        return 1;
    }
    output = fopen("after_macro.am", "w");
    if (!output){
        printf("Error Macro: can not open the output file");
        return 1;
    }
    /* here we starting to deploy the macros to the output file*/
    pass = maco_deploy(input, output, macrosNames);
    fclose(input);
    fclose(output);
    return pass;
}
/*
    Inserts a new line of text into the macro's information.
    @param Macros: Pointer to the macro node.
    @param line: The line of text to be inserted.
    @return: Returns 1 upon successful insertion, 0 otherwise.
*/
int insertLine(macro *Macros, char *line) {
    int current_length,new_line_length;
	char *new_macro_info;
	if (Macros == NULL || line == NULL) {
        printf("Error: Invalid macro or line.\n");
        return 0;
    }
    current_length = (Macros->macro_info == NULL) ? 0 : strlen(Macros->macro_info);
    new_line_length = strlen(line);
    new_macro_info = (char *)malloc(current_length + new_line_length + 1);
    if (new_macro_info == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }
    if (current_length > 0) {
        strcpy(new_macro_info, Macros->macro_info);
    } else {
        new_macro_info[0] = '\0'; 
    }
    strcat(new_macro_info, line);
    free(Macros->macro_info);
    Macros->macro_info = new_macro_info;
    return 1;
}
/*
    Deploys macros from the input file to the output file.
    @param input: Pointer to the input file.
    @param output: Pointer to the output file.
    @return: Returns 0 upon successful deployment.
*/
int maco_deploy(FILE *input, FILE *output, char* macrosNames){
    const char delimiter[] = " \n\t";
    char line_read[MAX_LINE_LENGTH];                  /* read line by linr from the input - will be tokenize*/
    char line_before_token[MAX_LINE_LENGTH];          /* save the line before it tokenize*/
    char *point_to_word;                        /* point to each word in the line*/
    int endmcrflag = 0;
    int point_to_word_length = 0;               /* to cmopere for str*n*cmp*/
    char temp_macro_name[MAX_LINE_LENGTH];            /* temp place to put the macro name*/
    int in_macro = 0;                           /* flag for us to know if we are in a macro defenation*/
    int num_of_macros = 0;                      /* the numbers of the macros in the file (also num of the nodes in the linked list)*/
    int macro_pass = 0;                         /* flag to know if we are in the word "endmcr" and we can pss it*/
    struct macro* headRef = NULL;               /* head for the macro linked list*/
    macro* temp_head_node = NULL;               /* other pointer to loop on all the macro list*/
    while (fgets(line_read, sizeof(line_read), input) != NULL){         /*read line by line*/
        strcpy(line_before_token, line_read);                           /*save the line before token*/
        point_to_word = strtok(line_read, delimiter);                         /*token the line*/
        while (point_to_word != NULL)                                   /*as long as the word is not null (line sill has word in it)*/
        {
            if (!in_macro){                                             /*if we are not in macro defenation*/
                if (strcmp(point_to_word, "mcr") == 0){                 /*if the next word is mcr*/
                    point_to_word = strtok(NULL, delimiter);                  /*go to next word, this is the macro name*/
                    strcpy(temp_macro_name, point_to_word);             /*save the macro name in temp variable*/
                    if (isASavedWord(temp_macro_name,NULL) == 1){     /*check if the macro name is valid name*/
                        printf("You are trying to deploy a macro name that is not allowed.\n");
                        return 1;
                    }             
                    in_macro = 1;                                       /*rise flag of in make defenation*/
                    num_of_macros++;                                    /*add 1 to macro sum*/
                    insertAtBeginning(&headRef, point_to_word);         /*add the macro to the linked list (first place)*/
                    break;                                              /*go to the next line*/
                }
            }
            if (strcmp(point_to_word, "endmcr") == 0){              /*if the word is endmcr -------- need to change to strcmp...*/ /*וגם ?*/
                in_macro = 0;                                           /*we are not in macro any more*/
                break;                                                  /*go to next line*/
            }
            if (in_macro){                                              /*beceuse we are in macro defentaion, save it in the node*/
                insertLine(headRef,line_before_token);
                break;                                                  /*go to next line*/
            }
            point_to_word = strtok(NULL, delimiter);                          /*move to next word*/
        }
    /*at this point we have all the macros in linked list while each node has name of macro and the macro info*/
    }
    fseek(input, 0, SEEK_SET);                                      /*go back to the start of the input file*/
    in_macro = 0;                                                   /*we are not in macro defenation*/
    while (fgets(line_read, sizeof(line_read), input) != NULL){     /*read line by line*/
        macro_pass = 0;                                            /*reset after macro print every time*/
        endmcrflag = 0;
        strcpy(line_before_token, line_read);                       /*save the line before token*/
        point_to_word = strtok(line_read, delimiter);                     /*token the line*/
        while (point_to_word != NULL)                               /*as long as the word is not null (line sill has word in it)*/
        {
            if (in_macro){                                          /*if we are in a macro defenation*/
                if (strcmp(point_to_word, "endmcr") == 0){      /*if the word is endmcr its the end of macro defenation, need to chane to strcmp..*/
                    endmcrflag = 1;
                    in_macro = 0;                                   /*we are not in macro defenation any more*/
                }
                break;                                              /*go to next line*/
            }
            if (strcmp(point_to_word, "mcr") == 0){             /*if the word is mcr its the start of macro defenation, need to chane to strcmp..*/
                in_macro = 1;                                       /*rise flag in macro*/
                break;                                              /*go to next line*/
            }
            temp_head_node = headRef;                               /*point to head node of the linked list*/
            while (temp_head_node != NULL){                         /*as long as we are not int the end of the linked list*/
                point_to_word_length = strlen(point_to_word);       /*save the size of point_to_word to compere*/
                if(strncmp(temp_head_node->name_of_macro, point_to_word, point_to_word_length) == 0){ /*check for every word in the text if it is a mcro nane*/
                    fputs(temp_head_node->macro_info, output);      /*if yes ? print to the output file the node info*/
                    macro_pass = 1;                                 /*rise the flag that the macro has been printed*/
                }
                temp_head_node = temp_head_node->next_macro;        /*go to the next node until null*/
            }
            point_to_word = strtok(NULL, delimiter);                      /*go to the next word*/
        }
        if (!macro_pass && !in_macro && endmcrflag == 0){  /*if we are after printed macro of after macro defenation, skip line*/
            fputs(line_before_token, output); /*if not, ptint the line to the output file*/
        }
    }


    temp_head_node = headRef;
    macrosNames[0] = '\0';
    while (temp_head_node != NULL)
    {
        macrosNames = (char*)realloc(macrosNames,strlen(temp_head_node->name_of_macro));
        strcat(macrosNames, temp_head_node->name_of_macro);
        strcat(macrosNames, " ");
        temp_head_node = temp_head_node->next_macro;
    }
    macrosNames = (char*)realloc(macrosNames,1);
    macrosNames[strlen(macrosNames)] = '\0';
    deleteList(&headRef); /*delete linked list*/
    return 0;
}