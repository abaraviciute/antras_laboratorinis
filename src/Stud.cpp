﻿/**
 * @file
 * @brief Failas su funkcijomis.
 * Faile implementuotas "Stud.h" header failas, kuriame yra funkcijų deklaracijos ir reikalingos klasės.
 */

#include "Stud.h"

/**
 * @brief Įveda studento duomenis rankinio įvedimo atveju.
 *
 * Funkcija leidžia įvesti studento vardą, pavardę, namų darbų ir egzamino įvertinimus.
 * Jei pasirinktas automatinis duomenų generavimas (generavimas == true), tada funkcija sugeneruoja 4 
 * atsitiktinius namų darbų ir egzamino pažymius. Jei generavimas yra false, funkcija leidžia vartotojui
 * įvesti duomenis rankiniu būdu.
 *
 * @param generavimas Jei  `true`, sugeneruojami atsitiktiniai namų darbų ir egzamino pažymiai.
 *                    Jei `false`, prašoma vartotojo įvesti duomenis rankiniu būdu.
 */
void Studentas::ivestis(bool generavimas)
{
    if (generavimas) {
        random_device rd;
        mt19937 mt(rd());
        uniform_int_distribution<int> dist(1, 10);

        cout << "Iveskite varda: ";
        cin >> vardas_;

        cout << "Iveskite pavarde: ";
        cin >> pavarde_;

        int ndSkaicius = 5;
        nd_.clear();
        for (int i = 0; i < ndSkaicius; i++) {
            nd_.push_back(dist(mt));
        }
        egzaminas_ = dist(mt);
    }
    else {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Iveskite varda, pavarde, namu darbu pazymius, ir egzamino pazymi (atskirtus tarpu): " << endl;
        cin >> *this;
    }
}


/**
 * @brief Apskaičiuoja galutinį studento pažymį pagal pasirinktą rodiklį.
 *
 * Rodikliai: vidurkis ("Vid") ir mediana ("Med"). Abu rodikliai skaičiuojami pagal namų darbus.
 *
 * @param pasirinkimas Pasirinktas metodas pažymio apskaičiavimui:
 *        - "Vid" – vidurkis.
 *        - "Med" – mediana.
 *
 * @return Apskaičiuotas galutinis pažymys, atsižvelgiant į pasirinkimą:
 *         - 0.4 * namų darbų vidurkis + 0.6 * egzaminas (jei pasirinktas "Vid").
 *         - 0.4 * namų darbų medianą + 0.6 * egzaminas (jei pasirinktas "Med").
 *         - 0, jei nėra namų darbų ir egzaminas neįvestas.
 */
double Studentas::rezultatai(const string& pasirinkimas)
{
    if (nd_.empty() && egzaminas_ == 0) {
        return 0.0; 
    }

    if (pasirinkimas == "Vid") {
        double vidurkis = 0.0;

        for (int i = 0; i < nd_.size(); i++) {
            vidurkis += nd_[i];
        }
        vidurkis /= nd_.size();

        return 0.4 * vidurkis + 0.6 * egzaminas_;
    }

    if (pasirinkimas == "Med") {
        double mediana;
        vector<int> pazymiai = nd_;
        sort(pazymiai.begin(), pazymiai.end());

        if (pazymiai.size() % 2 == 0) {
            mediana = (pazymiai[pazymiai.size() / 2 - 1] + pazymiai[pazymiai.size() / 2]) / 2.0;
        }
        else {
            mediana = pazymiai[pazymiai.size() / 2];
        }

        return 0.4 * mediana + 0.6 * egzaminas_;
    }

    return 0;
}


/**
 * @brief Išveda studento duomenis į ekraną.
 * Priklausomai nuo duomenų įvesties, išvedami duomenys apie studentą su arba be objekto saugojimo adreso.
 *
 * @param Lok Studentas, kurio duomenys bus išvedami.
 * @param ivestiesPasirinkimas Pasirinkimas, nurodantis, kokius duomenis išvesti:
 *        - 1: Išveda studento duomenis ir objekto saugojimo adresą.
 *        - 2: Išveda tik studento duomenis.
 */
