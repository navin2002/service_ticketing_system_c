#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define h_pswd "hardware101"
#define s_pswd "software202"
#define a_pswd "admin303"
#define maxl 200

int h_ids = 0, s_ids = 0;

struct qnode
{
    int sid, status;
    char *cname, *problem, *phno, type;
    struct qnode * next;
};
typedef struct qnode * node;

struct qhead
{
    node f, r;
};
typedef struct qhead * queue;

int isempty(queue q)
{
    return (q->f == NULL && q->r == NULL);
}

queue hq, sq, sl;

void create_queue()
{
    hq = (queue)malloc(sizeof(struct qhead));
    sq = (queue)malloc(sizeof(struct qhead));
    sl = (queue)malloc(sizeof(struct qhead));
    if(hq != NULL && sq != NULL && sl != NULL)
    {
        hq->f = hq->r = NULL;
        sq->f = sq->r = NULL;
        sl->f = sl->r = NULL;
    }
    else
    {
        printf("Fatal Error!\n");
    }
    
}

void dispose_queue(queue q)
{
    if(!isempty(q))
    {
        node t;
        while(q->f != NULL)
        {
            t = q->f;
            q->f = (q->f)->next;
            free(t);
        }
        q->r = NULL;
        free(q);
    }
}

void enqueue(queue q, node n)
{
    if(n != NULL && q->f == NULL )
	{
		n->next = NULL;
		q->f = q->r = n;
    }
    else if(q->f != NULL)
    {
    	n->next = NULL;
		q->r->next = n;
		q->r = n;
	}
    else
    {
    	printf("Queue is Full\n");
	}
}

void dequeue(queue q)
{
    if(isempty(q))
    {
        printf("\tQueue is empty\n");
    }
    else if(q->f != q->r)
    {
        node t = q->f;
        q->f = (q->f)->next;
        enqueue(sl, t);
    }
    else
    {
        node t = q->f;
        q->f = q->r = NULL;
        enqueue(sl, t);
    }
    
}

char * getstr(int size)
{
    char * s = (char*)malloc(size*sizeof(char));
    int i = 0;
    char a;
    do
    {
        a = getchar();
        s[i++] = a;
    } while (a != '\n' && i < size);
    s[i] = '\0';
    return s;
}

node new(char t)
{
    node n = (node)malloc(sizeof(struct qnode));
    if(!n)
    {
        printf("Fatal Error\n");
        return NULL;
    }
    else
    {
        if(t == 'h')
            n->sid = ++h_ids;
        else
            n->sid = ++s_ids;
        n->type = t;
        printf("\n\tYour service id = %d\n", n->sid);
        printf("\tEnter your name(max 50 characters): ");
        getchar();
        n->cname = getstr(50);
        printf("\tEnter mobile number: ");
        n->phno = getstr(11);
        printf("\tEnter problem in short(max 200 characters): ");
        n->problem = getstr(maxl);
        return n;
    }
}

void disp_node(node n)
{
    printf("\n\tService request details:\n");
    char *stat;
    if(n->status==1)
        stat = "Completed";
    else if(n->status == 0)
        stat = "Not Completed";
    else if(n->status == -1)
        stat = "Removed";
    if(n->type=='h')
    {
        printf("\n\tService Type: Hardware");
    }
    else
    {
        printf("\n\tService Type: Software");
    }
    printf("\n\tServiceID: %d\n\tCustomer Name: %s\tCustomer mobile number : %s\tIssue: %s\tService Status: %s\n", n->sid, n->cname,n->phno, n->problem, stat);
}

void disp_queue(queue q)
{
    if(!isempty(q)) 
    {
        node temp=q->f;
        printf("\n");
        while(temp!=NULL)
        {
            disp_node(temp);
            printf("\n");
            temp=temp->next;
        }
    }
    else
    {
      printf("\tQueue is empty\n");
    }
}

void complete(node n)
{
    if(!n)
    {
        printf("\tNo services available currently\n");
        return;
    }
    else if (n->status == 0)
    {
        n->status = 1;
        disp_node(n);
        printf("\n\tStatus changed\n");
    }
    return;
}

