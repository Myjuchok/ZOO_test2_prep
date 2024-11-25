#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Для округления чисел

using namespace std;

// Базовый класс "Автомобиль"
class Auto {
protected:
    static int nextID; // Автоматическое присвоение ID
    int id;
    string vyrobce;
    double delka; // Длина в метрах

public:
    Auto(const string& vyrobce, double delka)
        : vyrobce(vyrobce), delka(delka) {
        id = nextID++;
    }

    virtual double vypocetMyta() const = 0; // Чисто виртуальный метод

    int getID() const { return id; }
    string getVyrobce() const { return vyrobce; }
    double getDelka() const { return delka; }

    virtual ~Auto() {}
};

// Инициализация статической переменной
int Auto::nextID = 1;

// Класс "Легковой автомобиль"
class OsobniAuto : public Auto {
public:
    OsobniAuto(const string& vyrobce, double delka)
        : Auto(vyrobce, delka) {}

    double vypocetMyta() const override {
        return delka <= 5 ? 100 : 150;
    }
};

// Класс "Грузовой автомобиль"
class NakladniAuto : public Auto {
    double hmotnost; // Масса в тоннах

public:
    NakladniAuto(const string& vyrobce, double delka, double hmotnost)
        : Auto(vyrobce, delka), hmotnost(hmotnost) {}

    double vypocetMyta() const override {
        return 100 * hmotnost;
    }

    double getHmotnost() const { return hmotnost; }
};

// Класс "Эвиденция автомобилей"
class EvidenceSilnicnichVozidel {
    vector<Auto*> vozidla; // Хранение указателей на автомобили

public:
    ~EvidenceSilnicnichVozidel() {
        for (Auto* autoPtr : vozidla) {
            delete autoPtr; // Освобождение памяти
        }
    }

    void pridejVozidlo(const string& typ, const string& vyrobce, double delka, double hmotnost = 0) {
        if (typ == "osobni") {
            vozidla.push_back(new OsobniAuto(vyrobce, delka));
        } else if (typ == "nakladni") {
            vozidla.push_back(new NakladniAuto(vyrobce, delka, hmotnost));
        } else {
            cout << "Neznamy typ vozidla!" << endl;
        }
    }

    vector<Auto*> vratVozidlaVRozsahu(double minDelka, double maxDelka) const {
        vector<Auto*> vysledek;
        for (const Auto* autoPtr : vozidla) {
            if (autoPtr->getDelka() >= minDelka && autoPtr->getDelka() <= maxDelka) {
                vysledek.push_back(const_cast<Auto*>(autoPtr)); // Возвращаем найденные автомобили
            }
        }
        return vysledek;
    }

    double spocitejCelkemVybraneMyto() const {
        double celkem = 0;
        for (const Auto* autoPtr : vozidla) {
            celkem += autoPtr->vypocetMyta();
        }
        return celkem;
    }

    void vypisVozidla() const {
        for (const Auto* autoPtr : vozidla) {
            cout << "ID: " << autoPtr->getID()
                 << ", Vyrobce: " << autoPtr->getVyrobce()
                 << ", Delka: " << autoPtr->getDelka() << " m";
            const NakladniAuto* nakladni = dynamic_cast<const NakladniAuto*>(autoPtr);
            if (nakladni) {
                cout << ", Hmotnost: " << nakladni->getHmotnost() << " t";
            }
            cout << endl;
        }
    }
};

int main() {
    EvidenceSilnicnichVozidel evidence;

    // Добавление автомобилей в эвиденцию
    evidence.pridejVozidlo("osobni", "Skoda", 4.5); // Легковой
    evidence.pridejVozidlo("nakladni", "Volvo", 7.5, 2.5); // Грузовой
    evidence.pridejVozidlo("osobni", "Toyota", 6.0); // Легковой
    evidence.pridejVozidlo("nakladni", "MAN", 10.0, 3.0); // Грузовой

    // Вывод всех автомобилей
    cout << "Seznam vozidel:" << endl;
    evidence.vypisVozidla();

    // Автомобили в заданном диапазоне длины
    double minDelka = 2.0, maxDelka = 6.0;
    cout << "\nVozidla v rozsahu delky (" << minDelka << " - " << maxDelka << " m):" << endl;
    vector<Auto*> filtr = evidence.vratVozidlaVRozsahu(minDelka, maxDelka);
    for (const Auto* autoPtr : filtr) {
        cout << "ID: " << autoPtr->getID() << ", Vyrobce: " << autoPtr->getVyrobce() << endl;
    }

    // Подсчёт общего мыта
    double celkemMyto = evidence.spocitejCelkemVybraneMyto();
    cout << "\nCelkem vybrane myto: " << fixed << setprecision(2) << celkemMyto << " Kc" << endl;

    return 0;
}