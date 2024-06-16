#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

enum TipClient{
    Normal = 1, Student = 2, Pensionar = 3, Handicap = 4
};

enum Jocuri{
    Blackjack = 1, Zar = 2, Pacanele = 3
};

enum Simboluri{
    Cirese = 1, Portocala = 2, Lamaie = 3, Struguri = 4, Steluta = 5
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
        std::cout << "Bun venit la Arcade, distractie placuta." << '\n';
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
    int nrPuncte;
public:
    Client() = default;
    Client(std::string nume, double buget, std::string tipClient) {
        this->nume = nume;
        this->buget = buget;
        this->tipClient = tipClient;
        this->nrPuncte = 0;
    }
    void AdaugaPuncte(int nrPct){
        nrPuncte += nrPct;
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
    int getNrJetoane(){
        return ListaJetoane.size();
    }
    ~Client(){
        for(Jeton* jeton : ListaJetoane)
            delete jeton;
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
    virtual void Reset() = 0;
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
        std::cout << "Dealer-ul a tras o carte." << '\n';
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
    void Reset() override{
        Carti.clear();
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
    void Reset() override {
        Carti.clear();
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
            else
                if (p == d)
                    std::cout << "Remiza" << '\n';
                else
                    dealer->Win();
        }
        else {
            if (p > 21 && d <= 21)
                dealer->Win();
            else
                if (p <= 21 && d > 21)
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

        player->Reset();
        dealer->Reset();

        std::cout << "Bun venit la BlackJack, va uram mult noroc!" << '\n';
        std::cout << "---------------------------------------------------------" << '\n';
        std::cout << '\n';

        while (MaiSeTrageCartePlayer || MaiSeTrageCarteDealer) {
            if (MaiSeTrageCarteDealer) {
                dealer->ExtrageCarte();
                if (dealer->SumaCarti() > 21) {
                    MaiSeTrageCarteDealer = false;
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
            std::cout << "Catrile dealer-ului" << '\n';
            dealer->AfiseazaCarti();
            std::cout << '\n';
            std::cout << "Tura " << ++numarTure << " s-a terminat." << '\n';
            std::cout << '\n';
            std::cout << "---------------------------------------------------------" << '\n';
            std::cout << '\n';
        }
        std::cout << "Jocul s-a terminat, ";
        Castigator();
    }
    ~BlackJack(){
        delete player;
        delete dealer;
    }
};

class Zaruri{
private:
    void Castigator(std::pair<int, int> a, std::pair<int, int> b) {
        int sum1 = a.first + a.second;
        int sum2 = b.first + b.second;
        bool PrimaPerecheEgala = false;
        bool ADouaPerecheEgala = false;

        if (a.first == a.second)
            PrimaPerecheEgala = true;

        if (b.first == b.second)
            bool ADouaPerecheEgala = true;

        if (PrimaPerecheEgala && ADouaPerecheEgala) {
            if (sum1 == sum2)
                std::cout << "Remiza" << '\n';
            else
                if (sum1 > sum2)
                    std::cout << "Player" << '\n';
                else
                    std::cout << "Dealer" << '\n';
        }
        else
            if (PrimaPerecheEgala && !ADouaPerecheEgala)
                std::cout << "Player" << '\n';
            else
                if (!PrimaPerecheEgala && ADouaPerecheEgala)
                    std::cout << "Dealer" << '\n';
                else {
                    if (sum1 > sum2)
                        std::cout << "Player" << '\n';
                    else
                        if (sum1 < sum2)
                            std::cout << "Dealer" << '\n';
                        else
                            std::cout << "Remiza" << '\n';
        }
    }

public:
    std::pair<int, int> aruncaZaruri() {
        int zar1 = std::rand() % 6 + 1;
        int zar2 = std::rand() % 6 + 1;
        std::pair<int, int> zaruri = std::make_pair(zar1, zar2);
        return zaruri;
    }
    void Play(){
        std::pair<int, int> zaruri1 = aruncaZaruri();
        std::pair<int, int> zaruri2 = aruncaZaruri();

        std::cout << "Player: " << zaruri1.first << ", " << zaruri1.second << '\n';
        std::cout << "Dealer: " << zaruri2.first << ", " << zaruri2.second << '\n';

        Castigator(zaruri1, zaruri2);
    }
};

class Pacanea {
private:
    std::vector<int> display;
    void MesajBunVenit() {
        std::cout << "1 jeton = 5 rotiri." << '\n';
        std::cout << "Apasati pe . pentru a roti." << "\n";
    }

    void Roll() {
        int elem1 = std::rand() % 5 + 1;
        int elem2 = std::rand() % 5 + 1;
        int elem3 = std::rand() % 5 + 1;
        display = {elem1, elem2, elem3};
    }

    void AfisarePacanea() {
        std::cout << "| ";
        for (const auto &simbol: display) {
            switch (simbol) {
                case Cirese:
                    std::cout << "🍒" << " | ";
                    break;
                case Portocala:
                    std::cout << "🍊" << " | ";
                    break;
                case Lamaie:
                    std::cout << "🍋" << " | ";
                    break;
                case Struguri:
                    std::cout << "🍇" << " | ";
                    break;
                case Steluta:
                    std::cout << "⭐" << " | ";
                    break;
                default:
                    break;
            }
        }
    }

    void Castig() {
        if (display[0] == display[1] && display[1] == display[2])
            std::cout << "Castigator" << '\n';
        else
            std::cout << "Necastigator" << '\n';
    }
public:
    void Play(){
        MesajBunVenit();
        int rotiri = 5;
        while(rotiri != 0) {
            std::string Space;
            std::cin >> Space;
            if (Space == ".") {
                Roll();
                AfisarePacanea();
                std::cout << '\n';
                Castig();
            }
            rotiri--;
        }
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
    BlackJack jocBlackJack(&player, &dealer);

    Zaruri jocZar;

    Pacanea jocPacanea;

    int NrJetoane = client.getNrJetoane();
    bool MaiJoaca = true;
    while(MaiJoaca) {
        NrJetoane--;
        int alegeJoc;
        std::cout << "Ce joc vreti sa jucati?" << '\n';
        std::cout << "1. BlackJack" << '\n';
        std::cout << "2. Zaruri" << '\n';
        std::cout << "3. Pacanea" << '\n';
        std::cin >> alegeJoc;

        switch(alegeJoc) {
            case Blackjack:
                jocBlackJack.Play();
                break;
            case Zar:
                jocZar.Play();
                break;
            case Pacanele:
                jocPacanea.Play();
                break;
            default:
                std::cout << "Ati introdus gresit." << '\n';
                break;
        }

        if (NrJetoane == 0) {
            MaiJoaca = false;
            std::cout << "Nu mai aveti jetoane." << '\n';
        }
        else {
            std::string continuaJoc;
            std::cout << "Doriti sa mai jucat(Da/Nu)?" << '\n';
            std::cin >> continuaJoc;
            if (continuaJoc == "nu" || continuaJoc == "Nu")
                MaiJoaca = false;
        }
    }



    return 0;
}
// De facut:
// Delay cand intri in joc la BlackJack
//Sis de pct
