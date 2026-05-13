#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <vector>
#include "../core/Maze.h"
#include "../core/Player.h"
#include "../core/Ghost.h"

using namespace std;

// Console handle for faster access
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Console color codes
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

// Flicker-free Clear Screen using Win32 API
void clearScreen() {
    COORD coord = { 0, 0 };
    DWORD dwWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    DWORD dwSize = csbi.dwSize.X * csbi.dwSize.Y;
    
    FillConsoleOutputCharacterA(hConsole, ' ', dwSize, coord, &dwWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwSize, coord, &dwWritten);
    SetConsoleCursorPosition(hConsole, coord);
}

void gotoxy(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, coord);
}

// Forward declaration
void clearPosition(int row, int col, Maze& maze) {
    if (row < 0 || col < 0) return;
    gotoxy(col * 3, row + 3);
    
    if (maze.getCell(row, col) == 1) {
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        cout << "   ";
        setColor(7);
    } else {
        bool hasPower = false;
        for (const Position& p : maze.getPowerPellets()) {
            if (p.row == row && p.col == col) {
                setColor(14); cout << "🍓 "; setColor(7);
                hasPower = true; break;
            }
        }
        if (!hasPower) {
            bool hasPellet = false;
            for (const Position& p : maze.getPellets()) {
                if (p.row == row && p.col == col) {
                    setColor(15); cout << ".  "; setColor(7);
                    hasPellet = true; break;
                }
            }
            if (!hasPellet) cout << "   ";
        }
    }
}

void drawMazeOptimized(Maze& maze, Player& player, vector<Ghost*>& ghosts, bool fullRedraw = false) {
    static int lastScore = -1;
    static int lastLives = -1;
    static int lastPellets = -1;
    static Position lastP(-1, -1);
    static vector<Position> lastG(4, Position(-1, -1));

    if (fullRedraw) {
        // IMPORTANT: We do NOT use clearScreen() here because it causes blinking.
        // Instead, we gotoxy(0,0) and overwrite.
        gotoxy(0, 0);
        
        // Title
        setColor(14);
        cout << "============================================================" << endl;
        cout << "       AlgoMan - DSA Pac-Man (Console Edition)" << endl;
        cout << "============================================================" << endl;
        setColor(7);
        
        // Draw entire maze
        for (int i = 0; i < maze.getRows(); i++) {
            for (int j = 0; j < maze.getCols(); j++) {
                if (maze.getCell(i, j) == 1) {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                    cout << "   "; setColor(7);
                } else {
                    bool hasPower = false;
                    for (const Position& p : maze.getPowerPellets()) {
                        if (p.row == i && p.col == j) {
                            setColor(14); cout << "🍓 "; setColor(7);
                            hasPower = true; break;
                        }
                    }
                    if (!hasPower) {
                        bool hasPellet = false;
                        for (const Position& p : maze.getPellets()) {
                            if (p.row == i && p.col == j) {
                                setColor(15); cout << ".  "; setColor(7);
                                hasPellet = true; break;
                            }
                        }
                        if (!hasPellet) cout << "   ";
                    }
                }
            }
            cout << endl;
        }
        lastScore = -1; 
        lastP = Position(-1, -1);
        for(auto& p : lastG) p = Position(-1, -1);
    } else {
        // Erase old positions
        Position curP = player.getPosition();
        if (!(curP == lastP)) clearPosition(lastP.row, lastP.col, maze);
        for (size_t i = 0; i < ghosts.size(); i++) {
            Position curG = ghosts[i]->getPosition();
            if (!(curG == lastG[i])) clearPosition(lastG[i].row, lastG[i].col, maze);
        }
    }
    
    // Draw ghosts
    int colors[] = {12, 13, 11, 6}; 
    const char* ghostEmojis[] = {"👻", "👻", "👻", "👻"};
    for (size_t i = 0; i < ghosts.size(); i++) {
        Position gp = ghosts[i]->getPosition();
        lastG[i] = gp;
        gotoxy(gp.col * 3, gp.row + 3);
        if (ghosts[i]->getIsScared()) {
            setColor(15); cout << "💀 "; 
        } else {
            setColor(colors[i]); cout << ghostEmojis[i] << " "; 
        }
        setColor(7);
    }
    
    // Draw player
    Position pp = player.getPosition();
    lastP = pp;
    gotoxy(pp.col * 3, pp.row + 3);
    setColor(14); cout << "C " ; setColor(7);
    
    // Update stats line only if changed
    if (fullRedraw || player.getScore() != lastScore || player.getLives() != lastLives || maze.getPellets().getSize() != lastPellets) {
        lastScore = player.getScore(); lastLives = player.getLives(); lastPellets = maze.getPellets().getSize();
        gotoxy(0, maze.getRows() + 4);
        setColor(10); cout << "Score: " << lastScore << "    | ";
        setColor(12); cout << "Lives: " << lastLives << " | ";
        setColor(11); cout << "Pellets: " << lastPellets << "    " << endl;
        setColor(7);
        cout << "\nControls: W/A/S/D = Move | P = Pause | U = Undo" << endl;
        setColor(8);
        cout << "DSA: Graph(" << maze.getGraph()->getVertices() << " nodes) | ";
        cout << "Stack(" << (player.canUndo() ? "Has moves" : "Empty") << ") | ";
        cout << "LinkedList(" << lastPellets << " pellets)   " << endl;
        setColor(7);
    }
}

