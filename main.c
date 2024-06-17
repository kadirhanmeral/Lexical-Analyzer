#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isKeyword(char* str)
{
    const char* keywords[]
    = {"int","text","is","loop","times","read","write","newLine"};
    for (int i = 0;
        i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}
bool isDelimiter(char chr)
{
    return (chr == ' ' || chr == '+' || chr == '-'
            || chr == '*' || chr == '/' || chr == '.'
            || chr == ';' || chr == '%' || chr == '>'
            || chr == '<' || chr == '=' || chr == '('
            || chr == ')' || chr == '[' || chr == ']'
            || chr == '{' || chr == '}'|| chr == ','||chr=='"');
}

bool isOperator(char chr)
{
    return (chr == '+' || chr == '-' || chr == '*'
            || chr == '/' ) ;
}

bool isValidIdentifier(char* str,int num,bool* error2,bool* error3,FILE* dosya)
{
    *error2 = false;
    *error3 = false;
    if ((!isKeyword(str) && str[0] != '0' && str[0] != '1' && str[0] != '2'
            && str[0] != '3' && str[0] != '4'
            && str[0] != '5' && str[0] != '6'
            && str[0] != '7' && str[0] != '8'
         && str[0] != '9' && str[0]!='"' && str[0]!='.' && str[0]!=',' && !isDelimiter(str[0]))){
        
        if (strlen(str)<10){
            
        }
        else{
            if (num==1) {
                *error2 = true;
                fprintf(stderr, "Lexical error: Identifier cannot be longer than 10 characters!\n");
                fprintf(dosya,"Lexical error: Identifier cannot be longer than 10 characters!\n");
                return false;
            }

        }
        return  true;
        
        
    }
    
    else{
        if (num==1) {
            *error3 = true;
            fprintf(stderr, "Lexical error: Identifier should start with an alphabetic character!\n");
            fprintf(dosya,"Lexical error: Identifier should start with an alphabetic character!\n");

        }
    }
    
    return false;
    
    
    
}

bool isInteger(char* str,int num,bool* error)

{
    *error = false;
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    while (isdigit(str[i])) {
        i++;
    }
    if (i == strlen(str)) {
        if ( i> 8){
            *error = true;

        }
        return true;
    }
    
    
    return false;
    
}



bool isEndOfLine(char* str,char* input){
    if (str[0] == '.'){
        
        return true;
    }
    return false;
}

bool isComma(char* str){
    if (str[0] == ','){
        
        return true;
    }
    return false;
    
}


bool isString(char* str,bool* error,FILE* dosya)
{
    *error=false;
   
    if (str[0]=='"'){
        int i = 1;
        while (str[i]!='"') {
            i++;
        }
        if (i>255){
            fprintf(stderr, "Lexical error: String constants cannot be longer than 256 characters!\n");
            fprintf(dosya,"Lexical error: String constants cannot be longer than 256 characters!\n");
            *error = true;
            
        }

        return true;
    }
    return false;
    
}


char* getSubstring(char* str, int start, int end)
{
    int subLength = end - start + 1;
    char* subStr = (char*)malloc((subLength + 1) * sizeof(char));
    strncpy(subStr, str + start, subLength);
    subStr[subLength] = '\0';
    return subStr;
}

int lexicalAnalyzer(char* input,FILE *dosya)
{
    int left = 0, right = 0;
    int right2=0;
    long len = strlen(input);
    bool minus = false;
    bool check =false;
    int x = 0;
    char* subStr2 = (char*)malloc((strlen(input)) * sizeof(char));
    bool error = false;
    bool error1 = false;
    bool error2 = false;
    bool error3 = false;
   
    while (right <= len && left <= right) {
        x ++;
        if (!isDelimiter(input[right]))
            right++;
        
        if (input[right] == '/' && input[right + 1] == '*') {
                right += 2;
                
                while (right < len && (input[right] != '*' || input[right + 1] != '/')) {
                    right++;
                }
                
                if (right >= len) {
                 
                    printf("Lexical Error: Unclosed comment.\n");
                    fprintf(dosya, "Lexical Error: Unclosed comment.\n");
                    break;; // İşlemi sonlandır.
                }
                char* subStr = getSubstring(input, left+2, right-1);
                
                right++;
                right++;
                left=right;

                printf("Comment(%s)\n", subStr);
                fprintf(dosya, "Comment(%s)\n", subStr);
                
            }
        if (input[right]=='"'){
            right++;
            
            while(input[right]!='"'){
                right++;
                if (right==len-1) {
                    fprintf(stderr, "Lexical error: Unterminated string constant!\n");
                    fprintf(dosya,"Lexical error: Unterminated string constant!\n");
                    error = true;
                    break;
                }
            }
            char* subStr = getSubstring(input, left, right);
            right++;
            left=right;
            
            if (error==true) {
                break;
            }
            if (isString(subStr,&error,dosya)){
                if (error == false) {
                    printf("StringConst(%s)\n",subStr);
                    fprintf(dosya,"StringConst(%s)\n", subStr);
                }
                else{
                    break;
                }
                
            }

        }

        if (isDelimiter(input[right]) && left == right) {
            if (isOperator(input[right])){
                if(input[right]=='-'){
                    
                    minus = true;
                    if (isOperator(input[right-1])||(isOperator(input[right-2])&&input[right-1]=='(')) {
                        check =true;
                        
                    }
                    
                }
                else{
                    printf("Operator(%c)\n", input[right]);
                    fprintf(dosya,"Operator(%c)\n", input[right]);
                }
                if((input[right-1]=='-' && check==true) || (input[right-2]=='-' && input[right-1]=='(' && check==true)){
                    
                    printf("Operator(%c)\n", '-');
                    fprintf(dosya,"Operator(%c)\n", '-');
                    
                }
            
 
            }
               
            else if (input[right]=='{'){
                printf("LeftCurlyBracket\n");
                fprintf(dosya,"LeftCurlyBracket\n");
            }
           else if (input[right] == '}') {
            printf("RightCurlyBracket\n");
            fprintf(dosya, "RightCurlyBracket\n");
          } else if (input[right] == '(') {
            printf("LeftRoundBracket\n");
            fprintf(dosya, "LeftRoundBracket\n");
          } else if (input[right] == ')') {
            printf("RightRoundBracket\n");
            fprintf(dosya, "RightRoundBracket\n");
          } else if (input[right] == '[') {
            printf("LeftSquareBracket\n");
            fprintf(dosya, "LeftSquareBracket\n");
          } else if (input[right] == ']') {
            printf("RightSquareBracket\n");
            fprintf(dosya, "RightSquareBracket\n");
          }
          
          else if (input[right] == '.') {
            
              if (right==strlen(input)-1) {
                  
                  printf("EndOfLine\n");
                  fprintf(dosya, "EndOfLine\n");
              }
              else{
                  printf("Lexical error: There is a character '.' in the expression!\n");
                  fprintf(dosya,"Lexical error: There is a character '.' in the expression!\n");

                  break;
              }
            
          } else if (input[right] == ',') {
              printf("Comma\n");
              fprintf(dosya, "Comma\n");
          }
            right2=right;

            right++;
            left = right;
        }

        else if ((isDelimiter(input[right]) && left != right)
                || (right == len && left != right )) {
            
            char* subStr = getSubstring(input, left, right - 1);
            
            if (isKeyword(subStr)){
                printf("Keyword(%s)\n", subStr);
                fprintf(dosya,"Keyword(%s)\n", subStr);
            }
                

            
            else if ((isInteger(subStr,1,&error1) && error==false) ||(isValidIdentifier(subStr,1,&error2,&error3,dosya) && !isDelimiter(input[right - 1]) && error2==false &&error3==false)){
                
                
               
                if((minus==true && isInteger(subStr2,1,&error)&& error==false)||(minus==true && isValidIdentifier(subStr2,0,&error2,&error3,dosya) && error2==false && error3 == false && !isDelimiter(input[right - 1]))){
                    
                    if(check==false){
                        printf("Operator(%c)\n", '-');
                        fprintf(dosya,"Operator(%c)\n", '-');
                        minus= false;
                        if (isInteger(subStr,0,&error1)) {
                            if (error1==false) {
                                printf("IntConst(%s)\n", subStr);
                                fprintf(dosya,"IntConst(%s)\n", subStr);
                            }
                            else{
                                printf("Lexical error: Integer const cannot be longer than 8 digits!\n");
                                fprintf(dosya,"Lexical error: Integer const cannot be longer than 8 digits!\n");

                                break;
                            }
                            
                        }
                        else if(isValidIdentifier(subStr2,1,&error2,&error3,dosya) && !isDelimiter(input[right - 1])&&error2==false &&error3==false){
                            if (error2==false && error3 ==false) {
                                printf("Identifier(%s)\n", subStr);
                                fprintf(dosya,"Identifier(%s)\n", subStr);
                            }
                            
                            
                        }
                        
                    }
                    else{
                        if (isInteger(subStr,1,&error)) {
                            printf("IntConst(%d)\n",0);
                            fprintf(dosya,"IntConst(%d)\n",0);

                            check=false;
                        }
                        else{
                            printf("Operator(%c)\n", '-');
                            fprintf(dosya,"Operator(%c)\n", '-');
                            printf("Identifier(%s)\n", subStr);
                            fprintf(dosya,"Identifier(%s)\n", subStr);
                        }
                        
                    }
                    
                }
                else if( (minus==true && !isInteger(subStr2,1,&error)) ){
                    if (isInteger(subStr,1,&error)) {
                        printf("IntConst(%d)\n",0);
                        fprintf(dosya,"IntConst(%d)\n",0);

                        minus=false;
                    }
                    else{
                        printf("Operator(%c)\n", '-');
                        fprintf(dosya,"Operator(%c)\n", '-');
                        printf("Identifier(%s)\n", subStr);
                        fprintf(dosya,"Identifier(%s)\n", subStr);

                    }

                  
                }

                else{
                    if (isInteger(subStr,0,&error1)) {
                        if (error1==false) {
                            printf("IntConst(%s)\n", subStr);
                            fprintf(dosya,"IntConst(%s)\n", subStr);
                            
                        }
                        else{
                            printf("Lexical error: Integer const cannot be longer than 8 digits\n");
                            fprintf(dosya,"Lexical error: Integer const cannot be longer than 8 digits\n");
                            break;
                        }
                        
                    }
                    else if(isValidIdentifier(subStr,1,&error2,&error3,dosya) && !isDelimiter(input[right - 1])){
                        if (error2==false && error3 ==false) {
                            printf("Identifier(%s)\n", subStr);
                            fprintf(dosya,"Identifier(%s)\n", subStr);
                        }
                        
                        
                    }
                    
                }
    
            }
                    if (error2==true) {
                        break;
                    }
                    if (error3==true) {
                        break;
                    }

            subStr2 = subStr;


            left = right;
            
            
        }
    }
    return 0;
}

int main(void) {
   
   
   
  FILE *code_sta;
  FILE *code_lex;
      
  code_sta = fopen("code.sta.txt", "r");
  if (code_sta == NULL) {
    printf("File cannot be opened!\n");
    return 1;
  }
  code_lex = fopen("code.lex.txt", "w");
  if (code_lex == NULL) {
   printf("File cannot be opened!\n");
   return 1;
  }
    int maxTahminiSatirUzunlugu = 1024;
    char satir[maxTahminiSatirUzunlugu];
    
    
    
    while(fgets(satir, maxTahminiSatirUzunlugu, code_sta)) {
        char *parca;
        parca = strtok(satir,"\r\n");
        lexicalAnalyzer(satir, code_lex);
        

    }
    

  fclose(code_lex);
  fclose(code_sta);
   
  return 0;
}
