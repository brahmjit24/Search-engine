#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
using namespace std;
#define base_url "developer.mozilla.org"
#define url_length 1000


/*
*
*  NOTE that DEpth has been set to 4/5
   And get links has been set to 20;
   to change Depth go to line 991-1001;
   
   to change number of links go to (((get_links))) fn
*
*/


struct linklist
{
char *URL;
int depth;
int key;
bool isvisted;
linklist *next;
linklist *pre;
};

struct hashtable
{
 struct linklist *ptr;
 int count;
};


struct linklist* head;




void exit_sys()
{
exit(0);
}





void error_out(string err)
{
cout<<"Error -> "<<err<<endl;
}








bool chkdept(char *a)
{
   if(a[0]>='1'&&a[0]<='5'&&a[1]=='\0')
     return true;
   else
     return false;
}







bool chkparameter(int argc)
{
   if(argc==4)
    {
     return true;
    }
   else
    return false;
}







bool url_exist(char *seed_url)
{
    
    bool flag=false;
    char a[300]="wget --spider ";
    strcat(a,seed_url);
     if(!system(a))
       {cout<<"Valid Url\n";
       flag=true;}
     else
       {cout<<"Invalid\n";
       flag=false;}
   return flag;
}








bool seedchk(char *seed_url)
{
   int i=0;
   bool flag=true;
   while(seed_url[i]!='/'&& seed_url[i]!='\0')
    {
      if(seed_url[i]!=base_url[i])
       {
        flag=false;
        break;
        }
      i++;
    }
   if(flag)
   flag=url_exist(seed_url);
   return flag;
}




int generate_key(char * URL)
{
int size=strlen(URL);
static int s=3;
static int i=1;
int size_base=strlen(base_url);
int ch=(int)URL[size-size_base];
int key=(size+ch);
key=key-s;
key=key*100+i;
i++;
key=key%1877;
s=s%10+8;
return key;
}




bool targetdirchk(char *dir)
{
   struct stat statbuf;
     // chks wether there is something of name <<..dir..>>
   if(stat(dir,&statbuf)==-1) 
    {
     error_out("Invalid directory\n");
     return false;
    }
     // chks if <<..dir..>> is a directory
   if(!S_ISDIR(statbuf.st_mode)) 
    {
     error_out("Invalid directory entry. Your input isn't a directory\n");
     return false;
    }
     // chks if <<..dir..>> is in writeable mod or not
   if((statbuf.st_mode & S_IWUSR)!=S_IWUSR) 
    {
     error_out("Invalid directory entry. It isn't writable\n");
     return false;
    }
return true;
}






/*




void removeWhiteSpace(char* html) 
{
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++) 
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}

*/






















void NormalizeWord(char* word) {
  int i = 0;
  while (word[i]) {
      // NEW
    if (word[i] < 91 && word[i] > 64) // Bounded below so this funct. can run on all urls
      // /NEW
      word[i] += 32;
    i++;
  }
}






















int GetNextURL(char* html, char * urlofthispage, char* result, int pos) 
{
  char c;
  int len, i, j;
  char* p1;
  char* p2; 
/*  if(pos == 0) {
    removeWhiteSpace(html);
  }
*/
  while (0 != (c = html[pos])) 
  {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  if (c) 
  {  
    p1 = strchr(&(html[pos+1]), '=');
    
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10)) 
    {

      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;

    p1++;    

    p2 = strpbrk(p1, "\'\">");
    if (!p2) 
    {
      // keep going...
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#') 
    { // Why bother returning anything here....recursively keep going...

      return GetNextURL(html,urlofthispage,result,pos+1);
    }

    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4)) 
    {
      //! Nice! The URL we found is in absolute path.
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
      if (p1[0] == '.') {
        //! Some URLs are like <a href="../../../a.txt"> I cannot handle this. 
	// again...probably good to recursively keep going..
	// NEW
        
        return GetNextURL(html,urlofthispage,result,pos+1);
	// /NEW
      }

      if (p1[0] == '/') {
        //! this means the URL is the absolute path 
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);        
      }else {
        //! the URL is a absolute path.
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
          //! urlofthis page is like http://www.abc.com/
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          //! urlofthis page is like http://www.abc.com/~xyz
          //! or http://www.abc.com
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }    
  return -1;
}










