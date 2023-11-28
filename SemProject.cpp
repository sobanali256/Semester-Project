#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <fstream>
#include <iostream>
#include <ctime>

#define DELETE_KEY 8
#define ENTER_KEY 13

using namespace std;
using namespace sf;


//User defined datatype for platform generation
struct point
{
    int x, y;
};

void platformpos(point plat[])
{
    for (int i = 0;i < 10;i++)
    {
        plat[i].x = rand() % 399;
        plat[i].y = rand() % 600;

        for (int a = 0; a < i; a++)
        {
            if ((plat[a].x <= plat[i].x && plat[i].x <= plat[a].x + 50) || (plat[a].y <= plat[i].y && plat[i].y <= plat[a].y + 30))
            {
                i--;
                break;
            }
        }
    }
}

void changepos(int& y, int h, point plat[], int dy)
{
    //Changing green platform positions
    if (y < h)
        for (int i = 0;i < 10;i++)
        {
            y = h;
            plat[i].y = plat[i].y - dy; //Verticle Transalation

            if (plat[i].y > 600) //Setting new platforms above
            {
                plat[i].y = 0;
                plat[i].x = rand() % 380;
            }
        }
}

void readfile(string score[], string& name)
{
    string line;
    int index = 0;

    ifstream ReadFile;
    ReadFile.open("Score.txt");

    while (getline(ReadFile, line))
    {
        score[index] = line;
        index++;
    }

    ReadFile.close();

    ReadFile.open("Name.txt");
    
    while (getline(ReadFile, line))
    {
        name = line;
    }

    ReadFile.close();

}

void writingfile(string score[])
{
    ofstream WriteFile("Score.txt");

    for (int i = 0; i < 10; i++)
    {
        WriteFile << score[i] << endl;
    }

    WriteFile.close();

}

void storename(string name)
{
    ofstream WriteFile;
    WriteFile.open("Name.txt");

    WriteFile << name << endl;

    WriteFile.close();
}

