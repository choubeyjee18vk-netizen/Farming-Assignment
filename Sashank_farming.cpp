#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

using namespace std;

// Abstract Class Advisory
class Advisory {
public:
    virtual void giveAdvice() const = 0;
    virtual ~Advisory() {}
};

// Base Class Crop
class Crop : public Advisory {
protected:
    string name;
    double area; // in acres
    string season;
    double yieldPerAcre; // in tons per acre
    static int cropCount;

public:
    // Constructor Overloading: Default Constructor
    Crop() : name("Unknown"), area(0.0), season("Unknown"), yieldPerAcre(0.0) {
        cropCount++;
    }

    // Parameterized Constructor with Exception Handling
    Crop(string n, double a, string s, double y) {
        if (a <= 0 || y < 0) {
            throw invalid_argument("Exception: Area must be greater than 0 and yield cannot be negative!");
        }
        name = n;
        area = a;
        season = s;
        yieldPerAcre = y;
        cropCount++;
    }

    // Virtual Destructor
    virtual ~Crop() {
        cropCount--;
    }

    // Static Member Function
    static int getCropCount() {
        return cropCount;
    }

    // Runtime Polymorphism: Virtual Yield Calculation
    virtual double calculateTotalYield() const {
        return area * yieldPerAcre;
    }

    virtual void display() const {
        cout << left << setw(15) << name 
             << setw(10) << area 
             << setw(12) << season 
             << setw(15) << yieldPerAcre 
             << setw(15) << calculateTotalYield() << endl;
    }

    // Operator Overloading (> operator to compare yields)
    bool operator>(const Crop& other) const {
        return this->calculateTotalYield() > other.calculateTotalYield();
    }

    virtual void saveData(ofstream& outFile) const {
        outFile << name << "," << area << "," << season << "," << yieldPerAcre << ",";
    }

    string getName() const { return name; }
};

int Crop::cropCount = 0;

// Derived Class 1: CerealCrop
class CerealCrop : public Crop {
private:
    string fertilizerType;
    int irrigationFrequency; // in days

public:
    CerealCrop() : Crop(), fertilizerType("None"), irrigationFrequency(0) {}

    CerealCrop(string n, double a, string s, double y, string f, int i) : Crop(n, a, s, y) {
        if (i < 0) throw invalid_argument("Exception: Irrigation frequency cannot be negative!");
        fertilizerType = f;
        irrigationFrequency = i;
    }

    double calculateTotalYield() const override {
        return area * yieldPerAcre;
    }

    void display() const override {
        cout << "[Cereal]    ";
        Crop::display();
        cout << "            -> Fertilizer: " << fertilizerType << ", Irrigation every " << irrigationFrequency << " days\n";
    }

    void giveAdvice() const override {
        cout << "            -> Tip: Ensure proper nitrogen application and monitor moisture during grain filling.\n";
    }

    void saveData(ofstream& outFile) const override {
        outFile << "CEREAL," << name << "," << area << "," << season << "," << yieldPerAcre << "," << fertilizerType << "," << irrigationFrequency << "\n";
    }
};

// Derived Class 2: VegetableCrop
class VegetableCrop : public Crop {
private:
    string pestControlMethod;
    bool greenhouseGrown;

public:
    VegetableCrop() : Crop(), pestControlMethod("None"), greenhouseGrown(false) {}

    VegetableCrop(string n, double a, string s, double y, string p, bool g) : Crop(n, a, s, y) {
        pestControlMethod = p;
        greenhouseGrown = g;
    }

    double calculateTotalYield() const override {
        // Greenhouse vegetables get a 15% yield boost factor
        return area * yieldPerAcre * (greenhouseGrown ? 1.15 : 1.0);
    }

    void display() const override {
        cout << "[Vegetable] ";
        Crop::display();
        cout << "            -> Pest Control: " << pestControlMethod << ", Greenhouse: " << (greenhouseGrown ? "Yes" : "No") << "\n";
    }

    void giveAdvice() const override {
        cout << "            -> Tip: Maintain steady drip irrigation and watch closely for fungal blights.\n";
    }

    void saveData(ofstream& outFile) const override {
        outFile << "VEGETABLE," << name << "," << area << "," << season << "," << yieldPerAcre << "," << pestControlMethod << "," << greenhouseGrown << "\n";
    }
};