void isvestis(const Studentas& Lok, int ivestiesPasirinkimas) {
    if (ivestiesPasirinkimas == 1) {
        cout << Lok << &Lok << endl;
    }
    else if (ivestiesPasirinkimas == 2) {
        cout << Lok << endl;
    }
    
}


/**
 * @brief Nuskaito duomenis iš .txt failo į struktūrą (vector/list).
 *
 * @param failas Failo pavadinimas, iš kurio bus nuskaitomi duomenys.
 * @param struktura Struktūra, į kurią bus įrašyti duomenys (vector/list).
 * 
 * @return Grąžina 0, jei duomenys sėkmingai įrašyti į struktūrą, arba -1 kitu atveju.
 */
template <typename Struktura>
int ivestisIsFailo(const string& failas, Struktura& struktura)
{
    ifstream inFile(failas);

    if (!inFile) {
        cout << "Nepavyko atidaryti failo: " << failas << endl;
        return -1;
    }

    string line;
    getline(inFile, line);

    Studentas temp;
    while (inFile >> temp) {
        struktura.push_back(temp);
    }

    inFile.close();
    return 0;
}


/**
 * @brief Lygina du Studentas objektus pagal galutinį pažymį ir pavardę.
 *
 * Funkcija palygina du klasės Studentas objektus pagal jų galutinį pažymį. Jei pažymiai vienodi, tuomet lyginamos pavardės
 * pagal abėcėlę. Palyginimo sąlygą galima nustatyti pagal kintamąjį
 * `rikiavimoSalyga` (pažymiai bus rikiuojami didėjimo/mažėjimo tvarka).
 *
 * @param pavarde1 Pirmas Studentas objektas, su kuriuo lyginamas antras.
 * @param pavarde2 Antras Studentas objektas, su kuriuo lyginamas pirmas.
 * @return `true`, jei pirmo objekto pažymys yra mažesnis/didesnis (pagal nustatytą rikiavimo sąlygą);
 *         `false`, jei pirmo objekto pažymys nėra mažesnis/didesnis (pagal nustatytą rikiavimo sąlygą).
 */
bool rusiavimas(const Studentas& pavarde1, const Studentas& pavarde2)
{
    double pazymys1 = pavarde1.galutinisPazymysVid();
    double pazymys2 = pavarde2.galutinisPazymysVid();

    if (rikiavimoSalyga == 1) {
        return (pazymys1 == pazymys2) ? (pavarde1.pavarde_ < pavarde2.pavarde_) : (pazymys1 < pazymys2);
    }
    else if (rikiavimoSalyga == 2) {
        return (pazymys1 == pazymys2) ? (pavarde1.pavarde_ < pavarde2.pavarde_) : (pazymys1 > pazymys2);
    }
    else {
        return pavarde1.pavarde_ < pavarde2.pavarde_;
    }
}


/**
 * @brief Generuoja studentų duomenų failą su atsitiktiniais namų darbų ir egzamino įvertinimais.
 *
 * Ši funkcija sukuria failą su studentų vardais, pavardėmis, atsitiktiniais 5 namų darbų įvertinimais ir egzamino įvertinimu.
 *
 * @param studentuSkaicius Kiek studentų bus generuojama.
 * @param failoPavadinimas Failo pavadinimas, kuriame bus saugomi sugeneruotų studentų duomenys.
 *
 * @throws runtime_error Jei failo nepavyksta sukurti.
 */