bool checkhigh(string score[], int newscore)
{
    if (stoi(score[0]) < newscore)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void AddNewScore(string score[], int newscore)
{
    for (int i = 0; i < 10; i++)
    {
        if (stoi(score[i]) < newscore)
        {
            score[i] = to_string(newscore);
            break;
        }
    }
}


int main()
{
    srand((unsigned)time(0));

    RenderWindow app(VideoMode(400, 600), "Doodle Game", Style::Close);
    app.setFramerateLimit(65);


    //Stores the paths for doodles
    String doodles[] = { "images/doodle.png" , "images/doodle2.png", "images/doodle3.png","images/doodle4.png","images/doodle5.png","images/doodle6.png" };

    ////Stores the paths for backgrounds
    String background[] = { "images/background.png","images/soccer-background.png","images/space-background.png","images/space2-background.png" };

    //Generating random number to output different doodles and backgrounds
    int r1 = (rand() % 6);
    int r2 = (rand() % 4);


    //t1 = bgs, t2 = green platform, t3 = doodles, t4 = powerup
    Texture t1, t2, t3, start, pause, over, open, open2;

    t1.loadFromFile(background[r2]);
    t2.loadFromFile("images/green_platform.png");
    t3.loadFromFile(doodles[r1]);


    //Menu pages
    open.loadFromFile("images/Opening2.png"); //NEWLY ADDED!!
    open2.loadFromFile("images/Opening22.png");
    start.loadFromFile("images/Default.png");
    pause.loadFromFile("images/pause-cover.png");
    over.loadFromFile("images/gameover.png");

    Sprite sBackground(t1), sPlat(t2), sPers(t3), sMenu(start), pMenu(pause), gameover(over), opening(open), opening2(open2);

    //Sound of jumping
    SoundBuffer buffer, buffer2;
    buffer.loadFromFile("sound/jump_sound.wav");
    buffer2.loadFromFile("sound/falling.wav");

    Sound jump, falling;
    jump.setBuffer(buffer);
    falling.setBuffer(buffer2);


    //score + font 
    Font font;
    font.loadFromFile("Font/al-seana.ttf");
    Text scoreText, highscoretext;
    scoreText.setFont(font);
    scoreText.setCharacterSize(45);
    scoreText.setFillColor(Color{ 255,101,1 });
    scoreText.setOutlineThickness(0.4);
    scoreText.setOutlineColor(Color::Black);

    RectangleShape rectangle(Vector2f(80, 50)); //Background for live score

    RectangleShape rec(Vector2f(200, 60));
    RectangleShape rec2(Vector2f(260, 50));

    highscoretext.setFont(font);
    highscoretext.setCharacterSize(25);
    highscoretext.setFillColor(Color{ 255,101,1 });
    highscoretext.setOutlineThickness(0.4);
    highscoretext.setOutlineColor(Color::Black);

    //NEWLY ADDED!!
    Text nameText, openingT;
    nameText.setFont(font);
    nameText.setCharacterSize(45);
    nameText.setFillColor(Color::White);
    nameText.setPosition(100, 250);

    point plat[20];

    //Green Platfroms
    platformpos(plat);


    //Initial doodble position
    int x = 100, y = 100, h = 200, score = 0;
    string hightext;

    string SavedScores[10];
    string name = "";

    //NEWLY ADDED!!
    string playerName;
    string openingText = "Enter your name";

    openingT.setFont(font);
    openingT.setCharacterSize(45);
    openingT.setFillColor(Color::White);
    openingT.setPosition(89, 180);

    openingT.setString(openingText);
    // TILL here^

    readfile(SavedScores, name);


    float dy = 0;

    bool check = true;
    bool wait = false;
    bool end = false;
    bool next = true;

    bool highscore;


    //NEWLY ADDED!!
    bool EnterName = false;

    while (app.isOpen())
    {
        Event e;

        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }
       

        if (name == "")
        {
            //NEWLY ADDED!!
            while (!EnterName)
            {
                Event e;

                while (app.pollEvent(e))
                {
                    if (e.type == Event::Closed)
                        app.close();

                    if (e.type == Event::TextEntered)
                    {
                        if (e.text.unicode < 128 && e.text.unicode != DELETE_KEY && playerName.length() < 10)
                        {
                            playerName += static_cast<char>(e.text.unicode);
                            nameText.setString(playerName);
                        }
                        else if (e.text.unicode == DELETE_KEY && !playerName.empty())
                        {
                            // Handle backspace to erase the last character
                            playerName.pop_back();
                            nameText.setString(playerName);
                        }
                    }
                }



                app.clear();
                app.draw(opening);
                app.draw(rec);
                app.draw(rec2);

                rec.setPosition(100, 240);
                rec.setFillColor(Color(12, 13, 13, 50));

                rec2.setPosition(90, 180);
                rec2.setFillColor(Color(12, 13, 13, 50));


                app.draw(openingT);
                app.draw(nameText);
                app.display();

                if (Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    EnterName = true;
                    name = playerName;
                    storename(name);
                }
            }
        }
        else
        {
         
            string sentence = "Welcome Back " + name;
            Text greeting;
            greeting.setFont(font);
            greeting.setCharacterSize(45);
            greeting.setFillColor(Color::White);
            greeting.setPosition(50, 200);

            greeting.setString(sentence);
            
            RectangleShape back(Vector2f(350, 50));

            while (next)
            {
                app.draw(opening2);
                app.draw(back);

                back.setPosition(35, 200);
                back.setFillColor(Color(5,5,5,150));

                app.draw(greeting);
                app.display();

                if (Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    next = false;
                }

                if (Keyboard::isKeyPressed(Keyboard::C))
                {
                    next = false;
                    name = "";
                }

            }

        }

        if (name != "")
        {
            while (check)
            {
                app.draw(sMenu);
                app.display();
                if (Keyboard::isKeyPressed(Keyboard::S)) check = false;
                if (Keyboard::isKeyPressed(Keyboard::Escape)) return 0;
            }

            if (Keyboard::isKeyPressed(Keyboard::P))
            {
                wait = true;
            }

            while (wait)
            {
                app.draw(pMenu);
                app.display();
                if (Keyboard::isKeyPressed(Keyboard::K)) wait = false;
                if (Keyboard::isKeyPressed(Keyboard::Escape)) return 0;
            }

            if (y == h && dy < (-1.62)) {
                score += 1;
                scoreText.setString(std::to_string(score));
            }
            app.draw(scoreText);
            scoreText.setString(std::to_string(score));


            //Output for when each key is pressed
            if (Keyboard::isKeyPressed(Keyboard::Right)) x += 3;
            if (Keyboard::isKeyPressed(Keyboard::Left)) x -= 3;
            if (Keyboard::isKeyPressed(Keyboard::A)) x -= 3;
            if (Keyboard::isKeyPressed(Keyboard::D)) x += 3;

            if (Keyboard::isKeyPressed(Keyboard::Escape)) return 0;

            //Screen Wrapping
            if (x < -10)
            {
                x = 365;
            }

            if (x > 365)
            {
                x = -8;
            }

            //Jumping and falling of doodle
            dy += 0.175;
            y += dy;

            if (y > 600)  dy = -10;


            changepos(y, h, plat, dy);


            //Detecting doodle jumping on platform
            for (int i = 0;i < 10;i++)
                if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68) && (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))
                {
                    dy = -10;
                    jump.play();
                }


            if (y > 600)
            {
                end = true;
                falling.play();

                scoreText.setPosition(220, 372);
                scoreText.setFillColor(Color(255, 101, 1));

                highscore = checkhigh(SavedScores, score);
                AddNewScore(SavedScores, score);
                writingfile(SavedScores);
            }

            while (end)
            {
                app.draw(gameover);

                if (highscore)
                {
                    hightext = "NEW HIGH SCORE!";
                    highscoretext.setString(hightext);
                    highscoretext.setPosition(220, 430);
                    app.draw(highscoretext);
                }
                else
                {
                    string highestscore = SavedScores[0];
                    hightext = "Your high score " + highestscore;
                    highscoretext.setString(hightext);
                    highscoretext.setPosition(50, 430);
                    app.draw(highscoretext);
                }


                app.draw(scoreText);
                app.display();
                if (Keyboard::isKeyPressed(Keyboard::Escape)) return 0;

                //Restarting game
                if (Keyboard::isKeyPressed(Keyboard::S))
                {
                    app.clear();

                    x = 100;
                    y = 100;
                    h = 200;
                    dy = 0;

                    score = 0;
                    scoreText.setString(std::to_string(score));

                    r1 = (rand() % 6);
                    r2 = (rand() % 4);

                    t1.loadFromFile(background[r2]);
                    t3.loadFromFile(doodles[r1]);

                    platformpos(plat);

                    end = false;
                }
            }

            sPers.setPosition(x, y);

            app.draw(sBackground);
            app.draw(sPers);

            //Green platforms
            for (int i = 0;i < 10;i++)
            {

                sPlat.setPosition(plat[i].x, plat[i].y);
                app.draw(sPlat);

            }

            //Live score
            app.draw(scoreText);
            scoreText.setPosition(20, 15);
            scoreText.setFillColor(Color::Green);
            app.draw(rectangle);
            rectangle.setPosition(20, 15);
            rectangle.setFillColor(Color(12, 13, 13, 50));


            app.display();
        }

       
    }

    return 0;
}
