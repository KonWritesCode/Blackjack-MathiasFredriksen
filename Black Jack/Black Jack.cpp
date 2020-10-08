#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <time.h> 
#include <stdio.h>


int playerMoney{ 100 };
int dealerMoney{ 100 };

int betAmount{10};

char hitOrStand;
bool gameEnded{ false };
bool firstRound{ true };

class Cards 
{
public:
    int cardType;
    int cardValue;

};

std::vector<Cards> playerHand;
std::vector<Cards> dealerHand;
bool cardsInDeck[4][13];

//Turns the cardType into the different unicode symbols
std::wstring Symbol(int symbolType)
{
    //0 = spade, 1 = Heart, 2 = Clover, 3 = Diamond

    switch (symbolType)
    {
    case 0://Spade
        return(L"\u001b[90m♠\u001b[0m"); // L"♠"
        break;
    case 1://Heart
        //std::wcout << L"\u2665";
        return(L"\u001b[91m♥\u001b[0m");
        break;
    case 2://Clover
        return(L"\u001b[90m♣\u001b[0m"); // L"♣"
        break;
    case 3://Diamond
        //std::wcout << L"\u2666";
        return(L"\u001b[91m♦\u001b[0m");
        //std::wcout << L"\u001b[91m♦\u001b[0m\n";
        break;
    default:
        break;
    }
}

//Turns the cardValue for example into J,Q,K or A;
std::wstring CardValueConverter(int cardValue)
{
    switch (cardValue + 1)
    {
    case 1:
        return(L"A");
        break;

    case 11:
        return(L"J");
        break;

    case 12:
        return(L"Q");
        break;

    case 13:
        return(L"K");
        break;


    default:
        return(std::to_wstring(cardValue + 1));
        break;
    }
}

//Calculate the players current value in their hand
int playerPoints()
{
    int aCounter{0};
    int score{ 0 };
    //Find all the A's and store the amount as an int
    //Add the sum of all the other cards
    for (int i = 0; i < playerHand.size(); i++)
    {
        if(playerHand[i].cardValue == 0)
        {
            aCounter++;
        }
        else if(playerHand[i].cardValue >= 10)
        {
            score += 10;
        }
        else 
        {
            score += (1 + playerHand[i].cardValue);
        }
    }
    //Then apply all the A's
    for (int i = 0; i < aCounter; i++)
    {
        if (score + (11 + (aCounter - i - 1)) <= 21)
        {
            score += 11;
        }
        else { score += 1; }
    }

    //And finally return the score
    return  (score);
};

//Calculate the dealers current value in their hand
int dealerPoints()
{
    int aCounter{ 0 };
    int score{ 0 };
    //Find all the A's and store the amount as an int
    //Add the sum of all the other cards

    if(firstRound)
    {
        if (dealerHand[1].cardValue == 0)
        {
            score = 11;
        }
        else if (dealerHand[1].cardValue >= 10)
        {
            score += 10;
        }
        else
        {
            score += (1 + dealerHand[1].cardValue);
        }
    }
    else 
    {
        for (int i = 0; i < dealerHand.size(); i++)
        {
            if (dealerHand[i].cardValue == 0)
            {
                aCounter++;
            }
            else if (dealerHand[i].cardValue >= 10)
            {
                score += 10;
            }
            else
            {
                score += (1 + dealerHand[i].cardValue);
            }
        }
        //Then apply all the A's
        for (int i = 0; i < aCounter; i++)
        {
            if (score + (11 + (aCounter - i - 1)) <= 21)
            {
                score += 11;
            }
            else { score += 1; }
        }
    }
    
    //And finally return the score
    return  (score);
};

//Gives the player a random card from the deck
void PlayerDrawCard()
{
    int a; //Random card type
    int b; //Random card value
    do
    {
        a = rand() % 4;
        b = rand() % 13;
    } while (cardsInDeck[a][b] == false);

    cardsInDeck[a][b] = false;

    playerHand.push_back({ a,b });
}

//Gives the dealer a random card from the deck
void DealerDrawCard()
{
    int a; //Random card type
    int b; //Random card value
    do
    {
        a = rand() % 4;
        b = rand() % 13;
    } while (cardsInDeck[a][b] == false);

    cardsInDeck[a][b] = false;

    dealerHand.push_back({ a,b });
}

