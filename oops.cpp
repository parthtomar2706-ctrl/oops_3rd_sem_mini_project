#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// Base Vehicle Class
class Vehicle {
protected:
    string vehicleNumber;
    string model;
    double rentPerDay;
    bool isRented;
    string type;

public:
    Vehicle(string vNum, string mod, double rent, bool rented = false, string t = "Generic")
        : vehicleNumber(vNum), model(mod), rentPerDay(rent), isRented(rented), type(t) {}

    virtual ~Vehicle() {}

    string getVehicleNumber() const { return vehicleNumber; }
    string getModel() const { return model; }
    double getRentPerDay() const { return rentPerDay; }
    bool getIsRented() const { return isRented; }
    string getType() const { return type; }

    void rentVehicle() { isRented = true; }
    void returnVehicle() { isRented = false; }

    virtual void displayDetails() const {
        cout << left << setw(12) << type 
             << setw(15) << vehicleNumber 
             << setw(20) << model 
             << setw(12) << rentPerDay 
             << setw(12) << (isRented ? "Rented" : "Available") << endl;
    }

    virtual double calculateBill(int days) const {
        return rentPerDay * days;
    }
};

// Derived Car Class
class Car : public Vehicle {
private:
    int seatingCapacity;

public:
    Car(string vNum, string mod, double rent, int seats, bool rented = false)
        : Vehicle(vNum, mod, rent, rented, "Car"), seatingCapacity(seats) {}

    int getSeatingCapacity() const { return seatingCapacity; }

    void displayDetails() const override {
        cout << left << setw(12) << type 
             double total = rentPerDay * days;
             // Add luxury surcharge for cars if needed
             return total;
    }
};

// Derived Bike Class
class Bike : public Vehicle {
private:
    bool hasGear;

public:
    Bike(string vNum, string mod, double rent, bool gear, bool rented = false)
        : Vehicle(vNum, mod, rent, rented, "Bike"), hasGear(gear) {}

    bool getHasGear() const { return hasGear; }

    void displayDetails() const override {
        cout << left << setw(12) << type 
             << setw(15) << vehicleNumber 
             << setw(20) << model 
             << setw(12) << rentPerDay 
             << setw(12) << (isRented ? "Rented" : "Available") << endl;
    }
};

// Manager class to handle fleet and file operations
class RentalSystem {
private:
    vector<Vehicle*> vehicles;
    const string filename = "vehicles_data.txt";

public:
    RentalSystem() {
        loadFromFile();
        if (vehicles.empty()) {
            // Default sample data
            vehicles.push_back(new Car("DL01AB1234", "Honda City", 1500.0, 5, false));
            vehicles.push_back(new Bike("MH02XY5678", "Yamaha FZ", 500.0, true, false));
        }
    }

    ~RentalSystem() {
        saveToFile();
        for (auto v : vehicles) {
            delete v;
        }
    }

    void addVehicle() {
        int choice;
        string vNum, mod;
        double rent;
        bool rented = false;

        cout << "\n--- Add New Vehicle ---\n";
        cout << "1. Car\n2. Bike\nEnter type choice: ";
        cin >> choice;

        cout << "Enter Vehicle Number: ";
        cin >> vNum;
        cout << "Enter Model Name: ";
        cin >> mod;
        cout << "Enter Rent Per Day: ";
        cin >> rent;

        if (choice == 1) {
            int seats;
            cout << "Enter Seating Capacity: ";
            cin >> seats;
            vehicles.push_back(new Car(vNum, mod, rent, seats, rented));
        } else {
            char gearChoice;
            cout << "Does it have gears? (y/n): ";
            cin >> gearChoice;
            bool gear = (gearChoice == 'y' || gearChoice == 'Y');
            vehicles.push_back(new Bike(vNum, mod, rent, gear, rented));
        }
        saveToFile();
        cout << "Vehicle added successfully!\n";
    }

