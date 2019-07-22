#include<iostream>
#include<vector>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

/*



TO ADD A FUNCTION STARTING_LIST_LOADER SO THAT NEW DATA CAN BE APPEND INTO IT
*/





struct reflist
{
int pg;
int count;
struct reflist * next;
};

struct linklist
{
char * word;
int key;
struct reflist *nxt;
struct linklist *next;
struct linklist *pre;
int cc;
};

struct hashtable{
struct linklist* ptr;
int count;
};




int generate_key(char * URL)
{
 int size=strlen(URL);
 int s=37;
 int i=151;
 int size_base=strlen("testing");
 int ch=71;
 int key=(size+ch);
 key=key-s;
 key=key*100+i;
 i++;
 key=key%187;
 return key;
}




int get_number_files()
{
  struct dirent *de;  // Pointer for directory entry 
  DIR *dr = opendir("crawler_dir");  // opendir() returns a pointer of DIR type.  
  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
   printf("Could not open current directory" ); 
   return 0; 
  } 
  int count=0;
  while ((de = readdir(dr)) != NULL) 
   count++;   //To print name write in while loop printf("%s\n", de->d_name);    
  count =count-2;    
  closedir(dr);  
  return count;
}




void create_path(char * p,int file_id)
{
  char file[]="test";
  char path[]="crawler_dir/";
  char id_string[10];
  sprintf(id_string,"%d",file_id);
  char type[]=".txt";
  strcat(p,path);
  strcat(p,file);
  strcat(p,id_string);
  strcat(p,type);
}




void create_indexer_path(char * p)
{
  char file[]="index";
  char path[]="indexer_dir/";
  char type[]=".txt";
  strcat(p,path);
  strcat(p,file);
  strcat(p,type);
}




int get_size(char * p)
{
  FILE *ft;
  ft=fopen(p,"r");
  char ch;
  int size=0;
  ch=fgetc(ft);
  while(ch!=EOF)
  {
   size++;
   ch=fgetc(ft);
  }
  fclose(ft);
  return size;
}





void buffer_fill(char * html,char *p)
{
  FILE *ft;
  ft=fopen(p,"r");
  if(ft==NULL)
  {
   cout<<"DOESNOTT Exist";
  }
  char ch;
  int i=0;
  ch=fgetc(ft);
  while(ch!=EOF)
  {
   (html)[i]=ch;
   ch=fgetc(ft);
   i++;
  }
  fclose(ft);
}




void ltrim(int &strt,int &end,char * html)
{
  if(html[strt]!=' '||strt==end)
   return;
  else
   for(int i=strt;i<end;i++)
   {
     if(html[i]!=' ')
     {
      strt=i;
      break;
     }
   }
}





int words_(char * html,int strt,char * words[1000],int size,int wrd,char * ptrchr)
{
  int l=strlen(ptrchr);
  char *title=(char*)malloc((l+1)*sizeof(char));
  for(int i=0;i<=l;i++)
   title[i]=ptrchr[i];
  for(int i=l;i>0;i--)
   title[i]=title[i-1];
  title[1]='/';
  title[l+1]='\0';
  int i=strt,j=0;
  while(i<size)
  {
   if(html[i]=='<')
   {j=0;
    cout<<i<<"__";
    while(j!=(l))
    {
     if(html[i+j]==title[j])
      j++;
     else
      break;
    }
    cout<<j<<endl;
    if(j==(l))
     break;   
   }
   i++;
  }
  int end=i;
  if(end==size)
   return 0;
  else
   end=i-1;
  for(int s=strt;s<=end;s++)
   cout<<html[s];
  cout<<"------------the end is "<<end<<"---here\n";
  ltrim(strt,end,html);
  i=strt;
  int flag=1;
  int old=0;
  for(j=strt;j<=end;j++)
  {
    if(html[j]==','||html[j]=='?'||html[j]=='.'||html[j]=='!'||html[j]==':'||html[j]==';'||html[j]=='"'||html[j]=='\''||html[j]=='-'||html[j]=='_')
      html[j]=' ';
  }
  for(j=strt;j<end;j++)
  {
   if(html[j]=='<')
    flag=0;
   if(html[j]=='>')
   {
    flag=1;
    i=j+1;
   }
   if(flag==1||(flag==0&&old==1))
   {
     if((html[j]==' ' && html[j-1]!=' ' &&html[j-1]!='\n')||j+1==end||html[j]=='<')
      {
        int s=j-i+1;
        if(j+1==end)
         s=j-i+2;
        words[wrd]=(char *)malloc(s*sizeof(char));
        int k=0;
        int k2=0;
        while(k!=(s))
        {
         if(flag==1)
         { 
           words[wrd][k2]=html[i+k];
           k2++;
         }
         k++;  
        }
        words[wrd][k2]='\0';
        wrd++;
        i=j+1;
      }
    else if(html[j]==' '&&html[j+1]!=' '&&html[j+1]!='\n')
      {
        i=j+1;
       }
    }
  old=flag;
  }
  cout<<endl;
  return wrd;
}
 





