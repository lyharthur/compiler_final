#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define DELIMS " \t\r\n\0"
#define max_set_member 256
#define max_name 256
#define max_set_member_name 256
#define max_stack 256

char* push1(char*);
char* pop1();
char* get_scope(char*);
char* get_type(char*);
char* get_size(char*);
char* get_func(char*);

int S(FILE*);
int Program(FILE*);
int DeclList(FILE*);
int DeclList_(FILE*);

char* Type(FILE*);
char* id(FILE*);
char* Decl(FILE* fout,char*,char*);

char* VarDecl_(FILE*);
char* FunDecl(FILE*);

int ParamDeclList(FILE*);
char* ParamDeclListTail(FILE*);

char* Block(FILE*);
char* ParamDecl(FILE*);
char* ParamDeclListTail_(FILE*);
char* ParamDecl_(FILE*);

char* VarDeclList(FILE*);
char* StmtList(FILE*);
char* VarDecl(FILE*);

char* Stmt(FILE*);
char* StmtList_(FILE*);
char* Expr(FILE*);

char* UnaryOp(FILE*);
char* num(FILE*);
char* ExprIdTail(FILE*);
char* Expr_(FILE*);
char* print_(FILE*);
char* ExprArrayTail(FILE*);
char* ExprList(FILE*);
char* BinOp(FILE*);

char* ExprListTail_(FILE*);
char* ExprListTail(FILE*);
char* push_para(char*);
char* pop_para();
char* get_para_stack(char*);

int fun_para = 0;



typedef struct set{
    char name[max_name];
    char member[max_set_member][max_set_member_name];
    int number;
} set;

typedef struct stack{
    char stk[max_stack][256];
    int top;
    int tree[max_stack];
}stack;

char inla[2000][256];
int index1;
int tmp_index;


char LLtable[34][34][256];
char FIRST[256][256][256];
char FOLLOW[256][256];
char nonterminal[256][256];
char input_org[256][256];

//hw3
char symbol_array[256][256];
char type_array[256][256];
char size_array[256][256];
int scope_array[256];
int func_array[256];


char para_stack[50][50];
int para_stack_num = 0;
int para_mode = 0;
char para_in_func[50][2][50];
int para_in_func_num = 0;

int array_max;
int register_count;
int tmp_register_count;
char ssss[32];

int func_stk = 0;
int label1;
int label2;
int label3 = 0 ;
int double_flag =0;

int stack_num = 0;
int tmp_stack_num = 0;
char stack_[1024][50];
char tmp_stack_[1024][50];
int op_flag = 0 ;

//hw3
char terminal[34][8]={"int", "char" ,"double" ,"float", "epsilon" , ";" , "[" , "(" , "," , "{" , "}" , "$" , "]" , ")" , "num" , "id" , "-" , "!" , "+" , "*" , "/" , "==" , "!=","<","<=",">",">=","&&","||", "print" , "while" ,"if" ,"break" ,"return"};
char keyword[11][8]={"int","char","float","double","return","if","else","while","break","for","print"};

int new,m=0;
int count;
int temp_c=0;
set first[28];
set follow[28];
set grammar[28];
stack s;
char pop_out[256]=" ";



void push(char *arg,int in){
    if (s.top == (max_stack - 1)){
        printf ("Stack is Full\n");
        return;
    }else{
        
        s.top = s.top + 1;
        strcpy(s.stk[s.top],arg);
        s.tree[s.top-1]=in;
    }
    return;
}

char * pop(){
    
    if (s.top == - 1){
        printf ("Stack is Empty\n");
        return "EMPTY";
    }else{
        strcpy(pop_out,s.stk[s.top]);
        s.top = s.top - 1;
    }
    return pop_out;
}





int add_follow(int index,char *add){
    int i;
    for (i=0; i<follow[index].number; i++) {
        if(strcmp(follow[index].member[i],add)==0){
            return 0;
        }
    }
    strcpy(follow[index].member[follow[index].number],add);
    follow[index].number++;
    return 0;
}



int check_epsilon(int index){
    int i;
    for (i=0; i<first[index].number; i++) {
        if(strcmp(first[index].member[i],"epsilon")==0){
            return 1;
        }
    }
    return 0;
}

int check_epsilon_grammar(int index){
    int i;
    for (i=0; i<grammar[index].number; i++) {
        if(strcmp(grammar[index].member[i],"epsilon\n")==0){
            return 1;
        }
    }
    return 0;
}




int get_index(char *arg){
    int i;
    for (i=0; i<256; i++) {
        //printf("%s %lu:%s %lu\n",arg,strlen(arg),grammar[i].name,strlen(grammar[i].name));
        if(strcmp(nonterminal[i],arg)==0){
            return i;
        }
    }
    return 0;
}


int get_index_terminal(char *arg){
    int i;
    for (i=0; i<34; i++) {
        //printf("%s %lu:%s %lu\n",arg,strlen(arg),grammar[i].name,strlen(grammar[i].name));
        if(strcmp(terminal[i],arg)==0){
            return i;
        }
    }
    return 0;
    
}


int findfirst(int k,int l,int i,int cur){
    int j;
    
    if(isupper(FIRST[k][l][0])){
        for(i=0;i<=30;i++)
            if(strcmp(FIRST[k][l],nonterminal[i])==0)
                for(j=0;j<=16;j++)
                    findfirst(i,j,0,cur);
    }
    else if(FIRST[k][l][0]){
        strcpy(first[cur].member[new],FIRST[k][l]);
        new++;
    }
    return 0;
}



int findfollow(){
    int i,j,k,index1,index2;
    char tmp[256];
    for(i=0;i<=count;i++){
        //printf("%s    %d\n",grammar[i].name,grammar[i].number);
        for (j=0; j<grammar[i].number; j++) {
            strcpy(tmp,grammar[i].member[j]);
            char *seg2=strtok(tmp," \n\0");
            char *seg1;
            while (seg2!=NULL) {
                
                seg1=seg2;
                seg2=strtok(NULL," \r\n\0");
                
                index1=get_index(seg1);
                
                strcpy(follow[index1].name,seg1);
                if(seg2==NULL){
                    break;
                }
                index2=get_index(seg2);
                
                if(seg2!=NULL){
                    if(isupper(*seg2)){
                        for(k=0;k<=first[index2].number;k++){
                            add_follow(index1,first[index2].member[k]);
                        }
                    }else if(!isupper(*seg2)){
                        add_follow(index1,seg2);
                    }
                }
            }
        }
    }
    return 0;
}

void remove_epsilon(){
    int k,j;
    for(k=0;k<27;k++){
        for(j=0;j<follow[k].number;j++){
            if(strcmp(follow[k].member[j],"epsilon")==0)
                strcpy(follow[k].member[j],"\0");
        }
    }
}



int findfollow_epsilon(){
    
    int i,j,k,index,index1,index2;
    
    char tmp[256];
    for(i=0;i<=count;i++){
        index=get_index(grammar[i].name);
        //printf("%s    %d\n",grammar[i].name,i);
        for (j=0; j<grammar[i].number; j++) {
            strcpy(tmp,grammar[i].member[j]);
            char *seg2=strtok(tmp," \n\0");
            char *seg1;
            while (seg2!=NULL) {
                
                seg1=seg2;
                seg2=strtok(NULL," \r\n\0");
                
                
                index1=get_index(seg1);
                if(seg2)
                    index2=get_index(seg2);
                strcpy(follow[index1].name,seg1);
                if(seg2==NULL){
                    
                    for(k=0;k<follow[i].number;k++){
                        //printf("follow num %d\n",follow[i].number);
                        add_follow(index1,follow[i].member[k]);
                    }
                    
                    
                }else if(seg2!=NULL&&index2!=0){
                    //printf("seg2 index %d  %d\n",index1,i);
                    if(check_epsilon(index2)){
                        
                        for(k=0;k<follow[i].number;k++){
                            //printf("follow num %d\n",follow[i].number);
                            add_follow(index1,follow[i].member[k]);
                        }
                        
                    }
                    
                    
                }
                
            }
            
        }
        
        
    }
    return 0;
    
}


