#pragma once
#include <wx/wx.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <chrono>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Component;
class Wire;

struct NetlistComponent {
    int id;
    std::string type;
    wxPoint position;
    std::map<std::string, std::string> properties;

    NetlistComponent() : id(-1), position(0, 0) {}

    json ToJson() const {
        json j;
        j["id"] = id;
        j["type"] = type;
        j["position"] = { position.x, position.y };
        j["properties"] = properties;
        return j;
    }

    bool FromJson(const json& j) {
        try {
            id = j.value("id", -1);
            type = j.value("type", "");

            if (j.contains("position") && j["position"].is_array() && j["position"].size() >= 2) {
                position.x = j["position"][0];
                position.y = j["position"][1];
            }

            if (j.contains("properties") && j["properties"].is_object()) {
                properties = j["properties"].get<std::map<std::string, std::string>>();
            }

            return true;
        }
        catch (const std::exception& e) {
            wxLogError("NetlistComponent::FromJson error: %s", e.what());
            return false;
        }
    }
};

// 网表中的连线信息
struct NetlistWire {
    int id;
    int sourceComponentId;
    int sourcePortIndex;
    int targetComponentId;
    int targetPortIndex;
    std::vector<wxPoint> waypoints;

    NetlistWire() : id(-1), sourceComponentId(-1), sourcePortIndex(-1),
        targetComponentId(-1), targetPortIndex(-1) {
    }

    json ToJson() const {
        json j;
        j["id"] = id;
        j["sourceComponentId"] = sourceComponentId;
        j["sourcePortIndex"] = sourcePortIndex;
        j["targetComponentId"] = targetComponentId;
        j["targetPortIndex"] = targetPortIndex;

        json waypointsArray = json::array();
        for (const auto& wp : waypoints) {
            waypointsArray.push_back({ wp.x, wp.y });
        }
        j["waypoints"] = waypointsArray;

        return j;
    }

    bool FromJson(const json& j) {
        try {
            id = j.value("id", -1);
            sourceComponentId = j.value("sourceComponentId", -1);
            sourcePortIndex = j.value("sourcePortIndex", -1);
            targetComponentId = j.value("targetComponentId", -1);
            targetPortIndex = j.value("targetPortIndex", -1);

            waypoints.clear();
            if (j.contains("waypoints") && j["waypoints"].is_array()) {
                for (const auto& wp : j["waypoints"]) {
                    if (wp.is_array() && wp.size() >= 2) {
                        waypoints.push_back(wxPoint(wp[0], wp[1]));
                    }
                }
            }

            return true;
        }
        catch (const std::exception& e) {
            wxLogError("NetlistWire::FromJson error: %s", e.what());
            return false;
        }
    }
};

class Netlist {
public:
    std::string circuitName;
    std::string circuitDescription;
    std::string version;
    std::string author;
    std::string dateCreated;
    std::string dateModified;

    std::vector<NetlistComponent> components;
    std::vector<NetlistWire> wires;

    Netlist() {
        version = "1.0";
    }

    void Clear() {
        components.clear();
        wires.clear();
        circuitName.clear();
        circuitDescription.clear();
    }

    void AddComponent(const NetlistComponent& comp) {
        components.push_back(comp);
    }

    void AddWire(const NetlistWire& wire) {
        wires.push_back(wire);
    }

    void RemoveComponent(int componentId) {
        components.erase(
            std::remove_if(components.begin(), components.end(),
                [componentId](const NetlistComponent& c) {
                    return c.id == componentId;
                }),
            components.end()
        );

        wires.erase(
            std::remove_if(wires.begin(), wires.end(),
                [componentId](const NetlistWire& w) {
                    return w.sourceComponentId == componentId ||
                        w.targetComponentId == componentId;
                }),
            wires.end()
        );
    }

    NetlistComponent* FindComponent(int id) {
        for (auto& comp : components) {
            if (comp.id == id) return &comp;
        }
        return nullptr;
    }

    const NetlistComponent* FindComponent(int id) const {
        for (const auto& comp : components) {
            if (comp.id == id) return &comp;
        }
        return nullptr;
    }

    bool SaveToFile(const std::string& filename) const;
    bool LoadFromFile(const std::string& filename);
    json ToJson() const;
    bool FromJson(const json& j);

    int GetComponentCount() const { return static_cast<int>(components.size()); }
    int GetWireCount() const { return static_cast<int>(wires.size()); }

private:
    int FindRoot(std::map<int, int>& parent, int node);
    void Union(std::map<int, int>& parent, int node1, int node2);
};


class NetlistUtils {
public:
    static Netlist ExportFromCanvas(
        const std::vector<Component*>& components,
        const std::vector<Wire>& wires);

    static bool ImportToCanvas(
        const Netlist& netlist,
        std::vector<Component*>& components,
        std::vector<Wire>& wires,
        int& nextComponentId);
};



