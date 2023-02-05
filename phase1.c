#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

char file_name[50] = "", name[1000] = "", clipboard[10000] = "",last_char, *content, *input_str, *input_str2, grep_lines[1000][10000];
int col, row, isBack, length, grep_c = 0;

//**Errors:
void invalid_format() {
    printf("  > Invalid command format!\n");
    char c = getchar();
    while(c != '\n')
    c = getchar();
}

void invalid_name() {
    printf("  > Invalid command name!\n");
    char c = getchar();
    while(c != '\n')
    c = getchar();
}

void not_found_end(){
    printf("  > File does not exist in the path!\n");
}

void not_found_middle() {
    printf("  > File does not exist in the path!\n");
    char c = getchar();
    while(c != '\n')
    c = getchar();
}

void size_not_valid() {
    printf("  > This file doesn't contain this size of characters!\n");
}


//**Path:
void get_file_name() {
    strcpy(file_name,"");
    int k = 0;
    char tmp1[1000] = {""};
    for(int i = strlen(name) - 1; i >=0 ; i--)
    {
        if(name[i] == '/') break;
        tmp1[k] = name[i];
        k++;
    }
    for(int i = 0; i<= k; i++) {
        strncat(file_name, &tmp1[k - i], 1);
    }
}

int path_gen() {
    int res = 1;
    strcpy(file_name,"");
    strcpy(name,"");
    char format[50];
    char lttr = getchar();
    if(lttr == '"'){
        int file_rec = 0;
            lttr = getchar();
            if(lttr != '/') {
                invalid_format();
                res = 0;
            }
            else {
                lttr = getchar();
                while (lttr != '"') {
                    char tmp[50] = {""};
                    int i = 0;
                    while(lttr != '/') {
                        if(lttr == '"') {
                            file_rec = 1;
                            break;
                        }
                        else {
                            tmp[i] = lttr;
                            lttr = getchar();
                            i++;
                        }
                    }
                    if(file_rec == 1) {
                        strcpy(file_name,tmp);
                        strcat(name,tmp);
                        break;
                    }
                    else {
                        strcat(name,tmp);
                        mkdir(name);
                        strcat(name,"/");
                        lttr = getchar();
                    }
                }
            }
        }
        else {
        int file_rec = 0;
            if(lttr != '/') {
                invalid_format();
                res = 0;
            }
            else {
                lttr = getchar();
                while (lttr != '\n' && lttr != ' ') {
                    char tmp[50] = {""};
                    int i = 0;
                    while(lttr != '/') {
                        if(lttr == '\n' || lttr == ' ') {
                            file_rec = 1;
                            break;
                        }
                        else {
                            tmp[i] = lttr;
                            lttr = getchar();
                            i++;
                        }
                    }
                    if(file_rec == 1) {
                        strcpy(file_name,tmp);
                        strcat(name,tmp);
                        break;
                    }
                    else {
                        strcat(name,tmp);
                        mkdir(name);
                        strcat(name,"/");
                        lttr = getchar();
                    }
                }
            }
        }
    return res;
}

int path_get() {
    char path[500] = {""};
    char lttr = getchar();
    if(lttr == '"'){
            int i = 0;
            lttr = getchar();
            if(lttr != '/') {
                path[i] = lttr;
                i++;
            }
            lttr = getchar();
            while (lttr != '"') {
                path[i] = lttr;
                i++;
                lttr = getchar();
            }
        }
    else {
        int i = 0;
        if(lttr != '/') {
            path[i] = lttr;
            i++;
        }
        lttr = getchar();
        while (lttr != '\n' && lttr != ' ') {
            path[i] = lttr;
            i++;
            lttr = getchar();
        }
    }
    last_char = lttr;
    strcpy(name,path);
    get_file_name();
}

//**Read & Get info:
int read_content(char* path) {
    content = malloc(1000000);
    strcpy(content,"");
    char tmp[10000] = {""};
    int i = 0;
    char c;
    FILE* myFile;
    myFile = fopen(path, "r+");
    if (myFile) {
        while ((c = getc(myFile)) != EOF)
        {
            strncat(tmp, &c, 1);
        }
        fclose(myFile);
        strcpy(content,tmp);
        return 1;
    }
    else
        return 0;
}

void get_string() {
char str[1000] = {""};
    char c = getchar();
    if(c == '"'){
        c = getchar();
        while (c != '"') {
            if(c == '\\') {
                c = getchar();
                if(c == 'n') strcat(str,"\n");
                else strncat(str, &c, 1);
                c = getchar();
            }
            else {
                strncat(str, &c, 1);
                c = getchar();
            }
        }
        if(c = getchar() != ' ') invalid_format();
    }
    else {
        while (c != ' ') {
            if(c == '\\') {
                c = getchar();
                if(c == 'n') strcat(str,"\n");
                else strncat(str, &c, 1);
                c = getchar();
            }
            else {
                strncat(str, &c, 1);
                c = getchar();
            }
        }
    }
    input_str = malloc(1000000);
    strcpy(input_str,str);
}