int words_in_tag(char * html,char * words[1000],int size,int count,char * ptrchr)
{
  int i=0,j=0;
  int l=strlen(ptrchr);
  int k=0;
  int c=count;
  char *title=ptrchr;
  for(int i=0;i<size;i++)
  {
   if(html[i]=='<')
   {
    j=0;
    while(j!=(l-1))
    {
     if(html[i+j]==title[j])
       j++; 
     else
      break;
    }
    if(j==l-1)
    {
     k=i+j;
     while(html[k]!='>' && k<size)
       k++;
     for(int s=i;s<=k;s++)
     cout<<html[s];
     cout<<endl;
     cout<<"Tag Found at "<<i<<endl;
     c =words_(html,k+1,words,size,c,title);  
     cout<<":::::::::::::"<<"_____"<<c<<endl;  
    }
   }
  }
  cout<<"COUNT--"<<c<<endl;
  return c;
}







int get_words(char *html,char * words[1000],int size,int count)
{
  char *tt[7]={"<title>","<h1>","<h2>","<h3>","<h4>","<h5>","<h6>"};
  int c=0;
  for(int i=0;i<7;i++)
  {
   cout<<"###_______________________"<<tt[i]<<"_____________________________####\n";
   c= words_in_tag(html,words,size,c,tt[i]);
   cout<<":::::::::::::"<<c<<endl<<endl<<endl;
  }  
  return c;
}





int chkwhttodo(int k,struct hashtable *hash)
{
 if(hash[k].ptr==NULL)
  return 1;
 else
  return 0;
}





void to_the_list(char *words[1000],int s,struct linklist ** head_ref,struct hashtable *hash,int pg)
{
  cout<<"work this";
  struct linklist* last=*head_ref;
  struct linklist* node;
  for(int i=0;i<s;i++)
  {
  if(strcmp(words[i],"\0")>0)
  {
    int key=generate_key(words[i]);
    bool toappend=false,toinsert=false;
    int todo=chkwhttodo(key,hash);
    if(todo==1)
      toappend=true;
     else
      toinsert=true;
    if(toappend)
       {
        node=(struct linklist*)malloc(sizeof(struct linklist));
        int len= strlen(words[i])+1;
        cout<<len;
        node->word=(char *)malloc(len*sizeof(char));
        strcpy(node->word,words[i]);
        node->word[len]='\0';
        node->key=key;
        struct reflist* r=(struct reflist*)malloc(sizeof(struct reflist));
        r->pg=pg;
        r->count=1;
        r->next=NULL;
        node->cc=1;
        node->nxt=r;
        node->next=NULL;
        while(last->next!=NULL)
         last=last->next;
        last->next=node;
        node->pre=last;
        hash[node->key].ptr=node;
        hash[node->key].count=1;    
       }
   else if(toinsert)
     { 
        struct linklist *addr= hash[key].ptr;
        int totrace=hash[key].count;
        int chk=0;
        while(totrace!=1)
         {
           if(strcmp(words[i],(addr->word))==0)
             {
                chk=1;
                break;
               }
           addr=addr->next;
           totrace--;
          }
        if(chk==0&&strcmp(words[i],(addr->word))==0)
             chk=1;
        
        if(chk==0)
         {
        
        node=(struct linklist*)malloc(sizeof(struct linklist));
        int len= strlen(words[i])+1;
        cout<<len;
        node->word=(char *)malloc(len*sizeof(char));
        strcpy(node->word,words[i]);
        node->word[len]='\0';
        node->key=key;
        struct reflist* r=(struct reflist*)malloc(sizeof(struct reflist));
        r->pg=pg;
        r->count=1;
        r->next=NULL;
        node->cc=1;
        node->nxt=r;
        struct linklist *temp=addr->next;
        addr->next=node;
        node->next=temp;
        (hash[node->key].count)++; 
 
         }
        else if(chk==1)
         {
           struct reflist *raddr= addr->nxt;
           int race=addr->cc;
           int flag=0;
           while(race!=1)
            {
              if(raddr->pg==pg)
               {
                 flag=1;
                 break;
                  }
               raddr=raddr->next;
               race--;
             } 
           if(flag==0&&raddr->pg==pg)
                 flag=1;
           if(flag==1)
              {(raddr->count)++;
              }
           else if(flag==0)
              { struct reflist* r=(struct reflist*)malloc(sizeof(struct reflist));
        r->pg=pg;
        r->count=1;
        r->next=NULL;
        raddr->next=r;
        (addr->cc)++;
               }
         }
     }
   }
  }

}









