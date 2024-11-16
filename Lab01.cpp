// Lab01.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <random>
using namespace std;

struct GameData {
    int Round;
    int People;
    int Pshenitsa;
    int Land;
    int LandPrice;
    int End = 0;
};

struct UserData {
    int BuyLand;
    int SellLand;
    int Eaten;
    int Seaded;
};

struct UserStat {
    double P = 0;
    double L = 0;
};

void Print(int _round, 
    int _deadPeople, int _newPeople, bool _plague, int _countPeople, 
    int _sobrPsh, int _akrPsh, int _ratPsh, int _countPsh,
    int _land, int _priceLand) {
    cout << "Мой повелитель, соизволь поведать тебе" << endl;
    cout << "\tНа " << _round << " году твоего правления" << endl;

    if (_deadPeople > 0)
        cout << "\t" << _deadPeople << " человек умерли с голоду" << endl;

    if (_newPeople > 0)
        cout << "\t" << _newPeople << " человек прибыло в наш великий город" << endl;

    if (_plague)
        cout << "\tЧума уничтожила половину населения" << endl;
    else
        cout << "\tЧумы не было" << endl;

    cout << "\tНаселение города сейчас составляет " << _countPeople << " человек" << endl;

    cout << "\tМы собрали " << _sobrPsh << " бушелей пшеницы, по " << _akrPsh << " бушеля с акра" << endl;
    cout << "\tКрысы истребили " << _ratPsh << " бушелей пшеницы, оставив " << _countPsh << " бушеля в амбарах" << endl;

    cout << "\tГород сейчас занимает " << _land << " акров" << endl;
    cout << "\tАкр земли сейчас стоит " << _priceLand << " бушель" << endl << endl;
}


void End(GameData& GD, UserStat& US) {
    GD.End = 1;
    US.P /= GD.Round;
    US.L /= GD.Round;
    cout << "\nМой повелитель, твое правление закончено" << endl;
    cout << "\tСреднегодовой процент умерших от голода: " << US.P << endl;
    cout << "\tКоличество акров земли на одного жителя: " << US.L << endl;

    if (US.P > 0.33 && US.L < 7) {
        cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.Теперь вы вынуждены влачить жалкое существование в изгнании" << endl;
    }
    else if (US.P > 0.1 && US.L < 9) {
        cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем" << endl;
    }
    else if (US.P > 0.03 && US.L < 10) {
        cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова" << endl;
    }
    else {
        cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше" << endl;
    }
}

void Exit(GameData& GD) {
    GD.End = 1;
    ofstream out;          // поток для записи
    out.open("GameData.txt");      // открываем файл для записи
    if (out.is_open())
    {
        out << GD.Round << " " << GD.People << " " << GD.Land << " " << GD.Pshenitsa << std::endl;
    }
    out.close();
    cout << "\nВы вышли из игры\n";
}

int Check(int _people, int _pshenitsa, int _land, int _buyLand, int _sellLand, int _newPshEat, int _newPshSead, int price) {
    if (_buyLand < 0 || _sellLand < 0 || _newPshEat < 0 || _newPshSead < 0) {
        cout << "О великий Хаммурапи! Введи положительное значение свой воли! " << endl;
        return 0;
    }        

    if (price * _buyLand > _pshenitsa) {
        cout << "О великий Хаммурапи! У нас недостаточно пшеницы, чтобы расплатиться за новые земли! " << endl;
        return 0;
    }
    _land += _buyLand;
    _pshenitsa -= _buyLand * price;

    if (_sellLand > _land) {
        cout << "О великий Хаммурапи! У нас недостаточно земли, чтобы продать столько! " << endl;
        return 0;
    }
    _land -= _sellLand;
    _pshenitsa += _sellLand * price;

    _pshenitsa -= _newPshEat;
    _pshenitsa -= _newPshSead / 2;
    if (_pshenitsa < 0) {
        cout << "О повелитель, пощади нас! У нас только " << _people << " человек, " << _pshenitsa << " бушелей пшеницы и " << _land << " акров земли!" << endl;
        return 0;
    }

    if (_people * 10 < _newPshSead) {
        cout << "О повелитель, пощади нас! У нас только " << _people << " человек, " << _pshenitsa << " бушелей пшеницы и " << _land << " акров земли!" << endl;
        return 0;
    }
    if (_newPshSead > _land) {
        cout << "О повелитель, пощади нас! У нас только " << _people << " человек, " << _pshenitsa << " бушелей пшеницы и " << _land << " акров земли!" << endl;
        return 0;
    }

    return 1;
}

