#include "clui.h"
#include <iostream>
#include <time.h>
#include <string.h>

#define POS1 ((game->position[0]-3))
#define POS2 ((game->position[1]-16)/3)

using namespace std;
struct gamesave {
    int** printed = NULL;
    int** value = NULL;
    char** flag = NULL;
    int rows;
    int cols;
    int goodprint = 0;
    int flagprinted;
    int minecount;
    int position[2];
};

struct user {
    string username;
    double score=0;
    int mapcount=0;
    gamesave* usergames = NULL;
};

void menuprint(string currentuser);
int menuchoice();
void leaderboard(int usercount,user* playerlist);
void usersort(int usercount,user* playerlist);
void changeuser(int &usercount,user* &playerlist,int &currentuser);
user* newuser(int* usercount,user* playerlist,string newusername);
gamesave* newgame(int* mapcount,gamesave* game);
void mapprint(int rows,int cols,int flag);
void mapset(gamesave* game);
void play(gamesave* game,int &mapcount,double &score);
void draw(int pos1,int pos2);
void remove(int pos1,int pos2);
void print(gamesave* game,int x,int y);
void BOOM(gamesave* game,int wl);
void load (gamesave* game);
int checkuser(user* playerlist,int usercount, string usernamet);
void aftergame(gamesave* game,gamesave* tempsave,int &mapcount,double &score,int &flag);


int main(){
    srand(time(0));
    init_clui();
    char welcome[35]={'H','e','l','l','o',' ','P','l','a','y','e','r',' ',':',')','\n','W','h','a','t','\'','s',' ','y','o','u','r',' ','n','a','m','e',':',' '};
    for (int i=0;i<35;i++){
        cout << welcome[i];
        delay(50);
    }
    int usercount=0;
    string what;
    getline(cin, what);
    user* playerlist=NULL;
    playerlist=newuser(&usercount,playerlist,what);
    int currentuser=0,currentmap=0;
    playerlist->score=0;
    while(true){
       menuprint(playerlist[currentuser].username);
       int menuchoicev=menuchoice();
       switch(menuchoicev){
            case 1:{
                playerlist[currentuser].usergames=newgame(&(playerlist[currentuser].mapcount),playerlist[currentuser].usergames);
                currentmap=playerlist[currentuser].mapcount-1;
                mapset(&(playerlist[currentuser].usergames[currentmap]));
                cout << "map count " << playerlist[currentuser].mapcount << " currentmap: " << currentmap;
                getch();
                mapprint(playerlist[currentuser].usergames[currentmap].rows,playerlist[currentuser].usergames[currentmap].cols,playerlist[currentuser].usergames[currentmap].minecount);
                play(&(playerlist[currentuser].usergames[currentmap]),playerlist[currentuser].mapcount,playerlist[currentuser].score);
                break;
            }
            case 2:{
                clear_screen();
                if (playerlist[currentuser].mapcount>0){
                    for (int i=0;i<playerlist[currentuser].mapcount;i++){
                        cout << i+1 << ". " << playerlist[currentuser].usergames[i].rows << 'X' << playerlist[currentuser].usergames[i].cols << " mine count: "
                        << playerlist[currentuser].usergames[i].minecount << " " << playerlist[currentuser].usergames[i].goodprint << " printed\n";
                    }
                    int gamechose;
                    cout << "please enter the game: ";
                    cin >> gamechose;
                    if (gamechose>0 && gamechose<=playerlist[currentuser].mapcount){
                        mapprint(playerlist[currentuser].usergames[currentmap].rows,playerlist[currentuser].usergames[currentmap].cols,playerlist[currentuser].usergames[gamechose-1].minecount-playerlist[currentuser].usergames[gamechose-1].flagprinted);
                        load(&playerlist[currentuser].usergames[gamechose-1]);
                        play(&playerlist[currentuser].usergames[gamechose-1],playerlist[currentuser].mapcount,playerlist[currentuser].score);
                        getch();
                    }
                    else {
                        cin >> gamechose;
                    }
                }
                else {
                    cout << "you dont have any games";
                    delay(2000);
                    break;
                }
                //load game
                break;
            }
            case 3:{
                changeuser(usercount,playerlist,currentuser);
                break;
            }
            case 4:{
                what=playerlist[currentuser].username;
                leaderboard(usercount,playerlist);
                checkuser(playerlist,usercount,what);
                break;
            }
            case 5:{
                quit();
                break;
            }
       }
    }
    return 0;
}