int check_grammar(int k, int l){//LLTABLE
    int i,j,id=0;
    char *arg =first[k].member[l];// input
    char * seg,*seg2;
    char tmp[256],tmp2[256];
    
    for (i=0; i<grammar[k].number; i++) {
        strcpy(tmp2,grammar[k].member[i]);
        seg2=strtok(tmp2," \n\0");
        if(strcmp(arg,seg2)==0)
            
            return i;
    }
    for (i=0; i<grammar[k].number; i++) {
        strcpy(tmp2,grammar[k].member[i]);
        seg2=strtok(tmp2," \n\0");
        if(seg2)
            id=get_index(seg2);
        for (j=0; j<first[id].number; j++) {
            strcpy(tmp,first[id].member[j]);
            seg = strtok(tmp," \t\n\0\r");
            if(strcmp(seg,arg)==0){
                return i;
            }
            
        }
        
    }
    return j;
    
}
//***************************    hw3  *************************

char* check_type(char a[32],char b[32],int max,int scope,int index){
    int i;
    char id1[32],id2[32];
    char temp[32]="";
    if(strcmp(input_org[index+2],"+")==0||strcmp(input_org[index+2],"-")==0||
       strcmp(input_org[index+2],"*")==0||strcmp(input_org[index+2],"/")==0)
        strcpy(temp,check_type(input_org[index+1],input_org[index+3],max,scope,index+2));
    
    for (i=0; i<max; i++) {
        if(strcmp(symbol_array[i],a)==0)
            strcpy(id1,type_array[i]);
        if(strcmp(symbol_array[i],b)==0)
            strcpy(id2,type_array[i]);
    }
    if(isdigit(a[0])) //check num type
        for(i=0;i<strlen(a);i++){
            if(a[i]=='.'){
                strcpy(id1,"double");
                break;
            }
            else
                strcpy(id1,"int");
        }
    if(isdigit(b[0]))
        for(i=0;i<strlen(b);i++){
            if(b[i]=='.'){
                strcpy(id2,"double");
                break;
            }
            else
                strcpy(id2,"int");
        }
    if(strcmp(temp,"double")==0)
        strcpy(id2,"double");
    
    
    if(strcmp(id1,id2)!=0){
        if(strcmp(temp,"double")==0){
            printf("Warning (scope %d): %s %s , %s%d %s\n",scope,a,id1,"temp",temp_c,"double");
            temp_c++;
            return "double";
        }
        printf("Warning (scope %d): %s %s , %s %s\n",scope,a,id1,b,id2);
        return "double";
        
    }
    return "";
}