void GetFromPlayer(GameData& GD, UserData& UD) {
    cout << "Что пожелаешь, повелитель?" << endl;
    int _isCorrect = 0;

    while (!_isCorrect) {
        cout << "\tСколько акров земли повелеваешь купить? ";
        cin >> UD.BuyLand;

        char c = getchar();
        if (c == 'e') {
            Exit(GD);
            return;
        }

        cout << "\tСколько акров земли повелеваешь продать? ";
        cin >> UD.SellLand;

        c = getchar();
        if (c == 'e') {
            Exit(GD);
            return;
        }

        cout << "\tСколько бушелей пшеницы повелеваешь съесть? ";
        cin >> UD.Eaten;

        c = getchar();
        if (c == 'e') {
            Exit(GD);
            return;
        }

        cout << "\tСколько акров земли повелеваешь засеять? ";
        cin >> UD.Seaded;

        c = getchar();
        if (c == 'e') {
            Exit(GD);
            return;
        }

        _isCorrect = Check(GD.People, GD.Pshenitsa, GD.Land, UD.BuyLand, UD.SellLand, UD.Eaten, UD.Seaded, GD.LandPrice);
    }
    GD.Land += UD.BuyLand;
    GD.Pshenitsa -= UD.BuyLand * GD.LandPrice;

    GD.Land -= UD.SellLand;
    GD.Pshenitsa += UD.SellLand * GD.LandPrice;

    GD.Pshenitsa -= UD.Eaten;
    GD.Pshenitsa -= UD.Seaded / 2;
    cout << endl;
}

void NewRound(GameData& GD, UserData& UD, UserStat& US) {
    random_device rd;
    mt19937 gen(rd());

    GD.Round++;

    int _dead = GD.People - UD.Eaten / 20;
    US.P += (double)_dead / GD.People;

    if ((double)_dead / GD.People > 0.45) {
        End(GD, US);
        return;
    }

    GD.People -= _dead;

    int _plague = gen() % 100 < 15 ? 1 : 0;

    int _pshenitsaPerAcr = gen() % 6 + 1;
    int _pshenitsaSobr = _pshenitsaPerAcr * UD.Seaded;
    GD.Pshenitsa += _pshenitsaSobr;

    int _newPeople = _dead / 2 + (5 - _pshenitsaPerAcr) * GD.Pshenitsa / 600 + 1;
    _newPeople = _newPeople < 0 ? 0 : (_newPeople > 50 ? 50 : _newPeople);
    GD.People += _newPeople;

    if (_plague)
        GD.People /= 2;

    US.L += (double)GD.Land / GD.People;

    if (GD.People <= 0) {
        End(GD, US);
        return;
    }

    double _rat = ((double)(gen() % 700)) / 10000;
    int _ratEat = _rat * GD.Pshenitsa;

    GD.Pshenitsa -= _ratEat;

    GD.LandPrice = gen() % 10 + 17;

    Print(GD.Round, _dead, _newPeople, _plague, GD.People, _pshenitsaSobr, _pshenitsaPerAcr, _ratEat, GD.Pshenitsa, GD.Land, GD.LandPrice);

    if (GD.Round >= 10) {
        End(GD, US);
        return;
    }
}

void Start(GameData& GD) {
    random_device rd;
    mt19937 gen(rd());

    ifstream in("GameData.txt");
    if (in.is_open()) {
        in >> GD.Round;

        int answer;
                
        if (GD.Round <= 0 || GD.Round >= 10)
            answer = 0;
        else {
            cout << "Согласен ли ты, о великий повелитель, продолжить свое предыдущее правление? (1 - да, 0 - нет) ";
            cin >> answer;
        }

        if (answer) {
            cout << "С возвращением, великий повелитель!" << endl;
            in >> GD.People >> GD.Land >> GD.Pshenitsa;
            in.close();
            GD.LandPrice = gen() % 10 + 17;
        }
        else {
            in.close();
            remove("GameData.txt");

            GD.Round = 0;
            GD.People = 100;
            GD.Pshenitsa = 2800;
            GD.Land = 1000;
            GD.LandPrice = gen() % 10 + 17;
            cout << "Твое правление началось, о великий повелитель!" << endl;
        }
    }
    else {
        cout << "Твое правление началось, о великий повелитель!" << endl;
        GD.Round = 0;
        GD.People = 100;
        GD.Pshenitsa = 2800;
        GD.Land = 1000;
        GD.LandPrice = gen() % 10 + 17;
    }    
    cout << "\tНаселение города составляет " << GD.People << " человек" << endl;
    cout << "\tВ амбарах лежит " << GD.Pshenitsa << " бушеля пшеницы" << endl;
    cout << "\tГород занимает " << GD.Land << " акров" << endl;    
    cout << "\tАкр земли стоит " << GD.LandPrice << " бушель" << endl << endl;

}

void Play() {
    GameData GD;
    UserData UD;
    UserStat US;

    Start(GD);

    while (!GD.End) {
        GetFromPlayer(GD, UD);
        if (GD.End)
            return;
        NewRound(GD, UD, US);
    }
    if (GD.Round >= 10)
        remove("GameData.txt");
}

int main()
{
    setlocale(LC_ALL, "ru");    
    Play();
}
