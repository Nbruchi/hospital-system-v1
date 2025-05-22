#ifndef HOSPITAL_NETWORK_H
#define HOSPITAL_NETWORK_H

#include "Hospital.h"
#include <map>
#include <string>
#include <fstream>
#include <filesystem>

class HospitalNetwork {
private:
    std::map<std::string, Hospital> hospitals;
    std::filesystem::path dataDir;
    std::filesystem::path hospitalsFile;
    std::filesystem::path graphFile;

    bool validateHospitalId(const std::string& id) const;
    bool validateNumericInput(const std::string& input) const;
    void saveHospitalsToFile() const;
    void saveGraphToFile() const;
    void loadHospitalsFromFile();
    void loadGraphFromFile();
    void ensureDataDirectoryExists() const;

public:
    HospitalNetwork(const std::string& dataDirectory = "data");

    // CRUD Operations
    bool addHospital(const std::string& id, const std::string& name,
                     const std::string& location, int numPatients);
    bool updateHospital(const std::string& id, const std::string& name,
                       const std::string& location, int numPatients);
    bool deleteHospital(const std::string& id);
    void displayAllHospitals() const;
    Hospital* getHospital(const std::string& id);

    // Graph Operations
    bool addConnection(const std::string& hospital1Id, const std::string& hospital2Id,
                      const std::string& description);
    bool removeConnection(const std::string& hospital1Id, const std::string& hospital2Id);
    void displayConnections() const;
    void exportRelationshipsToCSV(const std::string& filename) const;

    // File Operations
    void saveToFiles() const;
    void loadFromFiles();
};

#endif // HOSPITAL_NETWORK_H 