int queue_pos(queue q, int serv_id)
{
    int pos = 0;
    node temp = q->f;
    while (temp != NULL && temp->sid != serv_id)
    {
        pos++;
        temp = temp->next;
    }
    return (temp != NULL) ? pos : -1;
}

void modify(queue q, int serv_id)
{
    char *nname, *nprob, *nphno;
    node t = q->f;
    int pos = queue_pos(q, serv_id);
    if(pos != -1)
    {
        while(t != NULL && t->sid != serv_id)
        {
            t = t->next;
        }
        printf("\n\tCurrent details: \n");
        disp_node(t);
        printf("\n\tModify details:");
        printf("\n\tEnter name(max 50 characters) or NULL if no change: ");
        getchar();
        nname = getstr(50);
        printf("\tEnter mobile number or NULL if no change: ");
        nphno = getstr(11);
        printf("\tEnter problem in short(max 200 characters)or NULL if no change: ");
        nprob = getstr(maxl);
        if(strcmp(nname, "NULL\n"))
            strcpy(t->cname, nname);
        if(strcmp(nprob, "NULL\n"))
            strcpy(t->problem, nprob);
        if(strcmp(nphno, "NULL\n"))
            strcpy(t->phno, nphno);
        printf("\n\tDetails modified\n");
    }
    else
    {
        printf("\tID not found\n");
        getchar();
    } 
    return;
}

void rem(queue q, int serv_id)
{
    if(queue_pos(q, serv_id) != -1)
    {
        node t1 = q->f, t2;
        t2 = t1;
        while(t1 != NULL && t1->sid != serv_id)
        {
            t2 = t1;
            t1 = t1->next;
        }
        t2->next = t1->next;
        t1->status = -1;
        if(q->f == q->r)
            q->f = q->r = NULL;
        enqueue(sl, t1);
        printf("\tRequest removed\n");
    }
    else
        printf("\tService id not found\n");
}

void find(queue q, int serv_id)
{
    if(!isempty(q))
    {
        node t = q->f;
        while(t != NULL && t->sid != serv_id)
            t = t->next;
        if(t == NULL)
            printf("\tService id not found\n");
        else
            disp_node(t); 
    }
    else
        printf("\tQueue is empty\n");
}

void find_sl(int serv_id, char t)
{
    if(!isempty(sl))
    {
        node temp = sl->f;
        while(temp != NULL)
        {
            if(temp->sid == serv_id && temp->type == t)
            {
                disp_node(temp);
                return;
            }

            temp = temp->next;
            
        }
        if(temp == NULL)
            printf("\n\n\tDetails of Your service request was not found\n");
    }
    
    else
        printf("\tQueue is empty\n");
}

int menu()//Main menu
{
    int c;
    printf("\n\tService Ticketing System");
    printf("\n\n\tMenu\n1. Customer\n2. Engineer\n3. Admin\n4. Exit\nEnter a choice: ");
    scanf("%d", &c);
    getchar();
    return c;
}

int cmenu()//Menu for the cutomers
{
    int c;
    printf("\n\n\t\tCustomer Menu\n\t1. Raise complaint\n\t2. Remove complaint\n\t3. View details\n\t4. Check queue position\n\t5. Modify complaint");
    printf("\n\t6. Back\n\tEnter choice: ");
    scanf("%d", &c);
    getchar();
    return c;
}

int emenu()//menu for the engineers
{
    int c;
    printf("\n\n\t\tEngineer Menu\n\t1. View all pending services\n\t2. View next in queue\n\t3. View details of a complaint\n\t");
    printf("4. Change status of current service\n\t5. Back\n\tEnter choice: ");
    scanf("%d", &c);
    getchar();
    return c;
}

int amenu()//menu for the admin
{
    int c;
    printf("\n\n\t\tAdmin Menu\n\t1. View all pending services\n\t2. View next in both queues\n\t3. View details of an ongoing complaint\n");
    printf("\t4. View service log\n\t5. Back\n\tEnter choice: ");
    scanf("%d", &c);
    getchar();
    return c;
}