int NormalizeURL(char* URL) 
{
  int len = strlen(URL);
  if (len <= 1 )
    return 0;
  //! Normalize all URLs.
  if (URL[len - 1] == '/') 
  {
    URL[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(URL);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the URL's suffix.
  for (i = len - 1; i >= 0; i--)
    if (URL[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (URL[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a URL link is to a file that are not in the file type of the following
  //! one of four, then we will discard this URL, and it will not be in the URL list.
  if ((j >= 7) && (i > j) && ((i + 2) < len)) 
  {
    if ((!strncmp((URL + i), ".htm", 4))
        ||(!strncmp((URL + i), ".HTM", 4))
        ||(!strncmp((URL + i), ".php", 4))
        ||(!strncmp((URL + i), ".jsp", 4))
        ) 
    {
      len = len; // do nothing.
    } 
    else 
    {
      return 0; // bad type
    }
  }
  return 1;
}


























bool perform_chks(int argc,char *argv[])
{
  bool parm=chkparameter(argc);
    if(parm==false)
      {
       error_out("Wrong number of parameters");
       return false;
       }
  bool target_dir=targetdirchk(argv[3]);
   if(!target_dir)
      {
       return false;
      }
  bool deep=chkdept(argv[2]);
   if(!deep){
      error_out("dept check");
     return false;
       }
  bool seed=seedchk(argv[1]);
   if(!seed)
      {
      error_out("Seed check");
      return false;
      }
  return true;
}








bool perform_fetch(char *url)
{
  char urlbuffer[url_length]={0}; 
  strcat(urlbuffer, "wget -O ");
  strcat(urlbuffer,"crawler_dir/temp.txt ");
  strcat(urlbuffer, url);
  system(urlbuffer);
  return true;
}





int file_id=1;

void create_path(char * p)
{
  //static 
  char file[]="test";
  char path[]="crawler_dir/";
  char id_string[10];
  sprintf(id_string,"%d",file_id);
  char type[]=".txt";
  strcat(p,path);
  strcat(p,file);
  strcat(p,id_string);
  strcat(p,type);
  file_id++;
}








void empty_file(char path[])
{   
  FILE *ft;
  ft=fopen(path,"w");
  fclose(ft);
}




int size=0;



void write_in_file(char path[],char *url,char * depth)
{
    FILE *fp,*ft;
    char ch;
    ft=fopen("crawler_dir/temp.txt","r");
    if(ft==NULL)
     {
        error_out("wget Temporary File ");
        exit_sys();
      }
    fp=fopen(path,"w");
    fprintf(fp,url);
    char a='\n';fputc(a,fp);
   //char  dd[10]=*depth;
depth[1]=0;
    fprintf(fp,depth);
    fputc(a,fp);
    while(true)
     {
        size++;
        ch=fgetc(ft);
        if(ch==EOF)
           break;
        else
          {
            fputc(ch,fp);
          }
     }
    fclose(fp);
    fclose(ft);
    char emptPath[]="crawler_dir/temp.txt";
    empty_file(emptPath);
}





void to_the_ram(char path[],char *html)
{
FILE *fp;
char ch;
fp=fopen(path,"r");
int i=0;
   while(true)
     {  ch=fgetc(fp);
        html[i]=ch;
        if(ch==EOF)
           break;
        i++;
     }
}



int get_links(char * links[100],char * html)
{

   char result[1000]={0};
   char url[]=base_url;
int n=0;
int i=0;
bool valid;

while(true)
  { n=  GetNextURL(html, url, result, n); 
    int j=0;
    j=strlen(result);
    valid=url_exist(result);
   //valid=1;
   if(valid)
   {
     cout<<"----------------->>>>>>>>>"<<i+1<<endl;
 links[i]=(char*)malloc(j*sizeof(char*));
     strcpy(links[i],result);
     if(n==-1||i==20)
       break;
      i++;
   }

}

cout<<"_____"<<i<<"_________"<<n;
return i;
}



void validateURL(char * links[100],int size)
{
int valid;
char c[]="\0";
for(int i=0;i<size;i++)
{
 valid=NormalizeURL(links[i]);
if(valid==0)
{
//cout<<"-----#####################################################------------>>>"<<links[i]<<endl;
strcpy(links[i],c);
}

}


}


void chkbaseURL(char * links[100],int size)
{
int pos=0;
for(int j=0;j<size;j++)
{
   char *p1;
   if(links[j][0]=='h'||links[j][0]=='H')
   {
    if(links[j][1]=='t'||links[j][1]=='T')
     {
       
       p1=strchr(&(links[j][pos+1]),'/');
       if(*(p1+1)=='/')
         p1=p1+2;
       int i=0;bool flag=true;
       while(*(p1+i)!='/')
        {
           if(*(p1+i)!=base_url[i])
           {
            flag=false;
             break;
           }
           i++;
        }
      //  cout<<flag<<endl;
        if(flag==false)
         { //cout<<"-----#####################################################------------>>>"<<links[j]<<endl;
          strcpy(links[j],"\0");
         }
     }
   }
   else
   {
     
        int i=0;
        bool flag= true;
        int s=strlen(links[j]);
        while(links[j][i]!='/'&&i!=s)
        {
           if(links[j][i]!=base_url[i])
           {
            flag=false;
             break;
           }
           i++;
        }
        if(flag==false)
         {
//cout<<"-----#####################################################------------>>>"<<links[j]<<endl;
          strcpy(links[j],"\0");
         }
        }
    
  pos=0;
 }

}




void removeduplicate(char * links[100],int size)
{

for(int i=0;i<size-1;i++)
{
for(int j=i+1;j<size;j++)
{
if(strcmp(links[i],links[j])==0&&links[i]!="\0")
{if(links[i]!="\0")
//cout<<"-----#####################################################------------>>>"<<links[j]<<endl;
strcpy(links[j],"\0");
}
}
}
}






void chklinks(char * links[100],int size)
{
  validateURL(links,size);
  chkbaseURL(links,size);
  removeduplicate(links,size);
  if(strcmp(links[0],base_url)==0)
   {
   strcpy(links[0],"\0");
    }
}




void addseedtolist(struct linklist *head,char * url_,int deep)
{
head->URL=url_;
head->depth=deep;
head->isvisted=true;
head->key=generate_key(url_);
head->next=NULL;
head->pre=NULL;
}


void empty_table(struct hashtable *hash)
{
for(int i=0;i<1877;i++)
{
hash[i].ptr = NULL;
hash[i].count = 0;

}
}


int chkwhttodo(int k,struct hashtable *hash)
{

if(hash[k].ptr==NULL)
return 1;
else
return 0;

}



void to_the_list(char *links[100],int s,struct linklist ** head_ref,int deep,struct hashtable *hash)
{
 struct linklist* last=*head_ref;
 struct linklist* node;
 for(int i=0;i<s;i++)
 {
   if(strcmp(links[i],"\0")>0)
    {
     node=(struct linklist*)malloc(sizeof(struct linklist));
     int urlSIZE=strlen(links[i]);
     node->URL=(char *)malloc(urlSIZE*sizeof(char*));
     strcpy(node->URL,links[i]);
     node->isvisted=false;
     node->key=generate_key(links[i]);
     node->depth=deep;
     bool toappend=false;
     bool toinsert=false;
     int todo=chkwhttodo(node->key,hash);
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
        
     }
   free(links[i]);
  }



}





void printlist(struct linklist* node)  
{
int i=1;


  cout<<"----------------------------------------LIST   IS  BEING  PRINTED  ------------------------------------------------\n";
  cout<<"--------------------------------------------------------------------------------------------------------------------->>>>>>>>>>>>>>>>>>\n";
  cout<<"---------------------------------------------------------------------------------------------------------------------\n";
  while(node!=NULL)
   {
    cout<<"__"<<i<<"__"<<node->URL<<endl<<node->depth<<"   "<<node->isvisted<<"__)))>>"<<node->key<<endl;
    node=node->next;
     i++;
    }
}




int geturl(struct linklist* node,int deep,char * url)
{

return 0;
}


void addseedtohash(struct linklist* node,struct hashtable *hash)
{
hash[node->key].ptr=node;
hash[node->key].count=1;

}



void printhash(struct hashtable *hash)
{

  cout<<"----------------------------------------TABLE IS GOING TO PRINT  ------------------------------------------------\n";
  cout<<"--------------------------------------------------------------------------------------------------------------------->>>>>>>>>>>>>>>>>>\n";
  cout<<"---------------------------------------------------------------------------------------------------------------------\n";
for(int i=0;i<1877;i++)
{

if(hash[i].ptr!=NULL)
cout<<"____"<<i<<"_______"<<hash[i].ptr<<"_______"<<hash[i].count<<endl;


}
 
}




void crawler(int argc,char *argv[])
{ 
   bool chked=perform_chks(argc,argv);
       if(!chked)
       { error_out("Checked");
         exit_sys();
           }    
     
   bool fetched=perform_fetch(argv[1]);
       if(!fetched)
       { error_out("File Not Fetched");
        exit_sys();
          }
   char path[100]={0};
   create_path(path);
 char  * d=(char*)malloc(sizeof(char));
 d[0]='1';
   write_in_file(path,argv[1],d);
   char  * html=(char *)malloc(size*sizeof(char*));
   to_the_ram(path,html);
   char  *links[100];
   int s = get_links(links,html);
   int current_depth=1;
   head=(struct linklist*)malloc(sizeof(struct linklist));
   addseedtolist(head,argv[1],current_depth);
   current_depth++;
   chklinks(links,s);
struct hashtable *hash=(struct hashtable *)malloc(1877*sizeof(struct hashtable));

static bool flag=true;
 if(flag)
  {
empty_table(hash);
   flag=false;
  }


 addseedtohash(head,hash);

  to_the_list(links,s,&head,current_depth,hash);

   printlist(head);
   printhash(hash);



cout<<"----------------<<<<<<<<<<<<<<<<<<-------GO NEXT-------->>>>>>>>>>>>>>>>>>-----------------\n-";



struct linklist *node=(struct linklist*)malloc(sizeof(struct linklist));
node=head;

cout<<"-------------------------??????"<<node->URL<<"_____"<<(node->next)->URL<<"____"<<node->depth<<endl;

while(true)
{
size=0;
node=node->next;


cout<<"iiiiiiiiiiiiiiiiiiiiinnnnnnnnnnnnnnnnnwhileeeeeeeeeee";

current_depth=node->depth;

chked=url_exist(node->URL);
       if(!chked)
       { error_out("Checked");
         
           }

cout<<current_depth<<"\n________";
fetched=perform_fetch(node->URL);
       if(!fetched)
       { error_out("File Not Fetched");
        exit_sys();
          }
   for(int i=0;i<100;i++)
   path[i]=0;
   create_path(path);
cout<<"-------------------------------.>>>>>>>>>>>>>>>>>>>>>>>>>......."<<path<<endl;
//cURENT FDEPTH
 char  * d=(char*)malloc(sizeof(char));
 d[0]=current_depth+'0';
   write_in_file(path,node->URL,d);

//--------------------------------------------------
    html=(char *)malloc(size*sizeof(char*));
   to_the_ram(path,html);

    s = get_links(links,html);
   chklinks(links,s);
current_depth++;
 to_the_list(links,s,&head,current_depth,hash);

   printlist(head);
   printhash(hash);




if(current_depth>=6)
{
break;
}



}



}






int main(int argc,char *argv[])
{
crawler(argc,argv);
}