void menuprint(string currentuser){
    clear_screen();
    cout << "username : " << currentuser << '\n';
    cout << " New Game" << '\n' << " Load Game" << '\n' << " Change Name" << '\n' << " Leaderboard" << '\n' << " Exit";
}

int menuchoice(){
    cursor_to_pos(2,1);
    cout << '>';
    cursor_to_pos(2,1);
    int y=2,input=0;
    while (true){
        input=getch();
        switch(input){
            case 119:
                if (y>2){
                    cursor_to_pos(y,1);
                    cout << ' ';
                    y-=1;
                    cursor_to_pos(y,1);
                    cout << '>';
                    cursor_to_pos(y,1);
                }
                break;
            case 115:
                if (y<6){
                    cursor_to_pos(y,1);
                    cout << ' ';
                    y+=1;
                    cursor_to_pos(y,1);
                    cout << '>';
                    cursor_to_pos(y,1);
                }
                break;
            case 32:
                return y-1;
        }
    }
}

void usersort(int usercount,user* playerlist){
    user temp;
    for (int i=usercount;i>1;i--){
        for (int j=1;j<i;j++){
            if (playerlist[j].score>playerlist[j-1].score){
                temp=playerlist[j];
                playerlist[j]=playerlist[j-1];
                playerlist[j-1]=temp;
            }
        }
    }
}

void leaderboard(int usercount,user* playerlist){
    clear_screen();
    cout << usercount;
    usersort(usercount,playerlist);
    cout << "--------------------------------------------------------\n";
    for (int i=0;i<5 && i<usercount;i++){
        cout << i+1 << ": " << playerlist[i].username;
        cursor_to_pos(i+2,35);
        cout << "score: " << playerlist[i].score << '\n';
    }
    cout << "\n--------------------------------------------------------";
    int input;
    while(true){
        input=getch();
        if (input==32){
            break;
        }
    }
}

int checkuser(user* playerlist,int usercount, string usernamet){
    int currentuser;
    for (int i=0;i<usercount;i++){
        if (usernamet==playerlist[i].username){
            currentuser=i;
            break;
        }
    }
    return currentuser;
}

void changeuser(int &usercount,user* &playerlist,int &currentuser){
    clear_screen();
    string tempstr;
    cout << "enter your username: ";
    getline(cin, tempstr);
    int stringcheckflag=0;
    for (int i=0;i<usercount;i++){
        if (tempstr==playerlist[i].username){
            currentuser=i;
            stringcheckflag=1;
            break;
        }
    }
    if (stringcheckflag==0){
        cout << tempstr;
        playerlist=newuser(&usercount,playerlist,tempstr);
        getch();
        currentuser=usercount-1;
    }
}

user* newuser(int* usercount,user* playerlist,string newusername){
    user* temp = new user[*usercount+1];
    for (int i=0;i<*usercount;i++){
        temp[i]=playerlist[i];
    }
    temp[*usercount].username=newusername;
    *usercount+=1;
    delete[] playerlist;
    return temp;
}

gamesave* newgame(int* mapcount,gamesave* game){
    gamesave* temp = new gamesave[*mapcount+1];
    for (int i=0;i<*mapcount;i++){
        temp[i]=game[i];
    }
    *mapcount+=1;
    delete[] game;
    return temp;
}

