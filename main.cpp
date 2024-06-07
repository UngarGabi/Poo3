#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

enum TipClient{
    Normal = 1, Student = 2, Pensionar = 3, Handicap = 4
};

// Metoda Strategy
class Jeton {
protected:
    double pret = 15;
public:
    virtual double calculPret() = 0;
    friend std::ostream& operator<<(std::ostream& out, const Jeton& jeton);
    virtual double getPret() = 0;
    virtual ~Jeton() = default;
};

std::ostream& operator<<(std::ostream& out, const Jeton& jeton) {
    out << "Pretul biletului este: " << '\n';
    out << jeton.pret;
    return out;
}

class JetonPretNormal : public Jeton {
public:
    double calculPret() override {
        return pret;
    }

    double getPret() override{
        return pret;
    }
};

class JetonPretStudent : public Jeton {
public:
    double calculPret() override {
        return pret * 0.2; // 80% reducere
    }

    double getPret() override{
        return pret;
    }
};

class JetonPretPensionar : public Jeton {
public:
    double calculPret() override {
        return  pret * 0.7; // 30% reducere
    }

    double getPret() override {
        return pret;
    }
};

class JetonPretHandicap : public Jeton {
public:
    double calculPret() override {
        return pret * 0.5; // 50% reducere
    }

    double getPret() override {
        return pret;
    }
};

//metoda Singleton
class Vanzator {
private:
    static Vanzator *instance;
    Vanzator() {}
public:
    Vanzator(Vanzator &) = delete;
    Vanzator &operator=(Vanzator &) = delete;

    static Vanzator *getInstance() {
        if (instance == nullptr)
            instance = new Vanzator();
        return instance;
    }

    static void BunVenit(){
        std::cout << "Bun venit la Arcade, distractie placuta.";
    }

    static int CeFelDeClient(){
        int tip;
        std::cout << "Ce fel de client sunteti? " << '\n';
        std::cout << "1.Normal" << '\n';
        std::cout << "2.Student" << '\n';
        std::cout << "3.Pensionar" << '\n';
        std::cout << "4.Handicap" << '\n';
        std::cin >> tip;
        return tip;
    }
};

class Client{
private:
    std::string nume;
    double buget;
    std::vector<Jeton*> ListaJetoane;
    std::string tipClient;
public:
    Client() = default;
    Client(std::string nume, double buget, std::string tipClient) {
        this->nume = nume;
        this->buget = buget;
        this->tipClient = tipClient;
    }

    template <typename T>
    void CumparaJeton(T jeton, int nrBilete) {
        double pretNouJeton = jeton.calculPret();
        for(int i = 1; i <= nrBilete; i++) {
            if (pretNouJeton <= buget) {
                buget -= pretNouJeton;
                ListaJetoane.push_back(new T(jeton));
                std::cout << "Bilet Cumparat Cu Succes." << '\n';
            } else {
                std::cout << "Buget insuficient" << '\n';
                break;
            }
        }
    }

    friend std::istream& operator>>(std::istream& in, Client& client);
};

std::istream& operator>>(std::istream& in, Client& client) {
    std::cout << "Nume: " << '\n';
    in >> client.nume;
    std::cout << "Buget: " << '\n';
    in >> client.buget;
    return in;
}

class JucatoriBlackJack {
protected:
    const int target = 21;
public:
    virtual void ExtrageCarte() = 0;
    virtual void AfiseazaCarti() = 0;
    virtual bool VerificaTragere() = 0;
    virtual int SumaCarti() = 0;
    virtual void Win() = 0;
};

class Dealer : public JucatoriBlackJack {
private:
    std::vector<int> Carti;
public:
    Dealer() = default;
    void ExtrageCarte() override {
        int CarteExtrasa = std::rand() % 11 + 1;
        Carti.push_back(CarteExtrasa);
        std::cout << "Cartea extrasa de dealer este: " << CarteExtrasa << '\n';
    }

    void AfiseazaCarti() override {
        std::cout << "Cartile dealer-ului sunt: " << '\n';
        for(int carte : Carti)
            std::cout << carte << " ";
        std::cout << '\n';
    }

    bool VerificaTragere() override {
        int sumPachet = 0;
        for(int carte : Carti)
            sumPachet += carte;
        return sumPachet < 17;
    }

    int SumaCarti() override {
        int sumPachet = 0;
        for(int carte : Carti)
            sumPachet += carte;
        return sumPachet;
    }

    void Win() override {
        std::cout << "Dealer-ul a castigat!" << '\n';
    }
};

class Player : public JucatoriBlackJack {
private:
    std::vector<int> Carti;
public:
    Player() = default;
    void ExtrageCarte() override {
        int CarteExtrasa = std::rand() % 11 + 1;
        Carti.push_back(CarteExtrasa);
        std::cout << "Cartea extrasa de player este: " << CarteExtrasa << '\n';
    }