void get_string_with_wildcard() {
char str[1000] = {""};
    char c = getchar();
    if(c == '"'){
        c = getchar();
        while (c != '"') {
            if(c == '\\') {
                c = getchar();
                if(c == 'n') strcat(str,"\n");
                else if(c == '*') strcat(str,"\\*");
                else strncat(str, &c, 1);
                c = getchar();
            }
            else {
                strncat(str, &c, 1);
                c = getchar();
            }
        }
        if(c = getchar() != ' ') invalid_format();
    }
    else {
        while (c != ' ') {
            if(c == '\\') {
                c = getchar();
                if(c == 'n') strcat(str,"\n");
                else if(c == '*') strcat(str,"\\*");
                else strncat(str, &c, 1);
                c = getchar();
            }
            else {
                strncat(str, &c, 1);
                c = getchar();
            }
        }
    }
    input_str = malloc(1000000);
    strcpy(input_str,str);
}

int pos_indx(int row, int col, char* path) {
    int counter = 0;
    char c;
    FILE* myFile;
    myFile = fopen(path, "r+");
    int row_cnt = 1;
    if(row != 1) {
        c = getc(myFile);
        while (c != EOF) {
            if(c == '\n') {
                row_cnt++;
                if(row_cnt == row) break;
            }
            c = getc(myFile);
            counter++;
        }
    }
    else counter--;
    if(row_cnt < row) {
        printf("  > This file doesn't have this much lines!\n");
        fclose(myFile);
        return -1;
    }
    else {
        counter++;
        for(int i = 0; i<col;i++) {
            c = getc(myFile);
            if(c == '\n' || c == EOF || c == '\0') {
                printf("  > This line doesn't have any characters in this position!\n");
                return -1;
            }
            counter++;
        }
        return counter;
    }
}

int get_removeCopyCut() {
    char format[50];
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--file") != 0)
            invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                path_get();
                FILE* myFile;
                myFile = fopen(name, "r+");
                if (myFile) {
                        fclose(myFile);
                        scanf("%s",format);
                        if(strcmp(format,"--pos") != 0)
                            invalid_format();
                        else {
                            if(getchar() != ' ') invalid_format();
                            else {
                                char x;
                                scanf("%d%c%d", &row, &x, &col);
                                if(x != ':') invalid_format();
                                else
                                {
                                    if(getchar() != ' ') invalid_format();
                                    else {
                                       scanf("%s",format);
                                        if(strcmp(format,"-size") != 0)
                                            invalid_format();
                                        else {
                                            if(getchar() != ' ') invalid_format();
                                            else {
                                                scanf("%d", &length);
                                                if(getchar() != ' ') invalid_format();
                                                else {
                                                    scanf("%s",format);
                                                    if(strcmp(format,"-b") == 0)
                                                    {
                                                        isBack = 1;
                                                        return 1;
                                                    }
                                                    else if(strcmp(format,"-f") == 0)
                                                    {
                                                        isBack = 0;
                                                        return 1;
                                                    }
                                                    else printf("  > Invalid command format!\n");
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                }
                else not_found_middle();
            }
        }
    }
    return 0;
}


//**Main Functions:
void createfile() {
    char format[50];
    strcpy(file_name,"");
    strcpy(name,"");
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--file") != 0)
            invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                if(path_gen() == 1) {
                    FILE* newFile;
                    if(fopen(name,"r")) {
                        printf("  > [%s] already exists in the path!\n",file_name);
                    }
                    else {
                        newFile = fopen(name,"w+");
                        printf("  > [%s] created in the following path: /%s\n",file_name,name);
                        fclose(newFile);
                    }
                }
            }
        }
    }
}

void cat() {
    char format[50];
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--file") != 0) invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                path_get();
                if(read_content(name) == 1){
                    if(strcmp(content,"") != 0) printf("%s\n",content);
                    else printf("  > This file is empty!\n");
                }
                else not_found_end();
            }
        }
    }
}

void write_in_pos(int row, int col, char* path, char* txt, int forInsert) {
    content = malloc(1000000);
    read_content(path);
    char undo_name[1000] = {""};
    strcpy(undo_name, "root/undo/");
    strcat(undo_name, file_name);
    FILE* newFile = fopen(undo_name, "w+");
    fputs(content, newFile);
    fclose(newFile);
    char tmp_before[10000] = {""}, tmp_after[10000] = {""};
    int pos_index = pos_indx(row,col,path);
    for(int i = 0; i<pos_index; i++) {
        tmp_before[i] = content[i];
    }
    int j = 0;
    for(int i = pos_index; i<strlen(content); i++) {
        tmp_after[j] = content[i];
        j++;
    }
    if(pos_index != -1) {
        FILE* newFile;
        newFile = fopen(path, "w+");
        strcpy(content, tmp_before);
        strcat(content, txt);
        strcat(content, tmp_after);
        fputs(content, newFile);
        fclose(newFile);
        if(forInsert == 1) printf("  > The string was inserted successfully.\n");
        else printf("  > The string was pasted successfully.\n");
    }

}

void insertstr() {
    char format[50];
    int row,col;
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--file") != 0)
            invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                path_get();
                FILE* myFile;
                myFile = fopen(name, "r+");
                if (myFile) {
                        fclose(myFile);
                        scanf("%s",format);
                        if(strcmp(format,"--str") != 0)
                            invalid_format();
                        else {
                            if(getchar() != ' ') invalid_format();
                            else {
                               get_string();
                               scanf("%s",format);
                                if(strcmp(format,"--pos") != 0)
                                    invalid_format();
                                else {
                                    if(getchar() != ' ') invalid_format();
                                    else {
                                        char x;
                                        scanf("%d%c%d", &row, &x, &col);
                                        if(x != ':') invalid_format();
                                        else
                                        {
                                            write_in_pos(row,col,name,input_str,1);
                                        }
                                    }
                                }
                            }
                        }
                }
                else not_found_middle();
            }
        }
    }
}