void mapprint(int rows,int cols,int flag){
    clear_screen();
    cout << "---";
    SetConsoleOutputCP(CP_UTF8);
    cout << "---";
    for (int i=0;i<3*cols+15;i++){
        if (i==13 || i==3*cols+14){
            change_color_rgb(175, 125, 0);
            cursor_to_pos(2, i+1);
            cout << "╦";
            for (int j=0;j<rows;j++){
                cursor_to_pos(j+3, i+1);
                cout << "║";
            }
            cursor_to_pos(3+rows, i+1);
            cout << "╩";
        }
        else{
            change_color_rgb(175, 125, 0);
            cursor_to_pos(2, i+1);
            cout << "═";
            cursor_to_pos(3+rows, i+1);
            cout << "═";
        }
    }
    cout << "---";
    reset_color();
    cursor_to_pos(3+(rows)/2,2);
    cout << "Flags : " << flag;
    cursor_to_pos(3+(rows)/2,15+((cols-1)/2)*3);
    cout << ">";
    cursor_to_pos(3+(rows)/2,17+((cols-1)/2)*3);
    cout << "<";
    cursor_to_pos(1,1);
    cout << "Press \"F\" to place flag and press \"S\" to select the place and arrow-keys to change the position";
    cursor_to_pos(1,1);
}

void mapset(gamesave* game){
    clear_screen();
    int mines=0,flag=0,maprows=0,mapcols;
    cursor_to_pos(1,1);
    cout << " please select difficulty:";
    cursor_to_pos(1,28);
    cout << "1: 5x5 4 mines";
    cursor_to_pos(2,28);
    cout << "2: 12x12 28 mines";
    cursor_to_pos(3,28);
    cout << "3: 20x20 96 mines";
    cursor_to_pos(4,28);
    cout << "4: Custom";
    cout << "...";
    while (flag==0){
        int input=getch();
        if (input==49){
            maprows=5;
            mapcols=5;
            mines=4;
            flag=1;
            break;
        }
        else if (input==50){
            maprows=12;
            mapcols=12;
            mines=28;
            flag=1;
            break;
        }
        else if (input==51){
            maprows=20;
            mapcols=20;
            mines=96;
            flag=1;
            break;
        }
        else if (input==52){
            clear_screen();
            cout << "please enter map rows: ";
            cin >> maprows;
            cout << "please enter map columns: ";
            cin >> mapcols;
            cout << "please enter mines count: ";
            cin >> mines;
            flag=1;
            break;
        }
    }
    cout << "...";
    game->rows=maprows;
    game->cols=mapcols;
    game->minecount=mines;
    game->flagprinted=0;
    game->goodprint=0;
    game->value=new int*[maprows];
    game->printed=new int*[maprows];
    game->flag=new char*[maprows];
    cout << "...";
    for (int i=0;i<maprows;i++){
        game->value[i]=new int[mapcols];
        game->printed[i]=new int[mapcols];
        game->flag[i]=new char[mapcols];
    }
    cout << "...";
    for (int i=0;i<maprows;i++){
        for (int j=0;j<mapcols;j++){
            game->value[i][j]=0;
            game->printed[i][j]=0;
            game->flag[i][j]='E';
        }
    }
    int temp1,temp2;
    for(int i=0;i<mines;i++){
        temp1=rand()%maprows;
        temp2=rand()%mapcols;
        if (game->value[temp1][temp2]==0){
            game->value[temp1][temp2]=-1;
        }
        else {
            i-=1;
            continue;
        }
    }
    for(int i=0;i<maprows;i++){
        for(int j=0;j<mapcols;j++){
            if (game->value[i][j]!=-1){
                for(int i2=-1;i2<2;i2++){
                    for(int j2=-1;j2<2;j2++){
                        if ( i+i2>=0 && i+i2<maprows && j+j2>=0 && j+j2<mapcols && game->value[i+i2][j+j2]==-1){
                            game->value[i][j]+=1;
                        }
                    }
                }
            }
        }
    }
}

