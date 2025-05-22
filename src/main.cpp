#include "../include/HospitalNetwork.h"
#include <iostream>
#include <limits>
#include <string>
#include <functional>
#include <vector>

void displayMenu() {
    std::cout << "\n=== Hospital Network Management System ===\n"
              << "1. Add Hospital\n"
              << "2. Update Hospital\n"
              << "3. Delete Hospital\n"
              << "4. Display All Hospitals\n"
              << "5. Add Connection\n"
              << "6. Remove Connection\n"
              << "7. Display Connections\n"
              << "8. Export Relationships to CSV\n"
              << "9. Initialize Predefined Scenario\n"
              << "0. Exit\n"
              << "Enter your choice: ";
}

std::string getLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void addOrUpdateHospital(HospitalNetwork& network, bool isUpdate) {
    std::string id = getLine(isUpdate ? "Enter Hospital ID to update: " : "ID (e.g., H1): ");
    std::string name = getLine("Name: ");
    std::string location = getLine("Location: ");
    int numPatients = 0;
    try {
        numPatients = std::stoi(getLine("Number of Patients: "));
    } catch (...) {
        std::cout << "Error: Invalid number of patients.\n";
        return;
    }
    bool ok = isUpdate
        ? network.updateHospital(id, name, location, numPatients)
        : network.addHospital(id, name, location, numPatients);
    if (ok) std::cout << (isUpdate ? "Hospital updated" : "Hospital added") << " successfully!\n";
}

void deleteHospital(HospitalNetwork& network) {
    std::string id = getLine("Enter Hospital ID to delete: ");
    if (network.deleteHospital(id)) std::cout << "Hospital deleted successfully!\n";
}

void addOrRemoveConnection(HospitalNetwork& network, bool isRemove) {
    std::string id1 = getLine("First Hospital ID: ");
    std::string id2 = getLine("Second Hospital ID: ");
    if (isRemove) {
        if (network.removeConnection(id1, id2)) std::cout << "Connection removed successfully!\n";
    } else {
        std::string desc = getLine("Connection Description: ");
        if (network.addConnection(id1, id2, desc)) std::cout << "Connection added successfully!\n";
    }
}

void initializePredefinedScenario(HospitalNetwork& network) {
    network.addHospital("H1", "Central Hospital", "Downtown", 500);
    network.addHospital("H2", "North Hospital", "North District", 300);
    network.addHospital("H3", "South Hospital", "South District", 400);
    network.addHospital("H4", "East Hospital", "East District", 350);
    network.addHospital("H5", "West Hospital", "West District", 250);
    network.addHospital("H6", "Emergency Hospital", "City Center", 600);
    network.addConnection("H1", "H6", "Emergency backup");
    network.addConnection("H1", "H4", "Referral support");
    network.addConnection("H2", "H3", "Standard route");
    network.addConnection("H6", "H5", "Emergency support");
    network.addConnection("H5", "H4", "Patient transfer");
    network.addConnection("H4", "H1", "Specialist referral");
    std::cout << "Predefined scenario initialized successfully!\n";
}

int main() {
    HospitalNetwork network;
    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: addOrUpdateHospital(network, false); break;
            case 2: addOrUpdateHospital(network, true); break;
            case 3: deleteHospital(network); break;
            case 4: network.displayAllHospitals(); break;
            case 5: addOrRemoveConnection(network, false); break;
            case 6: addOrRemoveConnection(network, true); break;
            case 7: network.displayConnections(); break;
            case 8: network.exportRelationshipsToCSV("relationships.csv"); break;
            case 9: initializePredefinedScenario(network); break;
            case 0: std::cout << "Thank you for using the Hospital Network Management System!\n"; return 0;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
    }
} 