//Used when the player or the dealer is out of money
void EndingScreen()
{
    system("cls");
    std::wcout << "\n\n\n\n                                            GAME OVER!\n\n";
    if(dealerMoney < 10)
    {
        std::wcout << "                                            You won!\n";
        std::wcout << "                                  The dealer is out of money!\n";
    }
    else if(playerMoney < 10)
    {
        std::wcout << "                                            You lost!\n";
        std::wcout << "                                       You're out of money!\n";
    }
    std::wcout << "                                     (Press any key to exit)\n\n";

    std::wcout << "                                              Input: ";
    getwchar();
    exit(0);
}

//Used to output the main part of the game.
void DrawDisplay()
{
    //std::wcout << "\n You: $" << playerMoney << "            Dealer: " << dealerMoney << "\n\n";
    std::wstring betText{ L"\n            Bet: $" };
    std::wcout << L"\u001b[36m" + betText + std::to_wstring(betAmount) + L"\u001b[0m"  << "\n\n";

    std::wcout << "         Dealer's hand\n";

    std::wcout << "               " << dealerPoints() << "\n";

    //Display the dealer's cards
    for(int cardLength = 0; cardLength < 7; cardLength++)
    {
        //Add in space before cards get drawn;
        float spaceValue = (12 - round((dealerHand.size() - 1) * 1.5f));
        for(int i = 0; i < spaceValue; i++)
        {
            std::wcout << " ";
        }

        switch (cardLength)
        {
        case 0:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                std::wcout << L"╔══";
            }
            std::wcout << L"════╗\n";
            break;

        case 1:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if(firstRound && i + 1 != dealerHand.size() && !gameEnded)
                {
                    std::wcout << L"║░░";
                }
                else 
                if (dealerHand[i].cardValue + 1 != 10)
                {
                    std::wcout << L"║" << CardValueConverter(dealerHand[i].cardValue) << " ";
                }
                else
                {
                    std::wcout << L"║" << CardValueConverter(dealerHand[i].cardValue);
                }
            }
            std::wcout << L"    ║\n";
            
            break;

        case 2:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if (firstRound && i + 1 != dealerHand.size() && !gameEnded)
                {
                    std::wcout << L"║░░";
                }
                else
                {
                    std::wcout << L"║" << Symbol(dealerHand[i].cardType) << " ";
                }
                
            }
            std::wcout << L"    ║\n";
            break;

        case 3:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if (firstRound && i + 1 != dealerHand.size() && !gameEnded)
                {
                    std::wcout << L"║░░";
                }
                else
                {
                    std::wcout << L"║  ";
                }
            }
            std::wcout << L"    ║\n";
            break;

        case 4:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if (firstRound && i + 1 != dealerHand.size() && !gameEnded)
                {
                    std::wcout << L"║░░";
                }
                else
                {
                    std::wcout << L"║  ";
                }
            }
            std::wcout << L"   " << Symbol(dealerHand[dealerHand.size() - 1].cardType) << L"║\n";
            break;

        case 5:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                if (firstRound && i + 1 != dealerHand.size() && !gameEnded)
                {
                    std::wcout << L"║░░";
                }
                else
                {
                    std::wcout << L"║  ";
                }
            }
            if(dealerHand[dealerHand.size() - 1].cardValue + 1 != 10)
            {
                std::wcout << L"   " << CardValueConverter(dealerHand[dealerHand.size() - 1].cardValue) << L"║\n";
            } else { std::wcout << L"  " << CardValueConverter(dealerHand[dealerHand.size() - 1].cardValue) << L"║\n"; }
            
            break;

        case 6:
            for (int i = 0; i < dealerHand.size(); i++)
            {
                std::wcout << L"╚══";
            }
            std::wcout << L"════╝\n";
            break;

        default:
            break;
        }
    }

    std::wcout << "\n";
    
    //Win Condition
    if(playerPoints() == 21 || dealerPoints() > 21)
    {
        std::wcout << L"\u001b[93m            You Win!\u001b[0m" << L"\n";
        std::wcout << "\u001b[90m  (Press any key to continue)\n\u001b[0m";
        gameEnded = true;
    }
    else if(playerPoints() > 21 || gameEnded && dealerPoints() > playerPoints() && dealerPoints() <= 21)
    {
        std::wcout << L"\u001b[93m            You Lose!\u001b[0m" << L"\n";
        std::wcout << "\u001b[90m  (Press any key to continue)\n\u001b[0m";
        gameEnded = true;
    }
    else if (gameEnded && playerPoints() == dealerPoints())
    {
        std::wcout << L"\u001b[93m              Tie!\u001b[0m" << L"\n";
        std::wcout << "\u001b[90m  (Press any key to continue)\n\u001b[0m";
        gameEnded = true;
    }
    else
    {
        std::wcout << L"\u001b[93m      HIT [H]      STAND [S]\u001b[0m" << L"\n\n";
    }

    std::wcout << "\n";


    std::wcout << "         Player's hand\n";

    std::wcout << "               " << playerPoints() << "\n";


    //Display the player's cards
    for (int cardLength = 0; cardLength < 7; cardLength++)
    {
        //Add in space before cards get drawn;
        float spaceValue = (12 - round((playerHand.size() - 1) * 1.5f));
        for (int i = 0; i < spaceValue; i++)
        {
            std::wcout << " ";
        }

        switch (cardLength)
        {
        case 0:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"╔══";
            }
            std::wcout << L"════╗\n";
            break;

        case 1:
            for (int i = 0; i < playerHand.size(); i++)
            {
                if (playerHand[i].cardValue + 1 != 10)
                {
                    std::wcout << L"║" << CardValueConverter(playerHand[i].cardValue) << " ";
                }
                else
                {
                    std::wcout << L"║" << CardValueConverter(playerHand[i].cardValue);
                }
            }
            std::wcout << L"    ║\n";
            break;

        case 2:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"║" << Symbol(playerHand[i].cardType) << " ";
            }
            std::wcout << L"    ║\n";
            break;

        case 3:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"║  ";
            }
            std::wcout << L"    ║\n";
            break;

        case 4:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"║  ";
            }
            std::wcout << L"   " << Symbol(playerHand[playerHand.size() - 1].cardType) << L"║\n";
            break;

        case 5:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"║  ";
            }
            if (playerHand[playerHand.size() - 1].cardValue + 1 != 10)
            {
                std::wcout << L"   " << CardValueConverter(playerHand[playerHand.size() - 1].cardValue) << L"║\n";
            }
            else { std::wcout << L"  " << CardValueConverter(playerHand[playerHand.size() - 1].cardValue) << L"║\n"; }

            break;

        case 6:
            for (int i = 0; i < playerHand.size(); i++)
            {
                std::wcout << L"╚══";
            }
            std::wcout << L"════╝\n";
            break;

        default:
            break;
        }
    }


    std::wcout << "\n             Input: ";
}

