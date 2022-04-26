
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
                AceInHand = AceInHand -1;
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

    void deal(Hand* hand){
        Card selected = this->deck[0];
        this->deck.erase(this->deck.begin());
        hand->add(selected);
    }

    void tricky_deal(Hand* hand){
        // If the win rate is below 55%, do a tricky deal
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
    // Different cases of win/loss
    void announce(Hand* casino){
                if((this->hand->getTotal() == 21) && this->hand->cards.size()==2){
                    cout << "BLACKJACK ;)" ;
                }
                if((casino->getTotal() > 21) && (this->hand->getTotal() > 21)){
                    cout << "Push: Both bust. No one wins." << endl ;
                }
                else if((casino->getTotal() > 21) && (this->hand->getTotal() <= 21)){
                    cout << "Player wins. Casino busts." << endl ;
                }
                else if((casino->getTotal() <= 21) && (this->hand->getTotal() > 21)){
                    cout << "Casino wins. Player busts." << endl ;
                    computerWins = computerWins+1.0;
                }
                else if(casino->getTotal() > this->hand->getTotal()){
                    cout << "Casino wins." << endl;
                    computerWins= computerWins + 1.0;
                }
                else if(casino->getTotal() < this->hand->getTotal()){
                    cout << "Player wins." << endl;
                } 
                else{cout << "Push: No one wins." << endl ;
                }
                // Clear hand for next game
                this->hand->clear();
                return;
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
    int hands;
    Deck* m_deck;
    ComputerPlayer* m_casino;
    // Vector of players to have different hands 
    vector<HumanPlayer*> m_human;
    // Constructor for the BlackJackGame class
    BlackJackGame(){
        Deck* deck = new Deck();
        this->m_deck = deck;
        this->m_deck->Populate();
        this->m_deck->shuffle();
        ComputerPlayer* casino = new ComputerPlayer();
        this->m_casino = casino;
        vector<HumanPlayer*> human;
        this->m_human = human;
    };

    // Populate the vector of players to create a multihand game
    void CreateHands(){
        for(int i =1;i<=this->hands;i++){
            HumanPlayer* human = new HumanPlayer();
            m_human.push_back(human);
        }
    }
    
    void play(){
        // Casino plays
        char answer = 'y';
        this->m_deck->deal(m_casino->hand); 
        cout << "Casino: ";
        m_casino->hand->cards[0].displayCard();
        cout << " [" + std::to_string(m_casino->hand->getTotal()) + "]" << endl << endl;
            
        // For loop to iterate through the hands and take the first 2 cards.
        for(int i=0;i<this->hands;i++){
            games = games+1;
            cout << "------------ Hand "+ std::to_string(i+1) + " ------------" << endl;
            // player plays twice
            this->m_deck->deal(m_human[i]->hand); 
            this->m_deck->deal(m_human[i]->hand); 
            cout << "Player:";
            for(int j = 0;j<m_human[i]->hand->cards.size();j++){
                cout << " ";
                m_human[i]->hand->cards[j].displayCard();
            }
            cout << " [" + std::to_string(m_human[i]->hand->getTotal()) + "]" << endl ;
            cout << "-------------------------------" << endl;
        }

        // For each hand check if the player wants to draw
        for(int i=0;i<this->hands;i++){
            bool keephand = true;
            char answer = 'y';  
            cout << endl<< "Do you want to draw for hand " + std::to_string(i+1) + "? (y/n):";
            cin >> answer;
            // Until the answer becomes no (or bust/blackjack), continue to draw card
            while(keephand){
                if (answer == 'y'){
                    this->m_deck->deal(m_human[i]->hand); 
                    cout << "Player:";
                    for(int j = 0;j<m_human[i]->hand->cards.size();j++){
                        cout << " ";
                        m_human[i]->hand->cards[j].displayCard();
                    }
                    cout << " [" + std::to_string(m_human[i]->hand->getTotal()) + "]" << endl;
                    // Stop asking for drawing in case of Bust or Blackjack
                    if(m_human[i]->isBusted() || m_human[i]->hand->getTotal() == 21){
                        keephand = false;
                        continue;
                    }
                }
                // Get to the next hand if the player does not want to draw anymore
                if(answer == 'n'){
                    keephand = false;
                    continue;
                }
                cout << endl<< "Do you still want to draw for hand " + std::to_string(i+1) + "? (y/n):";
                cin >> answer;
            }
        }
            cout << endl << "Casino's turn to play..." << endl;
            while(m_casino->isDrawing()){
                    // if the win rate is low, the casino will trick his deal
                    if(computerWins/games < 0.55 && games!=this->hands){
                        cout << "Casino's dealing in a suspicious way..." << endl;
                        cout << "Casino: ";
                        this->m_deck->tricky_deal(m_casino->hand); 
                    }
                    else {this->m_deck->deal(m_casino->hand);
                        cout << "Casino: ";} 

                    for(int i = 0;i<m_casino->hand->cards.size();i++){
                        cout << " ";
                        m_casino->hand->cards[i].displayCard();
                    }
                    cout << " [" + std::to_string(m_casino->hand->getTotal()) + "]" << endl;
            }
            // Display final scores
            cout << endl << "---------- Final Scores ----------" << endl;
            for(int i=0;i<this->hands;i++){
                cout << "Hand " + std::to_string(i+1) + ": ";
                m_human[i]->announce(m_casino->hand);
            }
            m_casino->hand->clear();
            m_deck->Populate();
            m_deck->shuffle();
            cout << "----------------------------------" << endl;
            
        
    }
};


int main() {
    int n_hands = 1;
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    cout << "How many hands do you want to play with? (1-3): ";
    cin >> n_hands;
    cout << endl;
    // Create a blackjack game with the wantednumber of hands
    BlackJackGame game;
    game.hands = n_hands;
    game.CreateHands();
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