int main(int argc, char **argv){
    
    FILE *fin = fopen(argv[1],"r");
    int l=0,j=-1,k=0,i=0,x=0,y=0;
    char str[256];
    
    for(k=0;k<34;k++)
        for (i=0; i<34; i++)
            strcpy(LLtable[k][i],"ERROR" );
    
    //find first
    while (1) {
        if (!fgets(str, 256, fin)) break;
        if(str[0]!='\t'){
            j++;
            char *arg=strtok(str,DELIMS);
            strcpy(nonterminal[j],arg);
            
            l=0;
        }else if(str[0]=='\t'){
            
            strcpy(grammar[j].member[l],&str[1]);
            
            char *arg=strtok(str,DELIMS);
            strcpy(FIRST[j][l],arg);
            l++;
            grammar[j].number=l;
            
        }
    }//first
    
    count = j;
    for(k=0;k<=count;k++){
        
        for (l=0;l<=20; l++)
            findfirst(k,l,0,k);
        
        first[k].number=new;
        new=0;
    }
    
    strcpy(first[0].name,"S");
    strcpy(grammar[0].name,"S");
    for(j=1;j<=count;j++){
        strcpy(first[j].name,nonterminal[j]);
        strcpy(grammar[j].name,nonterminal[j]);
    }
    
    
    fclose(fin);
    
    
    
    //findfollow
    findfollow();
    for (k=0; k<10; k++)
        findfollow_epsilon();
    //findfollow_epsilon();
    
    strcpy(follow[0].name,"S");
    for (k=0; k<follow[0].number; k++) {
        strcpy(follow[0].member[k],"\0");
    }
    strcpy(follow[0].member[0],"$");
    
    
    remove_epsilon();
    FILE *fout;
    fout=fopen("set.txt","w");
    //***** output set
    fprintf(fout,"\nFirst Set\n");//FIRST SET
    for ( k=0; k<=count;k++) {
        fprintf(fout,"%s :",first[k].name);
        for(l=0;l<=first[k].number;l++){
            fprintf(fout," %s",first[k].member[l]);
        }
        fprintf(fout,"\n");
    }
    
    
    fprintf(fout,"\nFollow Set\n");//FOLLOW SET
    for ( k=0; k<=count;k++) {
        fprintf(fout,"%s :",follow[k].name);
        for(l=0;l<=follow[k].number;l++){
            if (strcmp(follow[k].member[l],"\0")!=0)
                fprintf(fout," %s",follow[k].member[l]);
        }
        fprintf(fout,"\n");
    }
    /*
     fprintf(fout,"\nGrammar Set\n");//GRAMMAR SET
     for ( k=0; k<=count;k++) {
     fprintf(fout,"%s :\n",grammar[k].name);
     for(l=0;l<=grammar[k].number;l++){
     if (strcmp(grammar[k].member[l],"\0")!=0)
     fprintf(fout," %s",grammar[k].member[l]);
     }
     fprintf(fout,"\n");
     }*/
    fclose(fout);
    //********
    
    fout=fopen("LLTABLE.txt","w");
    
    
    fprintf(fout,"LLTable\n");//LLTABLE  CREATE
    for (k=0; k<=count; k++) {
        for (l=0; l<first[k].number; l++) {
            if(strcmp(first[k].member[l],"epsilon")==0)//follow
                for (j=0; j<follow[k].number; j++) {
                    if (strcmp(follow[k].member[j],"\0")!=0){
                        if(check_epsilon_grammar(k)==0){//follow
                            strtok(grammar[k].member[0],"\n\0");
                            fprintf(fout,"%-20s%-16s    %s\n",grammar[k].name,follow[k].member[j],grammar[k].member[0]);
                            
                            x =get_index(grammar[k].name);
                            y =get_index_terminal(follow[k].member[j]);
                            strcpy(LLtable[x][y],grammar[k].member[0]);
                            
                        }else{
                            fprintf(fout,"%-20s%-16s    %s\n",grammar[k].name,follow[k].member[j],"epsilon");
                            
                            x =get_index(grammar[k].name);
                            y =get_index_terminal(follow[k].member[j]);
                            strcpy(LLtable[x][y],"epsilon");
                            
                            
                        }
                    }
                }
            else{//first
                i=check_grammar(k,l);
                strtok(grammar[k].member[i],"\n\0");
                
                fprintf(fout,"%-20s%-16s",grammar[k].name,first[k].member[l]);
                fprintf(fout,"    %s\n",grammar[k].member[i]);
                
                x =get_index(grammar[k].name);
                y =get_index_terminal(first[k].member[l]);
                strcpy(LLtable[x][y],grammar[k].member[i]);
            }
        }
        
    }
    fclose(fout);
    
    /*
     for(k=0;k<34;k++) //check LLTABLE
     for (i=0; i<34; i++)
     if(strcmp(LLtable[k][i],"ERROR")!=0)
     printf("x:%d  y:%d  %s \n",k,i,LLtable[k][i] );
     */
    
    
    fin = fopen("main.c","r");//tree  get input
    push("S",1);
    char tmp[256];
    char grammar_ele[8][256];
    char input[256][256];
    char *in_element;
    i=0;
    while (1) {
        if (!fgets(str, 256, fin)) break;
        in_element=strtok(str,DELIMS);
        while (in_element!=NULL) {
            strcpy(input[i],in_element);
            strcpy(input_org[i],in_element);
            i++;
            if (!in_element)break;
            in_element=strtok(NULL,DELIMS);
        }
        
        
    }//read main.c
    fclose(fin);
    fout=fopen("Parsing_Tree.txt","w");
    count=i;
    char tmp_char[256];
    int char_flag=0;
    int break_flag=0,push_flag=1;
    int tree=1;
    for(k=0;k<count;k++){//PARSE TREE
        char_flag=0;
        //printf("input :%s  %d\n",input_org[k],count);
        strcpy(tmp_char,input[k]);
        
        for (i=0;i<=10;i++){//lexical check
            if(isdigit(input[k][0])){
                char_flag=3;//num
                break;
            }else if(!isalpha(input[k][0])||strcmp(input[k],keyword[i])==0){//if keyword;{( f=1
                char_flag=1;
                break;
            }
            
        }
        //printf("aplhabat  %s : %s\n" ,input[k],keyword[i]);
        if(char_flag==3)
            strcpy(input[k],"num");
        
        if(char_flag==0)
            strcpy(input[k],"id");
        
        
        
        while(1){
            j=0;
            push_flag=1;
            pop();
            
            //for(i=1;i<=s.tree[s.top]-1;i++) hw2
            //  fprintf(fout,"    ");hw2
            fprintf(fout,"%s\n",pop_out);
            
            if(strcmp(pop_out,"num")==0||strcmp(pop_out,"id")==0){ //recover lexical change
                //for(i=1;i<=s.tree[s.top];i++)
                //  fprintf(fout,"    ");
                fprintf(fout,"%s\n",tmp_char);
            }
            
            
            
            
            for(i=0;i<8;i++)
                strcpy(grammar_ele[i],"\0");
            
            x=get_index(pop_out);
            y=get_index_terminal(input[k]);
            //printf("x:%d y:%d",x,y);
            
            
            if(strcmp(input[k],LLtable[x][y])==0){
                //for(i=1;i<=s.tree[s.top];i++)
                //fprintf(fout,"    ");
                fprintf(fout,"%s\n",input[k]);
                break;
            }
            
            if(strcmp(input[k],pop_out)==0){
                break;
            }
            for(i=0;i<34;i++){
                if(strcmp("epsilon",LLtable[x][y])==0){
                    push_flag=0;
                    //for(i=1;i<=s.tree[s.top];i++)
                    //  fprintf(fout,"    ");
                    fprintf(fout,"%s\n","epsilon");
                    break;
                }
                if(strcmp(terminal[i],LLtable[x][y])==0){
                    break_flag=1;
                    //for(i=1;i<=s.tree[s.top];i++)
                    //fprintf(fout,"    ");
                    fprintf(fout,"%s\n",terminal[i]);
                    
                    break;
                    
                }
                
            }
            if(break_flag){
                break_flag=0;
                break;
            }
            
            
            
            strcpy(tmp,LLtable[x][y]);
            
            //printf("%s\n",tmp);
            //printf("                     %s+%s LL:%s\n",pop_out,input[k],LLtable[x][y]);
            
            char *seg2=strtok(tmp," \n\0");
            char *seg1;
            while (seg2!=NULL) {//grammar strtok
                seg1=seg2;
                strcpy(grammar_ele[j],seg1);
                seg2=strtok(NULL," \r\n\0");
                j++;
            }
            tree++;
            
            tree=s.tree[s.top]+1;
            if(push_flag)
                for(l=j-1;l>=0;l--){
                    push(grammar_ele[l],tree);
                }
        }
        
        //printf("%s\n",tmp_char);
        
    }
    
    
    
    while (1){//last part
        char *_empty = pop();
        if(strcmp(_empty,"")==0)break;
        //for(i=1;i<s.tree[s.top];i++)
        //  fprintf(fout,"    ");
        fprintf(fout,"%s\n",pop_out);
        
        i=get_index(pop_out);
        
        if(check_epsilon_grammar(i)){
            push_flag=0;
            // for(i=1;i<=s.tree[s.top];i++)
            //   fprintf(fout,"    ");
            fprintf(fout,"%s\n","epsilon");
        }
        
    }
    fclose(fout);
    
    /*****************  hw3 code symbol table **********************/
    
    fout = fopen("symbol_table.txt","w");//symbol table  get input
    fprintf(fout,"%-10s%-10s%-10s%-10s%-10s\n","scope","symbol","type","array","function");
    
    int scope_c=0,scope_max=0,scope_record=0;
    
    int array=0;
    int count_pair=0;
    for(k=0;k<count;k++){//input->symbol table
        
        
        if((((strcmp(input_org[k-2],"int")==0 || strcmp(input_org[k-2],"float")==0 || strcmp(input_org[k-2],"double")==0 ||strcmp(input_org[k-2],"char")==0)&&strcmp(input_org[k],"(")==0)
            ||strcmp(input_org[k],"else")==0||strcmp(input_org[k],"if")==0
            ||strcmp(input_org[k],"while")==0)){//caulate scope
            
            count_pair++;
            
            if(count_pair%2==1){
                scope_record=scope_c;
                //fprintf(fout,"%d1\n",scope_record);
                
            }
            scope_c=scope_max+1;
            scope_max=scope_c;
        }
        else if(strcmp(input_org[k],"}")==0){
            count_pair--;
            
            if(count_pair%2==0){
                scope_c=scope_c-1;
                //fprintf(fout,"%d2\n",scope_record);
                scope_c=scope_record;
                
            }
            else{
                scope_c=scope_record+1;
                
            }
        }
        if(strcmp(input_org[k],"int")==0 || strcmp(input_org[k],"float")==0 || strcmp(input_org[k],"double")==0 || strcmp(input_org[k],"char")==0){
            //scope
            fprintf(fout,"%-10d",scope_c);
            scope_array[array]=scope_c;
            
            fprintf(fout,"%-10s%-10s",input_org[k+1],input_org[k]);
            strcmp(input_org[k+2],"[")==0?fprintf(fout,"%-10s","true"):fprintf(fout,"%-10s","false");
            strcmp(input_org[k+2],"[")==0?strcpy(size_array[array],input_org[k+3]):strcpy(size_array[array],"0");
            strcmp(input_org[k+2],"(")==0?fprintf(fout,"%-10s\n","true"):fprintf(fout,"%-10s\n","false");
            if (strcmp(input_org[k+2],"(")==0)
                func_array[array]=1;
            else
                func_array[array]=0;
            
            
            strcpy(type_array[array],input_org[k]);//store type-symbol
            strcpy(symbol_array[array],input_org[k+1]);
            
            //printf("%s  %s \n",type_array[array],symbol_array[array]);
            array++;
            array_max=array;
        }
        if(strcmp(input_org[k],"=")==0){
            if(strcmp(input_org[k-1],"]")==0)
                check_type(input_org[k-4],input_org[k+1],array_max,scope_c,k);
            else
                check_type(input_org[k-1],input_org[k+1],array_max,scope_c,k);
        }
        
    }
    
    fclose(fout);
    
    //for (array=0; array<array_max; array++) {
    //printf("%s__%s___%s___%d\n",type_array[array],symbol_array[array],size_array[array],scope_array[array]);
    //}
    
    
    char s[256][256];
    char *sp;
    int start;
    j=0,index1=0;
    fin = fopen("Parsing_Tree.txt","r");
    i=0;
    if (!fin)
    {
        printf("Parsing_Tree.txt failed to open\n");
    }
    fout = fopen("output.ll","w");
    if (!fout)
    {
        printf("output.ll failed to open\n");
    }
    while(fgets(str, 256, fin) != NULL){
        
        sp = strtok(str,"\n");
        strcpy(inla[i],sp);
        
        
        
        //printf("%s\n",inla[i] );
        i++;
    }
    fclose(fin);
    
    register_count=1;
    
    //printf("hw2\n");
    if (strcmp(inla[index1],"S")==0)
    {
        S(fout);
    }
    return 0;
    
    
    
    
    
    
    
}


