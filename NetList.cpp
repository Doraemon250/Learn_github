#include "Netlist.h"
#include "Component.h"

bool Netlist::SaveToFile(const std::string& filename) const {
    try {
        json j = ToJson();
        std::ofstream file(filename);
        if (!file.is_open()) {
            wxLogError("Cannot open file for writing: %s", filename.c_str());
            return false;
        }
        file << j.dump(4);
        file.close();
        wxLogMessage("Netlist saved to: %s", filename.c_str());
        return true;
    }
    catch (const std::exception& e) {
        wxLogError("Error saving netlist: %s", e.what());
        return false;
    }
}

bool Netlist::LoadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            wxLogError("Cannot open file for reading: %s", filename.c_str());
            return false;
        }
        json j;
        file >> j;
        file.close();
        bool result = FromJson(j);
        if (result) {
            wxLogMessage("Netlist loaded from: %s", filename.c_str());
        }
        return result;
    }
    catch (const std::exception& e) {
        wxLogError("Error loading netlist: %s", e.what());
        return false;
    }
}

json Netlist::ToJson() const {
    json j;
    j["circuitName"] = circuitName;
    j["circuitDescription"] = circuitDescription;
    j["version"] = version;

    json componentsArray = json::array();
    for (const auto& comp : components) {
        componentsArray.push_back(comp.ToJson());
    }
    j["components"] = componentsArray;

    json wiresArray = json::array();
    for (const auto& wire : wires) {
        wiresArray.push_back(wire.ToJson());
    }
    j["wires"] = wiresArray;

    return j;
}

bool Netlist::FromJson(const json& j) {
    try {
        Clear();
        circuitName = j.value("circuitName", "");
        circuitDescription = j.value("circuitDescription", "");
        version = j.value("version", "1.0");

        if (j.contains("components") && j["components"].is_array()) {
            for (const auto& compJson : j["components"]) {
                NetlistComponent comp;
                if (comp.FromJson(compJson)) {
                    components.push_back(comp);
                }
            }
        }

        if (j.contains("wires") && j["wires"].is_array()) {
            for (const auto& wireJson : j["wires"]) {
                NetlistWire wire;
                if (wire.FromJson(wireJson)) {
                    wires.push_back(wire);
                }
            }
        }

        return true;
    }
    catch (const std::exception& e) {
        wxLogError("Netlist::FromJson error: %s", e.what());
        return false;
    }
}

Netlist NetlistUtils::ExportFromCanvas(
    const std::vector<Component*>& components,
    const std::vector<Wire>& wires) {

    Netlist netlist;
    netlist.circuitName = "Untitled Circuit";

    for (const auto& comp : components) {
        NetlistComponent nc;
        nc.id = comp->m_id;

        switch (comp->m_type) {
        case ANDGATE: nc.type = "AND"; break;
        case ORGATE: nc.type = "OR"; break;
        case NOTGATE: nc.type = "NOT"; break;
        default: nc.type = "UNKNOWN"; break;
        }

        nc.position = comp->GetCenter();
        netlist.AddComponent(nc);
    }

    int wireId = 0;
    for (const auto& wire : wires) {
        NetlistWire nw;
        nw.id = wireId++;
        nw.sourceComponentId = wire.startComponentId;
        nw.sourcePortIndex = wire.startPortIndex;
        nw.targetComponentId = wire.endComponentId;
        nw.targetPortIndex = wire.endPortIndex;
        nw.waypoints.push_back(wire.start);
        nw.waypoints.push_back(wire.end);
        netlist.AddWire(nw);
    }
    return netlist;
}

bool NetlistUtils::ImportToCanvas(
    const Netlist& netlist,
    std::vector<Component*>& components,
    std::vector<Wire>& wires,
    int& nextComponentId) {

    components.clear();
    wires.clear();

    Gate* gate;
    for (const auto& nc : netlist.components) {
        ComponentType type = NONE_TYPE;
        gate = NULL;
        if (nc.type == "AND")
        {
            gate = new ANDGate(nc.position, ANDGATE, nc.id);
        }
        else if (nc.type == "OR") type = ORGATE;
        else if (nc.type == "NOT") type = NOTGATE;

        if (gate) {
            components.push_back(gate);

            if (nc.id >= nextComponentId) {
                nextComponentId = nc.id + 1;
            }
        }
    }

    for (const auto& nw : netlist.wires) {
        Wire wire;
        wire.startComponentId = nw.sourceComponentId;
        wire.startPortIndex = nw.sourcePortIndex;
        wire.endComponentId = nw.targetComponentId;
        wire.endPortIndex = nw.targetPortIndex;

        if (!nw.waypoints.empty()) {
            wire.start = nw.waypoints.front();
            wire.end = nw.waypoints.back();
        }

        wires.push_back(wire);
    }

    return true;
}