void customer()
{
    int c, serv_id, pos;
    char p;
    do
    {
        system("clear");
        switch (cmenu())
        {
            case 1:
                printf("\tEnter type of problem hardware(h) / software(s): "); scanf(" %c", &p);
                if(p == 'h')
                {
                    node n = new(p);
                    enqueue(hq, n);
                    printf("\tComplaint raised!\n");
                }
                else if(p == 's')
                {  
                    node n = new(p);
                    enqueue(sq, n);
                    printf("\tComplaint raised!\n");
                }
                else
                {
                    getchar();
                    printf("\tPlease enter a valid option\n");
                    break;
                }
                break;
            
            case 2:
                printf("\tEnter type of problem hardware(h) / software(s): "); 
                scanf(" %c", &p);
                getchar();
                if(p == 'h')
                {
                    printf("\tEnter service id: "); 
                    scanf("%d", &serv_id);
                    rem(hq, serv_id);
                    getchar();
                }
                else if(p == 's')
                {
                    printf("\tEnter service id: "); 
                    scanf("%d", &serv_id);
                    rem(sq, serv_id);
                    getchar();
                }
                else
                {
                    printf("\tPlease enter a valid option\n");
                    break;
                }
                break;
            
            case 3:
                printf("\tEnter type of problem hardware(h) / software(s): "); scanf(" %c", &p);
                getchar();
                if(p == 'h')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    if(queue_pos(hq, serv_id) != -1)
                        find(hq, serv_id);
                    else
                        find_sl(serv_id, p);
                    getchar();
                }
                else if(p == 's')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    if(queue_pos(sq, serv_id) != -1)
                        find(sq, serv_id);
                    else
                        find_sl(serv_id, p);
                    getchar();
                }
                else
                {
                    printf("\tPlease enter a valid option\n");
                    break;
                }
                break;
            case 4:
                printf("\tEnter type of problem hardware(h) / software(s): "); scanf(" %c", &p);
                getchar();
                if(p == 'h')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    pos = queue_pos(hq, serv_id);
                    getchar();
                    if(pos != -1)
                        printf("\n\n\tYour Queue position is: %d\n", pos);
                    else
                        printf("\n\n\tYour service id was not found\n");
                    
                }
                else if(p == 's')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    pos = queue_pos(sq, serv_id);
                    getchar();
                    if(pos != -1)
                        printf("\n\n\tYour Queue position is: %d\n", pos);
                    else
                        printf("\n\n\tYour service id was not found\n");
                }
                else
                {
                    printf("Please enter a valid option\n");
                    break;
                }
                break;
            case 5:
                printf("\tEnter type of problem hardware(h) / software(s): "); scanf(" %c", &p);
                getchar();
                if(p == 'h')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    modify(hq, serv_id);    
                }
                else if(p == 's')
                {
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    modify(sq, serv_id);
                }
                else
                {
                    printf("\tPlease enter a valid option\n");
                    break;
                }
                break;
            
            case 6:
                return;
            
            default:
                printf("\n\n\tEnter a vaild choice\n");
                getchar();
                break;    
        }
        printf("\nEnter key to go back to menu: ");
        char c;
        scanf("%c",&c);
    } while (1);
}

void hengineer()
{
    char p[50];
    printf("\tEnter password (enter NULL to go back): "); scanf("%s", p);
    getchar();
    if(!strcmp(p, h_pswd))
    {
        int c, serv_id;
        do
        {
            system("clear");
            switch (emenu())
            {
                case 1:
                {
                    disp_queue(hq);
                    break;
                }
                case 2:
                {
                    if(!isempty(hq))
                    {
                        printf("\tNext in hardware: \n\n");
                        disp_node(hq->f);
                        
                    }
                    else
                    {
                        printf("\n\tNone left in hardware requests\n\n");
                    }
                    break;
                }
                case 3:
                {
                    printf("\tEnter service id: "); 
                    scanf("%d", &serv_id);
                    find(hq, serv_id);
                    getchar();
                    break;
                }
                case 4:
                {
                    complete(hq->f);
                    dequeue(hq);
                    break;
                }
                case 5:
                    return;
                
                default:
                {
                    printf("\n\tEnter a vaild choice\n");
                    getchar();
                    break;
                }
            }
            printf("\nEnter key to go back to menu: ");
            char c;
            scanf("%c",&c);
            
        } while (1); 
    }
    else if(!strcmp(p, "NULL"))
        return;
    else
        printf("\tEnter correct password\n");
}