//***************          hw3    code genertion




int S(FILE *fout)
{	//Program $
    fprintf(fout, "@.str = private constant[4 x i8] c\"%%d\\0A\\00\"\n");
    fprintf(fout, "@.str1 = private constant[4 x i8] c\"%%f\\0A\\00\"\n");
    fprintf(fout, "declare i32 @printf(i8*, ...)\n");
    //printf("@.str = private constant[4 x i8] c\"%%d\\0A\\00\"\n");
    //printf("I'm in s!\n");
    index1++;
    if(strcmp(inla[index1],"Program")==0)
    {
        //printf("%s\n", inla[index1]);
        Program(fout);
    }
    index1++;
    //printf("%s\n", inla[index1]);
    return 0;
}
int Program(FILE *fout)
{
    index1++;
    if(strcmp(inla[index1],"DeclList")==0)
    {
        //printf("%s\n", inla[index1]);
        DeclList(fout);
    }
    return 0;
}
int DeclList(FILE *fout)
{
    /*DeclList' DeclList
     epsilon*/
    index1++;
    if(strcmp(inla[index1],"DeclList'")==0)
    {
        //printf("%s\n", inla[index1]);
        DeclList_(fout);
        index1++;
        if(strcmp(inla[index1],"DeclList")==0)
        {
            //printf("%s\n", inla[index1]);
            DeclList(fout);
        }
    }
    else if(strcmp(inla[index1],"epsilon")==0)
    {
        //printf("%s\n", inla[index1]);
    }
    else
        printf("DeclList trap\n");
    
    return 0;
}
int DeclList_(FILE *fout)
{
    char *Type_name;
    char *id_name;
    char *Decl_name;
    //printf("%s\n", inla[index1]);
    index1++;
    if(strcmp(inla[index1],"Type")==0)
    {
        //printf("%s\n", inla[index1]);
        Type_name = Type(fout);
        //printf("%s\n", Type_name);
        index1++;
        
        // id
        //printf("%s\n", inla[index1]);
        id_name = id(fout);
        //printf("%s\n", id_name);
        
        //Decl
        index1++;
        //printf("%s\n",inla[index1]);
        Decl_name = Decl(fout,id_name,Type_name);
        
    }
    else
        printf("DeclList_ trap\n");
    return 0;
}
char* Type(FILE* fout)
{
    index1++;
    if (strcmp(inla[index1],"int")==0)
    {
        //printf("int\n");
        return "i32";
    }
    else if (strcmp(inla[index1],"char")==0)
    {
        //printf("char\n");
    }
    else if (strcmp(inla[index1],"double")==0)
    {
        //printf("double\n");
        return "double";
    }
}

char* id(FILE* fout)
{
    index1++;
    //printf("%s\n", inla[index1]);
    if (para_mode == 1 )
    {
        char buf2[50];
        fun_para ++;
        sprintf(buf2,"%s %d",inla[index1],register_count);
        push_para(buf2);
        //printf("11111111111111111111111111\n");
        return inla[index1];
    }
    char buf[50];
    if(strcmp(inla[index1+2],"=")==0||strcmp(inla[index1+2],"[")==0){
        sprintf(buf,"%%%s",inla[index1]);
        push1(buf);
        //register_count--;
        return inla[index1];
        
    }else{
        sprintf(buf,"%%%d",register_count);
        push1(buf);
        return inla[index1];
        
    }
    
}
char* Decl(FILE* fout,char* id_name,char* Type_name)//here define function/global variable
{
    char *VarDecl_data;
    index1++;
    if (strcmp(inla[index1],"VarDecl'")==0)
    {
        //printf("%s\n", inla[index1]);
        VarDecl_data = VarDecl_(fout);
        if (strcmp(VarDecl_data,"variable")==0)
        {
            //@x = global i32 0
            
            //printf("@%s = global %s 0\n",id_name,Type_name );
            fprintf(fout, "@%s = global %s 0\n", id_name,Type_name);
            
        }
        //@SS = common global [15 x i32] zeroinitializer, align 16
        else if (strncmp(VarDecl_data," ",1)==0)
        {
            //printf("@%s = global [%s x %s] zeroinitializer\n",id_name,VarDecl_data,Type_name );
            fprintf(fout, "@%s = global [%s x %s] zeroinitializer\n",id_name,VarDecl_data,Type_name);
        }
    }
    else if (strcmp(inla[index1],"FunDecl")==0)
    {	//define i32 @func
        //printf("%s\n", inla[index1]);
        //printf("define %s @%s",Type_name,id_name);
        fprintf(fout, "define %s @%s",Type_name,id_name);
        if (strcmp(Type_name,"i32") == 0)
        {
            func_stk = 1;
        }
        else
            func_stk = 2;
        FunDecl(fout);
    }
}

char* VarDecl_(FILE* fout)//array or variable
{
    index1++;
    char* num ;
    char s[30] = " ";
    if (strcmp(inla[index1],";")==0)
    {
        //printf("variable\n");
        return "variable";
    }
    else if(strcmp(inla[index1],"[")==0)
    {
        //printf("array\n");
        index1 = index1 + 2;
        strcat(s,inla[index1]);
        num = s;
        //printf("shit\n");
        index1 = index1 + 2;
        
        return num;
    }
    else
    {
        printf("VarDecl_ trap\n");
    }
    return " ";
}
char* FunDecl(FILE* fout)
{	//( ParamDeclList ) Block
    register_count = 1;
    
    int i = 0;
    
    int ParamDeclList_num = 0;
    para_mode = 1;
    fun_para = 0;
    index1++;//(
    //printf("%s",inla[index1]);
    fprintf(fout,"%s",inla[index1]);
    //parameter_stack = 0;
    index1++;//ParamDeclList
    ParamDeclList(fout);
    
    index1++;//)
    //printf("%s",inla[index1]);
    fprintf(fout,"%s",inla[index1]);
    
    para_mode = 0;
    index1++;//Block
    //printf("%s\n", inla[index1]);
    fprintf(fout, "{\n");
    char buf[50];;
    char* bfp;
    char jj_buffer[50];
    for (i = 1; i <= fun_para; ++i)
    {
        //%1 = alloca i32, align 4
        strcpy(buf,para_stack[i-1]);
        //store i32 %a, i32* %1, align 4
        //store i32 %b, i32* %2, align 4
        bfp = strtok(buf," ");
        strcpy(para_in_func[para_in_func_num][0],bfp);
        sprintf(jj_buffer,"%d",register_count);
        strcpy(para_in_func[para_in_func_num][1],jj_buffer);
        //printf("%s %s3333 \n", para_in_func[para_in_func_num][1],jj_buffer);
        para_in_func_num++;
        //fprintf(fout, "%s\n",bfp );
        fprintf(fout, "%%%d = alloca %s\n", register_count,get_type(bfp) );
        fprintf(fout, "store %s %%%s, %s* %%%d\n",get_type(bfp),bfp,get_type(bfp),register_count );
        register_count++;
    }
    Block(fout);
    fprintf(fout, "}\n");
    
}
int ParamDeclList(FILE* fout)
{
    /*ParamDeclListTail
     epsilon*/
    
    //printf("%s\n", inla[index1]);
    index1++;
    if (strcmp(inla[index1],"ParamDeclListTail")==0)
    {
        //printf("%s\n", inla[index1]);
        ParamDeclListTail(fout);
        
    }
    else if (strcmp(inla[index1],"epsilon")==0)
    {
        //printf("epsilon\n");
    }
    else
    {
        //printf("ParamDeclList trap\n");
    }
}

