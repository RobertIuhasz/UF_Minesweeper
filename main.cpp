#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

sf::Texture loadTexture(const string &filepath){
    sf::Texture SpriteTexture;
    SpriteTexture.loadFromFile(filepath);
    return SpriteTexture;
}

sf::Sprite loadSprite(const sf::Texture &SpriteTexture,float x, float y){
    sf::Sprite GameSprite;
    GameSprite.setTexture(SpriteTexture);
    GameSprite.setPosition(x,y);
    return GameSprite;
}

void setTime(sf::Sprite &DigitsSpriteSecondsOne, sf::Sprite &DigitsSpriteSecondsTwo, sf::Sprite &DigitsSpriteMinutesOne, sf::Sprite &DigitsSpriteMinutesTwo, int GameTimeSeconds ){
    for (int i = 0; i <= GameTimeSeconds; i++){
        DigitsSpriteSecondsOne.setTextureRect(sf::IntRect(21*(i%10),0,21,32));
        DigitsSpriteSecondsTwo.setTextureRect(sf::IntRect(21*((i/10)%6),0,21,32));
        DigitsSpriteMinutesOne.setTextureRect(sf::IntRect(21*((i/60)%10),0,21,32));
        DigitsSpriteMinutesTwo.setTextureRect(sf::IntRect(21*((i/600)%6),0,21,32));
    }
}

void setCounter(sf::Sprite &CounterOneSprite, sf::Sprite &CounterTwoSprite, sf::Sprite &CounterThreeSprite, int FlagsRemaining){
    if (FlagsRemaining >= 0) {
        for (int i = 0; i <= FlagsRemaining; i++) {
            CounterOneSprite.setTextureRect(sf::IntRect(21 * (i % 10), 0, 21, 32));
            CounterTwoSprite.setTextureRect(sf::IntRect(21 * ((i / 10) % 10), 0, 21, 32));
            CounterThreeSprite.setTextureRect(sf::IntRect(21 * ((i / 100) % 10), 0, 21, 32));
        }
    }
    if (FlagsRemaining < 0){
        for (int i = 0; i >= FlagsRemaining; i--) {
            CounterOneSprite.setTextureRect(sf::IntRect(21 * (-i % 10), 0, 21, 32));
            CounterTwoSprite.setTextureRect(sf::IntRect(21 * ((-i / 10) % 10), 0, 21, 32));
            CounterThreeSprite.setTextureRect(sf::IntRect(21 * ((-i / 100) % 10), 0, 21, 32));
        }
    }
}

void DrawPaused(bool Paused, sf::RenderWindow &GameWindow, sf::Sprite &PausedSprite, sf::Sprite &PlaySprite){
    if (Paused){
        GameWindow.draw(PlaySprite);
    }
    if (!Paused){
        GameWindow.draw(PausedSprite);
    }
}

void InitializeLeaderBoard(sf::Font font, int LeaderBoardWidth, int LeaderBoardHeight){
    sf::Text LeaderBoardText;
    LeaderBoardText.setFont(font);
    LeaderBoardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    LeaderBoardText.setCharacterSize(20);
    LeaderBoardText.setFillColor(sf::Color::White);
    LeaderBoardText.setString("LEADERBOARD");
    setText(LeaderBoardText,LeaderBoardWidth/2,(LeaderBoardHeight/2)-120);

    sf::Text LeaderBoardRowText;
    LeaderBoardRowText.setFont(font);
    LeaderBoardRowText.setStyle(sf::Text::Bold);
    LeaderBoardRowText.setCharacterSize(18);
    LeaderBoardRowText.setFillColor(sf::Color::White);

    ifstream LeaderBoardData("/leaderboard.txt");
    string LeaderBoardRow;
    for (int i = 1; i < 6; i++){
        string TempTime, TempName;
        getline(LeaderBoardData,TempTime,',');
        getline(LeaderBoardData,TempName);
        LeaderBoardRow += to_string(i) + ".\t" + TempTime + "\t" + TempName + "\n\n";
    }
    LeaderBoardData.close();
    LeaderBoardRowText.setString(LeaderBoardRow);
    setText(LeaderBoardRowText,LeaderBoardWidth/2,(LeaderBoardHeight/2)+20);

    sf::RenderWindow LeaderBoard(sf::VideoMode(LeaderBoardWidth,LeaderBoardHeight),"LeaderBoard",sf::Style::Close);
    while (LeaderBoard.isOpen()){
        sf::Event event;
        while (LeaderBoard.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                LeaderBoard.close();
            }
        }

        LeaderBoard.clear(sf::Color::Blue);
        LeaderBoard.draw(LeaderBoardText);
        LeaderBoard.draw(LeaderBoardRowText);
        LeaderBoard.display();
    }
}

