#pragma once

#include <unordered_set>

//=============================================================================
class CraseSelection {
public:
    static CraseSelection& inst() { static CraseSelection s; return s; }

    bool contains(int id) const { return ids_.count(id) > 0; }
    void add(int id)            { ids_.insert(id); }
    void remove(int id)         { ids_.erase(id); }
    void toggle(int id)         { if (!ids_.insert(id).second) ids_.erase(id); }
    void clear()                { ids_.clear(); }

    const std::unordered_set<int>& ids() const { return ids_; }

private:
    CraseSelection() = default;
    std::unordered_set<int> ids_;
};
