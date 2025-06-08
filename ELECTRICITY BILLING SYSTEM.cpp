#include <iostream>
#include <vector>
#include <string>
using namespace std;

// ===== Appliance Class =====
class Appliance {
public:
    string name;
    int hoursPerDay;
    int powerWatts;

    Appliance(string n, int h, int p) : name(n), hoursPerDay(h), powerWatts(p) {}

    double monthlyUnits() const {
        return (hoursPerDay * powerWatts * 30) / 1000.0; // kWh
    }
};

// ===== BillCalculator Class =====
class BillCalculator {
public:
    static double calculateBill(double units, int userType) {
        // Residential slab rates
        const double slab1_rate = 10.0;    // PKR per kWh for first 100 units
        const double slab2_rate = 15.0;    // PKR per kWh for 101-300 units
        const double slab3_rate = 20.0;    // PKR per kWh for above 300 units
        
        // Commercial slab rates (different from residential)
        const double commercial_slab1_rate = 12.0;
        const double commercial_slab2_rate = 18.0;
        const double commercial_slab3_rate = 25.0;
        
        double bill = 0.0;
        
        if (userType == 1) { // Residential
            if (units <= 100) {
                bill = units * slab1_rate;
            } 
            else if (units <= 300) {
                bill = 100 * slab1_rate + (units - 100) * slab2_rate;
            } 
            else {
                bill = 100 * slab1_rate + 200 * slab2_rate + (units - 300) * slab3_rate;
            }
        } 
        else { // Commercial
            if (units <= 100) {
                bill = units * commercial_slab1_rate;
            } 
            else if (units <= 300) {
                bill = 100 * commercial_slab1_rate + (units - 100) * commercial_slab2_rate;
            } 
            else {
                bill = 100 * commercial_slab1_rate + 200 * commercial_slab2_rate + (units - 300) * commercial_slab3_rate;
            }
        }
        
        return bill;
    }
};

// ===== SuggestionEngine Class =====
class SuggestionEngine {
public:
    static void suggestReduction(const vector<Appliance>& appliances, double currentUnits, double targetReduction) {
        cout << "\n--- Suggestions to Reduce Your Bill ---\n";
        double reduced = 0;
        for (int i = 0; i < appliances.size(); ++i) {
            double usage = appliances[i].monthlyUnits();
            if (usage > 5 && reduced < targetReduction) {
                cout << "- Try using the " << appliances[i].name << " for 1 hour less per day. ";
                double saved = (appliances[i].powerWatts * 30) / 1000.0;
                cout << "(Saves about " << saved << " units/month)\n";
                reduced += saved;
            }
        }
        if (reduced < targetReduction) {
            cout << "- Also consider replacing old appliances with energy-efficient ones like LED bulbs.\n";
        }
    }
};

class SolarUser {
private:
    bool hasSolarPanel;
    double SolarGeneration;
    double totalConsumption;
    const double ratePerKWh = 12.0;

public:
    void userDetails() {
        cout << "Do You Have Solar Panels Installed? (1 = Yes / 0 = No): ";
        cin >> hasSolarPanel;
        cout << "Enter Your Total Monthly Energy Consumption (kWh): ";
        cin >> totalConsumption;

        if (hasSolarPanel) {
            cout << "Enter Your Average Monthly Solar Energy Generation (kWh): ";
            cin >> SolarGeneration;
        } else {
            SolarGeneration = 0;
        }
    }

    double getNetUnits() {
        return totalConsumption - SolarGeneration;
    }

    void displaySolarSummary() {
        cout << "\n---- Solar Energy Summary ----\n";
        cout << "Total Consumption: " << totalConsumption << " kWh\n";
        cout << "Solar Generation: " << SolarGeneration << " kWh\n";

        double profitKWh = 0;
        double billKWh = 0;

        if (SolarGeneration > totalConsumption) {
            profitKWh = SolarGeneration - totalConsumption;
            cout << "Surplus Energy: " << profitKWh << " kWh can be sold to the grid\n";
            cout << "Profit: " << (profitKWh * ratePerKWh) << " PKR earned\n";
        } else {
            billKWh = totalConsumption - SolarGeneration;
            cout << "Net Billable Energy: " << billKWh << " kWh\n";
            cout << "Total Bill: " << (billKWh * ratePerKWh) << " PKR\n";
        }

        suggestOptimization();
    }