char* ParamDeclListTail(FILE* fout)
{
    char *ParamDeclListTail__name;
    //ParamDecl ParamDeclListTail'
    index1++;
    //printf("%s\n", inla[index1]);
    ParamDecl(fout);
    
    index1++;
    //printf("%s\n", inla[index1]);
    ParamDeclListTail__name = ParamDeclListTail_(fout);
}
char* Block(FILE* fout)
{
    //{ VarDeclList StmtList }
    index1++;
    //printf("%s\n", inla[index1]);//{
    
    index1++;
    //printf("%s\n", inla[index1]);
    VarDeclList(fout);
    
    index1++;
    //printf("%s\n", inla[index1]);
    StmtList(fout);
    
    index1++;
    //printf("%s\n", inla[index1]);//}
    
    
    
}
char* VarDeclList(FILE* fout)
{
    /*VarDecl VarDeclList
     epsilon*/
    index1++;
    if (strcmp(inla[index1],"VarDecl")==0)
    {	//VarDecl VarDeclList
        //printf("%s\n", inla[index1]);
        VarDecl(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        VarDeclList(fout);
    }
    else if (strcmp(inla[index1],"epsilon")==0)
    {	//epsilon
        //printf("epsilon\n");
    }
    else
    {
        printf("VarDeclList trap\n");
    }
    
}
char* VarDecl(FILE* fout)
{
    //Type id VarDecl' -> array or ;
    char *Type_name;
    char *id_name;
    char *VarDecl__name;
    index1++;
    //printf("%s\n", inla[index1]);
    Type_name = Type(fout);
    //printf("%s\n", Type_name);
    
    index1++;
    //printf("%s\n", inla[index1]);
    id_name = id(fout);
    //printf("%s\n", id_name);
    
    index1++;
    //printf("%s\n", inla[index1]);
    VarDecl__name = VarDecl_(fout);
    
    //%a = alloca i32
    if (strcmp(VarDecl__name,"variable")==0)
    {
        //printf("%%%s = alloca %s\n", id_name,Type_name);
        fprintf(fout, "%%%s = alloca %s\n", id_name,Type_name );
        
    }
    else if(strncmp(VarDecl__name," ",1)==0)
    {
        //%e = alloca [20 x i32], align 16
        //printf("%%%s = alloca [%s x %s], align 16\n", id_name,VarDecl__name,Type_name);
        fprintf(fout, "%%%s = alloca [%s x %s], align 16\n", id_name,VarDecl__name,Type_name );
        
    }
    else
    {
        printf("VarDecl trap\n");
    }
    
    
    
}

char* StmtList(FILE* fout)
{
    //Stmt StmtList'
    index1++;
    //printf("%s\n", inla[index1]);
    Stmt(fout);
    
    index1++;
    //printf("%s\n",inla[index1]);
    StmtList_(fout);
    
}
char* Stmt(FILE* fout)
{
    /*;
     Expr ;
     return Expr ;
     break ;
     if ( Expr ) Stmt else Stmt
     while ( Expr ) Stmt
     Block
     print id ;*/
    char* id_name;
    index1++;
    if (strcmp(inla[index1],";")==0)
    {	//;
        //printf(";\n");
    }
    else if (strcmp(inla[index1],"Expr")==0)
    {	//Expr ;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        
        index1++;
        //printf(";\n");
    }
    else if (strcmp(inla[index1],"return")==0)
    {	//return Expr ;
        char* ret_type;
        char* ret_name;
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        ret_name = pop1();
        if (func_stk == 1)
        {
            fprintf(fout,"%%%d = fptosi double %s to i32\n",register_count,ret_name);
            fprintf(fout, "ret i32 %%%d\n",register_count);
            register_count++;
            
        }
        else if (func_stk == 2)
        {
            fprintf(fout, "ret double %s\n",ret_name);
        }
        
        index1++;
        //printf("%s\n", inla[index1]);
    }
    else if (strcmp(inla[index1],"break")==0)
    {	//break ;
        //printf("%s\n", inla[index1]);
    }
    else if (strcmp(inla[index1],"if")==0)
    {	//if ( Expr ) Stmt else Stmt
        //printf("%s\n", inla[index1]);    //if
        
        int i;
        char *pop_name1=pop1();
        FILE *fake;
        fake = fopen("trash.txt","w");
        index1++;
        //printf("%s\n", inla[index1]);     //(
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        
        
        index1++;
        //printf("%s\n", inla[index1]);    //)
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        
        
        
        fprintf(fout,"%%%d = icmp eq i32 1, %s\n",register_count,pop_name1);
        
        register_count++;
        
        
        
        
        tmp_index=index1;
        tmp_stack_num =stack_num;
        tmp_register_count=register_count;
        for (i=0; i<stack_num; i++) {
            strcpy(tmp_stack_[i],stack_[i]);
        }
        
        fprintf(fout,"br i1 %%%d, label %%%d , label %%%d\n",tmp_register_count-1,tmp_register_count,Stmt(fake)+1);
        
        index1=tmp_index;
        stack_num =tmp_stack_num;
        register_count=tmp_register_count;
        for (i=0; i<=stack_num; i++) {
            strcpy(stack_[i],tmp_stack_[i]);
        }
        
        register_count++;
        
        Stmt(fout);///real
        
        //Stmt(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);     //else
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        tmp_index=index1;
        tmp_stack_num =stack_num;
        tmp_register_count=register_count;
        for (i=0; i<=stack_num; i++) {
            strcpy(tmp_stack_[i],stack_[i]);
        }
        
        fprintf(fout,"br label %%%d\n",Stmt(fake)+1);
        
        index1=tmp_index;
        stack_num =tmp_stack_num;
        register_count=tmp_register_count;
        for (i=0; i<stack_num; i++) {
            strcpy(stack_[i],tmp_stack_[i]);
        }
        
        register_count++;
        
        Stmt(fout);///real
        
        fprintf(fout,"br label %%%d\n",register_count);
        register_count++;
        
        fprintf(fout,"br label %%%d\n",register_count);
        register_count++;
        
        fclose(fake);
        //Stmt(fout);
    }
    else if (strcmp(inla[index1],"while")==0)
    {	//while ( Expr ) Stmt
        //printf("%s\n", inla[index1]);
        FILE *fake;
        char *pop_name1=pop1();
        int i;
        fake = fopen("trash.txt","w");
        int register_count_tmp =register_count;
        fprintf(fout,"br label %%%d\n",register_count);
        
        register_count++;
        
        
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        
        fprintf(fout,"%%%d = icmp eq i32 1, %s\n",register_count,pop_name1);
        
        register_count++;
        
        
        
        tmp_index=index1;
        tmp_stack_num =stack_num;
        tmp_register_count=register_count;
        for (i=0; i<stack_num; i++) {
            strcpy(tmp_stack_[i],stack_[i]);
        }
        
        fprintf(fout,"br i1 %%%d, label %%%d , label %%%d\n",tmp_register_count-1,tmp_register_count,Stmt(fake)+1);
        
        index1=tmp_index;
        stack_num =tmp_stack_num;
        register_count=tmp_register_count;
        for (i=0; i<=stack_num; i++) {
            strcpy(stack_[i],tmp_stack_[i]);
        }
        
        register_count++;
        
        
        Stmt(fout);
        
        
        fprintf(fout,"br label %%%d\n",register_count_tmp);
        register_count++;
        
        
        fclose(fake);
        
        
        
    }
    else if (strcmp(inla[index1],"Block")==0)
    {	//Block
        //SCOPE++;
        //printf("%s\n", inla[index1]);
        Block(fout);
    }
    else if (strcmp(inla[index1],"print")==0)
    {	//print id ;
        /*%4 = load i32* %c
         call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), i32 %4)*/
        char* print_name;
        //printf("%s\n", inla[index1]);
        print_name = print_(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        id_name = id(fout);
        //printf("%ssss\n",id_name);
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        fprintf(fout,"%%%d = load %s* %s%s\n",register_count,get_type(id_name),get_scope(id_name),id_name );
        
        (strcmp(get_type(id_name),"i32")==0)?
        fprintf(fout,"call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str, i32 0, i32 0), %s %%%d)\n",get_type(id_name),register_count ):fprintf(fout,"call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str1, i32 0, i32 0), %s %%%d)\n",get_type(id_name),register_count );
        
        
        
        register_count = register_count + 2;
    }
    else
    {
        printf("Stmt trap\n");
    }
    
    return register_count;
}
char* StmtList_(FILE* fout)
{
    /*StmtList
     epsilon*/
    index1++;
    //printf("%s\n",inla[index1] );
    //printf("%d\n", strcmp(inla[index1],"StmtList"));
    if( strcmp(inla[index1],"StmtList")==0 )
    {
        //printf("%s\n",inla[index1]);
        StmtList(fout);
    }
    else if (strcmp(inla[index1],"epsilon")==0)
    {
        //printf("epsilon\n");
    }
    else
    {
        printf("StmtList_ trap\n");
    }
}
char* Expr(FILE* fout)
{
    /*UnaryOp Expr
     num Expr'
     ( Expr ) Expr'
     id ExprIdTail*/
    
    index1++;
    if (strcmp(inla[index1],"UnaryOp")==0)
    {	//UnaryOp Expr
        //printf("%s\n", inla[index1]);
        UnaryOp(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
    }
    else if (strcmp(inla[index1],"num")==0)
    {	//num Expr'
        //printf("%s\n", inla[index1]);
        char* pop_name1;
        char* pop_name2;
        char buf[50];
        num(fout);
        if(strcmp(inla[index1+3],"]")!=0){
            if(op_flag==1){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                
                fprintf(fout,"%%%d = fmul double %s, %s\n",register_count,pop_name1,pop_name2);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
            }else if(op_flag==2){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                fprintf(fout,"%%%d = fdiv double %s, %s\n",register_count,pop_name2,pop_name1);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
                
            }
            
            
        }
        
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr_(fout);
    }
    else if (strcmp(inla[index1],"(")==0)
    {	//( Expr ) Expr'
        //printf("%s\n", inla[index1]);
        
        index1++;
        Expr(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr_(fout);
    }
    else if (strcmp(inla[index1],"id")==0)
    {	//id ExprIdTail
        //printf("%s\n", inla[index1]);
        char* id_name;
        id_name = id(fout);
        //printf("%s\n", id_name);
        //%1 = load i32* %a
        
        if(strcmp(inla[index1+2],"=")!=0 && strcmp(inla[index1+2],"[")!=0){
            char* pop_name1;
            char* pop_name2;
            char buf[50];
            
            if (strcmp("no",get_func(id_name))==0)
            {
                char bob[50];
                strcpy(bob,get_para_stack(id_name));
                if (strcmp(bob,"0")==0)
                    fprintf(fout, "%%%d = load %s* %s%s\n",register_count,get_type(id_name),get_scope(id_name),id_name );
                else
                    fprintf(fout, "%%%d = load %s* %%%s\n",register_count,get_type(id_name),bob );
                //printf("qabcabc    %s   \n",inla[index1+2]);
                pop_name1=pop1();
                
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                
                register_count++;
                
                
                
                if(strcmp(get_type(id_name),"i32")==0){
                    pop_name1=pop1();
                    
                    fprintf(fout,"%%%d = sitofp i32 %s to double\n",register_count,pop_name1);
                    sprintf(buf,"%%%d",register_count);
                    push1(buf);
                    register_count++;
                }
            }
            if(op_flag==1){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                
                fprintf(fout,"%%%d = fmul double %s, %s\n",register_count,pop_name1,pop_name2);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
            }else if(op_flag==2){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                fprintf(fout,"%%%d = fdiv double %s, %s\n",register_count,pop_name2,pop_name1);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
                
            }
            
            
        }
        
        
        index1++;
        //printf("%s\n", inla[index1]);
        ExprIdTail(fout);
        
        
        return register_count;
    }
    else
    {
        printf("Expr trap\n");
    }
}
char* Expr_(FILE* fout)
{
    /*BinOp Expr
     epsilon*/
    char* a;
    index1++;
    if (strcmp(inla[index1],"BinOp")==0)
    {
        char* BinOp_name;
        char* pop_name1;
        char* pop_name2;
        char buf[50];
        
        //printf("%s\n", inla[index1]);
        BinOp_name = BinOp(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        
        
        if(strcmp(BinOp_name,"+")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fadd double %s, %s\n",register_count,pop_name1,pop_name2);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            
            
        }else if(strcmp(BinOp_name,"-")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fsub double %s, %s\n",register_count,pop_name2,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,"*")==0){
            
        }else if(strcmp(BinOp_name,"/")==0){
            
        }else if(strcmp(BinOp_name,"==")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp oeq double %s, %s\n",register_count,pop_name1,pop_name2);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,"!=")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp une double %s, %s\n",register_count,pop_name1,pop_name2);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,">")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp ogt double %s, %s\n",register_count,pop_name2,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,"<")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp olt double %s, %s\n",register_count,pop_name2,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,">=")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp oge double %s, %s\n",register_count,pop_name2,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,"<=")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout,"%%%d = fcmp ole double %s, %s\n",register_count,pop_name2,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            pop_name1=pop1();
            fprintf(fout,"%%%d = zext i1 %s to i32\n",register_count,pop_name1);
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else if(strcmp(BinOp_name,"||")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout, "%%%d = fptosi double %s to i32 \n",register_count,pop_name1 );
            fprintf(fout, "%%%d = fptosi double %s to i32 \n",register_count+1,pop_name2 );
            fprintf(fout,"%%%d = or  i32 %%%d, %%%d\n",register_count+2,register_count,register_count+1);
            
            register_count++;
            register_count++;
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            label3=label2;
        }else if(strcmp(BinOp_name,"&&")==0){
            pop_name1 = pop1();
            //printf("%s\n", pop_name1);
            pop_name2 = pop1();
            //printf("%s\n", pop_name2);
            fprintf(fout, "%%%d = fptosi double %s to i32 \n",register_count,pop_name1 );
            fprintf(fout, "%%%d = fptosi double %s to i32 \n",register_count+1,pop_name2 );
            fprintf(fout,"%%%d = and  i32 %%%d, %%%d\n",register_count+2,register_count,register_count+1);
            
            register_count++;
            register_count++;
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            label3=label2;
        }
        
        
        
        
        
        
        
        
        
        
    }
    else if(strcmp(inla[index1],"epsilon")==0)
    {
        //printf("epsilon\n");
        return "epsilon";
    }
    else
    {
        printf("Expr_ trap\n");
    }
    
}
char* BinOp(FILE* fout)
{
    /*+
     -
     *
     /
     ==
     !=
     <
     <=
     >
     >=
     &&
     ||*/
    index1++;
    if (strcmp(inla[index1],"+")==0)
    {	//+
        //printf("%s\n", inla[index1]);
        return "+";
    }
    else if (strcmp(inla[index1],"-")==0)
    {	//-
        //printf("%s\n", inla[index1]);
        return "-";
    }
    else if (strcmp(inla[index1],"*")==0)
    {	//*
        //printf("%s\n", inla[index1]);
        op_flag = 1;
        return "*";
    }
    else if (strcmp(inla[index1],"/")==0)
    {	// /
        //printf("%s\n", inla[index1]);
        op_flag = 2;
        return "/";
    }
    else if (strcmp(inla[index1],"==")==0)
    {	// ==
        //printf("%s\n", inla[index1]);
        return "==";
    }
    else if (strcmp(inla[index1],"!=")==0)
    {	//!=
        //printf("%s\n", inla[index1]);
        return "!=";
    }
    else if (strcmp(inla[index1],"<")==0)
    {	//<
        //printf("%s\n", inla[index1]);
        return "<";
    }
    else if (strcmp(inla[index1],"<=")==0)
    {	//<=
        //printf("%s\n", inla[index1]);
        return "<=";
    }
    else if (strcmp(inla[index1],">")==0)
    {	//>
        //printf("%s\n", inla[index1]);
        return ">";
    }
    else if (strcmp(inla[index1],">=")==0)
    {	//>=
        //printf("%s\n", inla[index1]);
        return ">=";
    }
    else if (strcmp(inla[index1],"&&")==0)
    {	//&&
        //printf("%s\n", inla[index1]);
        return "&&";
    }
    else if (strcmp(inla[index1],"||")==0)
    {	// ||
        //printf("%s\n", inla[index1]);
        return "||";
    }
    else
    {
        printf("BinOp trap\n");
    }
}
char* ExprIdTail(FILE* fout)
{
    /*Expr'
     ( ExprList ) Expr'
     [ Expr ] ExprArrayTail
     = Expr*/
    index1++;
    if (strcmp(inla[index1],"Expr'")==0)
    {	//Expr'
        //printf("%s\n", inla[index1]);
        Expr_(fout);
    }
    else if(strcmp(inla[index1],"(")==0)
    {	//( ExprList ) Expr'
        /*
         %63 = load i32* %a, align 4
         %64 = load i32* %b, align 4
         %65 = call i32 @func(i32 %63, i32 %64) */
        //printf("%s\n", inla[index1]);//(
        fun_para = 0;
        int i = 0;
        char buf[50];
        char func_name[50];
        para_mode = 1;
        strcpy(func_name,inla[index1-2]);
        index1++;
        //printf("%s\n", inla[index1]);
        ExprList(fout);
        
        index1++;
        //printf("%s\n", inla[index1]);//)
        int para_num = fun_para;
        //printf("%d 12312312\n", para_num);
        //printf("%s 534534534\n",get_scope(func_name) );
        fprintf(fout, "%%%d = call i32 %s%s(",register_count,get_scope(func_name),func_name);
        sprintf(buf,"%%%d",register_count);
        register_count++;
        char bb[50];
        char *buffer;
        if (para_num != 0)
        {
            strcpy(bb,para_stack[0]);
            buffer = strtok(bb," ");
            fprintf(fout, "%s ", get_type(buffer));
            buffer = strtok(NULL," ");
            fprintf(fout, "%%%s ", buffer);
            //pop1();
        }
        for (i = 1; i < para_num; ++i)
        {
            strcpy(bb,para_stack[i]);
            buffer = strtok(bb," ");
            fprintf(fout, ",%s ", get_type(buffer));
            buffer = strtok(NULL," ");
            fprintf(fout, "%%%s ",buffer);
            pop1();
        }
        fprintf(fout, ")\n");
        push1(buf);
        //%78 = sitofp i32 %77 to double
        //printf("%s11111111111111\n", get_type(func_name));
        if (strcmp(get_type(func_name),"i32")==0)
        {
            //fprintf(fout, "111111111111111\n");
            fprintf(fout,"%%%d = sitofp i32 %s to double\n",register_count,pop1());
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }
        
        para_mode = 0;
        index1++;
        //printf("%s\n", inla[index1]);
        Expr_(fout);
    }
    else if (strcmp(inla[index1],"[")==0)
    {	//[ Expr ] ExprArrayTail
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        index1++;
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        
        
        
        if(strcmp(inla[index1+1],"=")!=0 ){
            char* pop_name1;
            char* pop_name2;
            char buf[50];
            pop_name1=pop1();
            pop_name2=pop1();
            printf("qweqweqwe    %s  %s %s \n",pop_name1,pop_name2,inla[index1+10]);
            
            fprintf(fout,"%%%d = getelementptr inbounds [%s x %s]* %s, i32 0, i64 %s\n",register_count,get_size(pop_name2),get_type(&pop_name2[1]),pop_name2,pop_name1);
            
            
            fprintf(fout,"%%%d= load %s* %%%d\n",register_count+1,get_type(&pop_name2[1]),register_count);
            
            
            //store i32 1, i32* %28, align 4
            
            // printf("                       %s\n",&pop_name2[1]);
            if(strcmp(get_type(&pop_name2[1]),"i32")==0){
                register_count++;
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                
                register_count++;
                pop_name1=pop1();
                fprintf(fout,"%%%d = sitofp i32 %s to double\n",register_count,pop_name1);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
            }else{
                
                register_count++;
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                
                register_count++;
            }
            if(op_flag==1){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                
                fprintf(fout,"%%%d = fmul double %s, %s\n",register_count,pop_name1,pop_name2);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
            }else if(op_flag==2){
                pop_name1 = pop1();
                //printf("%s\n", pop_name1);
                pop_name2 = pop1();
                //printf("%s\n", pop_name2);
                fprintf(fout,"%%%d = fdiv double %s, %s\n",register_count,pop_name2,pop_name1);
                sprintf(buf,"%%%d",register_count);
                push1(buf);
                register_count++;
                op_flag=0;
                
                
                
            }
            
        }
        
        ExprArrayTail(fout);
        
        
        
        
        
        
    }
    else if (strcmp(inla[index1],"=")==0)
    {	//= Expr
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        
        char* pop_name1;
        char* pop_name2;
        char buf[50];
        
        //printf("%s\n", inla[index1]);
        
        pop_name1 = pop1();
        //printf("%s\n", pop_name1);
        pop_name2 = pop1();
        //printf("%s\n", pop_name2);
        //store i32 0, i32* %1
        if (strcmp(get_type(&pop_name2[1]),"i32")==0) {
            fprintf(fout,"%%%d = fptosi double %s to i32\n",register_count,pop_name1);
            
            fprintf(fout,"store i32 %%%d, i32* %s%s\n",register_count,get_scope(&pop_name2[1]),&pop_name2[1]);
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else{
            
            fprintf(fout,"store double %s, double* %s%s\n",pop_name1,get_scope(&pop_name2[1]),&pop_name2[1]);
            
        }
        
        
        
        
    }
    else
    {
        printf("ExprIdTail trap\n");
    }
}
char* ExprArrayTail(FILE* fout)
{
    /*Expr'
     = Expr*/
    index1++;
    if (strcmp(inla[index1],"Expr'")==0)
    {
        //printf("%s\n", inla[index1]);
        Expr_(fout);
    }
    else if(strcmp(inla[index1],"=")==0)
    {
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        Expr(fout);
        char* pop_name1;
        char* pop_name2;
        char* pop_name3;
        char buf[50];
        pop_name1=pop1();
        pop_name2=pop1();
        pop_name3=pop1();
        //printf("qweqweqwe    %s  %s  %s\n",pop_name1,pop_name2,pop_name3);
        fprintf(fout,"%%%d = getelementptr inbounds [%s x %s]* %s, i32 0, i64 %s\n",register_count,get_size(pop_name3),get_type(&pop_name3[1]),pop_name3,pop_name2);
        
        
        //printf("wwqwe    %s",&pop_name3[1]);
        if (strcmp(get_type(&pop_name3[1]),"i32")==0) {
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            pop_name3=pop1();
            
            fprintf(fout,"%%%d = fptosi double %s to i32\n",register_count,pop_name1);
            
            fprintf(fout,"store i32 %%%d, i32* %s\n",register_count,pop_name3);
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
        }else{
            
            sprintf(buf,"%%%d",register_count);
            push1(buf);
            register_count++;
            
            
            
            pop_name2=pop1();
            
            fprintf(fout,"store double %s, double* %s\n",pop_name1,pop_name2);
            
        }
        
        
        
        
        
        
        
        
    }
    else
    {
        printf("ExprArrayTail trap\n");
    }
    
}
char* ExprList(FILE* fout)
{
    /*ExprListTail
     epsilon*/
    index1++;
    if (strcmp(inla[index1],"ExprListTail")==0)
    {
        //printf("%s\n", inla[index1]);
        ExprListTail(fout);
    }
    else if (strcmp(inla[index1],"epsilon")==0)
    {
        //printf("%s\n", inla[index1]);
    }
    
}
char* ExprListTail(FILE* fout)
{
    //Expr ExprListTail'
    index1++;
    //printf("%s\n", inla[index1]);
    Expr(fout);
    
    index1++;
    //printf("%s\n", inla[index1]);
    ExprListTail_(fout);
}
char* ExprListTail_(FILE* fout)
{
    /*, ExprListTail
     epsilon*/
    index1++;
    if (strcmp(inla[index1],",")==0)
    {
        //printf("%s\n", inla[index1]);
        
        index1++;
        //printf("%s\n", inla[index1]);
        ExprListTail(fout);
    }
    else if (strcmp(inla[index1],"epsilon")==0)
    {
        //printf("%s\n", inla[index1]);
    }
}
char* num(FILE* fout)
{
    char *num;
    char buf[50];
    index1++;
    
    if (strcmp(inla[index1+3],"]")== 0) {
        push1(inla[index1]);
        return inla[index1];
    }
    if (strcmp(get_type(inla[index1]),"i32")== 0) {
        sprintf(buf,"%s.000000e+00",inla[index1]);
    }
    else
    {
        sprintf(buf,"%s000000e+00",inla[index1]);
    }
    push1(buf);
    
}
char* UnaryOp(FILE* fout)
{	/*-
     !*/
    index1++;
    if (strcmp(inla[index1],"-")==0)
    {
        //printf("%s\n", inla[index1]);
        return "-";
    }
    else if (strcmp(inla[index1],"!")==0)
    {
        //printf("%s\n", inla[index1]);
        return "!";
    }
}

char* ParamDecl(FILE* fout)
{
    char *Type_name;
    char *id_name;
    char *ParamDecl__name;
    //Type id ParamDecl'
    index1++;
    //printf("%s\n", inla[index1]);
    Type_name = Type(fout);
    //printf("%s\n", Type_name);
    
    index1++;
    //printf("%s\n", inla[index1]);
    id_name = id(fout);
    //printf("%s\n", id_name);
    
    index1++;
    //printf("%s\n", inla[index1]);
    ParamDecl__name = ParamDecl_(fout);
    //i32 %a, i32 %b
    if(strcmp(ParamDecl__name,"epsilon")==0)
    {
        //printf("%s %%%s", Type_name,id_name);
        fprintf(fout, "%s %%%s", Type_name,id_name);
        //parameter_stack ++;
        
    }
    else
    {
        printf("not yet for \"[]\"\n");
    }
    
    
}
char* ParamDecl_(FILE* fout)
{
    /*[ ]
     epsilon*/
    index1++;
    if (strcmp(inla[index1],"[")==0)
    {
        //printf("[]\n");
        index1++;
        return "[]";
    }
    else if(strcmp(inla[index1],"epsilon")==0)
    {
        //printf("epsilon\n");
        return "epsilon";
    }
    else
    {
        printf("ParamDecl_ trap\n");
    }
    
    
}
char* ParamDeclListTail_(FILE* fout)
{
    /*, ParamDeclListTail
     epsilon*/
    index1++;
    if (strcmp(inla[index1],",")==0)
    {
        //printf("%s\n", inla[index1]);
        fprintf(fout, "%s",inla[index1] );
        index1++;
        ParamDeclListTail(fout);
        return ",";
    }
    else if(strcmp(inla[index1],"epsilon")==0)
    {
        //printf("%s\n", inla[index1]);
        return "epsilon";
    }
    else
    {
        printf("ParamDeclListTail_ trap\n");
    }
}
char* print_(FILE* fout)
{
    //index1++;
    return "print";
}
char* get_scope(char* id)
{
    int i;
    for (i=0; i<array_max; i++) {
        if(strcmp(id,symbol_array[i])==0 && scope_array[i]==0)
            return "@";
        else if(strcmp(id,symbol_array[i])==0)
            return "%";
        
    }
    
    return "ERROR";
}
char* get_type(char *id){
    int i;
    for (i=0; i<array_max; i++) {
        if(strcmp(id,symbol_array[i])==0 && strcmp(type_array[i],"int")==0)
            return "i32";
        else if(strcmp(id,symbol_array[i])==0 && strcmp(type_array[i],"double")==0)
            return "double";
        
    }
    if(isdigit(id[0])){
        for(i=0;i<strlen(id);i++){
            if(id[i]=='.'){
                return "double" ;
            }
        }
        return "i32";
    }
    return "Error";
}
char* get_size(char *id){
    int i;
    for (i=0; i<array_max; i++) {
        if(strcmp(id+1,symbol_array[i])==0)
            return size_array[i];
    }
    
    return "Error";
}
char* get_func(char *id){
    int i;
    for (i=0; i<array_max; i++) {
        if(strcmp(id,symbol_array[i])==0 && func_array[i]==0)
            return "no";
        else if(strcmp(id,symbol_array[i])==0)
            return "yes";
        
    }
    
    return "ERROR";
}




char* push1(char* id)
{
    
    strcpy(stack_[stack_num],id);
    printf("push %s %d\n",stack_[stack_num],stack_num);
    stack_num++;
    
}
char* pop1()
{
    stack_num--;
    char* top;
    char buf[50];
    printf("pop %s %d\n", stack_[stack_num],stack_num);
    
    
    return stack_[stack_num];
}
char* push_para(char* id)
{
    /*char para_stack[50][50];
     int para_stack_num = 0;*/
    strcpy(para_stack[para_stack_num],id);
    //printf("para_ push %s %d\n",para_stack[para_stack_num],para_stack_num);
    para_stack_num++;
    
}
char* pop_para()
{
    para_stack_num--;
    char* top;
    char buf[50];
    //printf("para_ pop %s %d\n", para_stack[para_stack_num],para_stack_num);
    
    
    return para_stack[para_stack_num];
}
char* get_para_stack(char* id)
{
    int i = 0 ;
    for (i = 0; i < para_in_func_num; ++i)
    {
        if (strcmp(id,para_in_func[i][0])==0)
        {
            
            //printf("%siioh\n",para_in_func[i][1] );
            return para_in_func[i][1];
        }
    }
    return "0";
}


