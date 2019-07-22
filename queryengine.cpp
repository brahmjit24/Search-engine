#include<string.h>
#include<cstring>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<iostream>
using namespace std;



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

struct pagelist{
int page;
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





int get_the_words(char *words[100],string str)
{

int wrd=0;
int j=0;
int i=0;
str=' '+str+' ';
cout<<str;
while(str[i]!='\0')
{
  if(str[i]==' '&&str[i-1]!=' ')
  {
    int x;
    int k=0;
    words[wrd]=(char*)malloc((i-j)*sizeof(char));
    for( x=j+1;x<i;x++)
    {words[wrd][k]=str[x];
    k++;}
    words[wrd][k]='\0';
    wrd++; 
    j=i;
  }
  else if(str[i]==' '&&str[i+1]==' ')
    j=i+1;

i++;
}
return wrd;
}





bool chkwrds(char * word)
{
char n[34][20]={"is","am","are","who","why","they","when","how","them","the","i","!","@","#","$","%","^","&","*","(",")","-","+","=","ok","to","by","in","on","of","to","their","there","a"};
for(int i=0;i<34;i++)
{if(strcmp(word,n[i])==0)
{
return true;
}
}
return false;
}







void remove_useless(char *words[100],int n)
{
for(int x=0;x<n;x++)
{
 bool remove=chkwrds(words[x]);
 if(remove==true)
  strcpy(words[x],"\0"); 
}
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



int toint(string n)
{
int num=0;
int s=n.length();
for(int i=0;i<s;i++)
{
  num=num*10+(int)n[i]-(int)'0';
}
return num;


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





int chkwhttodo(int k,struct hashtable *hash)
{
 if(hash[k].ptr==NULL)
  return 1;
 else
  return 0;
}





void loader(char *html,struct linklist ** head_ref,struct hashtable *hash)
{
struct linklist* last=*head_ref;
struct linklist* node;
int i=0;
int j=0;
while(html[i]!='\0')
{


if(html[i]=='\n')
{
char s[30]={0};
int k=0;
int x=0;
for( x=j+1;html[x]!=' ';x++)
 {s[k]=html[x];
k++;}
int key=generate_key(s);
      node=(struct linklist*)malloc(sizeof(struct linklist));
        int len= strlen(s)+1;
        cout<<len;
        node->word=(char *)malloc(len*sizeof(char));
        strcpy(node->word,s);
        node->word[len]='\0';
        node->key=key;
        int pn=0;
        int tempx=x;
struct reflist *old;
        while(html[x]!='\n')
         {
            if(html[x]=='P')
             {pn++;
                int xx=x+1;
               string ss="";
                   while(html[xx]!=' ')
                     {ss=ss+html[xx];xx++;}
                  while(html[xx]!='T')
                    xx++;
                    xx++;
               string ss2="";
                   while(html[xx]!=' ')
                     {ss2=ss2+html[xx];xx++;}
                     struct reflist* r=(struct reflist*)malloc(sizeof(struct reflist));
             r->pg=toint(ss);
             r->count=toint(ss2);
             r->next=NULL;
              if(pn==1)
             {node->cc=1;
              node->nxt=r;
             old=r;}
          else
           {(node->cc)++;
            old->next=r;
            old=r;
            }
              }
           x++;
          }
        cout<<pn<<">>";

     bool toappend=false;
     bool toinsert=false;
     int todo=chkwhttodo(node->key,hash);

    cout<<todo<<"____\n";
     if(todo==1)
      toappend=true;
     else
      toinsert=true;
     if(toappend)
       {
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
       struct linklist *addr= hash[node->key].ptr;
       int totrace=hash[node->key].count;
       while(totrace!=1)
        { addr=addr->next;
          totrace--;
         }

         if(addr->next!=NULL)
         {
            struct linklist *temp=addr->next;
            addr->next=node;
            node->next=temp;
            node->pre=addr;
            temp->pre=node;
            (hash[node->key].count)++;  
          }else{
            node->next=NULL;
            while(last->next!=NULL)
            last=last->next;
            last->next=node;
            node->pre=last;
           }  
       }



cout<<s<<"___"<<key<<endl;
j=i;
}




i++;
}
cout<<i;


}



string link_open(char * p)
{
string html="";
FILE *ft;
  ft=fopen(p,"r");
  if(ft==NULL)
  {
   cout<<"DOESNOTT Exist";
  }
  char ch;
  int i=0;
  ch=fgetc(ft);
  while(ch!='\n')
  {
   (html)=html+ch;
   ch=fgetc(ft);
   i++;
  }
  fclose(ft);

return html;

}







void load_to_buffer(char * html,char *p)
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







void empty_table(struct hashtable *hash)
{
for(int i=0;i<187;i++)
{
hash[i].ptr = NULL;
hash[i].count = 0;

}
}






int main()
{
string search;
getline(cin,search);
char *words[100];
int number_of_words = get_the_words(words,search);

cout<<"WORDS COUNTED :--"<<endl;
for(int i=0;i<number_of_words;i++)
cout<<words[i]<<endl;
cout<<endl<<endl;
remove_useless(words,number_of_words);
cout<<"WORDS COUNTED AFTER REMOVAL:--"<<endl;
for(int i=0;i<number_of_words;i++)
if(strcmp(words[i],"\0")!=0)
cout<<words[i]<<endl;
char  p[100]={0};
create_indexer_path(p);
cout<<p;

char *html;
int size=get_size(p);
html=(char *)malloc(size*sizeof(char));
load_to_buffer(html,p);
//cout<<html;
for(int i=0;i<100;i++)
p[i]=0;
cout<<"\nBUFFER IS LOADED\n";

struct linklist * head=(struct linklist*)malloc(sizeof(struct linklist));
head->word="#000";
head->next=NULL;
struct hashtable *hash=(struct hashtable *)malloc(187*sizeof(struct hashtable));
empty_table(hash);
loader(html,&head,hash);
cout<<"on the go";
printhash(hash);
print(&head);


int count=0;
for(int i=0;i<number_of_words;i++)
{
if(strcmp(words[i],"\0")!=0)
count++;
}

//-------------------------------------------TEMPORARY ALGO FOR SEARCH----------------------------------------------------
int arr[100]={0};

for(int i=0;i<number_of_words;i++)
{
if(strcmp(words[i],"\0")!=0)
   { 
     int key=generate_key(words[i]);
     cout<<key<<"---\n";
     if(hash[key].ptr==NULL)
       {cout<<"Unsuccesful serach";
          break;
       }
     else
        {
        
      ///continue from here.....................
       struct linklist *addr= hash[key].ptr;
       int totrace=hash[key].count;
       while(totrace!=0)
        {   if(strcmp(addr->word,words[i])==0)
              {
                struct reflist *raddr= addr->nxt;
                   while(raddr!=NULL)
                         {
                             arr[raddr->pg]++;
                               raddr=raddr->next;  
                               }
                break;

                  }
            addr=addr->next;
          totrace--;
         }




         }
    }
}

//check max;

for(int i=0;i<100;i++)
if(arr[i]==count)
{
create_path(p,i);
string a=link_open(p);
cout<<"Reffer This ->>   "<<a<<endl;
for(int i=0;i<100;i++)
p[i]=0;
}


//------------------------------------------------------------------------------------------------------------------------------



}




