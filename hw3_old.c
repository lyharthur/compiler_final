#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define DELIMS " \t\r\n\0"
#define max_set_member 256
#define max_name 256
#define max_set_member_name 256
#define max_stack 256


int S(FILE*);
int Program(FILE*);
int DeclList(FILE*);
int DeclList_(FILE*);

char* Type(FILE*);
char* id(FILE*);
char* Decl(FILE* fout,char*,char*);

char* VarDecl_(FILE*);
char* FunDecl(FILE*);

char* ParamDeclList(FILE*);
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
char* ExprIdTail(FILE*,char*);
char* Expr_(FILE*,char*);
char* print_(FILE*);
char* ExprArrayTail(FILE*);
char* ExprList(FILE*);
char* BinOp(FILE*);

char* ExprListTail_(FILE*);
char* ExprListTail(FILE*);




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


int array_max;
int register_count;
char ssss[32];

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

        for (int i=0;i<=10;i++){//lexical check
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
    fout = fopen("gogo.ll","w");
    if (!fout)
    {
        printf("gogo.ll failed to open\n");
    }
    while(fgets(str, 256, fin) != NULL){
        
        sp = strtok(str,"\n");
        strcpy(inla[i],sp);
        
        
        
        //printf("%s\n",inla[i] );
        i++;
    }
    fclose(fin);
    
    register_count=1;
    
    if (strcmp(inla[index1],"S")==0)
    {
        S(fout);
    }
    return 0;
    
    
    
}


//***************          hw3    code genertion



            
