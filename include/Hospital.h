#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include <map>

class Hospital {
private:
    std::string id;
    std::string name;
    std::string location;
    int numPatients;
    std::map<std::string, std::string> connections; // Map of connected hospital IDs to descriptions

public:
    // Constructors
    Hospital();
    Hospital(const std::string& id, const std::string& name, 
             const std::string& location, int numPatients);

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getLocation() const;
    int getNumPatients() const;
    std::map<std::string, std::string> getConnections() const;

    // Setters
    void setName(const std::string& name);
    void setLocation(const std::string& location);
    void setNumPatients(int numPatients);

    // Connection management
    void addConnection(const std::string& hospitalId, const std::string& description);
    void removeConnection(const std::string& hospitalId);
    bool hasConnection(const std::string& hospitalId) const;

    // CSV conversion
    std::string toCSV() const;
    static Hospital fromCSV(const std::string& csvLine);
};

#endif // HOSPITAL_H 