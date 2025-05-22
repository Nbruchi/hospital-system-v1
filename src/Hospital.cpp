#include "../include/Hospital.h"
#include <sstream>
#include <algorithm>

Hospital::Hospital() : numPatients(0) {}

Hospital::Hospital(const std::string& id, const std::string& name, 
                   const std::string& location, int numPatients)
    : id(id), name(name), location(location), numPatients(numPatients) {}

std::string Hospital::getId() const { return id; }
std::string Hospital::getName() const { return name; }
std::string Hospital::getLocation() const { return location; }
int Hospital::getNumPatients() const { return numPatients; }
std::map<std::string, std::string> Hospital::getConnections() const { return connections; }

void Hospital::setName(const std::string& name) { this->name = name; }
void Hospital::setLocation(const std::string& location) { this->location = location; }
void Hospital::setNumPatients(int numPatients) { this->numPatients = numPatients; }

void Hospital::addConnection(const std::string& hospitalId, const std::string& description) {
    connections[hospitalId] = description;
}

void Hospital::removeConnection(const std::string& hospitalId) {
    connections.erase(hospitalId);
}

bool Hospital::hasConnection(const std::string& hospitalId) const {
    return connections.find(hospitalId) != connections.end();
}

std::string Hospital::toCSV() const {
    std::stringstream ss;
    ss << id << "," << name << "," << location << "," << numPatients;
    return ss.str();
}

Hospital Hospital::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string id, name, location, numPatientsStr;
    
    std::getline(ss, id, ',');
    std::getline(ss, name, ',');
    std::getline(ss, location, ',');
    std::getline(ss, numPatientsStr, ',');
    
    int numPatients = std::stoi(numPatientsStr);
    return Hospital(id, name, location, numPatients);
} 