void generuotiFaila(int studentuSkaicius, const string& failoPavadinimas)
{
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1, 10);

    ofstream out(failoPavadinimas);
    if (!out) {
        throw runtime_error("Nepavyko sukurti failo.");
    }

    auto pradzia = high_resolution_clock::now();

    out << left << setw(15) << "Vardas"
        << left << setw(15) << "Pavarde"
        << right << setw(5) << "ND1"
        << right << setw(5) << "ND2"
        << right << setw(5) << "ND3"
        << right << setw(5) << "ND4"
        << right << setw(5) << "ND5"
        << right << setw(5) << "Egz." << '\n';

    ostringstream studentoDuomenys;

    for (int i = 0; i < studentuSkaicius; i++) {
        studentoDuomenys << left << setw(15) << "Vardas" + to_string(i + 1)
            << left << setw(15) << "Pavarde" + to_string(i + 1);

        for (int j = 0; j < 6; j++) {
            studentoDuomenys << right << setw(5) << dist(mt);
        }

        studentoDuomenys << '\n';

        if ((i + 1) % 100 == 0 || i + 1 == studentuSkaicius) {
            out << studentoDuomenys.str();
            studentoDuomenys.str("");
            studentoDuomenys.clear();
        }
    }

    out.close();

    auto pabaiga = high_resolution_clock::now();
    duration<double> trukme = pabaiga - pradzia;
    cout << "Failas " << failoPavadinimas << " sugeneruotas. Failo kurimo trukme: " << trukme.count() << "\n";
}


/**
 * @brief Padalina studentus į du konteinerius: 'galvociai' (galutinis įvertinimas >= 5) ir 'nuskriaustukai' (galutinis įvertinimas < 5).
 *
 * Funkcija padalina studentus į du konteinerius (vector/list) pagal jų galutinį pažymį (vidurkinį arba medianinį), priklausomai nuo įvesto parametro.
 *
 * @param struktura Studentų konteineris, kurį reikia apdoroti.
 * @param duomenuIvedimoBudas Duomenų įvedimo metodas, nustatantis, ar turi būti naudojamas "Vid" ar "Med" įvertinimas.
 * @param galvociai Konteineris, kuriame bus laikomi geriausi studentai.
 * @param nuskriaustukai Konteineris, kuriame bus laikomi blogiausi studentai.
 */
template <typename Struktura>
void studentoKategorija1(const Struktura& struktura, int duomenuIvedimoBudas, Struktura& galvociai, Struktura& nuskriaustukai)
{
    for (const auto& studentas : struktura) {
        if ((duomenuIvedimoBudas == 2 && studentas.galutinisPazymysVid() >= 5) ||
            (duomenuIvedimoBudas != 2 && studentas.galutinisPazymys() >= 5)) {
            galvociai.push_back(studentas);
        }
        else {
            nuskriaustukai.push_back(studentas);
        }
    }
}


/**
 * @brief Rūšiuoja studentus pagal jų pažymius ir perkelia blogus studentus į 'nuskriaustukai' konteinerį (vector/list).
 *
 * Funkcija rūšiuoja studentus pagal jų galutinį pažymį (vidurkinį arba medianinį), priklausomai nuo įvesto parametro.
 * Po rūšiavimo studentai su mažesniu nei 5 pažymiu perkeliami į 'nuskriaustukai' konteinerį ir pašalinami iš bendro 'struktura' konteinerio.
 *
 * @param struktura Studentų konteineris, kurį reikia apdoroti.
 * @param duomenuIvedimoBudas Duomenų įvedimo metodas, nustatantis, ar turi būti naudojamas "Vid" ar "Med" įvertinimas.
 * @param nuskriaustukai Konteineris, kuriame bus laikomi blogiausi studentai.
 */
template <typename Struktura>
void studentoKategorija2(Struktura& struktura, int duomenuIvedimoBudas, Struktura& nuskriaustukai)
{
    if constexpr (is_same_v<Struktura, vector<typename Struktura::value_type>>) {

        sort(struktura.begin(), struktura.end(),
            [duomenuIvedimoBudas](const auto& a, const auto& b) {
                if (duomenuIvedimoBudas == 2) {
                    return a.galutinisPazymysVid() > b.galutinisPazymysVid();
                }
                else {
                    return a.galutinisPazymys() > b.galutinisPazymys();
                }
            });
    }
    else if constexpr (is_same_v<Struktura, list<typename Struktura::value_type>>) {

        struktura.sort(
            [duomenuIvedimoBudas](const auto& a, const auto& b) {
                if (duomenuIvedimoBudas == 2) {
                    return a.galutinisPazymysVid() > b.galutinisPazymysVid();
                }
                else {
                    return a.galutinisPazymys() > b.galutinisPazymys();
                }
            });
    }

    while (!struktura.empty() &&
        ((duomenuIvedimoBudas == 2 && struktura.back().galutinisPazymysVid() < 5) ||
            (duomenuIvedimoBudas != 2 && struktura.back().galutinisPazymys() < 5))) {
        nuskriaustukai.push_back(struktura.back());
        struktura.pop_back();
    }
}


