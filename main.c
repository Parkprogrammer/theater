#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAXLINE 100
#define MAXMOVIE 100

#define ID(x,y) ((x) / (y))

typedef enum { RED, BLACK } Color;

/* using uint64_t to avoid overflow in making hash values. */
typedef uint64_t Hashint;    

typedef struct Node {
    Hashint key;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
    Node *nil;  // 경계 노드
    int insertions;  
    int deletions;   
    int leftRotations;  
    int rightRotations; 
} RBT;

typedef struct Info{
    int mv_id;
    int day;
    int time;
    int seatnum;
} Info;

/* Declaration of RBT functions */



/*
 * LEVEL 1: Implementation of the user interface
 */

/* Global variables */
int movieNum=35;
int fcount=0; int seatCount=0; int movieCount=0; int change_flag=0;
int executionCount = 0;

/* 
 * Declaration of Arrays and Variables for movies. (L1)
 *  1~2. Movie List, Day List 
 *  3~4. Movie, Day element getter
 *  5. Getting local time by function
 */
char movies[MAXMOVIE][MAXMOVIE] = { 
    "Inception", "Dark Knight", "Avengers", "Interstellar", "Old Boy",
    "Kingdom", "The Matrix", "Fight Club", "Pulp Fiction", "Forrest Gump",
    "The Godfather", "The Shawshank Redemption", "The Dark Knight Rises",
    "The Lord of the Rings I", "Star Wars: Episode V ",
    "The Lord of the Rings II", "Titanic", "Goodfellas",
    "The Lord of the Rings III", "The Silence of the Lambs",
    "Saving Private Ryan", "Coco", "The Green Mile", "Gladiator",
    "The Lion King", "Jurassic Park", "The Prestige", "The Departed",
    "Whiplash", "Django Unchained", "The Wolf of Wall Street", "Inglourious Basterds",
    "Mad Max: Fury Road", "Joker", "Parasite"
};
char weekdays[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", 
    "Saturday"};
int  getMovie(char* input);
int  getDay(char* input);
struct tm* gettime();


/* 
 * Making User Interface && Seat Print-out View (L1)
 *  1. userInterface    : Making the Starting User-interface
 *  2. theaterCommand   : Mapping the User input command to integers for switch-case
 *  3. printdayTable    : Print 5 Movie seat Views for the chosen day
 *  4. printsingleTable : Print 1 Movie seat View for the chosen day
 *  5. printdayList     : Print the movie-time table view for the chosen day
 *  6. printIdlist      : print the ID-list for the chosen day-movie
 */
int  userInterface(char* date);
int  theaterCommand(char* user_command);
void printdayTable(int movieTable[5][2], int seatTable[5][200]);
void printsingleTable(int* movieTable, int* seatTable);
void printdayList(int movieTable[5][2]);
void printIdlist(int day, int* movieTable, int* seatTable);


/*
 * LEVEL 2: Implementation of the submodules (various functionalies)
 */

/* 
 * Searching through RBT-Tree and Finding the required Information (L2)
 *  && Implementing information
 *  1. showInfo         : Search through RBT-Tree and show reservation info table from today by printdayList() or printdayTable()
 *  2. showSingleinfo   : Search through RBT-Tree and show the information by printsingleTable() 
 *  3. showMovieid      : Search through RBT-Tree and show the information by printIdlist() <L1&&L2> for function modulation
 *  4. showMovieschdule : Search throgut RBT-Tree and show the total information view of 1 week <L1&&L2> for function modulation
 */
void showInfo(RBT* T,int tdate, int option);
int  showSingleinfo(RBT *T,int tdate, char* movieName);
void showMovieid(RBT* T);
void showMovieschdule(RBT* T);


/*
 * LEVEL 3: Implementation of the data structures and primitive functions
 */


/* 
 * Implementing information && Actual RB-Tree functualization (L3)
 *  1. idEncoder        : Encoding User information int uint64_t Hash value for RB-Tree insert
 *  2. idDecoder        : Decoding Hash value into a Structure Info* for getting data from RB-Tree key value
 *  3. userEncoder      : Encoding reservation ID into a Hash string for later canclelation
 *  4. userDecoder      : Decoding reservation ID for comparison with the reservation ID and continuing 
 *  5. randomSrand      : Making random seats, time table for initialization of RB-Tree
 *  6. initialize       : Initialzing first RB-Tree with Movies and Time&&Seat Tables
 *  7. **reservation    : Getting reservation from User <L1&L2&L3> for function modulization
 *  8. **canclelation   : Getting canclelation from User <L1&L2&L3> for function modulization
 *  9. findNode         : Search through the RB-Tree and store the required information in a 2D-Array
 *  10~26.              : Functions for RB-Tree implementation
 */