struct tile {
    int positionx;
    int positiony;
    bool IsMine = false;
    bool IsFlagged = false;
    bool IsRevealed = false;
    bool HasNumber = false;
    sf::Texture HiddenTile;
    sf::Texture RevealedTile;
    sf::Texture Mine;
    sf::Texture Flag;
    sf::Texture Number;
    vector<tile*> Neighbors;

    tile(){}

    tile(int positionx, int positiony) {
        this->positionx = positionx;
        this->positiony = positiony;
    }

    void setNumberTexture(sf::Texture Number){
        this->Number = Number;
    }

    void setMine(sf::Texture Mine){
        this->Mine = Mine;
    }

    void setHidden(sf::Texture HiddenTile){
        this->HiddenTile = HiddenTile;
    }

    void setRevealed(sf::Texture RevealedTile){
        this->RevealedTile = RevealedTile;
    }

    void setFlag(sf::Texture Flag){
        this->Flag = Flag;
    }

    void drawTile(sf::RenderWindow &Window){
        if (this->IsRevealed){
            sf::Sprite temp = loadSprite(RevealedTile,positionx,positiony);
            Window.draw(temp);
        }
        if (this->IsMine){
            sf::Sprite temp = loadSprite(Mine,positionx,positiony);
            Window.draw(temp);
        }
        if (this->HasNumber && !this->IsMine){
            sf::Sprite temp = loadSprite(Number,positionx,positiony);
            Window.draw(temp);
        }
        if (!this->IsRevealed){
            sf::Sprite temp = loadSprite(HiddenTile,positionx,positiony);
            Window.draw(temp);
        }
        if (this->IsFlagged){
            sf::Sprite temp = loadSprite(Flag,positionx,positiony);
            Window.draw(temp);
        }
    }
};

void RevealEmptyTiles(vector<vector<tile>> &TileList, int positionx, int positiony) {
    int Row = positiony / 32;
    int Col = positionx / 32;
    if (Row < 0 || Row >= TileList.size() || Col < 0 || Col >= TileList[0].size()){
        return;
    }
    if (TileList[Row][Col].IsRevealed || TileList[Row][Col].IsFlagged)
        return;
    if (TileList[Row][Col].IsMine || TileList[Row][Col].HasNumber) {
        TileList[Row][Col].IsRevealed = true;
        return;
    }
    TileList[Row][Col].IsRevealed = true;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int neighborRow = Row + i;
            int neighborCol = Col + j;
            if (neighborRow >= 0 && neighborRow < TileList.size() && neighborCol >= 0 && neighborCol < TileList[0].size()) {
                RevealEmptyTiles(TileList, neighborCol * 32, neighborRow * 32);
            }
        }
    }
}

void RevealAllMines(vector<vector<tile>>&TileList, bool DebugOn){
    if (DebugOn) {
        for (int i = 0; i < TileList.size(); i++) {
            for (int j = 0; j < TileList[0].size(); j++) {
                if (TileList[i][j].IsMine) {
                    TileList[i][j].IsRevealed = true;
                }
            }
        }
    }
    if (!DebugOn) {
        for (int i = 0; i < TileList.size(); i++) {
            for (int j = 0; j < TileList[0].size(); j++) {
                if (TileList[i][j].IsMine) {
                    TileList[i][j].IsRevealed = false;
                }
            }
        }
    }
}

void RevealAllMines(vector<vector<tile>>&TileList){
    for (int i = 0; i < TileList.size(); i++) {
        for (int j = 0; j < TileList[0].size(); j++) {
            if (TileList[i][j].IsMine) {
                TileList[i][j].IsRevealed = true;
            }
        }
    }
}