void play(gamesave* game,int &mapcount,double &score){
    gamesave* tempsave= new gamesave;
    tempsave->value=new int*[game->rows];
    tempsave->printed=new int*[game->rows];
    tempsave->flag=new char*[game->rows];
    for (int i=0;i<game->rows;i++){
        tempsave->value[i]=new int[game->cols];
        tempsave->printed[i]=new int[game->cols];
        tempsave->flag[i]=new char[game->cols];
    }
    game->position[0]=3+(game->rows)/2;
    game->position[1]=16+((game->cols-1)/2)*3;
    int saveflag=0;
    int flag=0;
    while (flag==0){
        cursor_to_pos(1,1);
        if(game->goodprint==(game->cols*game->rows)-game->minecount){
            BOOM(game,1);
            aftergame(game,tempsave,mapcount,score,flag);
            if (saveflag==1){
                game=tempsave;
            }
            break;
        }
        int input=getch();
        switch (input){
            case 83:
            case 115:{
                //s
                if (game->position[0]<2+(game->rows)){
                    remove(game->position[0],game->position[1]);
                    game->position[0]+=1;
                    draw(game->position[0],game->position[1]);
                }
                break;
            }
            case 87:
            case 119:{
                //w
                if (game->position[0]>3){
                    remove(game->position[0],game->position[1]);
                    game->position[0]-=1;
                    draw(game->position[0],game->position[1]);
                }
                break;
            }
            case 68:
            case 100:{
                //d
                if (game->position[1]<3*game->cols+13){
                    remove(game->position[0],game->position[1]);
                    game->position[1]+=3;
                    draw(game->position[0],game->position[1]);
                }
                break;
            }
            case 65:
            case 97:{
                //a
                if (game->position[1]>16){
                    remove(game->position[0],game->position[1]);
                    game->position[1]-=3;
                    draw(game->position[0],game->position[1]);
                }
                break;
            }
            case 32:{
                if (game->value[POS1][POS2]!=-1 && game->printed[POS1][POS2]==0 && game->flag[POS1][POS2]=='E'){
                    print(game,game->position[0],game->position[1]);
                    score+=(double)((double)game->minecount/(double)((double)game->cols*(double)game->rows))*10.0;
                }
                else if (game->value[POS1][POS2]==-1 && game->flag[POS1][POS2]=='E'){
                    BOOM(game,0);
                    aftergame(game,tempsave,mapcount,score,flag);
                    if (saveflag==1){
                        game=tempsave;
                    }
                }
                break;
            }
            case 70:
            case 102:{
                if (game->flagprinted<game->minecount && game->printed[POS1][POS2]==0 && game->flag[POS1][POS2]=='E'){
                    cursor_to_pos(game->position[0],game->position[1]);
                    change_color_rgb(0, 125, 255);
                    cout << "F";
                    reset_color();
                    cursor_to_pos(3+(game->rows)/2,10);
                    game->flagprinted+=1;
                    game->flag[POS1][POS2]='F';
                    cout << game->minecount-game->flagprinted << " ";
                    cursor_to_pos(1,1);
                }
                else if (game->printed[POS1][POS2]==0 && game->flag[POS1][POS2]=='F'){
                    cursor_to_pos(game->position[0],game->position[1]);
                    cout << " ";
                    cursor_to_pos(3+(game->rows)/2,10);
                    game->flagprinted-=1;
                    game->flag[POS1][POS2]='E';
                    cout << game->minecount-game->flagprinted << " ";
                    cursor_to_pos(1,1);
                }
                break;
            }
            case 111:
            case 79:{
                //o
                *(tempsave)=*game;
                saveflag=1;
                break;
            }
            case 81:
            case 113:{
                //q
                cout << game->rows;
                if (saveflag==0){
                    for (int i=0;i<game->rows;i++){
                        delete[] game->value[i];
                        delete[] game->printed[i];
                        delete[] game->flag[i];
                        delete[] tempsave->value[i];
                        delete[] tempsave->printed[i];
                        delete[] tempsave->flag[i];
                    }
                    delete[] game->value;
                    delete[] game->printed;
                    delete[] game->flag;
                    delete[] tempsave->value;
                    delete[] tempsave->printed;
                    delete[] tempsave->flag;
                    delete game;
                    delete tempsave;
                    mapcount-=1;
                }
                else {
                    *game=*tempsave;
                }
                flag=1;
                break;
            }
        }
    }
}

