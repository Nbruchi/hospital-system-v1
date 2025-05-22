#include "../include/HospitalNetwork.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

HospitalNetwork::HospitalNetwork(const std::string& dataDirectory)
    : dataDir(dataDirectory) {
    hospitalsFile = dataDir / "hospitals.csv";
    graphFile = dataDir / "graph.txt";
    ensureDataDirectoryExists();
    loadFromFiles();
}

void HospitalNetwork::ensureDataDirectoryExists() const {
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directories(dataDir);
    }
}

bool HospitalNetwork::validateHospitalId(const std::string& id) const {
    if (id.empty() || id.length() < 2) return false;
    if (id[0] != 'H') return false;
    return std::all_of(id.begin() + 1, id.end(), ::isdigit);
}

bool HospitalNetwork::validateNumericInput(const std::string& input) const {
    return !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
}

bool HospitalNetwork::addHospital(const std::string& id, const std::string& name,
                                 const std::string& location, int numPatients) {
    if (!validateHospitalId(id)) {
        std::cout << "Error: Invalid hospital ID format. Must start with 'H' followed by numbers.\n";
        return false;
    }

    if (hospitals.find(id) != hospitals.end()) {
        std::cout << "Error: Hospital ID already exists.\n";
        return false;
    }

    hospitals[id] = Hospital(id, name, location, numPatients);
    saveToFiles();
    return true;
}

bool HospitalNetwork::updateHospital(const std::string& id, const std::string& name,
                                    const std::string& location, int numPatients) {
    if (hospitals.find(id) == hospitals.end()) {
        std::cout << "Error: Hospital ID not found.\n";
        return false;
    }

    hospitals[id].setName(name);
    hospitals[id].setLocation(location);
    hospitals[id].setNumPatients(numPatients);
    saveToFiles();
    return true;
}

bool HospitalNetwork::deleteHospital(const std::string& id) {
    if (hospitals.find(id) == hospitals.end()) {
        std::cout << "Error: Hospital ID not found.\n";
        return false;
    }

    // Remove all connections to this hospital
    for (auto& pair : hospitals) {
        pair.second.removeConnection(id);
    }

    hospitals.erase(id);
    saveToFiles();
    return true;
}

void HospitalNetwork::displayAllHospitals() const {
    std::cout << "\nHospital List:\n";
    std::cout << std::setw(10) << "ID" << std::setw(20) << "Name" 
              << std::setw(20) << "Location" << std::setw(15) << "Patients\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& pair : hospitals) {
        const Hospital& h = pair.second;
        std::cout << std::setw(10) << h.getId() 
                  << std::setw(20) << h.getName()
                  << std::setw(20) << h.getLocation()
                  << std::setw(15) << h.getNumPatients() << "\n";
    }
}

Hospital* HospitalNetwork::getHospital(const std::string& id) {
    auto it = hospitals.find(id);
    return it != hospitals.end() ? &it->second : nullptr;
}

bool HospitalNetwork::addConnection(const std::string& hospital1Id, const std::string& hospital2Id,
                                  const std::string& description) {
    if (hospitals.find(hospital1Id) == hospitals.end() || 
        hospitals.find(hospital2Id) == hospitals.end()) {
        std::cout << "Error: One or both hospital IDs not found.\n";
        return false;
    }

    hospitals[hospital1Id].addConnection(hospital2Id, description);
    hospitals[hospital2Id].addConnection(hospital1Id, description);
    saveToFiles();
    return true;
}

bool HospitalNetwork::removeConnection(const std::string& hospital1Id, const std::string& hospital2Id) {
    if (hospitals.find(hospital1Id) == hospitals.end() || 
        hospitals.find(hospital2Id) == hospitals.end()) {
        std::cout << "Error: One or both hospital IDs not found.\n";
        return false;
    }

    hospitals[hospital1Id].removeConnection(hospital2Id);
    hospitals[hospital2Id].removeConnection(hospital1Id);
    saveToFiles();
    return true;
}