/**
 * @brief Padalina studentus į dvi kategorijas pagal studentoKategorija2() funkciją, naudojant partition metodą.
 *
 * Funkcija padalina studentus į dvi grupes: tuos, kurie atitinka galutinio įvertinimo sąlygą (>= 5) ir tuos, kurie ne.
 * Blogi studentai (su pažymiu < 5) perkeliami į 'nuskriaustukai' konteinerį ir ištrinami iš bendro 'struktura' konteinerio.
 *
 * @param struktura Studentų konteineris, kurį reikia apdoroti.
 * @param duomenuIvedimoBudas Duomenų įvedimo metodas, nustatantis, ar turi būti naudojamas "Vid" ar "Med" įvertinimas.
 * @param nuskriaustukai Konteineris, kuriame bus laikomi blogiausi studentai.
 */
template <typename Struktura>
void studentoKategorija3(Struktura& struktura, int duomenuIvedimoBudas, Struktura& nuskriaustukai)
{
    function<bool(const typename Struktura::value_type&)> condition;

    if (duomenuIvedimoBudas == 2) {
        condition = [](const auto& studentas) { return studentas.galutinisPazymysVid() >= 5; };
    }
    else {
        condition = [](const auto& studentas) { return studentas.galutinisPazymys() >= 5; };
    }

    auto it = partition(struktura.begin(), struktura.end(), condition);

    nuskriaustukai.insert(nuskriaustukai.end(), it, struktura.end());

    struktura.erase(it, struktura.end());
}


/**
 * @brief studentoKategorija3() funkcija, pritaikyta išskirtinai tik vector tipo konteineriams.
 * 
 * @param struktura Studentų vektorius, kurį reikia apdoroti.
 * @param duomenuIvedimoBudas Duomenų įvedimo metodas, nustatantis, ar turi būti naudojamas "Vid" ar "Med" įvertinimas.
 * @param nuskriaustukai vektorius, kuriame bus laikomi blogiausi studentai.
 */
void studentoKategorijaVector(vector<Studentas>& struktura, int duomenuIvedimoBudas, vector<Studentas>& nuskriaustukai)
{
    function<bool(const Studentas&)> condition;

    if (duomenuIvedimoBudas == 2) {
        condition = [](const Studentas& studentas) { return studentas.galutinisPazymysVid() < 5; };
    }
    else {
        condition = [](const Studentas& studentas) { return studentas.galutinisPazymys() < 5; };
    }

    auto it = partition(struktura.begin(), struktura.end(), [condition](const Studentas& studentas) {
        return !condition(studentas);
        });

    nuskriaustukai.insert(nuskriaustukai.end(), it, struktura.end());

    struktura.erase(it, struktura.end());
}


/**
 * @brief Įrašo studentų duomenis į failą.
 *
 * Funkcija išveda studentų duomenis į nurodytą failą (galvociai.txt/nuskriaustukai.txt).
 * Funkcija taip pat matuoja duomenų įrašymo į failą trukmę.
 *
 * @param failas Failo pavadinimas, į kurį bus įrašyti duomenys.
 * @param studentai Studentų struktūra, kurios duomenis norima įrašyti.
 * @param duomenuIvedimoBudas Nurodo, kaip bus išvedami duoemys (priklausomai jei buvo įvesti ranka ar nuskaityti iš failo).
 * @param pazymioTipas Galutinio pažymio tipas ("Vid."/"Med.").
 * @param trukme Laikas, per kurį buvo atliktas įrašymas į failą.
 *
 * @throw runtime_error Jei failo nepavyko sukurti.
 */