    void suggestOptimization() const {
        cout << "\n--- Optimization Suggestions ---\n";
        if (!hasSolarPanel) {
            cout << "Consider installing solar panels to reduce your electricity bills.\n";
        } else if (SolarGeneration < totalConsumption) {
            cout << "You can optimize energy use or increase your solar generation.\n";
        } else {
            cout << "Excellent! You've achieved zero billing or generated extra income.\n";
        }
    }

    double getAdjustedUnits() {
        return getNetUnits();
    }
};

class Residential {
private:
    SolarUser solarUser;
    vector<Appliance> appliances;
    
public:
    void run() {
        cout << "\n--- Residential User ---\n";
        cout << "\n--- Solar Panel Section ---\n";
        solarUser.userDetails();
        solarUser.displaySolarSummary();

        cout << "\n--- Appliance Entry Section ---\n";
        int count;
        cout << "How many appliances do you want to enter? ";
        cin >> count;

        for (int i = 0; i < count; ++i) {
            string name;
            int hours, watts;
            cout << "Appliance #" << (i + 1) << " name: ";
            cin >> name;
            cout << "Hours per day: ";
            cin >> hours;
            cout << "Power (Watts): ";
            cin >> watts;
            appliances.push_back(Appliance(name, hours, watts));
        }

        double totalUnits = 0;
        for (int i = 0; i < appliances.size(); ++i) {
            totalUnits += appliances[i].monthlyUnits();
        }

        cout << "\n--- Appliance Usage Summary ---\n";
        cout << "Total Units by Appliances: " << totalUnits << " kWh\n";

        double adjustedUnits = solarUser.getAdjustedUnits();
        if (adjustedUnits < 0) adjustedUnits = 0;

        double bill = BillCalculator::calculateBill(adjustedUnits, 1); // 1 for residential
        cout << "Estimated Electricity Bill (based on appliance use - solar): Rs. " << bill << "\n";

        double target;
        cout << "\nEnter how many units you want to reduce per month: ";
        cin >> target;
        SuggestionEngine::suggestReduction(appliances, totalUnits, target);
    }
};

class Commercial {
private:
    SolarUser solarUser;
    vector<Appliance> appliances;
    
public:
    void run() {
        cout << "\n--- Commercial User ---\n";
        cout << "\n--- Solar Panel Section ---\n";
        solarUser.userDetails();
        solarUser.displaySolarSummary();

        cout << "\n--- Appliance Entry Section ---\n";
        int count;
        cout << "How many appliances do you want to enter? ";
        cin >> count;

        for (int i = 0; i < count; ++i) {
            string name;
            int hours, watts;
            cout << "Appliance #" << (i + 1) << " name: ";
            cin >> name;
            cout << "Hours per day: ";
            cin >> hours;
            cout << "Power (Watts): ";
            cin >> watts;
            appliances.push_back(Appliance(name, hours, watts));
        }

        double totalUnits = 0;
        for (int i = 0; i < appliances.size(); ++i) {
            totalUnits += appliances[i].monthlyUnits();
        }

        cout << "\n--- Appliance Usage Summary ---\n";
        cout << "Total Units by Appliances: " << totalUnits << " kWh\n";

        double adjustedUnits = solarUser.getAdjustedUnits();
        if (adjustedUnits < 0) adjustedUnits = 0;

        double bill = BillCalculator::calculateBill(adjustedUnits, 2); // 2 for commercial
        cout << "Estimated Electricity Bill (based on appliance use - solar): Rs. " << bill << "\n";

        double target;
        cout << "\nEnter how many units you want to reduce per month: ";
        cin >> target;
        SuggestionEngine::suggestReduction(appliances, totalUnits, target);
    }
};

int main() {
    int userType;
    cout << "Select User Type:\n";
    cout << "1. Residential\n";
    cout << "2. Commercial\n";
    cout << "Enter choice: ";
    cin >> userType;

    if (userType == 1) {
        Residential residential;
        residential.run();
    } else if (userType == 2) {
        Commercial commercial;
        commercial.run();
    } else {
        cout << "Invalid choice!\n";
    }

    return 0;
}
