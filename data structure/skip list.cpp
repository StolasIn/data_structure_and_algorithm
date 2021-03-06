#include<stdio.h>  
#include<stdlib.h>  
    
#define MAX_LEVEL 10 //最大層數  
    
//node  
typedef  struct nodeStructure  
{  
    int key;  
    int value;  
    struct nodeStructure *forward[1];  
}nodeStructure;  
    
//skiplist
typedef  struct skiplist  
{  
    int level;  
    nodeStructure *header;  
}skiplist;  
    
//創建節點  
nodeStructure* createNode(int level,int key,int value)  
{  
    nodeStructure *ns=(nodeStructure *)malloc(sizeof(nodeStructure)+level*sizeof(nodeStructure*));    
    ns->key=key;    
    ns->value=value;    
    return ns;    
}  
    
//初始化skiplist  
skiplist* createSkiplist()  
{  
    skiplist *sl=(skiplist *)malloc(sizeof(skiplist));    
    sl->level=0;    
    sl->header=createNode(MAX_LEVEL-1,0,0);    
    for(int i=0;i<MAX_LEVEL;i++)    
    {    
        sl->header->forward[i]=NULL;    
    }  
    return sl;  
}  
    
//隨機產生層數  
int randomLevel()    
{  
    int k=1;  
    while (rand()%2)    
        k++;    
    k=(k<MAX_LEVEL)?k:MAX_LEVEL;  
    return k;    
}  
    
//插入節點  
bool insert(skiplist *sl,int key,int value)  
{  
    nodeStructure *update[MAX_LEVEL];  
    nodeStructure *p, *q = NULL;  
    p=sl->header;  
    int k=sl->level;  
    //從最高層往下找可以插入的位置  
    //填充update  
    for(int i=k-1; i >= 0; i--){  
        while((q=p->forward[i])&&(q->key<key))  
        {  
            p=q;  
        }  
        update[i]=p;  
    }  
    //不能插入相同的key  
    if(q&&q->key==key)  
    {  
        return false;  
    }  
    
    //產生隨機層數K  
    //新創一個待插入節點q  
    //一層一層插入  
    k=randomLevel();  
    //更新跳表的level  
    if(k>(sl->level))  
    {  
        for(int i=sl->level; i < k; i++){  
            update[i] = sl->header;  
        }  
        sl->level=k;  
    }  
    
    q=createNode(k,key,value);  
    //逐層更新pointer  
    for(int i=0;i<k;i++)  
    {  
        q->forward[i]=update[i]->forward[i];  
        update[i]->forward[i]=q;  
    }  
    return true;  
}  
    
//搜索指定的key : value  
int search(skiplist *sl,int key)  
{  
    nodeStructure *p,*q=NULL;  
    p=sl->header;  
    //從最高層開始搜尋  
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--){  
        while((q=p->forward[i])&&(q->key<=key))  
        {  
            if(q->key == key)  
            {  
                return q->value;  
            }  
            p=q;  
        }  
    }  
    return NULL;  
}  
    
//删除指定的key  
bool deleteSL(skiplist *sl,int key)  
{  
    nodeStructure *update[MAX_LEVEL];  
    nodeStructure *p,*q=NULL;  
    p=sl->header;  
    //從最高層開始搜尋  
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--){  
        while((q=p->forward[i])&&(q->key<key))  
        {  
            p=q;  
        }  
        update[i]=p;  
    }  
    if(q&&q->key==key)  
    {  
        //逐層刪除  
        for(int i=0; i<sl->level; i++){    
            if(update[i]->forward[i]==q){    
                update[i]->forward[i]=q->forward[i];    
            }  
        }   
        free(q);  
        //如果删除的是最大層的節點，那麼需要重新維護  
        for(int i=sl->level - 1; i >= 0; i--){    
            if(sl->header->forward[i]==NULL){    
                sl->level--;    
            }    
        }    
        return true;  
    }  
    else  
        return false;  
}  
    
void printSL(skiplist *sl)  
{  
    //從最高層開始印出  
    nodeStructure *p,*q=NULL;  
    
    //從最高層開始搜尋  
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--)  
    {  
        p=sl->header;  
        while(q=p->forward[i])  
        {  
            printf("%d -> ",p->value);  
            p=q;  
        }  
        printf("\n");  
    }  
    printf("\n");  
}  
int main()  
{  
    skiplist *sl=createSkiplist();  
    for(int i=1;i<=19;i++)  
    {  
        insert(sl,i,i*2);  
    }  
    printSL(sl);  
    //搜索  
    int i=search(sl,4);  
    printf("i=%d\n",i);  
    //删除  
    bool b=deleteSL(sl,4);  
    if(b)  
        printf("刪除成功\n");  
    printSL(sl);  
    system("pause");  
    return 0;  
}  