bool AllCleared(vector<vector<tile>>&TileList,int RowCount, int ColCount, int MineCount){
    int PlayableMines = RowCount*ColCount-MineCount;
    for (int i = 0; i < TileList.size(); i++) {
        for (int j = 0; j < TileList[0].size(); j++) {
            if (!TileList[i][j].IsMine && TileList[i][j].IsRevealed) {
                PlayableMines--;
            }
        }
    }
    if (PlayableMines == 0){
        return true;
    }
    return false;
}

void FlagAllMines(vector<vector<tile>>&TileList){
    for (int i = 0; i < TileList.size(); i++) {
        for (int j = 0; j < TileList[0].size(); j++) {
            if (TileList[i][j].IsMine) {
                TileList[i][j].IsFlagged = true;
            }
        }
    }
}

void UpdateLeaderBoard(const string &Filepath,const int &NewEntryTime, const string &NewEntryName){
    map<int,string> LeaderBoard;

    ifstream ReadLeaderBoard(Filepath);
    string Standing;
    while (getline(ReadLeaderBoard,Standing)){
        int Minutes = stoi(Standing.substr(0,Standing.find(':')));
        int Seconds = stoi(Standing.substr(Standing.find(':')+1,Standing.find(':')+3));
        int Total = Minutes * 60 + Seconds;
        string Name = Standing.substr(Standing.find(',')+2);
        LeaderBoard[Total] = Name;
    }
    ReadLeaderBoard.close();
    LeaderBoard[NewEntryTime] = NewEntryName;

    ofstream  WriteLeaderboard(Filepath);
    map<int,string>::iterator iter;
    int count = 0;
    for (iter = LeaderBoard.begin();iter != LeaderBoard.end(); iter++){
        int minutes = iter->first / 60;
        int seconds = iter->first % 60;
        string Sminutes = to_string(minutes);
        string Sseconds = to_string(seconds);
        string NameString = iter->second;
        if (minutes < 10){
            Sminutes = '0' + Sminutes;
        }
        if (seconds < 10){
            Sseconds = '0' + Sseconds;
        }
        if (count < 5){
            if (NameString == NewEntryName){
                WriteLeaderboard << Sminutes << ":" << Sseconds << ", " << NameString << "*" << endl;
            }else{
                WriteLeaderboard << Sminutes << ":" << Sseconds << ", " << NameString << endl;
            }
        }
        count++;
    }
}