int main() {
    vector<Crop*> farmCrops;
    int choice;

    do {
        cout << "\n============================================\n";
        cout << "   SMART FARM MANAGEMENT SYSTEM (STUDENT 1)\n";
        cout << "============================================\n";
        cout << "1. Add Cereal Crop\n";
        cout << "2. Add Vegetable Crop\n";
        cout << "3. Display All Crops & Advisory\n";
        cout << "4. Compare Two Crops (Operator Overloading)\n";
        cout << "5. Save Data to File\n";
        cout << "6. View Total Crop Count (Static Member)\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input type! Please enter an integer.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        try {
            if (choice == 1) {
                string name, season, fert;
                double area, yield;
                int irrig;
                cout << "Name: "; cin >> name;
                cout << "Area (acres): "; cin >> area;
                cout << "Season: "; cin >> season;
                cout << "Yield per Acre: "; cin >> yield;
                cout << "Fertilizer Type: "; cin >> fert;
                cout << "Irrigation Frequency (days): "; cin >> irrig;

                farmCrops.push_back(new CerealCrop(name, area, season, yield, fert, irrig));
                cout << "Cereal crop added successfully!\n";
            } 
            else if (choice == 2) {
                string name, season, pest;
                double area, yield;
                char gh;
                cout << "Name: "; cin >> name;
                cout << "Area (acres): "; cin >> area;
                cout << "Season: "; cin >> season;
                cout << "Yield per Acre: "; cin >> yield;
                cout << "Pest Control Method: "; cin >> pest;
                cout << "Greenhouse Grown? (y/n): "; cin >> gh;

                bool greenhouse = (gh == 'y' || gh == 'Y');
                farmCrops.push_back(new VegetableCrop(name, area, season, yield, pest, greenhouse));
                cout << "Vegetable crop added successfully!\n";
            }
            else if (choice == 3) {
                if (farmCrops.empty()) {
                    cout << "No crops recorded yet.\n";
                } else {
                    cout << "\n----------------------------------------------------------------------\n";
                    cout << left << setw(12) << "Type" << setw(15) << "Name" << setw(10) << "Area" << setw(12) << "Season" << setw(14) << "Yield/Ac" << setw(12) << "Total" << endl;
                    cout << "----------------------------------------------------------------------\n";
                    for (size_t i = 0; i < farmCrops.size(); ++i) {
                        cout << i + 1 << ". ";
                        farmCrops[i]->display();
                        farmCrops[i]->giveAdvice();
                        cout << "----------------------------------------------------------------------\n";
                    }
                }
            }
            else if (choice == 4) {
                if (farmCrops.size() < 2) {
                    cout << "Add at least 2 crops first.\n";
                } else {
                    int i1, i2;
                    cout << "Enter index of crop 1 (1 to " << farmCrops.size() << "): "; cin >> i1;
                    cout << "Enter index of crop 2 (1 to " << farmCrops.size() << "): "; cin >> i2;
                    if (i1 < 1 || i1 > farmCrops.size() || i2 < 1 || i2 > farmCrops.size()) {
                        cout << "Invalid index range!\n";
                    } else {
                        if (*farmCrops[i1 - 1] > *farmCrops[i2 - 1]) {
                            cout << farmCrops[i1 - 1]->getName() << " yields MORE than " << farmCrops[i2 - 1]->getName() << ".\n";
                        } else {
                            cout << farmCrops[i1 - 1]->getName() << " does NOT yield more than " << farmCrops[i2 - 1]->getName() << ".\n";
                        }
                    }
                }
            }
            else if (choice == 5) {
                ofstream fout("farm_records_1.txt");
                for (const auto& c : farmCrops) {
                    c->saveData(fout);
                }
                fout.close();
                cout << "Data saved to 'farm_records_1.txt' successfully!\n";
            }
            else if (choice == 6) {
                cout << "Total Active Crop Objects: " << Crop::getCropCount() << endl;
            }
        } catch (const exception& ex) {
            cout << ex.what() << endl;
        }

    } while (choice != 7);

    for (auto c : farmCrops) delete c;
    return 0;
}