Hashint idEncoder(int mv_id, int day, int time, int seatnum);
Info* idDecoder(Hashint hashid);
char* userEncoder(Hashint Reserveid);
Hashint userDecoder(char* encodedStr);
void randomSrand(int* array, int arrsize, int picks, int randomVal);
void initialize(RBT* T);
void reservation(RBT* T);
void canclelation(RBT* T);
void findNode(Node* node, int tdate, int movieTable[5][2], int seatTable[5][200]);
Node* createNode(Hashint key,Color color,Node* left,Node* right,Node* parent);
Node* treeMinimum(RBT* T, Node* x);
Node* treeMaximum(RBT* T, Node* x);
Node* search(RBT* T, Hashint k);
RBT* createRBT();
void leftRotate(RBT* T, Node* x);
void rightRotate(RBT* T, Node* y);
void rbInsertFixup(RBT* T, Node* z);
void rbInsert(RBT* T, Hashint k);
void rbDeleteFixup(RBT* T, Node* x);
void rbDelete(RBT* T, Hashint k);
void rbTransplant(RBT* T, Node* u, Node* v);
int treeHeight(RBT* T,Node* node);
void printLevel(RBT* T, Node* node, int level, int totalLevels);
void PRINT_BST(RBT* T);
void PRINT_BST_TO_FILE(RBT* T, char* filename);
void printLevelToFile(RBT* T, Node* node, int level, int totalLevels, FILE *fp);

int main(){
  
    int command;

    srand(time(NULL));

    /* Initializing Date */
    char date[MAXLINE];
    struct tm *t=gettime();
    sprintf(date,"%d-%d-%02d %s",t->tm_year+1900,t->tm_mon+1,
        t->tm_mday,weekdays[t->tm_wday]);
    
    /* Pre-Proc: making reservation-info table with RANDOM VALUE */
    /* Initializing RB Tree info table */
    RBT *movieTree = createRBT();
    initialize(movieTree);
    //PRINT_BST(movieTree);
    // showInfo(movieTree,4);
    // reservation(movieTree);
    // canclelation(movieTree);
    
    // showMovieid(movieTree);

    // return 0;
    while(1)
    {
        /* Printing tree for test case */
        char filename[20];
        sprintf(filename, "tree%d.txt", executionCount + 1);
        PRINT_BST_TO_FILE(movieTree,filename);
        
        /* User Interface */
        command = userInterface(date);     

        
        
        switch (command) {
            case 0:
                showMovieschdule(movieTree);
                break;

            case 1:
                reservation(movieTree);
                /* reservation must be shown in a Table ( RB Tree print where? )*/
                break;

            case 2:
                canclelation(movieTree);
                break;

            case 3:
                showMovieid(movieTree);
                break;

            case 4:
                printf("Bye!\n");
                return 0;

            default:
                printf("Unknown Command.Try Again!\n");
                break;
                
        }
        executionCount++;
    }   

    return 0;
}





/* LEVEL3 Manual Functions */