// ─── Logo ────────────────────────────────────────────────────────────────────
void drawLogo() {
    gotoxy(0,0);
    setColor(9);
    cout << "\n  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557\n";
    setColor(14);
    cout << "  \u2551  \u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2557     \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557  \u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557   \u2551\n";
    cout << "  \u2551 \u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2551    \u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255d\u2588\u2588\u2554\u2550\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557  \u2551\n";
    cout << "  \u2551 \u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2551    \u2588\u2588\u2551 \u2588\u2588\u2588\u2557\u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2557\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2557 \u2551\n";
    cout << "  \u2551 \u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2551\u2588\u2588\u2551    \u2588\u2588\u2551  \u2588\u2588\u2551\u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2551\u255a\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2551\u2588\u2588\u2551\u255a\u2588\u2557\u2551\n";
    cout << "  \u2551 \u2588\u2588\u2551  \u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2588\u2557\u255a\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255d\u255a\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255d\u2588\u2588\u2551 \u255a\u2588\u2588\u2588\u2551\u2588\u2588\u2551  \u2588\u2588\u2551\u2588\u2588\u2551 \u255a\u2557\u2551\n";
    cout << "  \u2551 \u255a\u2550\u255d  \u255a\u2550\u255d\u255a\u2550\u2550\u2550\u2550\u2550\u255d \u255a\u2550\u2550\u2550\u2550\u2550\u255d  \u255a\u2550\u2550\u2550\u2550\u2550\u255d \u255a\u2550\u255d  \u255a\u2550\u2550\u255d\u255a\u2550\u255d  \u255a\u2550\u255d\u255a\u2550\u255d  \u2551\n";
    setColor(11);
    cout << "  \u2551        D S A   P a c - M a n   E d i t i o n              \u2551\n";
    setColor(9);
    cout << "  \u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d\n";
    setColor(7);
}