void printrel(struct reflist *ptr)
{
  while(ptr!=NULL)
  {
   cout<<":: "<<ptr->pg<<":: "<<ptr->count<<endl;
   ptr=ptr->next;
  }
}





void printhash(struct hashtable *hash)
{

  cout<<"----------------------------------------TABLE IS GOING TO PRINT  ------------------------------------------------\n";
  cout<<"--------------------------------------------------------------------------------------------------------------------->>>>>>>>>>>>>>>>>>\n";
  cout<<"---------------------------------------------------------------------------------------------------------------------\n";
  for(int i=0;i<187;i++)
  {
   if(hash[i].ptr!=NULL)
   {
    cout<<"____"<<i<<"_______"<<hash[i].ptr<<"_______"<<hash[i].count<<"__________"<<hash[i].ptr->word<<"-----------"<<hash[i].ptr->cc<<endl;
     printrel(hash[i].ptr->nxt);
   }
  }
}






void print(struct linklist ** head_ref)
{
  struct linklist * node=*head_ref;
  while(node!=NULL)
  {
   cout<<"___________word______"<<node->word<<"______\n____";
   node=node->next;
  }
}





void clean_the_words(char* words[1000],int s)
{
  for(int i=0;i<s;i++)
  {
   int j=0;
   while(words[i][j]!='\0')
   {
    if(j!=0&&(words[i][j]==' '||words[i][j]=='\n')&&words[i][j-1]!=' ')
    {
     words[i][j]='\0';
     break;
    }
    if(j==0&&words[i][j]==' ')
    {
     words[i][j]='\0';
     break;
    }
    j++;
   }
  }
}






void to_the_file(struct hashtable** hsh)
{
 struct hashtable *hash=*hsh;
 char p[100]={0};
 create_indexer_path(p);
 FILE *ft;
 ft=fopen(p,"w");
 char b=' ';
 char s1='P';
 char s2='T';
 char a='\n';fputc(a,ft);
 for(int i=0;i<187;i++)
 {
  if(hash[i].ptr!=NULL)
  {
  struct linklist *addr=hash[i].ptr;
  int c=hash[i].count;
  while(c!=0)
  {
   struct reflist* raddr=addr->nxt;
   int race=addr->cc;
   fprintf(ft,addr->word);
   fputc(b,ft);
   fputc(b,ft);
   while(race!=0)
    {
    fputc(s1,ft);
    char pg_[10];
    sprintf(pg_,"%d",raddr->pg);
    fprintf(ft,pg_);
    fputc(b,ft);
    fputc(s2,ft);
    char count_[10];
    sprintf(count_,"%d",raddr->count);
    fprintf(ft,count_);
    fputc(b,ft);
    fputc(b,ft);
    raddr=raddr->next;
    race--;
    }
   fputc(a,ft);
   addr=addr->next;
   c--;
   }
  }
 }
 fclose(ft);
}





int main()
{
  int files_count=0;
  files_count=get_number_files();
  cout<<"Total Files-->>"<<files_count<<endl;
  files_count--;//because we have one temp file;
  cout<<"---"<<files_count<<endl;
  int i=1;
  char  p[100]={0};
  int size=0;
  char *html;
  cout<<"-----------";
  struct linklist * head=(struct linklist*)malloc(sizeof(struct linklist));
  cout<<"ONTER";
  char *wrd="#000";
  head->word="#000";
  head->next=NULL;
  struct hashtable *hash=(struct hashtable *)malloc(187*sizeof(struct hashtable));
  while(i<=files_count)
  {
   create_path(p,i);
   size=0;
   cout<<"##############################################################################       "<<i<<"   ###########################################################################################"<<endl;
   size=get_size(p);
   html=(char *)malloc(size*sizeof(char));
   buffer_fill(html,p);
   char *words[1000];
   int count=0,c=0;
   c= get_words(html,words,size,count);
   cout<<c<<endl;
   free(html);
   for(int j=0;j<100;j++)
     p[j]=0;
   clean_the_words(words,c);
   to_the_list(words,c,&head,hash,i);
   for(int i=0;i<c;i++)
   free(words[i]);
   i++;
  }
 print(&head);
 printhash(hash);
 to_the_file(&hash);
 cout<<"System on work\n";
}