Hashint idEncoder(int mv_id, int day, int time, int seatnum){

    Hashint encoding=0;
    /*
    day  time   id  seat
    00    00   000   000
    */
    encoding += seatnum *pow(10,0); 
    encoding += mv_id   *pow(10,3);
    encoding += time    *pow(10,6);
    encoding += day     *pow(10,8);
    

    return encoding;
}
Info* idDecoder(Hashint hashid){

    Info* decoder = (Info*)malloc(sizeof(Info));

    /* Exception Handling for Sunday */
    decoder->seatnum = hashid % 1000;
    hashid /= 1000;

    decoder->mv_id = hashid % 1000;
    hashid /= 1000;

    decoder->time = hashid % 100;
    hashid /= 100;

    decoder->day = hashid;

    return decoder;
}
char* userEncoder(Hashint Reserveid){

    char    processHash[50];  // Array for processing ID hash
    char*   encoding;

    encoding = (char*)calloc(50, sizeof(char));

    sprintf(processHash,"%llu",Reserveid);
    
    for (int i = 0; i < strlen(processHash); i++){
        
        char mapping[2];
        
        mapping[0] = 'A' + (processHash[i] - '0');
        mapping[1] = '\0';

        strcat(encoding,mapping);
    }

    return encoding; 
}
Hashint userDecoder(char* encodedStr) {
    
    int len = strlen(encodedStr);
    Hashint decoded = 0;

    for (int i = 0; i < len; i++) {
        decoded = decoded * 10 + (encodedStr[i] - 'A');
    }

    return decoded;
}
void randomSrand(int* array, int arrsize, int picks, int randomVal){

    // srand(time(NULL));
    int count=0;

    while (count < picks)
    {
        int random = rand() % randomVal; // seats number additional addition
        int chk=0;

        for (int i = 0; i < count; i++)
        {
            if (array[i]==random){
                chk=1;
                break;
            }
        }
        
        if (!chk)
            array[count++] = random;
    }

}
void initialize(RBT* T){

    struct tm *t=gettime();

    int wday = t->tm_wday; // (0)sun ~ (6)sat
    int maxseats = 200; // number of seats
    int initseats = 60; // number of initial seats
    int timestamp = 5;  // max movies and timeline for one day

    int timeArray[5];
    int seatArray[200];

    Info* decoder = (Info*)malloc(sizeof(Info*)); 

    for (int i = 0; i < 7; i++) {
        wday = wday % 7; // (0)sun ~ (6)sat
        
        randomSrand(timeArray,timestamp,5,24);

        for (int j = 0; j < timestamp; j++){
            
            int count=0, movieindex=0;
            randomSrand(seatArray,maxseats,initseats,maxseats);
            movieindex = timeArray[j] * (35.0/24);

            while (count < initseats)
            {
                Hashint hashnum = idEncoder(movieindex, wday,
                    timeArray[j], seatArray[count]);
                Info* decoder = (Info*)malloc(sizeof(Info));
                // printf("(wday): %2d (time): %2d (seat): %2d (hash): %llu\n",wday, timeArray[j],
                //    count, hashnum);
                decoder = idDecoder(hashnum);
                // if(wday==1)
                //     printf("mvid:%d  day:  %d  time:  %d,  seatnum:  %d,  hashnum:  %llu\n",decoder->mv_id,
                //     decoder->day,decoder->time,decoder->seatnum,hashnum);
                rbInsert(T,hashnum);  /* Insertion of movie hash into RB Tree */
                count++;
            }
            
        }

        wday++;
    }
}
void reservation(RBT* T){
    
    int option=-1;      //   default:    -1, MovieList: 0, 
                        //   MovieTable:  1, DayTable : 2, 
                        //   Reservation: 3, exit     : 4

    struct tm *t=gettime();

    int wday = t->tm_wday;

    while (1)
    {
        int     date=-1;        // used for searching user Input date
        int     movieIndex=-1;  // used for searching user Input movie index
        int     reservTime=-1;  // used for Storing reservation Movie time
        char    caseInput[50];  // used for processing user Input date
        char    caseInput2[50]; // used for processing user Input Movie
        RBT*    searchTree = T; // used for searching movies

        
        printf("Which information do you need?\n");
        printf("Movie List  : type 0\n");
        printf("Movie Table : type 1\n");
        printf("Day Table   : type 2\n");
        printf("Reservation : type 3\n");
        printf("Exit        : type 4\n");
        scanf("%d",&option);
        getchar();
        switch (option)
        {
            case 0:
                printf("Which day would you like to search? ex) Monday, Tuesday... ");
                scanf("%[^\n]s",caseInput);
                getchar();
                date = getDay(caseInput);
                
                if(date==-1){
                    printf("Invalid day String\n");
                    break;
                }

                searchTree = T;
                showInfo(searchTree,date,option);
                break;
            
            case 1:
                printf("Which day would you like to search? ex) Monday, Tuesday... ");
                scanf("%[^\n]s",caseInput);
                getchar();
                date = getDay(caseInput);

                if(date==-1){
                    printf("Invalid day String\n");
                    break;
                }

                printf("Which movie would you like to search?   ");
                scanf("%[^\n]s",caseInput2);
                getchar();
                movieIndex = getMovie(caseInput2);

                if(movieIndex==-1){
                    printf("Invalid movie String\n");
                    break;
                }              
                
                searchTree = T;
                reservTime = showSingleinfo(searchTree,date,caseInput2);

                if(reservTime==-1){
                    printf("No such movie available that day!\n");
                }
                break;

            case 2:
                printf("Which day would you like to search? ex) Monday, Tuesday... ");
                scanf("%[^\n]s",caseInput);
                getchar();
                date = getDay(caseInput);
                
                if(date==-1){
                    printf("Invalid day String\n");
                    break;
                }

                searchTree = T;
                showInfo(searchTree,date,option);
                break;

            case 3:
                printf("Which day would you like to reserve?  ");
                scanf("%[^\n]s",caseInput);
                getchar();
                date = getDay(caseInput);

                if(date==-1){
                    printf("Invalid day String\n");
                    break;
                }

                searchTree = T;
                showInfo(searchTree,date,option-3); // Option-3 for Movie List

                printf("Which movie would you like to reserve?");
                scanf("%[^\n]s",caseInput2);
                getchar();
                movieIndex = getMovie(caseInput2);

                if(movieIndex==-1){
                    printf("Invalid movie String\n");
                    break;
                }              
                
                searchTree = T;
                reservTime = showSingleinfo(searchTree,date,caseInput2);

                if(reservTime==-1){
                    printf("No such movie available that day!\n");
                }
                break;

            case 4:
                break;

            default:
                break;
        }

        /* Reservation Logic */
        if((reservTime!=-1) && (option==3)){
            
            Hashint Reserveid;

            int seatnum;

            printf("Which Seat do you prefer?");
            scanf("%d",&seatnum);
            getchar();

            if(seatnum<0 || seatnum>199){
                printf("Invalid seat number\n");
                return;
            }


            Reserveid = idEncoder(movieIndex,date,reservTime,seatnum);

            searchTree = T;
            if(search(searchTree,Reserveid)->key!=Reserveid){
                rbInsert(T,Reserveid);
                printf("Reservation Completed!\n");
                printf("Your ID: %s\n",userEncoder(Reserveid));
                //printf("(testcase)Your key value: %llu\n",Reserveid);
            }

            else{
                printf("The seat is already occupied.\n");
            }
        }

        if(option==4)
            break;
    }

}
void canclelation(RBT* T){

    Hashint cancleId;

    int     seatnum;        // used for getting seat number from user
    int     date=-1;        // used for searching user Input date
    int     movieIndex=-1;  // used for searching user Input movie index
    int     reservTime=-1;  // used for Storing reservation Movie time
    char    caseInput[50];  // used for processing user Input date
    char    caseInput2[50]; // used for processing user Input Movie
    char    decodeId[50];   // used for decoding the string of User id
    RBT*    searchTree = T; // used for searching movies

    printf("Which day would you like to cancle?  ");
    scanf("%[^\n]s",caseInput);
    getchar();
    date = getDay(caseInput);

    if(date==-1){
        printf("Invalid day String\n");
        return;
    }

    showInfo(searchTree,date,0);

    printf("Which movie would you like to cancle?  ");
    scanf("%[^\n]s",caseInput2);
    getchar();
    movieIndex = getMovie(caseInput2);

    if(movieIndex==-1){
        printf("Invalid movie String\n");
        return;
    }              
    
    searchTree = T;
    reservTime = showSingleinfo(searchTree,date,caseInput2);

    if(reservTime==-1){
        printf("No such movie available that day!\n");
        return;
    }

    

    printf("Which Seat would you like to cancle?  ");
    scanf("%d",&seatnum);
    getchar();

    if(seatnum<0 || seatnum>199){
        printf("Invalid seat number\n");
        return;
    }

    cancleId = idEncoder(movieIndex,date,reservTime,seatnum);

    printf("Enter your reservation ID(HINT:%s):  ",userEncoder(cancleId));
    scanf("%[^\n]s",decodeId);
    getchar();

    if(strcmp(decodeId,userEncoder(cancleId))){
        printf("Unvalid ID!\n");
        return;
    }    

    searchTree = T;
    if(search(searchTree,cancleId)->key==cancleId){
        rbDelete(T,cancleId);
        printf("Canclelation Completed!\n");
        printf("Your ID: %s\n",userEncoder(cancleId));
    }

    else{
        printf("The seat is not reserved.\n");
        return;
    }
    
}
void findNode(Node* node, int tdate, int movieTable[5][2], int seatTable[5][200]){


    if (node==NULL) {
        return;
    }

    Info* decoder = idDecoder(node->key);

    if (decoder->day == tdate) {
        findNode(node->left, tdate, movieTable, seatTable);

        /* Avoiding Dummy datas for 0 index */
        if(/*tdate==0 &&*/ node->key==0){
            return;
        }

        if(fcount==0){
            movieTable[movieCount][0] = decoder->mv_id;
            movieTable[movieCount][1] = decoder->time;
            fcount += 1;
        }

        if(movieTable[movieCount][1] != decoder->time){
            movieCount++;
            change_flag=1;
        }

        if(movieCount==6){
            movieCount=0;
            fcount=0;
            return;
        }

        if(change_flag==1){
            movieTable[movieCount][0] = decoder->mv_id;
            movieTable[movieCount][1] = decoder->time;
            change_flag=0;
        }

        /* set seat numbers to 1~200 and date to 1~7 */
        seatTable[movieCount][decoder->seatnum] = 1;

        findNode(node->right, tdate, movieTable, seatTable);
    } else if (decoder->day > tdate) {
        findNode(node->left, tdate, movieTable, seatTable);
    } else {
        findNode(node->right, tdate, movieTable, seatTable);
    }

    free(decoder);
}