// ─── Single button row ────────────────────────────────────────────────────────
void drawBtn(const string& icon, const string& label, bool sel) {
    string body = "  " + icon + "  " + label;
    int pad = 44 - (int)body.size(); if (pad < 0) pad = 0;
    string spaces(pad, ' ');
    if (sel) {
        setColor(14); cout << "  \u25ba ";
        setColor(11); cout << "\u250c"; for(int i=0; i<45; i++) cout << "\u2500"; cout << "\u2510\n";
        setColor(14); cout << "  \u25ba ";
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
        cout << "\u2502" << body << spaces << "\u2502"; setColor(7); cout << "\n";
        setColor(14); cout << "  \u25ba ";
        setColor(11); cout << "\u2514"; for(int i=0; i<45; i++) cout << "\u2500"; cout << "\u2518\n";
    } else {
        setColor(8);
        cout << "    \u250c"; for(int i=0; i<45; i++) cout << "\u2500"; cout << "\u2510\n";
        cout << "    \u2502"; setColor(7); cout << body << spaces; setColor(8); cout << "\u2502\n";
        cout << "    \u2514"; for(int i=0; i<45; i++) cout << "\u2500"; cout << "\u2518\n";
    }
    setColor(7);
}

// ─── Interactive menu: returns chosen index, -1 on ESC ───────────────────────
int runMenu(const string& title, const vector<pair<string,string>>& items) {
    int sel = 0, n = (int)items.size();
    bool first = true;
    while (true) {
        if (first) { clearScreen(); first = false; }
        drawLogo();
        setColor(9); cout << "\n  \u251c\u2500\u2500 "; setColor(15); cout << title; setColor(9); cout << " \u2500\u2500\u2524\n\n";
        for (int i = 0; i < n; i++) { drawBtn(items[i].first, items[i].second, i == sel); cout << "\n"; }
        setColor(8); cout << "  [ \u2191\u2193 Arrow Keys  |  ENTER = Select  |  Shortcut Key  |  ESC = Back ]\n"; setColor(7);
        
        char k = _getch();
        if (k == 0 || k == (char)-32) {
            k = _getch();
            if (k == 72) sel = (sel-1+n)%n;
            if (k == 80) sel = (sel+1)%n;
        } else if (k == '\r') return sel;
        else if (k == 27)    return -1;
        else {
            k = toupper(k);
            for (int i = 0; i < n; i++) {
                string lbl = items[i].second;
                if (lbl.size() > 2 && lbl[0] == '[' && lbl[2] == ']') {
                    if (toupper(lbl[1]) == k) return i;
                }
            }
        }
    }
}

// ─── How To Play screen ───────────────────────────────────────────────────────
void showHowToPlay() {
    clearScreen(); drawLogo();
    setColor(9);  cout << "\n  \u251c\u2500\u2500 "; setColor(15); cout << "HOW TO PLAY"; setColor(9); cout << " \u2500\u2500\u2524\n\n";
    setColor(9);  cout << "  \u250c"; for(int i=0; i<57; i++) cout << "\u2500"; cout << "\u2510\n";
    auto row = [](string lbl, int lc, string val, int vc){
        setColor(9); cout << "  \u2502  "; setColor(lc); cout << lbl; setColor(vc); cout << val;
        int pad = 51-(int)lbl.size()-(int)val.size(); while(pad-->0) cout<<' '; setColor(9); cout << "\u2502\n";
    };
    row("MOVEMENT   ",14,"W / A / S / D  or  Arrow Keys",7);
    row("UNDO       ",10,"U  \u2013 undo your last move",7);
    row("PAUSE      ",13,"P  \u2013 open the pause menu",7);
    row("QUIT       ",12,"Q  \u2013 quit to main menu",7);
    row("",9,"",9);
    row("PELLET \u00b7   ",15,"Eat all dots to win!",7);
    row("POWER  🍓   ",14,"Eat a strawberry \u2013 ghosts flee for 10s",7);
    row("GHOSTS 👻   ",8,"4 ghosts with BFS / Dijkstra / Random AI",7);
    setColor(9); cout << "  \u2514"; for(int i=0; i<57; i++) cout << "\u2500"; cout << "\u2518\n\n";
    drawBtn("←","[B] BACK TO MAIN MENU",true);
    setColor(8); cout << "\n  Press ENTER, B, or ESC to go back\n"; setColor(7);
    char k = _getch(); while(k!='\r' && k!=27 && toupper(k)!='B') k=_getch();
}