void Menu()
{
    std::wcout << "\n";
    std::wcout << L"  ███████   ██         █████       ██████    ██    ██     █████    █████       ██████    ██    ██\n";
    std::wcout << L"  ██    ██  ██        ██   ██    ██      ██  ██   ██         ██   ██   ██    ██      ██  ██   ██ \n";
    std::wcout << L"  ██    ██  ██       ██     ██  ██           ██  ██          ██  ██     ██  ██           ██  ██  \n";
    std::wcout << L"  ███████   ██       ██     ██  ██           █████           ██  ██     ██  ██           █████   \n";
    std::wcout << L"  ██    ██  ██       █████████  ██           ██  ██    ██    ██  █████████  ██           ██  ██  \n";
    std::wcout << L"  ██    ██  ██       ██     ██   ██      ██  ██   ██   ██    ██  ██     ██   ██      ██  ██   ██ \n";
    std::wcout << L"  ███████   ███████  ██     ██     ██████    ██    ██    █████   ██     ██     ██████    ██    ██\n\n";

    std::wcout << L"                                               INFO:\n";
    std::wcout << L"                                      You start out with $100\n";
    std::wcout << L"                  At the start of every game, you must bet a sum of money (minimum $10)\n";
    std::wcout << L"                       The winner gets their own and their opponents bet money\n";


    std::wcout << L"\n\n                                       "<< L"\033[97mPRESS ANY KEY TO START\033[0m";
    std::wcout << L"\n\n                                              Input: ";
    getwchar();
    std::wcin.ignore(32767, '\n');
}

