
#include <iostream>
#include<vector>
#include <random>


using namespace std;
string ranks[] = {"", "A", "2", "3", "4", "5", "6", "7","8","9","10","J","Q","K"}; 
string types[] = {"C", "D", "H", "S"}; 
double games = 0.0;
double computerWins = 0.0;


class Card{
    public:
    // Constructor for the card class
    Card(int rank, int type){
        this->rank = rank;
        this->type = type;
    };
    enum rank {ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK= 11, QUEEN= 12, KING= 13};
    enum type {CLUBS, DIAMONDS, HEARTS, SPADES};

    int rank;
    int type;

    int getValue(){
        return this->rank;
    };
    void displayCard(){
        // print out the rank and the first string of the type
        cout << ranks[this->rank] +  types[this->type];
    };
};

class Hand{
    public:
    Hand(){
        vector<Card> cards;
        this->cards = cards;
    };
    vector<Card> cards;
    void add(Card card){
        cards.push_back(card);
    }
    void clear(){
        cards.clear();
    }
    int getTotal(){
        int total = 0;
        int AceInHand = 0;
        for (int i = 0; i < cards.size(); i++){
            if (cards[i].rank == 1){
                AceInHand += 1;
            }
            if (cards[i].rank >= 10){
                total = total + 10;
                continue;
            } 
            total = total + cards[i].rank;
        }
        // Handle the Ace case (value between 1 or 11)
        while(AceInHand > 0){
            if(total+10<=21){
                total=total+10;
            }
            AceInHand = AceInHand -1;
        }
        return total;
    }
};

class Deck{
    public:
    vector<Card> deck;
    Deck(){
        vector<Card> deck;
        this->deck = deck;
    };
    void Populate(){
        // Clear the deck at the beginning for the next games
        deck.clear();
        for (int rank = Card::ACE; rank <= Card::KING; rank++){
            for(int type = 0; type<=3; type++) {
                Card temp = Card(rank,type);
                deck.push_back(temp);
        }
    }

    }
       
    void shuffle(){
        std::random_device rd;
        std::shuffle(this->deck.begin(),this->deck.end(),rd);
    }
    // give a pointer as input
    void deal(Hand* hand){
        Card selected = this->deck[0];
        this->deck.erase(this->deck.begin());
        hand->add(selected);
    }

    void tricky_deal(Hand* hand){
        // If the win rate is below 55%, do a tricky deal
        cout << "Casino's dealing in a suspicious way..." << endl;
        int i;
        Card selected = this->deck[0];
        for(int i = 0;i<this->deck.size();i++){
            int rank = this->deck[i].rank;
            if(rank >= 11){
                rank = 10;
            }
            if (rank + hand->getTotal() == 21){
                selected = this->deck[i];
                this->deck.erase(this->deck.begin()+i);
                hand->add(selected);
                return;
            }
        }
        this->deck.erase(this->deck.begin());
        hand->add(selected);
    }
};

class AbstractPlayer{
    public:
    Hand* hand;
    AbstractPlayer(){
        Hand* hand = new Hand();
        this->hand = hand;
    };
    virtual ~AbstractPlayer(){};
    virtual bool isDrawing() const = 0; 
    bool isBusted(){return (this->hand->getTotal()>=22);};
};

class HumanPlayer:public AbstractPlayer{
    public:
    void announce(Hand* casino){
        if(casino->getTotal() > this->hand->getTotal()){
                    cout << "Casino wins." << endl << endl;
                    // Increase computer wins to keep track of the win rate
                    computerWins= computerWins + 1.0;
                }
                else if(casino->getTotal() < this->hand->getTotal()){
                    cout << "Player wins." << endl << endl;
                } 
                else{cout << "Push: No one wins." << endl << endl;}
    }
    bool isDrawing()const {
        return 1;
    }
};