// ─── Pause menu: returns 0=resume 1=restart 2=quit-to-menu ───────────────────
int showPauseMenu() {
    vector<pair<string,string>> items = {{"▶","[R] RESUME GAME"},{"↺","[T] RESTART"},{"◄","[M] QUIT TO MENU"}};
    return runMenu("GAME PAUSED", items);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTitleA("AlgoMan - DSA Pac-Man");
    CONSOLE_CURSOR_INFO ci; GetConsoleCursorInfo(hConsole,&ci);
    ci.bVisible=false; SetConsoleCursorInfo(hConsole,&ci);

    bool exitApp = false;
    while (!exitApp) {
        vector<pair<string,string>> mainItems = {{"▶","[S] START GAME"},{"?","[H] HOW TO PLAY"},{"✕","[Q] EXIT"}};
        int choice = runMenu("MAIN MENU", mainItems);
        if (choice == 1) { showHowToPlay(); continue; }
        if (choice == 2 || choice == -1) { exitApp = true; break; }

        bool playAgain = true;
        while (playAgain && !exitApp) {
            playAgain = false;
            Maze maze(15,15); maze.loadDefaultMaze();
            Position playerStart(7,7);
            Player player(playerStart, maze.getNodeAt(7,7));
            vector<Ghost*> ghosts;
            ghosts.push_back(new Ghost(Position(1,5),  maze.getNodeAt(1,5),  GhostAI::BFS_AI,      "Blinky"));
            ghosts.push_back(new Ghost(Position(1,9),  maze.getNodeAt(1,9),  GhostAI::DIJKSTRA_AI, "Pinky"));
            ghosts.push_back(new Ghost(Position(13,5), maze.getNodeAt(13,5), GhostAI::BFS_AI,      "Inky"));
            ghosts.push_back(new Ghost(Position(13,9), maze.getNodeAt(13,9), GhostAI::RANDOM,      "Clyde"));

            clearScreen(); drawLogo();
            setColor(9); cout<<"\n  \u251c\u2500\u2500 "; setColor(15); cout<<"LOADING"; setColor(9); cout<<" \u2500\u2500\u2524\n\n";
            setColor(8); cout<<"  Initialising  "; setColor(9); cout<<"[";
            for(int b=0;b<30;b++){setColor(10+(b%4));cout<<"|";cout.flush();Sleep(20);}
            setColor(9);cout<<"]"; setColor(10);cout<<" READY!\n\n";
            setColor(14); cout<<"       >>> Press ANY KEY to begin <<<\n"; setColor(7);
            _getch();

            bool running=true, quitToMenu=false;
            int totalMoves=0, pelletsCollected=0;
            int initialPellets=maze.getPellets().getSize();
            auto lastGhostMove=chrono::steady_clock::now();
            auto ghostInterval=chrono::milliseconds(500);
            drawMazeOptimized(maze,player,ghosts,true);

            auto lastPlayerMove = chrono::steady_clock::now();
            auto playerInterval = chrono::milliseconds(200);
            char currentDirection = 0; 

            bool needsRedraw = false;
            while(running && player.getLives()>0 && maze.getPellets().getSize()>0) {
                auto now=chrono::steady_clock::now();
                needsRedraw = false;

                if(_kbhit()) {
                    char key=_getch();
                    if(key==0||key==(char)-32){
                        key=_getch();
                        if(key==72)key='w'; else if(key==80)key='s';
                        else if(key==75)key='a'; else if(key==77)key='d';
                    }
                    key=tolower(key);
                    if(key=='p') {
                        int pc=showPauseMenu();
                        if(pc==1){running=false;playAgain=true;} 
                        else if(pc==2){running=false;quitToMenu=true;}
                        else { drawMazeOptimized(maze,player,ghosts,true); } 
                        continue;
                    }
                    if(key=='q'){running=false;quitToMenu=true;break;}
                    if (key == 'w' || key == 'a' || key == 's' || key == 'd') currentDirection = key;
                    else if(key=='u'&&player.canUndo()){
                        player.undoMove(maze);
                        currentDirection = 0; 
                        needsRedraw = true;
                    }
                }

                if(currentDirection != 0 && chrono::duration_cast<chrono::milliseconds>(now-lastPlayerMove)>=playerInterval){
                    lastPlayerMove = now;
                    bool moved = false;
                    if(currentDirection=='w') { if(player.moveDirection(0,maze)) moved=true; }
                    else if(currentDirection=='s') { if(player.moveDirection(1,maze)) moved=true; }
                    else if(currentDirection=='a') { if(player.moveDirection(2,maze)) moved=true; }
                    else if(currentDirection=='d') { if(player.moveDirection(3,maze)) moved=true; }
                    
                    if(!moved) currentDirection = 0; 
                    else {
                        totalMoves++;
                        int cp=maze.getPellets().getSize();
                        if(cp<initialPellets-pelletsCollected) pelletsCollected++;
                        Position pp=player.getPosition();
                        for(auto it=maze.getPowerPellets().begin();it!=maze.getPowerPellets().end();++it){
                            if((*it).row==pp.row&&(*it).col==pp.col){
                                maze.getPowerPellets().remove(*it);
                                player.addScore(50);
                                for(Ghost*g:ghosts)g->setScared(20);
                                break;
                            }
                        }
                        bool caught=false;
                        for(Ghost*g:ghosts){
                            if(g->collidesWith(player)){
                                if(g->getIsScared()){player.addScore(200);g->reset(Position(7,7),maze.getNodeAt(7,7));}
                                else{
                                    caught=true; player.loseLife();
                                    currentDirection = 0;
                                    if(player.getLives()>0){
                                        player.setPosition(playerStart); player.setCurrentNode(maze.getNodeAt(7,7));
                                        ghosts[0]->reset(Position(1,5),maze.getNodeAt(1,5));
                                        ghosts[1]->reset(Position(1,9),maze.getNodeAt(1,9));
                                        ghosts[2]->reset(Position(13,5),maze.getNodeAt(13,5));
                                        ghosts[3]->reset(Position(13,9),maze.getNodeAt(13,9));
                                        drawMazeOptimized(maze,player,ghosts,true);
                                        gotoxy(0,maze.getRows()+8); setColor(12);
                                        cout<<"*** CAUGHT! Life lost! *** "; setColor(7);
                                        this_thread::sleep_for(chrono::milliseconds(1000));
                                        // REMOVED redundant drawMazeOptimized(maze,player,ghosts,true);
                                        // Instead, clear only the message line
                                        gotoxy(0,maze.getRows()+8); cout<<"                           ";
                                    }
                                    break;
                                }
                            }
                        }
                        if(!caught) needsRedraw = true;
                    }
                }

                if(chrono::duration_cast<chrono::milliseconds>(now-lastGhostMove)>=ghostInterval){
                    lastGhostMove=now;
                    for(size_t i=0;i<ghosts.size();i++){
                        ghosts[i]->update(player,maze);
                        if(ghosts[i]->collidesWith(player)){
                            if(ghosts[i]->getIsScared()){
                                player.addScore(200); ghosts[i]->reset(Position(7,7),maze.getNodeAt(7,7));
                            } else {
                                player.loseLife(); currentDirection = 0;
                                if(player.getLives()>0){
                                    player.setPosition(playerStart); player.setCurrentNode(maze.getNodeAt(7,7));
                                    ghosts[0]->reset(Position(1,5),maze.getNodeAt(1,5));
                                    ghosts[1]->reset(Position(1,9),maze.getNodeAt(1,9));
                                    ghosts[2]->reset(Position(13,5),maze.getNodeAt(13,5));
                                    ghosts[3]->reset(Position(13,9),maze.getNodeAt(13,9));
                                    drawMazeOptimized(maze,player,ghosts,true);
                                    gotoxy(0,maze.getRows()+8); setColor(12);
                                    cout<<"*** CAUGHT! Life lost! *** "; setColor(7);
                                    this_thread::sleep_for(chrono::milliseconds(1000));
                                    // REMOVED redundant drawMazeOptimized(maze,player,ghosts,true);
                                    gotoxy(0,maze.getRows()+8); cout<<"                           ";
                                    break;
                                } else break;
                            }
                        }
                    }
                    needsRedraw = true;
                }
                if(needsRedraw) drawMazeOptimized(maze,player,ghosts,false);
                this_thread::sleep_for(chrono::milliseconds(20));
            }
            for(Ghost*g:ghosts) delete g; ghosts.clear();
            if(playAgain) continue;
            if(quitToMenu) break;

            float completion=(float)pelletsCollected/initialPellets*100;
            clearScreen(); drawLogo();
            setColor(9); cout<<"\n  \u251c\u2500\u2500 "; setColor(15); cout<<"POST-GAME SCOREBOARD"; setColor(9); cout<<" \u2500\u2500\u2524\n\n";
            setColor(11);
            cout << "  \u2554"; for(int i=0; i<20; i++) cout << "\u2550"; cout << "\u2566"; for(int i=0; i<25; i++) cout << "\u2550"; cout << "\u2557" << endl;
            cout << "  \u2551 "; setColor(15); cout << "STATISTIC          "; setColor(11); cout << "\u2551 "; setColor(15); cout << "VALUE / STATUS           "; setColor(11); cout << "\u2551" << endl;
            cout << "  \u2560"; for(int i=0; i<20; i++) cout << "\u2550"; cout << "\u256c"; for(int i=0; i<25; i++) cout << "\u2550"; cout << "\u2563" << endl;
            auto statRow = [](string label, string value, int valColor) {
                setColor(11); cout << "  \u2551 "; setColor(7); cout << label;
                for(int i=0; i<18-(int)label.length(); i++) cout << " ";
                setColor(11); cout << "\u2551 "; setColor(valColor); cout << value;
                for(int i=0; i<23-(int)value.length(); i++) cout << " ";
                setColor(11); cout << "\u2551" << endl;
            };
            statRow("Total Score", to_string(player.getScore()), 14);
            statRow("Lives Remaining", to_string(player.getLives()), (player.getLives() > 0 ? 10 : 12));
            statRow("Pellets Eaten", to_string(pelletsCollected) + " / " + to_string(initialPellets), 11);
            statRow("Total Moves", to_string(totalMoves), 13);
            statRow("Completion", to_string((int)completion) + "%", (completion > 80 ? 10 : 14));
            cout << "  \u2560"; for(int i=0; i<20; i++) cout << "\u2550"; cout << "\u256c"; for(int i=0; i<25; i++) cout << "\u2550"; cout << "\u2563" << endl;
            statRow("Graph Nodes", "[OK] " + to_string(maze.getGraph()->getVertices()), 8);
            statRow("Pathfinding", "[OK] BFS + Dijkstra", 8);
            statRow("Undo Stack", "[OK] Enabled", 8);
            cout << "  \u255a"; for(int i=0; i<20; i++) cout << "\u2550"; cout << "\u2569"; for(int i=0; i<25; i++) cout << "\u2550"; cout << "\u255d" << endl << endl;
            setColor(14); cout<<"       >>> Press [SPACE] to view final result... <<<\n"; setColor(7);
            char scoreKey = _getch(); while(scoreKey != ' ') scoreKey = _getch();

            clearScreen(); drawLogo();
            setColor(9); cout<<"\n  \u251c\u2500\u2500 "; setColor(15); cout<<"GAME COMPLETE"; setColor(9); cout<<" \u2500\u2500\u2524\n\n";
            if(maze.getPellets().getSize()==0){
                setColor(10);
                cout<<"  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557\n";
                cout<<"  \u2551  "; setColor(14); cout<<" \u2588\u2588\u2557  \u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2557   \u2588\u2588\u2557    \u2588\u2588\u2557    \u2588\u2588\u2557\u2588\u2588\u2557\u2588\u2588\u2588\u2557  \u2588\u2588\u2557 "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(14); cout<<" \u255a\u2588\u2588\u2557\u2588\u2588\u2554\u255d\u2588\u2588\u2554\u2550\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2551   \u2588\u2588\u2551    \u2588\u2588\u2551    \u2588\u2588\u2551\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2551 "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(14); cout<<"  \u255a\u2588\u2588\u2588\u2588\u2554\u255d \u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2551   \u2588\u2588\u2551    \u2588\u2588\u2551 \u2588\u2557 \u2588\u2588\u2551\u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2557\u2588\u2588\u2551 "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(14); cout<<"   \u255a\u2588\u2588\u2554\u255d  \u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2551   \u2588\u2588\u2551    \u2588\u2588\u2551\u2588\u2588\u2588\u2557\u2588\u2588\u2551\u2588\u2588\u2551\u2588\u2588\u2551\u255a\u2588\u2588\u2588\u2588\u2551 "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(14); cout<<"    \u2588\u2588\u2551   \u255a\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255d\u255a\u2588\u2588\u2588\u2588\u2588\u2588\u2554\u255d    \u255a\u2588\u2588\u2588\u2554\u2588\u2588\u2588\u2554\u255d\u2588\u2588\u2551\u2588\u2588\u2551 \u255a\u2588\u2588\u2588\u2551 "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u2551       "; setColor(11); cout<<"  All pellets devoured! Ghosts defeated!      "; setColor(10); cout<<"\u2551\n";
                cout<<"  \u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d\n";
            } else if(player.getLives()<=0){
                setColor(12);
                cout<<"  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557\n";
                cout<<"  \u2551  "; setColor(9); cout<<" \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557   \u2588\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2557  \u2588\u2588\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2557   \u2588\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2557"; setColor(12); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(9); cout<<" \u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255d\u2588\u2588\u2554\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255d  \u2588\u2588\u2554\u2550\u2550\u2550\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2557 \u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2550\u2550\u2550\u2550\u255d"; setColor(12); cout<<"\u2551\n";
                cout<<"  \u2551  "; setColor(9); cout<<" \u2588\u2588\u2551  \u2588\u2588\u2588\u2557\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2588\u2588\u2554\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2557     \u2588\u2588\u2551   \u2588\u2588\u2551\u2588\u2588\u2554\u2588\u2588\u2588\u2588\u2554\u2588\u2588\u2551\u2588\u2588\u2588\u2588\u2588\u2557  "; setColor(12); cout<<"\u2551\n";
                cout<<"  \u2551       "; setColor(14); cout<<"  The ghosts got you. Better luck next time!  "; setColor(12); cout<<"\u2551\n";
                cout<<"  \u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d\n";
            } else {
                setColor(14);
                cout<<"  \u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d\n";
                cout<<"  \u2551            "; setColor(11); cout<<"  G A M E   A B A N D O N E D              "; setColor(14); cout<<"\u2551\n";
                cout<<"  \u255a\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255d\n";
            }
            vector<pair<string,string>> endItems={{"↺","[P] PLAY AGAIN"},{"◄","[M] MAIN MENU"},{"✕","[Q] EXIT"}};
            int endChoice = runMenu("GAME OVER", endItems);
            if (endChoice == 0) playAgain = true;
            else if (endChoice == 2 || endChoice == -1) exitApp = true;
            else playAgain = false;
            break;
        }
    }
    clearScreen();
    setColor(14); cout<<"\n  Thanks for playing AlgoMan!\n"; setColor(7);
    return 0;
}