int removestr(int forRemove) {
    int res = 0;
    if(forRemove == 0) res = 1;
    else if (get_removeCopyCut() == 1) res = 1;
    else res = 0;
    if(res == 1) {
        int start, end;
        content = malloc(1000000);
        read_content(name);
        char undo_name[1000] = {""};
        strcpy(undo_name, "root/undo/");
        strcat(undo_name, file_name);
        FILE* newFile = fopen(undo_name, "w+");
        fputs(content, newFile);
        fclose(newFile);
        char tmp_before[10000] = {""}, tmp_after[10000] = {""};
        int pos_index = pos_indx(row,col,name);
        if(isBack == 1) {
            end = pos_index;
            start = pos_index - length - 1;
        }
        else {
            start = pos_index;
            end = pos_index + length + 1;
        }
        if(start < 0 || end > strlen(content)) size_not_valid();
        else {
            for(int i = 0; i<=start; i++) {
                tmp_before[i] = content[i];
            }
            int j = 0;
            for(int i = end; i<strlen(content); i++) {
                tmp_after[j] = content[i];
                j++;
            }
            if(pos_index != -1) {
                FILE* newFile;
                newFile = fopen(name, "w+");
                strcpy(content, tmp_before);
                strcat(content, tmp_after);
                fputs(content, newFile);
                fclose(newFile);
                if(forRemove == 1) printf("  > The string was removed successfully.\n");
                return 1;
            }
        }
    }
    else return 0;
}

//forCopy: makes sure that this is not used to cut string(print successs)
int copystr(int forCopy) {
    int res = 0;
    if(forCopy == 0) res = 1;
    else if (get_removeCopyCut() == 1) res = 1;
    else res = 0;
    if(res == 1) {
        int start, end;
        read_content(name);
        int pos_index = pos_indx(row,col,name);
        char tmp[1000] = {""};
        if(isBack == 1) {
            end = pos_index;
            start = pos_index - length - 1;
        }
        else {
            start = pos_index;
            end = pos_index + length + 1;
        }
        if(start < 0 || end > strlen(content)) size_not_valid();
        else {
            int j = 0;
            for(int i = start+1; i<end; i++) {
                tmp[j] = content[i];
                j++;
            }
            strcpy(clipboard, tmp);
            if(forCopy == 1) printf("  > The string was copied to clipboard successfully.\n");
            return 1;
        }
    }
    else return 0;
}

void cutstr() {
    if(get_removeCopyCut() == 1) {
        if(copystr(0) == 1 && removestr(0) == 1) {
            printf("  > The string was cut successfully.\n");
        }
    }
}

void pastestr() {
        char format[50];
        int row,col;
        if(getchar() != ' ') invalid_format();
        else {
            scanf("%s",format);
            if(strcmp(format,"--file") != 0)
                invalid_format();
            else {
                if(getchar() != ' ') invalid_format();
                else {
                    path_get();
                    FILE* myFile;
                    myFile = fopen(name, "r+");
                    if (myFile) {
                            fclose(myFile);
                            scanf("%s",format);
                            if(strcmp(format,"--pos") != 0)
                                invalid_format();
                            else {
                                if(getchar() != ' ') invalid_format();
                                else {
                                    char x;
                                    scanf("%d%c%d", &row, &x, &col);
                                    if(x != ':') invalid_format();
                                    else
                                    {
                                        if(strcmp(clipboard,"") == 0) printf("  > Clipboard is empty; nothing to paste!\n");
                                        else write_in_pos(row,col,name,clipboard,0);
                                    }
                                }
                            }
                    }
                    else not_found_middle();
                }
            }
        }
}

//return values: {1- b: *u test string / 2- e: test string u* / 3- n: not wild / 4- B: test string *h / 5- E: h* test string}
char is_wildcard() {
    for(int i = 0; i<strlen(input_str); i++) {
        if(input_str[i] == '*') {
            if(i == 0) return 'b';
            else if (input_str[i-1] != '\\' && i == (strlen(input_str) - 1)) return 'e';
            else if (input_str[i-1] != '\\' && input_str[i-1] == ' ') return 'B';
            else if (input_str[i-1] != '\\' && input_str[i+1] == ' ') return 'E';
        }
    }
    return 'n';
}

