#include <iostream>
#include <vector>
#include <string>
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
private :
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
         this->tipClient=tipClient;
     }
    template <typename T>
    void CumparaJeton(T jeton, int nrBilete) {
        double pretNouJeton = jeton.calculPret();
        for(int i=1;i<=nrBilete;i++) {
            if (pretNouJeton <= buget) {
                buget -= pretNouJeton;
                ListaJetoane.push_back(&jeton);
                std::cout << "Bilet Cumparat Cu Succes." << '\n';
            }
            else {
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
    std::cout << "Ce fel de client sunteti?(Normal/Pensionar/Student/Handicap): " << '\n';
    in >> client.tipClient;
    return in;
}

Vanzator* Vanzator::instance = nullptr;

int main() {
    // Obținem instanța singleton a vânzătorului
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

    switch(tip)
    {
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



    return 0;
}