void remove(int pos1,int pos2){
    cursor_to_pos(pos1,pos2-1);
    cout << " ";
    cursor_to_pos(pos1,pos2+1);
    cout << " ";
    cursor_to_pos(1,1);
}

void draw(int pos1,int pos2){
    cursor_to_pos(pos1,pos2-1);
    cout << ">";
    cursor_to_pos(pos1,pos2+1);
    cout << "<";
    cursor_to_pos(1,1);
}

void print(gamesave* game,int x,int y){
    int xx=x-3,yy=(y-16)/3;
    change_color_rgb(game->value[xx][yy]*50, (6-game->value[xx][yy])*50, 50);
    cursor_to_pos(x, y);
    cout << game->value[xx][yy];
    reset_color();
    game->printed[xx][yy]=1;
    game->goodprint+=1;
    cursor_to_pos(1,1);
    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++){
            if (xx+i>-1 && xx+i<game->rows && yy+j>-1 && yy+j<game->cols){
                if (i==0 && j==0){
                    continue;
                }
                else if (game->value[xx+i][yy+j]!=-1 && game->value[xx][yy]==0 && game->printed[xx+i][yy+j]==0){
                    print(game,x+i,y+(j*3));
                }
            }
        }
    }
}

void BOOM(gamesave* game,int wl){
    char bomb;
    if (wl==1) bomb='B';
    else bomb='X';
    for (int i=0;i<game->rows;i++){
        for (int j=0;j<game->cols;j++){
            if (game->value[i][j]==-1){
                cursor_to_pos(i+3,(j*3)+16);
                change_color_rgb(255, 0, 0);
                cout << bomb;
                reset_color();
                delay(70);
            }
        }
    }
}

void load (gamesave* game){
    for (int i=0;i<game->rows;i++){
        for (int j=0;j<game->cols;j++){
            if (game->printed[i][j]==1){
                change_color_rgb(game->value[i][j]*50, (6-game->value[i][j])*50, 50);
                cursor_to_pos(3+i,16+j*3);
                cout << game->value[i][j];
                reset_color();
            }
            else if (game->flag[i][j]=='F'){
                change_color_rgb(0, 125, 255);
                cursor_to_pos(3+i,16+j*3);
                cout << 'F';
                reset_color();
            }
        }
    }
}

void  aftergame(gamesave* game,gamesave* tempsave,int &mapcount,double &score,int &flag){
    cursor_to_pos(1,1);
    cout << "                                                                                                      ";
    cursor_to_pos(1,1);
    cout << "press q to quit to main menu";
    for (int i=0;i<game->rows;i++){
        delete[] game->value[i];
        delete[] game->printed[i];
        delete[] game->flag[i];
        delete[] tempsave->value[i];
        delete[] tempsave->printed[i];
        delete[] tempsave->flag[i];
    }
    delete[] game->value;
    delete[] game->printed;
    delete[] game->flag;
    delete[] tempsave->value;
    delete[] tempsave->printed;
    delete[] tempsave->flag;
    game->goodprint = 0;
    int winloseflag=1;
    while (winloseflag==1){
        switch((int)getch()){
            case 81:
            case 113:{
                delete game;
                delete tempsave;
                mapcount-=1;
                flag=1;
                winloseflag=0;
                break;
            }
        }
    }
}