    void displayAvailableVehicles() const {
        cout << "\n-----------------------------------------------------------------\n";
        cout << left << setw(12) << "Type" << setw(15) << "Vehicle No" << setw(20) << "Model" << setw(12) << "Rent/Day" << setw(12) << "Status" << endl;
        cout << "-----------------------------------------------------------------\n";
        bool found = false;
        for (const auto& v : vehicles) {
            if (!v->getIsRented()) {
                v->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "No vehicles currently available.\n";
        cout << "-----------------------------------------------------------------\n";
    }

    void searchVehicle() const {
        string vNum;
        cout << "Enter Vehicle Number to search: ";
        cin >> vNum;
        for (const auto& v : vehicles) {
            if (v->getVehicleNumber() == vNum) {
                cout << "\nVehicle Found:\n";
                cout << "-----------------------------------------------------------------\n";
                cout << left << setw(12) << "Type" << setw(15) << "Vehicle No" << setw(20) << "Model" << setw(12) << "Rent/Day" << setw(12) << "Status" << endl;
                cout << "-----------------------------------------------------------------\n";
                v->displayDetails();
                cout << "-----------------------------------------------------------------\n";
                return;
            }
        }
        cout << "Vehicle with number " << vNum << " not found.\n";
    }

    void rentVehicle() {
        string vNum;
        int days;
        cout << "Enter Vehicle Number to rent: ";
        cin >> vNum;

        for (auto& v : vehicles) {
            if (v->getVehicleNumber() == vNum) {
                if (v->getIsRented()) {
                    cout << "Error: Vehicle is already rented!\n";
                    return;
                }
                cout << "Enter number of rental days: ";
                cin >> days;
                if (days <= 0) {
                    cout << "Error: Invalid number of rental days!\n";
                    return;
                }
                v->rentVehicle();
                double totalAmount = v->calculateBill(days);
                saveToFile();
                cout << fixed << setprecision(2);
                cout << "Vehicle rented successfully! Total rental charge for " << days << " days is: INR " << totalAmount << endl;
                return;
            }
        }
        cout << "Vehicle not found.\n";
    }

    void returnVehicle() {
        string vNum;
        cout << "Enter Vehicle Number to return: ";
        cin >> vNum;

        for (auto& v : vehicles) {
            if (v->getVehicleNumber() == vNum) {
                if (!v->getIsRented()) {
                    cout << "Error: Vehicle was not rented!\n";
                    return;
                }
                v->returnVehicle();
                saveToFile();
                cout << "Vehicle returned successfully and availability updated!\n";
                return;
            }
        }
        cout << "Vehicle not found.\n";
    }

    void saveToFile() const {
        ofstream outFile(filename);
        if (!outFile) return;
        for (const auto& v : vehicles) {
            outFile << v->getType() << "," 
                    << v->getVehicleNumber() << "," 
                    << v->getModel() << "," 
                    << v->getRentPerDay() << "," 
                    << v->getIsRented() << "\n";
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) return;
        string type, vNum, mod;
        double rent;
        bool rented;
        char comma;

        while (getline(inFile, type, ',') &&
               getline(inFile, vNum, ',') &&
               getline(inFile, mod, ',') &&
               inFile >> rent >> comma >> rented) {
            inFile.ignore(); // skip newline
            if (type == "Car") {
                vehicles.push_back(new Car(vNum, mod, rent, 4, rented));
            } else {
                vehicles.push_back(new Bike(vNum, mod, rent, true, rented));
            }
        }
        inFile.close();
    }
};

int main() {
    RentalSystem system;
    int choice;

    do {
        cout << "\n===============================\n";
        cout << "   VEHICLE RENTAL MANAGEMENT   \n";
        cout << "===============================\n";
        cout << "1. Add New Vehicle\n";
        cout << "2. Display Available Vehicles\n";
        cout << "3. Search Vehicle by Number\n";
        cout << "4. Rent a Vehicle\n";
        cout << "5. Return a Vehicle\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.addVehicle(); break;
            case 2: system.displayAvailableVehicles(); break;
            case 3: system.searchVehicle(); break;
            case 4: system.rentVehicle(); break;
            case 5: system.returnVehicle(); break;
            case 6: cout << "Exiting application. Thank you!\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}