    void AfiseazaCarti() override {
        std::cout << "Cartile player-ului sunt: " << '\n';
        for(int carte : Carti)
            std::cout << carte << " ";
        std::cout << '\n';
    }

    bool VerificaTragere() override {
        int sumPachet = 0;
        for(int carte : Carti)
            sumPachet += carte;
        std::cout << "Suma cartilor dumneavoastra este de " << sumPachet << "." << '\n';
        std::string alegere;
        std::cout << "Doriti sa mai trageti o carte?(Da/Nu)" << '\n';
        std::cin >> alegere;
        return alegere == "Da" || alegere == "da";
    }

    int SumaCarti() override {
        int sumPachet = 0;
        for(int carte : Carti)
            sumPachet += carte;
        return sumPachet;
    }

    void Win() override {
        std::cout << "Player-ul a castigat!" << '\n';
    }
};

class BlackJack {
private:
    Player* player;
    Dealer* dealer;

    void Castigator() {
        int p = player->SumaCarti();
        int d = dealer->SumaCarti();
        if (p <= 21 && d <= 21) {
            if (p > d)
                player->Win();
            else if (p == d)
                std::cout << "Remiza" << '\n';
            else
                dealer->Win();
        } else {
            if (p > 21 && d <= 21)
                dealer->Win();
            else if (p <= 21 && d > 21)
                player->Win();
            else
                std::cout << "Remiza" << '\n';
        }
    }

public:
    BlackJack(Player* player, Dealer* dealer) : player(player), dealer(dealer) {}

    void Play() {
        bool MaiSeTrageCartePlayer = true;
        bool MaiSeTrageCarteDealer = true;
        int numarTure = 0;

        std::cout << "Bun venit la BlackJack, va uram mult noroc!" << '\n';
        std::cout << "---------------------------------------------------------" << '\n';
        std::cout << '\n';

        while (MaiSeTrageCartePlayer || MaiSeTrageCarteDealer) {
            if (MaiSeTrageCarteDealer) {
                dealer->ExtrageCarte();
                dealer->AfiseazaCarti();
                if (dealer->SumaCarti() > 21) {
                    MaiSeTrageCarteDealer = false;
                    std::cout << "Suma este mai mare decat 21, dealer-ul nu mai poate trage carti." << '\n';
                } else {
                    MaiSeTrageCarteDealer = dealer->VerificaTragere();
                    if(!MaiSeTrageCarteDealer)
                        std::cout << "Dealer-ul nu mai vrea sa traga din pachet" << '\n';
                }
            }

            if (MaiSeTrageCartePlayer) {
                player->ExtrageCarte();
                player->AfiseazaCarti();
                if (player->SumaCarti() > 21) {
                    MaiSeTrageCartePlayer = false;
                    std::cout << "Suma este mai mare decat 21, player-ul nu mai poate trage carti." << '\n';
                } else {
                    MaiSeTrageCartePlayer = player->VerificaTragere();
                }
            }
            std::cout << '\n';
            std::cout << "Tura " << ++numarTure << " s-a terminat." << '\n';
            std::cout << '\n';
            std::cout << "---------------------------------------------------------" << '\n';
            std::cout << '\n';
        }
        std::cout << "Jocul s-a terminat, ";
        Castigator();
    }
};

Vanzator* Vanzator::instance = nullptr;

int main() {
    std::srand(static_cast<unsigned int>(std::time(0))); // ca sa nu mi mai dea aceleasi carti mereu
    Vanzator* vanzatorInstance = Vanzator::getInstance();

    JetonPretStudent jetonStudent;
    JetonPretPensionar jetonPensionar;
    JetonPretNormal jetonNormal;
    JetonPretHandicap jetonHandicap;

    Client client;

    int nrBilete;

    Vanzator::BunVenit();

    std::cin >> client;

    int tip = Vanzator::CeFelDeClient();

    switch(tip) {
        case Student:
            std::cout << "Cate jetoane doriti sa cumparati?" << '\n';
            std::cin >> nrBilete;
            client.CumparaJeton(jetonStudent, nrBilete);
            break;
        case Handicap:
            std::cout << "Cate jetoane doriti sa cumparati?" << '\n';
            std::cin >> nrBilete;
            client.CumparaJeton(jetonHandicap, nrBilete);
            break;
        case Pensionar:
            std::cout << "Cate jetoane doriti sa cumparati?" << '\n';
            std::cin >> nrBilete;
            client.CumparaJeton(jetonPensionar, nrBilete);
            break;
        case Normal:
            std::cout << "Cate jetoane doriti sa cumparati?" << '\n';
            std::cin >> nrBilete;
            client.CumparaJeton(jetonNormal, nrBilete);
            break;
        default:
            std::cout << "Tip ales gresit" << '\n';
            break;
    }

    Player player;
    Dealer dealer;
    BlackJack joc(&player, &dealer);

    joc.Play();

    return 0;
}