/* RBT instructions */

Node* createNode(Hashint key,Color color,Node* left,Node* right,Node* parent){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->color = color;
    newNode->left = left;
    newNode->right = right;
    newNode->parent = parent;
    return newNode;
}

RBT* createRBT() {
    RBT* Tree = (RBT*)malloc(sizeof(RBT));
    Tree->nil = createNode(0, BLACK, NULL, NULL, NULL);
    Tree->root = Tree->nil;
    Tree->insertions = 0;
    Tree->deletions = 0;
    Tree->leftRotations = 0;
    Tree->rightRotations = 0;

    return Tree; 
}

Node* search(RBT* T, Hashint k) {
    Node* x = T->root;
    while (x != T->nil && k != x->key) {
        if (k < x->key) x = x->left;
        else x = x->right;
    }
    return x;   // returns nil if not FOUND
}

void leftRotate(RBT* T, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != T->nil) 
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil) 
        T->root = y;
    else if (x == x->parent->left) 
        x->parent->left = y;
    else 
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    T->leftRotations++;
}

void rightRotate(RBT* T, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == T->nil) 
        T->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else 
        y->parent->right = x;
    x->right = y;
    y->parent = x;
    T->rightRotations++;
}

void rbInsertFixup(RBT* T, Node* z) {
    while (z->parent->color == RED) 
    {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(T, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

void rbInsert(RBT* T, Hashint k) {
    Node* z = createNode(k, RED, T->nil, T->nil, NULL);
    Node* y = T->nil;
    Node* x = T->root;

    while (x != T->nil)
    {
        y = x;
        if (z->key < x->key) 
            x = x->left;
        else if (z->key > x->key) 
            x = x->right;
        else 
            return;  // 키가 있으면 그냥 return
    }

    z->parent = y;
    if (y == T->nil) 
        T->root = z;
    else if (z->key < y->key) 
        y->left = z;
    else 
        y->right = z;

    rbInsertFixup(T, z);
    T->insertions++;
}

void rbDeleteFixup(RBT* T, Node* x) {
    while (x != T->root && x->color == BLACK) 
    {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(T, w);
                    w = x->parent->right;
                    continue; // 수정
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(T, x->parent);
                x = T->root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(T, w);
                    w = x->parent->left;
                    continue; // 수정
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

void rbDelete(RBT* T, Hashint k) {
    Node* z = search(T, k);
    if (z == T->nil) return; // 키 없을 시

    Node* y = z;
    Color y_original_color = y->color;
    Node* x;

    if (z->left == T->nil) {
        x = z->right;
        rbTransplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        rbTransplant(T, z, z->left);
    } else {
        //y = treeMinimum(T, z->right);
        y = treeMaximum(T, z->left);
        y_original_color = y->color;
        // x = y->right;
        x = y->left;
        if (y->parent == z) {
            x->parent = y;
        } else {
            // rbTransplant(T, y, y->right);
            rbTransplant(T, y, y->left);
            // y->right = z->right;
            // y->right->parent = y;
            y->left = z->left;
            y->left->parent = y;
        }
        rbTransplant(T, z, y);
        //y->left = z->left;
        //y->left->parent = y;
        y->right = z->right;
        y->right->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK) {
        rbDeleteFixup(T, x);
    }
    T->deletions++;
}

void rbTransplant(RBT* T, Node* u, Node* v) {
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node* treeMinimum(RBT* T, Node* x) {
    while (x->left != T->nil) {
        x = x->left;
    }
    return x;
}

Node* treeMaximum(RBT* T, Node* x) {
    while (x->right != T->nil) {
        x = x->right;
    }
    return x;
}

int treeHeight(RBT* T,Node* node) {
    if (node == NULL || node == T->nil) return 0;
    int leftHeight = treeHeight(T,node->left);
    int rightHeight = treeHeight(T,node->right);
    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

// void printLevel(RBT* T, Node* node, int level, int totalLevels) {
//     if (level == 1) {
//         for (int i = 0; i < totalLevels - level; i++) printf("   ");
//         if (node == T->nil) {
//             printf("nil ");
//         } else {
//             printf("%llu(%c) ", node->key, node->color == RED ? 'R' : 'B');
//         }
//     } else if (level > 1) {
//         if (node == T->nil) {
//             printLevel(T, T->nil, level - 1, totalLevels);
//             printLevel(T, T->nil, level - 1, totalLevels);
//         } else {
//             printLevel(T, node->left, level - 1, totalLevels);
//             printLevel(T, node->right, level - 1, totalLevels);
//         }
//     }
// }

// void PRINT_BST(RBT* T) {
//     int height = treeHeight(T,T->root);
//     for (int i = 1; i <= height; i++) {
//         printLevel(T, T->root, i, height);
//         printf("\n");
//     }
//     printf("Insertion:%d   Deletion:%d   Left_Rotate:%d   Right_Rotate:%d\n",T->insertions,T->deletions, T->leftRotations, T->rightRotations);
//     //printf("Average left rotations per operation: %.2f\n", (float)T->leftRotations / (T->insertions + T->deletions));
//     //printf("Average right rotations per operation: %.2f\n", (float)T->rightRotations / (T->insertions + T->deletions));
//     //printf("\n");
// }

void printLevel(RBT* T, Node* node, int level, int totalLevels) {
    if (node == T->nil) {
        if (level == 1) {
            for (int i = 0; i < totalLevels - level; i++) printf("   ");
            printf("nil ");
        }
        return;
    }
    if (level == 1) {
        for (int i = 0; i < totalLevels - level; i++) printf("   ");
        printf("%llu(%c) ", node->key, node->color == RED ? 'R' : 'B');
    } else {
        printLevel(T, node->left, level - 1, totalLevels);
        printLevel(T, node->right, level - 1, totalLevels);
    }
}

void PRINT_BST(RBT* T) {
    int height = treeHeight(T, T->root);
    for (int i = 1; i <= height; i++) {
        printf("Level %03d: ", i);
        printLevel(T, T->root, i, height);
        printf("\n");
    }
    
    printf("Insertion:%d   Deletion:%d   Left_Rotate:%d   Right_Rotate:%d\n", T->insertions, T->deletions, T->leftRotations, T->rightRotations);
}

void PRINT_BST_TO_FILE(RBT* T, char* filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    int height = treeHeight(T, T->root);
    for (int i = 1; i <= height; i++) {
        fprintf(fp, "Level %03d: ", i); 
        printLevelToFile(T, T->root, i, height, fp);
        fprintf(fp, "\n");
    }

    fprintf(fp, "Insertion:%d   Deletion:%d   Left_Rotate:%d   Right_Rotate:%d\n", 
            T->insertions, T->deletions, T->leftRotations, T->rightRotations);

    fclose(fp);
}

void printLevelToFile(RBT* T, Node* node, int level, int totalLevels, FILE *fp) {
    if (node == T->nil) {
        if (level == 1) {
            for (int i = 0; i < totalLevels - level; i++) fprintf(fp, " ");
            fprintf(fp, "nil ");
        }
        return;
    }
    if (level == 1) {
        for (int i = 0; i < totalLevels - level; i++) fprintf(fp, " ");
        fprintf(fp, "%llu(%c) ", node->key, node->color == RED ? 'R' : 'B');
    } else {
        printLevelToFile(T, node->left, level - 1, totalLevels, fp);
        printLevelToFile(T, node->right, level - 1, totalLevels, fp);
    }
}



/* LEVEL2 Manual Functions */


void showInfo(RBT* T,int tdate, int option){
    
    int mv_mask = 0; // set to make sure that every movie counts
    int size = 200 * 5; // seats X movie time

    /* initializing infoTable */
    int seatTable[5][200]={0};
    int movieTable[5][2]={0};   // set to 2D array for storing mv_id and time
    
    fcount=0; seatCount=0; movieCount=0; // setting Global value to 0 for further search

    findNode(T->root, tdate, movieTable, seatTable);
    
    if(option==0)
        printdayList(movieTable);
    
    /* Need of date data and MovieString passed as a indiv func */
    // if(option==1)
    //     printsingleTable(movieTable[0],seatTable[0]);

    if(option==2)
        printdayTable(movieTable,seatTable);

}
int showSingleinfo(RBT *T,int tdate, char* movieName){

    /* initializing infoTable */
    int seatTable[5][200]={0};
    int movieTable[5][2]={0};   // set to 2D array for storing mv_id and time

    fcount=0; seatCount=0; movieCount=0; // setting Global value to 0 for further search

    findNode(T->root, tdate, movieTable, seatTable);

    int index=-1; // Set index for finding movie

    for (int i = 0; i < 5; i++){
        if(!strcmp(movieName,movies[movieTable[i][0]])){
            printsingleTable(movieTable[i],seatTable[i]);
            return movieTable[i][1]; // Returning time for moovie
        }
            
    }
    return -1;
}
void showMovieid(RBT* T){
    
    int     searchTime=-1;  // used for checking movie
    int     date=-1;        // used for searching user Input date
    int     movieIndex=-1;  // used for searching user Input movie index
    char    caseInput[50];  // used for processing user Input date
    char    caseInput2[50]; // used for processing user Input Movie
    RBT*    searchTree = T; // used for searching movies

    /* initializing infoTable */
    int seatTable[5][200]={0};
    int movieTable[5][2]={0};   // set to 2D array for storing mv_id and time
    
    printf("Which day would you like to search?  ");
    scanf("%[^\n]s",caseInput);
    getchar();
    date = getDay(caseInput);

    if(date==-1){
        printf("Invalid day String\n");
        return;
    }

    searchTree = T;
    showInfo(searchTree,date,0); 

    printf("Which movie would you like to search?");
    scanf("%[^\n]s",caseInput2);
    getchar();
    movieIndex = getMovie(caseInput2);

    if(movieIndex==-1){
        printf("Invalid movie String\n");
        return;
    }              
    
    searchTree = T;

    fcount=0; seatCount=0; movieCount=0; // setting Global value to 0 for further search

    findNode(searchTree->root, date, movieTable, seatTable);

    int searchMovie=0;

    while (searchMovie<5)
    {
        if(!strcmp(movies[movieTable[searchMovie][0]],caseInput2)){
            printIdlist(date,movieTable[searchMovie],seatTable[searchMovie]);
            break;
        }
        searchMovie++;
    }
    

    if(searchMovie==5){
        printf("No such movie available that day!\n");
        return;
    }
}
void showMovieschdule(RBT* T){

    /* Initializing Date */
    struct tm *t=gettime();

    int mday = t->tm_mday;
    int wday = t->tm_wday;

    for(int tdate=0; tdate<7; tdate++){

        wday = wday % 7;
        /* initializing infoTable */
        int seatTable[5][200]={0};
        int movieTable[5][2]={0};   // set to 2D array for storing mv_id and time

        fcount=0; seatCount=0; movieCount=0; // setting Global value to 0 for further search                
        findNode(T->root, wday, movieTable, seatTable);

        printf("Date:    %d-%2d-%02d  %s\n",t->tm_year+1900,t->tm_mon+1,mday,weekdays[wday]);

        for (int i = 0; i < 5; i++)
        {
            int seatCount=0;

            printf("%2d:00    %-25s   ",movieTable[i][1],movies[movieTable[i][0]]);
            for (int j = 0; j < 200; j++)
            {
                if(seatTable[i][j]==1)
                    seatCount++;    
            }
            printf("Reserved:  %2d  Avaliable:  %2d\n",seatCount,(200-seatCount));
            
        }
        
        printf("\n\n");
        wday++; mday++;
    }
}


/* LEVEL 1 */

int userInterface(char* date){
    
    char user_command[MAXLINE];
    memset(user_command,0,MAXLINE);

    printf("**************************************\n");
    printf("* Welcome to Movie Algorithm Theater *\n");
    printf("************ 2020311595 **************\n");
    printf("************ Park Je Hyun ************\n");
    printf("**************************************\n");
    printf("******* What do you want to do? ******\n");
    printf("**************************************\n");
    printf("* Options ****************************\n");
    printf("* 1. movie schedule ******************\n");
    printf("* 2. reservation *********************\n");
    printf("* 3. canclellation *******************\n");
    printf("* 4. id list *************************\n");
    printf("* 5. exit ****************************\n");
    printf("**************************************\n");
    printf("TODAY: %s\n",date);
    printf("Enter your command: ");
    scanf("%[^\n]s",user_command);
    getchar();
    return theaterCommand(user_command);
}
int theaterCommand(char* user_command){
    
    if(!strcmp(user_command,"movie schedule"))
        return 0;
    
    if(!strcmp(user_command,"reservation"))
        return 1;
    
    if(!strcmp(user_command,"canclellation"))
        return 2;
    
    if(!strcmp(user_command,"id list"))
        return 3;    

    if(!strcmp(user_command,"exit"))
        return 4;

    return -1;
    /* If there is no equals it goes to default */

}
void printdayTable(int movieTable[5][2], int seatTable[5][200]){

    for (int i = 0; i < 5; i++)
    {
        printf("movie name: %s\ntime:  %02d:00",movies[movieTable[i][0]],movieTable[i][1]);
        for (int j = 0; j < 200; j++)
        {
            if(j%20==0){
                printf("\n_________________________________________________________________________________________________________________________\n");
                printf("| ");
            }
            
            if (seatTable[i][j]==0){
                printf("%03d | ",j);
                
            }
            else {
                printf(" X  | ");
            }
        }
        printf("\n\n-------------------------------------------------------------------------------------------------------------------------\n\n\n");   
    }
}
void printsingleTable(int* movieTable, int* seatTable){
    
    printf("movie name: %s\ntime:  %02d:00",movies[movieTable[0]],movieTable[1]);
    for (int j = 0; j < 200; j++)
    {
        if(j%20==0){
            printf("\n_________________________________________________________________________________________________________________________\n");
            printf("| ");
        }
        
        if (seatTable[j]==0){
            printf("%03d | ",j);
            
        }
        else {
            printf(" X  | ");
        }
    }
    printf("\n\n-------------------------------------------------------------------------------------------------------------------------\n\n\n");

}
void printdayList(int movieTable[5][2]){
    for (int i = 0; i < 5; i++)
    {
        if (i==0){
            printf(" ________________________________________________________________\n");
        }
        
        printf("|   %-45s     %02d:00      |\n",movies[movieTable[i][0]],movieTable[i][1]);
        printf("|________________________________________________________________|\n");
    }
    
}
void printIdlist(int day, int* movieTable, int* seatTable){

    printf("movie name: %s  time:  %02d:00\n",movies[movieTable[0]],movieTable[1]);
    for (int i = 0; i < 200; i++)
    {
        if(seatTable[i]==1){
            Hashint userID;
            userID = idEncoder(movieTable[0],day,movieTable[1],i);
            printf("%-15s\n",userEncoder(userID));
        }
    } 
}

int getMovie(char* input){
    for (int i = 0; i < movieNum; i++)
    {
        if(!strcmp(input,movies[i]))
            return i;
    }
    return -1;
}
int getDay(char* input){

    for (int i = 0; i < 7; i++)
    {
        if(!strcmp(input,weekdays[i]))
            return i;
    }
    return -1;
}
struct tm* gettime() {
    time_t timer = time(NULL);
    struct tm *result;

    struct tm *t = localtime(&timer);
    if (t != NULL) {
        result = t;
    } else {
        memset(result, 0, sizeof(*result));
    }

    return result;
}