int find_at(int n, int isByWord, int for_replace) {
    //vaseye *, hatman aval , akhar kalame ro midam
    //f_line: first line wanted strings resulted 1 less word counter
    //is_new_line: It will ignore the extra spaces at the beginning of a line
    int counter = 0, words = 0, f_line = 1, is_new_line = 1,beginning_pos = 0, all_beginnings[1000], for_b = 0, for_e = 0, firs_beginning = 0, is_checking = 0, end_pos = 0;
    char wild_check = is_wildcard();
    if(wild_check == 'b') for_b = -1;
    if(wild_check == 'e') for_e = -1;
    for(int i = 0; i < strlen(content); i++) {
        if(content[i-1] == ' ' || content[i-1] == '\n') beginning_pos = i;
        if(content[i] == ' ' || content[i] == '\n') words ++;
        if(content[i] == '\n') {
            f_line = 0;
            is_new_line = 1;
        }
        if(is_new_line == 1 && content[i] == ' ') words--;
        else if(is_new_line == 1 && content[i] != ' '&& content[i] != '\n') is_new_line = 0;
        if(content[i] == input_str[0 + -1*(for_b)]) {
            int res = 1;
            if(wild_check == 'n' || wild_check == 'b' || wild_check == 'e') {
                for (int j = 1; j<strlen(input_str) + for_e; j++) {
                    if (content[i+j + for_b] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
                if(res == 1) {
                    end_pos = i + strlen(input_str) - 1 + for_b;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            else if(wild_check == 'E') {
                int k = 0;
                for (int j = 1; j<strlen(input_str); j++) {
                    if(input_str[j] == '*') {
                        j++;
                        while(content[i+j + k] != ' ') k++;
                    }
                    if (content[i+j + k] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
                if(res == 1) {
                    end_pos = i+k+strlen(input_str)-1;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            else if(wild_check == 'B') {
                //x: last letter index in content with *
                //y: length of input word letters after *
                int k = 0;
                int j = 1;
                int p = 0;
                int star_indx = 0, cnt = 0;
                while (j<strlen(input_str)) {
                    if(input_str[j] == '*') {
                        int y = 0;
                        j++;
                        while(content[i+j + k] != ' ') k++;
                        int x = i + j + k -1;
                        while(input_str[j] != ' ' && input_str[j] != '\0') {
                            y++;
                            j++;
                        }
                        for(int m = 0; m < y; m++) {
                            if(content[x - m] != input_str[j-m-1]) {
                                res = 0;
                                break;
                            }
                        }
                        if(res == 1) {
                            star_indx = x-y;
                            while(content[star_indx] != ' ') { cnt++; star_indx--;}
                            if(cnt == 1) p = -1;
                            else if(cnt>2) p =(cnt-2);
                        }
                        k = 1;
                    }
                    if(res == 0 || j == strlen(input_str))
                    {
                        break;
                    }
                    if (content[i+j + k + p] != input_str[j]) {
                        res = 0;
                        break;
                    }
                    j++;
                }
                if(res == 1)
                {
                    end_pos = i+k+strlen(input_str)-1 + p;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            if(res == 1) {
                int res2 = 1; //ignores repeated begining pos
                if(wild_check == 'b') {
                    for(int x = 0; x<counter; x++) {
                        if(all_beginnings[x] == beginning_pos) res2 = 0;
                    }
                }
                else if(wild_check == 'e') {
                    if (beginning_pos != i) res2 =0;
                }
                if(res2 == 1)
                {
                    all_beginnings[counter] = beginning_pos;
                    counter ++;
                    if (counter == n) {
                        if(for_replace == 0) {
                            if(isByWord == 0 && wild_check == 'n') {
                                return i;
                            }
                            else if(isByWord == 0 && (wild_check == 'b' || wild_check == 'e' || wild_check == 'E' || wild_check == 'B')) {
                                return beginning_pos;
                            }
                            else if (isByWord == 1){
                                if(f_line == 1) words ++;
                                return words;
                            }
                        }
                        else {
                            char tmp_before[10000] = {""}, tmp_after[10000] = {""};
                            for(int i = 0; i<beginning_pos; i++) {
                                tmp_before[i] = content[i];
                            }
                            int j = 0;
                            for(int i = end_pos+1; i<strlen(content); i++) {
                                tmp_after[j] = content[i];
                                j++;
                            }
                            FILE* newFile = fopen(name, "w+");
                            strcpy(content, tmp_before);
                            strcat(content, input_str2);
                            strcat(content, tmp_after);
                            fputs(content, newFile);
                            fclose(newFile);
                            printf("  > The string was replaced successfully.\n");
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return -1;
}

void find_all(int isByWord, int for_replace) {
    long long* indexes = (long long*)malloc(10000);
    int counter = 0, words = 0, f_line = 1, is_new_line = 1,beginning_pos = 0, all_beginnings[1000], for_b = 0, for_e = 0, firs_beginning = 0, is_checking = 0, end_pos = 0;
    char wild_check = is_wildcard();
    if(wild_check == 'b') for_b = -1;
    if(wild_check == 'e') for_e = -1;
    for(int i = 0; i < strlen(content); i++) {
        if(content[i-1] == ' ' || content[i-1] == '\n') beginning_pos = i;
        if(content[i] == ' ' || content[i] == '\n') words ++;
        if(content[i] == '\n') {
            f_line = 0;
            is_new_line = 1;
        }
        if(is_new_line == 1 && content[i] == ' ') words--;
        else if(is_new_line == 1 && content[i] != ' '&& content[i] != '\n') is_new_line = 0;
        if(content[i] == input_str[0 + -1*(for_b)]) {
            int res = 1;
            if(wild_check == 'n' || wild_check == 'b' || wild_check == 'e') {
                for (int j = 1; j<strlen(input_str) + for_e; j++) {
                    if (content[i+j + for_b] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
                if(res == 1) {
                    end_pos = i + strlen(input_str) - 1 + for_b;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            else if(wild_check == 'E') {
                int k = 0;
                for (int j = 1; j<strlen(input_str); j++) {
                    if(input_str[j] == '*') {
                        j++;
                        while(content[i+j + k] != ' ') k++;
                    }
                    if (content[i+j + k] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
                if(res == 1) {
                    end_pos = i+k+strlen(input_str)-1;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            else if(wild_check == 'B') {
                int k = 0;
                int j = 1;
                int p = 0;
                int star_indx = 0, cnt = 0;
                while (j<strlen(input_str)) {
                    if(input_str[j] == '*') {
                        int y = 0;
                        j++;
                        while(content[i+j + k] != ' ') k++;
                        int x = i + j + k -1;
                        while(input_str[j] != ' ' && input_str[j] != '\0') {
                            y++;
                            j++;
                        }
                        for(int m = 0; m < y; m++) {
                            if(content[x - m] != input_str[j-m-1]) {
                                res = 0;
                                break;
                            }
                        }
                        if(res == 1) {
                            star_indx = x-y;
                            while(content[star_indx] != ' ') { cnt++; star_indx--;}
                            if(cnt == 1) p = -1;
                            else if(cnt>2) p =(cnt-2);
                        }
                        k = 1;
                    }
                    if(res == 0 || j == strlen(input_str))
                    {
                        break;
                    }
                    if (content[i+j + k + p] != input_str[j]) {
                        res = 0;
                        break;
                    }
                    j++;
                }
                if(res == 1)
                {
                    end_pos = i+k+strlen(input_str)-1 + p;
                    while(content[end_pos] != ' ') end_pos++;
                    end_pos--;
                }
            }
            if(res == 1) {
                int res2 = 1; //ignores repeated begining pos
                if(wild_check == 'b') {
                    for(int x = 0; x<counter; x++) {
                        if(all_beginnings[x] == beginning_pos) res2 = 0;
                    }
                }
                else if(wild_check == 'e') {
                    if (beginning_pos != i) res2 =0;
                }
                if(res2 == 1)
                {
                    if(for_replace == 0) {
                        all_beginnings[counter] = beginning_pos;
                        if(isByWord == 0 && wild_check == 'n') {
                            indexes[counter] = i;
                        }
                        else if(isByWord == 0 && (wild_check == 'b' || wild_check == 'e' || wild_check == 'E' || wild_check == 'B')) {
                            indexes[counter] = beginning_pos;
                        }
                        else if (isByWord == 1){
                            if(f_line == 1)
                                indexes[counter] = words+ 1;
                            else indexes[counter] = words;
                        }
                    }
                    else {
                        char tmp_before[10000] = {""}, tmp_after[10000] = {""};
                        for(int i = 0; i<beginning_pos; i++) {
                            tmp_before[i] = content[i];
                        }
                        int j = 0;
                        for(int i = end_pos+1; i<strlen(content); i++) {
                            tmp_after[j] = content[i];
                            j++;
                        }
                        FILE* newFile = fopen(name, "w+");
                        strcpy(content, tmp_before);
                        strcat(content, input_str2);
                        strcat(content, tmp_after);
                        fputs(content, newFile);
                        fclose(newFile);
                    }
                    counter++;
                }
            }
        }
    }
    if(counter != 0) {
            if(for_replace == 0) {
                printf("  > %d",indexes[0]);
                for(int i = 1; i<counter; i++) printf(", %d",indexes[i]);
                printf("\n");
            }
            else printf("  > The string was replaced successfully.\n");
    }
    else {
        if(for_replace == 0) printf("  > -1\n");
        else printf("  > The string does not exist in the file!\n");
    }
}

int find_count(int for_grep) {
    int counter = 0, words = 0, f_line = 1, is_new_line = 1,beginning_pos = 0, all_beginnings[1000], for_b = 0, for_e = 0, firs_beginning = 0, is_checking = 0;
    char wild_check = is_wildcard();
    if(wild_check == 'b') for_b = -1;
    if(wild_check == 'e') for_e = -1;
    for(int i = 0; i < strlen(content); i++) {
        if(content[i-1] == ' ' || content[i-1] == '\n') beginning_pos = i;
        if(content[i] == ' ' || content[i] == '\n') words ++;
        if(content[i] == '\n') {
            f_line = 0;
            is_new_line = 1;
        }
        if(is_new_line == 1 && content[i] == ' ') words--;
        else if(is_new_line == 1 && content[i] != ' '&& content[i] != '\n') is_new_line = 0;
        if(content[i] == input_str[0 + -1*(for_b)]) {
            int res = 1;
            if(wild_check == 'n' || wild_check == 'b' || wild_check == 'e') {
                for (int j = 1; j<strlen(input_str) + for_e; j++) {
                    if (content[i+j + for_b] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
            }
            else if(wild_check == 'E') {
                int k = 0;
                for (int j = 1; j<strlen(input_str); j++) {
                    if(input_str[j] == '*') {
                        j++;
                        while(content[i+j + k] != ' ') k++;
                    }
                    if (content[i+j + k] != input_str[j]) {
                        res = 0;
                        break;
                    }
                }
            }
            else if(wild_check == 'B') {
                int k = 0;
                int j = 1;
                while (j<strlen(input_str)) {
                    if(input_str[j] == '*') {
                        int y = 0;
                        j++;
                        while(content[i+j + k] != ' ') k++;
                        int x = i + j + k -1;
                        while(input_str[j] != ' ' && input_str[j] != '\0') {
                            y++;
                            j++;
                        }
                        for(int m = 0; m < y; m++) {
                            if(content[x - m] != input_str[j-m-1]) {
                                res = 0;
                                break;
                            }
                        }
                        k = 1;
                    }
                    if(res == 0 || j == strlen(input_str))
                    {
                        break;
                    }
                    if (content[i+j + k] != input_str[j]) {
                        res = 0;
                        break;
                    }
                    j++;
                }
            }
            if(res == 1) {
                int res2 = 1;
                if(wild_check == 'b') {
                    for(int x = 0; x<counter; x++) {
                        if(all_beginnings[x] == beginning_pos) res2 = 0;
                    }
                }
                else if(wild_check == 'e') {
                    if (beginning_pos != i) res2 =0;
                }
                if(res2 == 1) {
                    counter ++;
                }
            }
        }
    }
    if(for_grep == 0) printf("  > %d\n", counter);
    return counter;
}

void find() {
    int at = 0, byword = 0, all = 0, count = 0, n;
    char format[50];
    int row,col;
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--str") != 0) invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                get_string_with_wildcard();
                scanf("%s",format);
                if(strcmp(format,"--file") != 0) invalid_format();
                else {
                    if(getchar() != ' ') invalid_format();
                    else {
                        path_get();
                        FILE* myFile;
                        myFile = fopen(name, "r+");
                        if (myFile) {
                            read_content(name);
                            char c = last_char;
                            while(c != '\n') {
                                scanf("%s",format);
                                if(strcmp(format, "-count") == 0) {
                                    count = 1;
                                }
                                else if(strcmp(format, "-at") == 0) {
                                    scanf(" %d", &n);
                                    at = 1;
                                }
                                else if(strcmp(format, "-byword") == 0) {
                                    byword = 1;
                                }
                                else if(strcmp(format, "-all") == 0) {
                                    all = 1;
                                }
                                else {
                                    invalid_format();
                                    break;
                                }
                                c = getchar();
                                if(c == '\n') break;
                                else if(c != ' ') {
                                    invalid_format();
                                    break;
                                }
                            }
                            if(at == 0 && all == 0 && byword == 0 && count == 0) printf("  > %d\n", find_at(1,0,0)); //No additional command; -at(1)
                            //Counter Focus:
                            else if (at == 0 && count == 1) find_count(0); // byword and all are optional here; counts the number of matching records
                            else if (at == 1 && all == 0 && count == 1) { //byword is optional
                                if (find_at(n,0,0) == -1) printf("  > 0\n"); //string not found; count = 0
                                else printf("  > 1\n"); //at always points to just one of the matching records; so count = 1
                            }
                            //At Focus:
                            else if(at == 1 && all == 0 && byword == 0 && count == 0) printf("  > %d\n", find_at(n,0,0));
                            else if(at == 1 && all == 0 && byword == 1 && count == 0) printf("  > %d\n", find_at(n,1,0));
                            //All Focus:
                            else if(at == 0 && all == 1 && byword == 0 && count == 0) find_all(0,0);
                            else if(at == 0 && all == 1 && byword == 1 && count == 0) find_all(1,0);
                            //Single Byword:
                            if(at == 0 && all == 0 && byword == 1 && count == 0) printf("  > %d\n", find_at(1,1,0)); //No additional command; -at(1)


                        }
                        else {
                            char c;
                            if (last_char == '"') {
                                if(c = getchar() == '\n') not_found_end;
                                else not_found_middle();
                            }
                            else if(last_char == '\n') not_found_end();
                            else not_found_middle();
                        }
                    }
                }
            }
        }
    }
}

void replace() {
    int at = 0, all = 0, n;
    char format[50];
    int row,col;
    input_str2 = (char*)malloc(10000);
    input_str = (char*)malloc(10000);
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--str1") != 0) invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                get_string_with_wildcard();
                strcpy(input_str2, input_str);
                scanf("%s",format);
                if(strcmp(format,"--str2") != 0) invalid_format();
                else {
                    if(getchar() != ' ') invalid_format();
                    else {
                        get_string();
                        char temp[1000000] = {""};
                        strcpy(temp, input_str2);
                        strcpy(input_str2, input_str);
                        strcpy(input_str, temp);
                        scanf("%s",format);
                        if(strcmp(format,"--file") != 0) invalid_format();
                        else {
                            if(getchar() != ' ') invalid_format();
                            else {
                                path_get();
                                FILE* myFile;
                                myFile = fopen(name, "r+");
                                if (myFile) {
                                    read_content(name);
                                    char undo_name[1000] = {""};
                                    strcpy(undo_name, "root/undo/");
                                    strcat(undo_name, file_name);
                                    FILE* newFile = fopen(undo_name, "w+");
                                    fputs(content, newFile);
                                    fclose(newFile);
                                    char c = last_char;
                                    while(c != '\n') {
                                        scanf("%s",format);
                                        if(strcmp(format, "-at") == 0) {
                                            scanf(" %d", &n);
                                            at = 1;
                                        }
                                        else if(strcmp(format, "-all") == 0) {
                                            all = 1;
                                        }
                                        else {
                                            invalid_format();
                                            break;
                                        }
                                        c = getchar();
                                        if(c == '\n') break;
                                        else if(c != ' ') {
                                            invalid_format();
                                            break;
                                        }
                                    }

                                    if(at == 0 && all == 0) {if(find_at(1,0,1)==-1) printf("  > The string does not exist in the file!\n");} //No additional command; -at(1)
                                    else if(at == 1 && all == 0) {if(find_at(n,0,1) == -1) printf("  > The string does not exist in the file!\n");}
                                    else if(at == 0 && all == 1) find_all(0,1);
                                }
                                else {
                                    char c;
                                    if (last_char == '"') {
                                        if(c = getchar() == '\n') not_found_end;
                                        else not_found_middle();
                                    }
                                    else if(last_char == '\n') not_found_end();
                                    else not_found_middle();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void auto_indent() {
    char txt[10000] = {""};
    if(getchar() != ' ') invalid_format();
    else {
        path_get();
        FILE* myFile;
        myFile = fopen(name, "r+");
        if (myFile) {
            read_content(name);
            char undo_name[1000] = {""};
            strcpy(undo_name, "root/undo/");
            strcat(undo_name, file_name);
            FILE* newFile = fopen(undo_name, "w+");
            fputs(content, newFile);
            fclose(newFile);
            int i = 0, k = 0;
            char c = content[k];
            int open_counter = 0;
            int is_open = 1;
            while (k < strlen(content)) {
                if(c == '{') {
                    is_open = 1;
                    open_counter ++;
                    txt[i] = c;
                    i++;
                    txt[i] = '\n';
                    i++;
                    for(int j=0; j<(4*open_counter); j++) {
                        txt[i] = ' ';
                        i++;
                    }
                }
                else if (c == '}') {
                    is_open = 0;
                    open_counter --;
                    if(open_counter < 0) break;
                    txt[i] = '\n';
                    i++;
                    for(int j=0; j<(4*open_counter); j++) {
                        txt[i] = ' ';
                        i++;
                    }
                    txt[i] = c;
                    i++;
                }
                else {
                    if(is_open == 1 && content[k] == ' ') {
                        is_open = 1;
                    }
                    else {
                        is_open = 0;
                        txt[i] = c;
                        i++;
                    }
                }
                k++;
                c = content[k];
            }
            fputs(txt, myFile);
            fclose(myFile);
            printf("  > The file was rearranged successfully.\n");
        }
        else not_found_end();
    }
}

void undo() {
    char format[50];
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"--file") != 0) invalid_format();
        else {
            if(getchar() != ' ') invalid_format();
            else {
                path_get();
                if(read_content(name) == 1){
                    char tmp[10000] = {""};
                    strcpy(tmp,content);
                    char undo_name[1000] = {""};
                    strcpy(undo_name, "root/undo/");
                    strcat(undo_name, file_name);
                    read_content(undo_name);
                    FILE* newFile = fopen(name, "w+");
                    fputs(content, newFile);
                    fclose(newFile);
                    FILE* undoFile = fopen(undo_name, "w+");
                    fputs(tmp, undoFile);
                    fclose(undoFile);
                    printf("  > The undo command was successful.\n");
                }
                else not_found_end();
            }
        }
    }
}

int line_counter(char* path) {
    int ln_counter = 0, f_end = 0;
    FILE* myFile = fopen(path, "r+");
    char c;
    while(1) {
        while ((c = getc(myFile)) != '\n')
        {
            if(c == EOF) {
                f_end = 1;
                break;
            }
        }
        ln_counter++;
        if(f_end == 1) break;
    }
    return ln_counter;
}

void compare() {
    char format[50];
    char* fname1 = (char*)malloc(500);
    char* fname2 = (char*)malloc(500);
    strcpy(fname1, "");
    strcpy(fname2, "");
    if(getchar() != ' ') invalid_format();
    else {
        path_get();
        if(read_content(name) == 1){
            strcpy(fname1, name);
            path_get();
            if(read_content(name) == 1) {
                strcpy(fname2, name);
                int f1_end = 0, f2_end = 0, ln_counter = 0;
                FILE* myFile1 = fopen(fname1, "r+");
                FILE* myFile2 = fopen(fname2, "r+");
                char c;
                while(1) {
                    char* l1 = (char*)malloc(10000);
                    strcpy(l1, "");
                    while ((c = getc(myFile1)) != '\n')
                    {
                        if(c == EOF) {
                            f1_end = 1;
                            break;
                        }
                        strncat(l1, &c, 1);
                    }
                    char* l2 = (char*)malloc(10000);
                    strcpy(l2, "");
                    while ((c = getc(myFile2)) != '\n')
                    {
                        if(c == EOF) {
                            f2_end = 1;
                            break;
                        }
                        strncat(l2, &c, 1);
                    }
                    ln_counter++;
                    if(strcmp(l1,l2) != 0) {
                        printf("============ #%d ============\n",ln_counter);
                        printf("%s\n",l1);
                        printf("%s\n",l2);
                    }
                    if(f1_end == 1 || f2_end == 1) break;
                }
                if(f1_end == 0) {
                    printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",ln_counter+1, line_counter(fname1));
                    while(1) {
                        char* l1 = (char*)malloc(10000);
                        strcpy(l1, "");
                        while ((c = getc(myFile1)) != '\n')
                        {
                            if(c == EOF) {
                                f1_end = 1;
                                break;
                            }
                            strncat(l1, &c, 1);
                        }
                        ln_counter++;
                            printf("%s\n",l1);
                        if(f1_end == 1) break;
                    }
                }
                if(f2_end == 0) {
                    printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",ln_counter+1, line_counter(fname2));
                    while(1) {
                        char* l2 = (char*)malloc(10000);
                        strcpy(l2, "");
                        while ((c = getc(myFile2)) != '\n')
                        {
                            if(c == EOF) {
                                f2_end = 1;
                                break;
                            }
                            strncat(l2, &c, 1);
                        }
                        ln_counter++;
                            printf("%s\n",l2);
                        if(f2_end == 1) break;
                    }
                }
                fclose(myFile1);
                fclose(myFile2);
            }
            else not_found_end();
        }
        else not_found_middle();
    }
}

void grep_ln() {
    FILE* myFile = fopen(name, "r+");
    char c;
    int f_end = 0;
    content = (char*)malloc(10000);
    while(1) {
        char* ln = (char*)malloc(10000);
        strcpy(ln, "");
        while ((c = getc(myFile)) != '\n')
        {
            if(c == EOF) {
                f_end = 1;
                break;
            }
            strncat(ln, &c, 1);
        }
        strcpy(content,ln);
        if(find_count(1) > 0) {
            strcpy(grep_lines[grep_c], ln);
            grep_c ++;
        }
        if(f_end == 1) break;
    }
}

void grep() {
    char format[50], file_names[100][1000], res_names[100][1000];
    strcpy(grep_lines, "");
    int res = 1,is_c = 0, is_l = 0, l_cnt = 0;
    if(getchar() != ' ') invalid_format();
    else {
        scanf("%s",format);
        if(strcmp(format,"-c") == 0) {
            is_c = 1;
            scanf("%s",format);
            if(strcmp(format,"--str") != 0) invalid_format();
        }
        else if(strcmp(format,"-l") == 0) {
            is_l = 1;
            scanf("%s",format);
            if(strcmp(format,"--str") != 0) invalid_format();
        }
        else if(strcmp(format,"--str") != 0) {
            invalid_format();
            res = 0;
        }
        if(res == 1) {
            if(getchar() != ' ' && is_l == 0 && is_c == 0) invalid_format();
            else {
                get_string();
                scanf("%s",format);
                if(strcmp(format,"--files") != 0) invalid_format();
                else {
                    if(getchar() != ' ') invalid_format();
                    else {
                        char c = last_char;
                        int i = 0;
                        while(c != '\n') {
                            path_get();
                            grep_ln();
                            strcpy(file_names[i],name);
                            if(read_content(name) == 1) {
                                if(find_count(1) > 0) {
                                    strcpy(res_names[l_cnt],name);
                                    l_cnt ++;
                                }
                            }
                            else
                                not_found_middle();
                            i++;
                            c = last_char;
                            if(c == '\n') break;
                            else if(c != ' ') {
                                invalid_format();
                                break;
                            }
                        }
                        if(is_l == 1) {
                            for(int x = 0; x<= l_cnt; x++) {
                                if(strcmp(res_names[x], "") != 0) printf("%s\n", res_names[x]);
                            }
                            for(int x = 0; x<= l_cnt; x++) strcpy(res_names[x], "");
                        }
                        else if(is_c == 1) {
                            printf("  > %d\n", grep_c);
                            grep_c = 0;
                        }
                        else {
                            for(int x = 0; x< grep_c; x++) {
                                    if(strcmp(grep_lines[x], "") != 0) printf("%s\n", grep_lines[x]);
                            }
                            for(int x = 0; x< grep_c; x++) strcpy(grep_lines[x], "");
                        }
                        last_char = '\0';
                    }
                }
            }
        }
        else invalid_format();
    }
}

void tree(char *path, int tmp, int depth)
{
    struct dirent *d;
    DIR *dir = opendir(path);
    char tmp_path[10000];
    if (!dir) return;
    while ((d = readdir(dir)) != NULL)
    {
        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
        {
            for (int i = 0; i < 2 * tmp; i++)
            {
                if (i % 2 == 0)
                    printf("%c", '|');
                else
                    printf(" ");
            }
            printf("%s%s\n", "|__", d->d_name);
            if (depth == -2 || tmp < depth)
            {
                int j;
                int n = strlen(path);
                for (j = 0; j < n; j++)
                    tmp_path[j] = path[j];
                tmp_path[j] = '/';
                j++;
                int len = strlen(d->d_name);
                int i;
                for (i = 0; i < len; i++)
                    tmp_path[j + i] = d->d_name[i];
                tmp_path[j + i] = '\0';
                tree (tmp_path ,tmp + 1, depth);
            }
        }
    }
    closedir(dir);
}

int main()
{
    mkdir("root");
    mkdir("root/undo");
    char command[100];
    printf("~ ");
    scanf("%s",command);
    while (strcmp(command, "ESC") != 0)
    {
        if(strcmp(command,"createfile") == 0)
            createfile();
        else if(strcmp(command,"insertstr") == 0)
            insertstr();
        else if(strcmp(command,"cat") == 0)
            cat();
        else if(strcmp(command,"removestr") == 0)
            removestr(1);
        else if(strcmp(command,"copystr") == 0)
            copystr(1);
        else if(strcmp(command,"cutstr") == 0)
            cutstr();
        else if(strcmp(command,"pastestr") == 0)
            pastestr();
        else if(strcmp(command,"find") == 0)
            find();
        else if(strcmp(command,"replace") == 0)
            replace();
        else if(strcmp(command,"grep") == 0)
            grep();
        else if(strcmp(command,"undo") == 0)
            undo();
        else if(strcmp(command,"auto-indent") == 0)
            auto_indent();
        else if(strcmp(command,"compare") == 0)
            compare();
        else if(strcmp(command,"tree") == 0) {
            int depth;
            scanf(" %d", &depth);
            tree("root", 0, depth);
        }
        else
            invalid_name();
        printf("\n~ ");
        scanf("%s",command);
    }
    return 0;
}