int main() {

    ifstream ConfigData("/config.cfg");
    string Scols;
    string Srows;
    string Smines;
    getline(ConfigData,Scols);
    getline(ConfigData,Srows);
    getline(ConfigData, Smines);
    int ColCount = stoi(Scols);
    int RowCount = stoi(Srows);
    int MineCount = stoi(Smines);
    int FlagCount = stoi(Smines);
    int WelcomeWindowHeight = (RowCount*32) + 100;
    int WelcomeWindowWidth = (ColCount*32);
    int LeaderBoardHeight = (RowCount*16) + 50;
    int LeaderBoardWidth = (ColCount*16);

    sf::Font font;
    font.loadFromFile("/font.ttf");

    sf::Text WelcomeText;
    WelcomeText.setFont(font);
    WelcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    WelcomeText.setCharacterSize(24);
    WelcomeText.setFillColor(sf::Color::White);
    WelcomeText.setString("WELCOME TO MINESWEEPER!");
    setText(WelcomeText,WelcomeWindowWidth/2,(WelcomeWindowHeight/2)-150);

    sf::Text EnterNameText;
    EnterNameText.setFont(font);
    EnterNameText.setStyle(sf::Text::Bold);
    EnterNameText.setCharacterSize(20);
    EnterNameText.setFillColor(sf::Color::White);
    EnterNameText.setString("Enter your name:");
    setText(EnterNameText,WelcomeWindowWidth/2,(WelcomeWindowHeight/2)-75);

    sf::Text UserText;
    UserText.setFont(font);
    UserText.setStyle(sf::Text::Bold);
    UserText.setCharacterSize(18);
    UserText.setFillColor(sf::Color::White);
    UserText.setString("|");
    setText(UserText,WelcomeWindowWidth/2,(WelcomeWindowHeight/2)-45);
    string NameString;

    sf::RenderWindow WelcomeWindow(sf::VideoMode(WelcomeWindowWidth,WelcomeWindowHeight),"Minesweeper Welcome Window",sf::Style::Close);
    bool GameWindowOpen = true;

    while (WelcomeWindow.isOpen()){
        sf::Event event;
        while(WelcomeWindow.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                GameWindowOpen = false;
                WelcomeWindow.close();
            }
            if (event.type == sf::Event::TextEntered){
                bool BackSpace = false;
                if(event.text.unicode == '\b' && !NameString.empty()){
                    NameString.pop_back();
                    BackSpace = true;
                }
                if (isalpha((char)event.text.unicode)){
                    if(!BackSpace){
                        NameString += (char)event.text.unicode;
                    }
                }
                NameString[0] = toupper(NameString[0]);
                if (NameString.size() > 1){
                    for (int i = 1; i < NameString.size(); i++){
                        NameString[i] = tolower(NameString[i]);
                    }
                }
                if (NameString.size() > 10){
                    NameString.pop_back();
                    break;
                }
                UserText.setString(NameString + "|");
                setText(UserText,WelcomeWindowWidth/2,(WelcomeWindowHeight/2)-45);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && NameString.size() > 0){
                WelcomeWindow.close();
            }
        }
        WelcomeWindow.clear(sf::Color::Blue);
        WelcomeWindow.draw(WelcomeText);
        WelcomeWindow.draw(EnterNameText);
        WelcomeWindow.draw(UserText);
        WelcomeWindow.display();
    }

    if (!GameWindowOpen){
        return 0;
    }
    sf::Texture HappyFaceTexture = loadTexture("/images/face_happy.png");
    sf::Texture DebugTexture = loadTexture("/images/debug.png");
    sf::Texture PauseTexture = loadTexture("/images/pause.png");
    sf::Texture PlayTexture = loadTexture("/images/play.png");
    sf::Texture LeaderBoardTexture = loadTexture("/images/leaderboard.png");
    sf::Texture LoseFaceTexture = loadTexture("/images/face_lose.png");
    sf::Texture WinFaceTexture = loadTexture("/images/face_win.png");
    sf::Texture FlagTexture = loadTexture("/images/flag.png");
    sf::Texture MineTexture = loadTexture("/images/mine.png");
    sf::Texture NumberOneTexture = loadTexture("/images/number_1.png");
    sf::Texture NumberTwoTexture = loadTexture("/images/number_2.png");
    sf::Texture NumberThreeTexture = loadTexture("/images/number_3.png");
    sf::Texture NumberFourTexture = loadTexture("/images/number_4.png");
    sf::Texture NumberFiveTexture = loadTexture("/images/number_5.png");
    sf::Texture NumberSixTexture = loadTexture("/images/number_6.png");
    sf::Texture NumberSevenTexture = loadTexture("/images/number_7.png");
    sf::Texture NumberEightTexture = loadTexture("/images/number_8.png");
    sf::Texture TileHiddenTexture = loadTexture("/images/tile_hidden.png");
    sf::Texture TileRevealedTexture = loadTexture("/images/tile_revealed.png");
    sf::Texture DigitsTexture = loadTexture("/images/digits.png");

    sf::Sprite HappyFaceSprite = loadSprite(HappyFaceTexture,32*(ColCount/2.0)-32,32*(RowCount+0.5));
    sf::Sprite LoseFaceSprite = loadSprite(LoseFaceTexture,32*(ColCount/2.0)-32,32*(RowCount+0.5));
    sf::Sprite WinFaceSprite = loadSprite(WinFaceTexture,32*(ColCount/2.0)-32,32*(RowCount+0.5));
    sf::Sprite DebugSprite = loadSprite(DebugTexture,(ColCount*32)-304,32*(RowCount+0.5));
    sf::Sprite PauseSprite = loadSprite(PauseTexture,(ColCount*32)-240,32*(RowCount+0.5));
    sf::Sprite PlaySprite = loadSprite(PlayTexture,(ColCount*32)-240,32*(RowCount+0.5));
    sf::Sprite LeaderBoardSprite = loadSprite(LeaderBoardTexture,(ColCount*32)-176,32*(RowCount+0.5));

    sf::Sprite DigitsSpriteSecondsOne = loadSprite(DigitsTexture,32*ColCount-33,32*(RowCount+0.5)+16);
    sf::Sprite DigitsSpriteSecondsTwo = loadSprite(DigitsTexture,32*ColCount-54,32*(RowCount+0.5)+16);
    sf::Sprite DigitsSpriteMinutesOne = loadSprite(DigitsTexture,32*ColCount-76,32*(RowCount+0.5)+16);
    sf::Sprite DigitsSpriteMinutesTwo = loadSprite(DigitsTexture,32*ColCount-97,32*(RowCount+0.5)+16);
    setTime(DigitsSpriteSecondsOne,DigitsSpriteSecondsTwo,DigitsSpriteMinutesOne,DigitsSpriteMinutesTwo,0);

    sf::Sprite CounterOneSprite = loadSprite(DigitsTexture, 75, 32*(RowCount+0.5)+16);
    sf::Sprite CounterTwoSprite = loadSprite(DigitsTexture, 54, 32*(RowCount+0.5)+16);
    sf::Sprite CounterThreeSprite = loadSprite(DigitsTexture, 33, 32*(RowCount+0.5)+16);
    sf::Sprite NegativeSprite = loadSprite(DigitsTexture,12,32*(RowCount+0.5)+16);
    NegativeSprite.setTextureRect(sf::IntRect(210,0,21,32));
    setCounter(CounterOneSprite,CounterTwoSprite,CounterThreeSprite,MineCount);

    sf::RenderWindow GameWindow(sf::VideoMode(WelcomeWindowWidth,WelcomeWindowHeight),"Minesweeper Game Window (Hi Remmy!)",sf::Style::Close);
    sf::Clock GameClock;
    bool Paused = false;
    bool DebugOn = false;
    bool GameOver = false;
    bool GameWin = false;
    bool UpdatedLeaderboard = true;
    int SavedTime = 0;

    vector<vector<tile>> TileList(RowCount,vector<tile>(ColCount));
    vector<vector<tile>> HiddenOverlay(RowCount,vector<tile>(ColCount));
    for (int i = 0; i < RowCount; i++) {
        for (int j = 0; j < ColCount; j++) {
            tile temp(j * 32, i * 32);
            temp.IsRevealed = true;
            temp.setRevealed(TileRevealedTexture);
            HiddenOverlay[i][j] = temp;
        }
    }

    for (int i = 0; i < RowCount; i++){
        for (int j = 0; j < ColCount; j++){
            tile temp(j*32,i*32);
            temp.IsMine = false;
            temp.IsFlagged = false;
            temp.IsRevealed = false;
            temp.HasNumber = false;
            temp.setHidden(TileHiddenTexture);
            temp.setMine(MineTexture);
            temp.setRevealed(TileRevealedTexture);
            temp.setFlag(FlagTexture);
            TileList[i][j] = temp;
        }
    }
    int minesPlaced = 0;
    while (minesPlaced < MineCount){
        int row = rand() % RowCount;
        int col = rand() % ColCount;
        if (!TileList[row][col].IsMine){
            TileList[row][col].IsMine = true;
            minesPlaced++;
        }
    }
    for (int i = 0; i < TileList.size(); i++){
        for (int j = 0; j < TileList[0].size(); ++j){
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    int ni = i + x;
                    int nj = j + y;
                    if (ni >= 0 && ni < RowCount && nj >= 0 && nj < ColCount && !(x == 0 && y == 0)) {
                        TileList[i][j].Neighbors.push_back(&TileList[ni][nj]);
                    }
                }
            }
        }
    }

    vector<sf::Texture> NumberTextures;
    NumberTextures.insert(NumberTextures.end(), {NumberOneTexture,NumberTwoTexture,NumberThreeTexture,NumberFourTexture,NumberFiveTexture,NumberSixTexture,NumberSevenTexture,NumberEightTexture});

    for (int i = 0; i < TileList.size(); i++) {
        for (int j = 0; j < TileList[0].size(); j++) {
            int NumberOfMineNeighbors = 0;
            for (int k = 0; k < TileList[i][j].Neighbors.size(); k++){
                if (TileList[i][j].Neighbors[k]->IsMine){
                    NumberOfMineNeighbors++;
                }
            }
            if (NumberOfMineNeighbors > 0){
                TileList[i][j].HasNumber = true;
            }
            for (int l = 1; l < 9; l++){
                if (NumberOfMineNeighbors == l){
                    TileList[i][j].setNumberTexture(NumberTextures[l-1]);
                }
            }
        }
    }
    while (GameWindow.isOpen()) {
        sf::Time GameTime = GameClock.getElapsedTime();
        int GameTimeSeconds =  GameTime.asSeconds();
        if (Paused){
            GameClock.restart();
        }
        sf::Event event;
        sf::Mouse mouse;
        while (GameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                GameWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if (PauseSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y) && !GameOver && !GameWin){
                    Paused = !Paused;
                    if (Paused) {
                        SavedTime += GameTimeSeconds;
                        GameClock.restart();
                    }
                }
                if (LeaderBoardSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y)){
                    SavedTime += GameTimeSeconds;
                    GameWindow.draw(PlaySprite);
                    for (int i = 0; i < RowCount; i++) {
                        for (int j = 0; j < ColCount; j++) {
                            HiddenOverlay[i][j].drawTile(GameWindow);
                        }
                    }
                    GameWindow.display();
                    InitializeLeaderBoard(font,LeaderBoardWidth,LeaderBoardHeight);
                    GameClock.restart();
                }
                if (DebugSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y) && !GameOver && !GameWin && !Paused){
                    DebugOn = !DebugOn;
                    RevealAllMines(TileList,DebugOn);
                }
                if ((HappyFaceSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y) || LoseFaceSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y) || WinFaceSprite.getGlobalBounds().contains(mouse.getPosition(GameWindow).x,mouse.getPosition(GameWindow).y))){
                    GameClock.restart();
                    GameTimeSeconds = 0;
                    Paused = false;
                    DebugOn = false;
                    GameOver = false;
                    GameWin = false;
                    UpdatedLeaderboard = true;
                    FlagCount = MineCount;
                    SavedTime = 0;
                    for (int i = 0; i < RowCount; i++){
                        for (int j = 0; j < ColCount; j++){
                            tile temp(j*32,i*32);
                            temp.IsMine = false;
                            temp.IsFlagged = false;
                            temp.IsRevealed = false;
                            temp.HasNumber = false;
                            temp.setHidden(TileHiddenTexture);
                            temp.setMine(MineTexture);
                            temp.setRevealed(TileRevealedTexture);
                            temp.setFlag(FlagTexture);
                            TileList[i][j] = temp;
                        }
                    }
                    int minesPlaced = 0;
                    while (minesPlaced < MineCount){
                        int row = rand() % RowCount;
                        int col = rand() % ColCount;
                        if (!TileList[row][col].IsMine){
                            TileList[row][col].IsMine = true;
                            minesPlaced++;
                        }
                    }
                    for (int i = 0; i < TileList.size(); i++){
                        for (int j = 0; j < TileList[0].size(); ++j){
                            for (int x = -1; x <= 1; ++x) {
                                for (int y = -1; y <= 1; ++y) {
                                    int ni = i + x;
                                    int nj = j + y;
                                    if (ni >= 0 && ni < RowCount && nj >= 0 && nj < ColCount && !(x == 0 && y == 0)) {
                                        TileList[i][j].Neighbors.push_back(&TileList[ni][nj]);
                                    }
                                }
                            }
                        }
                    }
                    for (int i = 0; i < TileList.size(); i++) {
                        for (int j = 0; j < TileList[0].size(); j++) {
                            int NumberOfMineNeighbors = 0;
                            for (int k = 0; k < TileList[i][j].Neighbors.size(); k++){
                                if (TileList[i][j].Neighbors[k]->IsMine){
                                    NumberOfMineNeighbors++;
                                }
                            }
                            if (NumberOfMineNeighbors > 0){
                                TileList[i][j].HasNumber = true;
                            }
                            for (int l = 1; l < 9; l++){
                                if (NumberOfMineNeighbors == l){
                                    TileList[i][j].setNumberTexture(NumberTextures[l-1]);
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Left && !GameOver && !GameWin && !Paused){
                    for (int i = 0; i < TileList.size(); i++) {
                        for (int j = 0; j < TileList[0].size(); j++) {
                            sf::Vector2i localPosition = sf::Mouse::getPosition(GameWindow);
                            if (localPosition.x > TileList[i][j].positionx && localPosition.x < TileList[i][j].positionx + 32 && localPosition.y > TileList[i][j].positiony && localPosition.y < TileList[i][j].positiony + 32 && !TileList[i][j].IsFlagged) {
                                RevealEmptyTiles(TileList,localPosition.x,localPosition.y);
                                if (TileList[i][j].IsMine){
                                    RevealAllMines(TileList);
                                    GameOver = true;
                                    Paused = true;
                                    SavedTime+=GameTimeSeconds;
                                }
                                if (AllCleared(TileList,RowCount,ColCount,MineCount)){
                                    FlagAllMines(TileList);
                                    GameWin = true;
                                    Paused = true;
                                    SavedTime+=GameTimeSeconds;
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right && !GameOver && !GameWin && !Paused){
                    for (int i = 0; i < TileList.size(); i++) {
                        for (int j = 0; j < TileList[0].size(); j++) {
                            sf::Vector2i localPosition = sf::Mouse::getPosition(GameWindow);
                            if (localPosition.x > TileList[i][j].positionx && localPosition.x < TileList[i][j].positionx + 32 && localPosition.y > TileList[i][j].positiony && localPosition.y < TileList[i][j].positiony + 32 && !TileList[i][j].IsRevealed) {
                                TileList[i][j].IsFlagged = !TileList[i][j].IsFlagged;
                                if (TileList[i][j].IsFlagged){
                                    FlagCount--;
                                }
                                if (!TileList[i][j].IsFlagged){
                                    FlagCount++;
                                }
                            }
                        }
                    }
                }
            }
        }
        setTime(DigitsSpriteSecondsOne,DigitsSpriteSecondsTwo,DigitsSpriteMinutesOne,DigitsSpriteMinutesTwo,GameTimeSeconds+SavedTime);
        setCounter(CounterOneSprite,CounterTwoSprite,CounterThreeSprite,FlagCount);
        if (UpdatedLeaderboard && GameWin){
            UpdateLeaderBoard("/leaderboard.txt",SavedTime,NameString);
            UpdatedLeaderboard = false;
        }
        GameWindow.clear(sf::Color::White);
        GameWindow.draw(DebugSprite);
        if (!GameOver && !GameWin) {
            GameWindow.draw(HappyFaceSprite);
        }else if(GameOver){
            GameWindow.draw(LoseFaceSprite);
        }else if(GameWin){
            GameWindow.draw(WinFaceSprite);
        }
        GameWindow.draw(DigitsSpriteSecondsTwo);
        GameWindow.draw(DigitsSpriteSecondsOne);
        GameWindow.draw(DigitsSpriteMinutesOne);
        GameWindow.draw(DigitsSpriteMinutesTwo);
        GameWindow.draw(CounterOneSprite);
        GameWindow.draw(CounterTwoSprite);
        GameWindow.draw(CounterThreeSprite);
        GameWindow.draw(LeaderBoardSprite);
        if (FlagCount < 0){
            GameWindow.draw(NegativeSprite);
        }
        for (int i = 0; i < TileList.size(); i++){
            for (int j = 0; j < TileList[0].size(); j++){
                if (Paused && !GameOver && !GameWin){
                    HiddenOverlay[i][j].drawTile(GameWindow);
                }else {
                    TileList[i][j].drawTile(GameWindow);
                }
            }
        }
        DrawPaused(Paused,GameWindow,PauseSprite,PlaySprite);
        GameWindow.display();
    }
    return 0;
}