void HospitalNetwork::displayConnections() const {
    std::cout << "\nHospital Connections:\n";
    std::cout << std::setw(15) << "Hospital" << std::setw(25) << "Connected To" 
              << std::setw(30) << "Description\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& pair : hospitals) {
        const Hospital& h = pair.second;
        const auto& connections = h.getConnections();
        
        if (connections.empty()) {
            std::cout << std::setw(15) << h.getId() << std::setw(25) << "None" 
                      << std::setw(30) << "No connections\n";
        } else {
            bool first = true;
            for (const auto& conn : connections) {
                if (first) {
                    std::cout << std::setw(15) << h.getId() 
                              << std::setw(25) << conn.first
                              << std::setw(30) << conn.second << "\n";
                    first = false;
                } else {
                    std::cout << std::setw(15) << "" 
                              << std::setw(25) << conn.first
                              << std::setw(30) << conn.second << "\n";
                }
            }
        }
    }
}

void HospitalNetwork::exportRelationshipsToCSV(const std::string& filename) const {
    std::filesystem::path exportPath = dataDir / filename;
    std::ofstream file(exportPath);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for writing: " << exportPath << "\n";
        return;
    }

    file << "Hospital Center,Connected Hospitals,Description\n";
    
    for (const auto& pair : hospitals) {
        const Hospital& h = pair.second;
        const auto& connections = h.getConnections();
        
        if (connections.empty()) {
            file << h.getId() << ",None,No connections\n";
        } else {
            for (const auto& conn : connections) {
                file << h.getId() << "," << conn.first << "," << conn.second << "\n";
            }
        }
    }
    
    file.close();
    std::cout << "Relationships exported to " << exportPath << "\n";
}

void HospitalNetwork::saveHospitalsToFile() const {
    std::ofstream file(hospitalsFile);
    if (!file.is_open()) {
        std::cout << "Error: Could not open hospitals file for writing: " << hospitalsFile << "\n";
        return;
    }

    file << "ID,Name,Location,Patients\n";
    for (const auto& pair : hospitals) {
        file << pair.second.toCSV() << "\n";
    }
    file.close();
}

void HospitalNetwork::saveGraphToFile() const {
    std::ofstream file(graphFile);
    if (!file.is_open()) {
        std::cout << "Error: Could not open graph file for writing: " << graphFile << "\n";
        return;
    }

    for (const auto& pair : hospitals) {
        const Hospital& h = pair.second;
        const auto& connections = h.getConnections();
        
        if (!connections.empty()) {
            file << h.getId();
            for (const auto& conn : connections) {
                file << "," << conn.first << ":" << conn.second;
            }
            file << "\n";
        }
    }
    file.close();
}

void HospitalNetwork::loadHospitalsFromFile() {
    std::ifstream file(hospitalsFile);
    if (!file.is_open()) {
        std::cout << "Note: Hospitals file not found. Starting with empty database.\n";
        return;
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        try {
            Hospital h = Hospital::fromCSV(line);
            hospitals[h.getId()] = h;
        } catch (const std::exception& e) {
            std::cout << "Error loading hospital data: " << e.what() << "\n";
        }
    }
    file.close();
}

void HospitalNetwork::loadGraphFromFile() {
    std::ifstream file(graphFile);
    if (!file.is_open()) {
        std::cout << "Note: Graph file not found. Starting with empty connections.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string hospitalId, connection;
        
        std::getline(ss, hospitalId, ',');
        
        while (std::getline(ss, connection, ',')) {
            size_t colonPos = connection.find(':');
            if (colonPos != std::string::npos) {
                std::string connectedId = connection.substr(0, colonPos);
                std::string description = connection.substr(colonPos + 1);
                
                if (hospitals.find(hospitalId) != hospitals.end() && 
                    hospitals.find(connectedId) != hospitals.end()) {
                    hospitals[hospitalId].addConnection(connectedId, description);
                    hospitals[connectedId].addConnection(hospitalId, description);
                }
            }
        }
    }
    file.close();
}

void HospitalNetwork::saveToFiles() const {
    saveHospitalsToFile();
    saveGraphToFile();
}

void HospitalNetwork::loadFromFiles() {
    loadHospitalsFromFile();
    loadGraphFromFile();
} 