template <typename Struktura>
void iFaila(const string& failas, const Struktura& studentai, int duomenuIvedimoBudas, const string& pazymioTipas, duration<double>& trukme) {
    ofstream ived(failas);
    if (!ived) {
        throw runtime_error("Nepavyko sukurti failo: " + failas);
    }

    auto pradzia = high_resolution_clock::now();

    if (duomenuIvedimoBudas == 2) {
        ived << setw(15) << left << "Pavarde" << setw(15) << left << "Vardas"
            << setw(20) << left << "Galutinis (Vid.)" << setw(10) << left << "Galutinis (Med.)" << endl;
        ived << "-------------------------------------------------------------" << endl;
    }
    else {
        ived << setw(15) << left << "Pavarde" << setw(15) << left << "Vardas"
            << setw(3) << left << "Galutinis (" << pazymioTipas << ".)" << endl;
        ived << "-------------------------------------------------" << endl;
    }

    for (const auto& studentas : studentai) {
        ived << studentas << endl;
    }

    auto pabaiga = high_resolution_clock::now();
    trukme = pabaiga - pradzia;
}


/**
 * @brief Pagalbinė duomenų išvedimo į failus (galvociai.txt/nuskriaustukai.txt) funkcija.
 *
 * @param galvociai Konteineris studentų, kurių galutiniai pažymiai yra >= 5.
 * @param nuskriaustukai Konteineris studentų, kurių galutiniai pažymiai yra < 5.
 * @param duomenuIvedimoBudas Nurodo, kaip bus išvedami duoemys (priklausomai jei buvo įvesti ranka ar nuskaityti iš failo).
 * @param pazymioTipas Galutinio pažymio tipas ("Vid."/"Med.").
 * @param trukmeGalvociu Laikas, per kurį duomenys buvo įrašyti į "galvociai.txt" failą.
 * @param trukmeNuskriaustuku Laikas, per kurį duomenys buvo įrašyti į "nuskriaustukai.txt" failą.
 */
template <typename Struktura>
void isvestisIFaila(const Struktura& galvociai, const Struktura& nuskriaustukai, int duomenuIvedimoBudas, string pazymioTipas, duration<double>& trukmeGalvociu, duration<double>& trukmeNuskriaustuku) {
    iFaila("galvociai.txt", galvociai, duomenuIvedimoBudas, pazymioTipas, trukmeGalvociu);

    iFaila("nuskriaustukai.txt", nuskriaustukai, duomenuIvedimoBudas, pazymioTipas, trukmeNuskriaustuku);

    cout << "Failai \"galvociai.txt\" ir \"nuskriaustukai.txt\" sugeneruoti." << endl;
}

template int ivestisIsFailo<vector<Studentas>>(const string& failas, vector<Studentas>& struktura);
template int ivestisIsFailo<list<Studentas>>(const string& failas, list<Studentas>& struktura);
template void studentoKategorija1<vector<Studentas>>(const vector<Studentas> &struktura, int duomenuIvedimoBudas, vector<Studentas>& galvociai, vector<Studentas>& nuskriaustukai);
template void studentoKategorija1<list<Studentas>>(const list<Studentas>& struktura, int duomenuIvedimoBudas, list<Studentas>& galvociai, list<Studentas>& nuskriaustukai);
template void studentoKategorija2<vector<Studentas>>(vector<Studentas>& struktura, int duomenuIvedimoBudas, vector<Studentas>& nuskriaustukai);
template void studentoKategorija2<list<Studentas>>(list<Studentas>& struktura, int duomenuIvedimoBudas, list<Studentas>& nuskriaustukai);
template void studentoKategorija3<vector<Studentas>>(vector<Studentas>& struktura, int duomenuIvedimoBudas, vector<Studentas>& nuskriaustukai);
template void studentoKategorija3<list<Studentas>>(list<Studentas>& struktura, int duomenuIvedimoBudas, list<Studentas>& nuskriaustukai);
template void isvestisIFaila<vector<Studentas>>(const vector<Studentas>& galvociai, const vector<Studentas>& nuskriaustukai, int duomenuIvedimoBudas, string pazymioTipas, duration<double>& trukmeGalvociu, duration<double>& trukmeNuskriaustuku);
template void isvestisIFaila<list<Studentas>>(const list<Studentas>& galvociai, const list<Studentas>& nuskriaustukai, int duomenuIvedimoBudas, string pazymioTipas, duration<double>& trukmeGalvociu, duration<double>& trukmeNuskriaustuku);