//This is where the player places their bet
void BetScreen()
{
    bool invalidInput{ false };
    do
    {

        system("cls");

        std::wcout << "\n";
        std::wcout << L"                                     ███████   ████████  ████████\n";
        std::wcout << L"                                     ██    ██  ██           ██   \n";
        std::wcout << L"                                     ██    ██  ██           ██   \n";
        std::wcout << L"                                     ███████   ██████       ██   \n";
        std::wcout << L"                                     ██    ██  ██           ██   \n";
        std::wcout << L"                                     ██    ██  ██           ██   \n";
        std::wcout << L"                                     ███████   ████████     ██   \n\n";
        std::wcout << L"\n                                  You: $" << playerMoney << "             Dealer: $" << dealerMoney << "\n";
        std::wcout << L"\n                           How much are you willing to bet (minimum $10)?\n";

        if(invalidInput && betAmount < 10)
        {
            std::wcout << L"                                (Error: You must bet $10 or more)\n";
        }
        else if(invalidInput && betAmount > playerMoney)
        {
            std::wcout << L"                             (Error: You do not have that much money)\n";
        }
        else if(invalidInput && betAmount > dealerMoney)
        {
            std::wcout << L"                              (Error: The dealer can't bet that high)\n";
        }
        std::wcout << L"\n                                             Input: $";
        std::wcin >> betAmount;
        invalidInput = true;
        std::wcin.clear();

        std::wcin.ignore(32767, '\n');
        if(std::wcin.fail())
        {
        }

    } while (betAmount < 10 || betAmount > playerMoney || betAmount > dealerMoney);
    playerMoney -= betAmount;
    dealerMoney -= betAmount;
}

//Puts all the cards back into the deck
void ShuffleDeck()
{
    for(int i = 0; i < 4; i++)
    {
        for(int k = 0; k < 13; k++)
        {
            cardsInDeck[i][k] = true;
        }
    }
}

int main(int) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    srand(time(NULL));

    //Start the player off in the menu
    Menu();

    //Game Loop
    do
    {
        //Make the player place a bet
        BetScreen();
        //Place all the cards back into the deck
        ShuffleDeck();

        //Player and Dealer draws two cards the first round
        for (int i = 0; i < 4; i++)
        {
            int a; //Random card type
            int b; //Random card value
            do
            {
                a = rand() % 4;
                b = rand() % 13;
            } while (cardsInDeck[a][b] == false);

            if (i < 2)
            {
                playerHand.push_back({ a,b });
            }
            else if (i < 4 && i >= 2)
            {

                dealerHand.push_back({ a,b });
            }

            cardsInDeck[a][b] = false;
        }
        firstRound = true;

        //Untill the game has ended, keep playing
        while (gameEnded == false)
        {
            system("cls");
            DrawDisplay();


            hitOrStand = getwchar();

            if (hitOrStand == 'h' || hitOrStand == 'H')
            {
                PlayerDrawCard();
                if (playerPoints() > 21)
                {
                    gameEnded = true;
                    firstRound = false;
                }
            }
            else if (hitOrStand == 's' || hitOrStand == 'S')
            {
                firstRound = false;
                while (dealerPoints() < playerPoints() && dealerPoints() < 21)
                {
                    DealerDrawCard();
                }
                if (dealerPoints() >= playerPoints() && dealerPoints() <= 21)
                {
                    gameEnded = true;
                }
            }
        }
        if (gameEnded && dealerPoints() < playerPoints() && playerPoints() <= 21 || gameEnded && dealerPoints() > 21)
        {
            //Player won
            playerMoney += betAmount * 2;
        }
        else if (gameEnded && dealerPoints() > playerPoints() && dealerPoints() <= 21 || gameEnded && playerPoints() > 21)
        {
            //Dealer won
            dealerMoney += betAmount * 2;
        }
        else if (gameEnded && playerPoints() == dealerPoints())
        {
            //Tie
            dealerMoney += betAmount;
            playerMoney += betAmount;
        }

        system("cls");
        DrawDisplay();
        //Get any key from user to continue the loop
        getwchar();
        std::wcin.ignore(32767, '\n');

        //Reset values for a new game
        playerHand.clear();
        dealerHand.clear();
        gameEnded = false;
        firstRound = true;

    } while(playerMoney >= 10 && dealerMoney >= 10);

    EndingScreen();
}