class ComputerPlayer: public AbstractPlayer{
    public:
    ComputerPlayer(){};
    virtual ~ComputerPlayer(){};
    bool isDrawing() const { return (this->hand->getTotal()<=16);}
};

class BlackJackGame{
    public:
    Deck* m_deck;
    ComputerPlayer* m_casino;
    HumanPlayer* m_human;
    // Constructor for the BlackJackGame class
    BlackJackGame(){
        Deck* deck = new Deck();
        this->m_deck = deck;
        this->m_deck->Populate();
        this->m_deck->shuffle();
        ComputerPlayer* casino = new ComputerPlayer();
        this->m_casino = casino;
        HumanPlayer* human = new HumanPlayer();
        this->m_human = human;
    };
    
    void play(){
        if(games != 0){
            cout << "Casino's winRate : ";
            cout << std::to_string((int)((computerWins/games)*100)) + "%" << endl;
        }
        games = games + 1.0;
        char answer = 'y';
        bool playAgain = true;

        // Casino plays once
        this->m_deck->deal(m_casino->hand); 
        cout << "Casino: ";
        m_casino->hand->cards[0].displayCard();
        cout << " [" + std::to_string(m_casino->hand->getTotal()) + "]" << endl;
        
        // player plays twice
        this->m_deck->deal(m_human->hand); 
        this->m_deck->deal(m_human->hand); 
        cout << "Player:";
        for(int i = 0;i<m_human->hand->cards.size();i++){
            cout << " ";
            m_human->hand->cards[i].displayCard();
        }
        cout << " [" + std::to_string(m_human->hand->getTotal()) + "]" << endl;

        cout << "Do you want to draw? (y/n):";
        cin >> answer;
        while(true){
            // Players decide to draw a card
            if (answer == 'y'){
                this->m_deck->deal(m_human->hand); 
                cout << "Player:";
                for(int i = 0;i<m_human->hand->cards.size();i++){
                    cout << " ";
                    m_human->hand->cards[i].displayCard();
                }
                cout << " [" + std::to_string(m_human->hand->getTotal()) + "]" << endl;
                // If player is busted reset everything and exit the method
                if (m_human->isBusted()){
                    cout << "Player busts." << endl << "Casino wins." << endl << endl;
                    computerWins = computerWins + 1.0;
                    m_human->hand->clear();
                    m_casino->hand->clear();
                    m_deck->Populate();
                    m_deck->shuffle();
                    return;
                }
                cout << "Do you want to draw? (y/n):";
                cin >> answer;
                if(answer == 'y'){
                    continue;
                }
            }
            // Casino is drawing a card
            else if (answer == 'n'){
                // Check if the casino can draw and draw until 16 is obtained
                while(m_casino->isDrawing()){
                    // If the win rate is below 55%, the casino will do a tricky deal and pick the best card
                    if(computerWins/games < 0.55 && games!=1){
                        this->m_deck->tricky_deal(m_casino->hand); 
                    }
                    else {this->m_deck->deal(m_casino->hand);}
                    cout << "Casino: ";
                    for(int i = 0;i<m_casino->hand->cards.size();i++){
                        cout << " ";
                        m_casino->hand->cards[i].displayCard();
                    }
                    cout << " [" + std::to_string(m_casino->hand->getTotal()) + "]" << endl;
                    if(m_casino->isBusted()){
                        cout << "Casino busts." << endl << "Player wins." << endl << endl;
                        m_human->hand->clear();
                        m_casino->hand->clear();
                        m_deck->Populate();
                        m_deck->shuffle();
                        return;
                    }
                }
                m_human->announce(m_casino->hand);
                break;
            }
            else{
                cout << "Answer must be y or n. Other answers are not supported by the casino."; 
            break;
        }
    }
    // Clear hands 
    m_human->hand->clear();
    m_casino->hand->clear();
    m_deck->Populate();
    m_deck->shuffle();
    return;
    };
};

int main() {
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
}
    cout <<"Gave over!";
return 0; 
}