void sengineer()
{
    char p[50];
    printf("\tEnter password (enter NULL to go back): "); scanf("%s", p);
    getchar();
    if(!strcmp(p, s_pswd))
    {
        int c, serv_id;
        do
        {
            system("clear");
            switch (emenu())
            {
                case 1:
                    disp_queue(sq);
                    break;

                case 2:
                    if(!isempty(sq))
                    {
                        printf("\nt\tNext in software: \n");
                        disp_node(sq->f);
                    }
                    else
                    {
                        printf("\n\n\tNone left in software requests\n");
                    }
                    break;
                
                case 3:
                    printf("\tEnter service id: "); scanf("%d", &serv_id);
                    find(sq, serv_id);
                    getchar();
                    break;

                case 4:
                    complete(sq->f);
                    dequeue(sq);
                    break;
                
                case 5:
                    return;
                
                default:
                {
                    printf("\tEnter a vaild choice\n");
                    getchar();
                    break;
                }
            }
            printf("\nEnter key to go back to menu: ");
            char c;
            scanf("%c",&c);
        } while (1); 
    }
    else if(!strcmp(p, "NULL"))
        return;
    else
        printf("\tEnter correct password\n");
}

void admin()
{
    char p[50];
    printf("\tEnter password (enter NULL to go back): "); scanf("%s", p);
    getchar();
    if(!strcmp(p, a_pswd))
    {
        int serv_id;
        char t;
        do
        {
            system("clear");
            switch (amenu())
            {
                case 1:
                    printf("\tHardware services:\n");
                    disp_queue(hq);
                    printf("\tSoftware services:\n");
                    disp_queue(sq);
                    break;
                
                case 2:
                    if(!isempty(hq))
                    {
                        printf("\tNext in hardware: \n");
                        disp_node(hq->f);
                    }
                    else
                    {
                        printf("\n\tNone left in hardware requests\n");
                    }
                    if(!isempty(sq))
                    {
                        printf("\tNext in software: \n");
                        disp_node(sq->f);
                    }
                    else
                    {
                        printf("\n\tNone left in software requests\n");
                    }
                    break;

                case 3:
                    printf("\tEnter type of problem hardware(h) / software(s): "); scanf(" %c", &t);
                    getchar();
                    if(t == 'h')
                    {
                        printf("\tEnter service id: "); scanf("%d", &serv_id);
                        find(hq, serv_id);
                        getchar();
                    }
                    else if(t == 's')
                    {
                        printf("\tEnter service id: "); scanf("%d", &serv_id);
                        find(sq, serv_id);
                        getchar();
                    }
                    else
                    {
                        printf("\tPlease enter a valid option\n");
                        break;
                    }
                    break;

                case 4:
                    printf("\tService log:\n");
                    disp_queue(sl);
                    break;
                
                case 5:
                    return;
                
                default:
                {
                    printf("\tEnter a vaild choice\n");
                    getchar();
                    break;
                }
            }
            printf("\nEnter key to go back to menu: ");
            char c;
            scanf("%c",&c);
        } while (1);
    }
    else if(!strcmp(p, "NULL"))
        return;
    else
        printf("Enter correct password\n");
}

void main()
{
    create_queue();    
    char p, x;
    do
    {
        system("clear");
        switch (menu())
        {
            case 1:
                customer();
                printf("\nEnter key to continue:");
                scanf("%c",&x);
                break;
            case 2:
                printf("Hardware engineer(h) or Software engineer(s)?: "); scanf(" %c", &p);
                getchar();
                if(p == 'h')
                    hengineer();
                else if(p == 's')
                    sengineer();
                else
                {
                    printf("\tEnter a valid choice\n");
                }
                printf("\nEnter key to continue:");
                scanf("%c",&x);
                break;
            
            case 3:
                admin();
                printf("\nEnter key to continue:");
                scanf("%c",&x);
                break;
            
            case 4:
                dispose_queue(hq);
                dispose_queue(sq);
                dispose_queue(sl);
                printf("Exiting Program.....\n");
                return;
            
            default:
            {
                printf("Enter a valid choice\n");
                printf("\nEnter key to go back to menu: ");
                char c;
                scanf("%c",&c);
                break; 
            }
        }
    } while (1);
}