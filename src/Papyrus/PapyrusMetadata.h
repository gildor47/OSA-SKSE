#pragma once

#include "Graph/LookupTable.h"
#include "Util.h"

namespace PapyrusMetadata {
    using VM = RE::BSScript::IVirtualMachine;

    // I know this should be in utils but when I put it there the code no longer compiles
    // I don't get c++ ...
    bool contains(std::vector<int> haystack, int needle) {
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }

    template<class T>
    bool contains(std::vector<T> haystack, T needle) {
        return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
    }

    template<class T>
    bool containsAny(std::vector<T> haystack, std::vector<T> needles) {
        for (T needle : needles) {
            if (contains(haystack, needle)) {
                return true;
            }
        }
        return false;
    }

    template<class T>
    bool containsAll(std::vector<T> haystack, std::vector<T> needles) {
        for (T needle : needles) {
            if (!contains(haystack, needle)) {
                return false;
            }
        }
        return true;
    }

    int findAction(std::string id, std::function<bool(Graph::Action)> condition) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            size_t size = node->actions.size();
            for (int i = 0; i < size; i++) {
                auto action = node->actions[i];
                if (condition(*action)) {
                    return i;
                }
            }
        }
        return -1;
    }

    std::vector<int> findActions(std::string id, std::function<bool(Graph::Action)> condition) {
        std::vector<int> ret;
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            size_t size = node->actions.size();
            for (int i = 0; i < size; i++) {
                auto action = node->actions[i];
                if (condition(*action)) {
                    ret.push_back(i);
                }
            }
        }
        return ret;
    }

    // *********************************************************
    // start of papyrus bound functions
    // *********************************************************

    std::vector<std::string> GetTags(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            return node->tags;
        }
        return std::vector<std::string>();
    }

    bool HasTag(RE::StaticFunctionTag*, std::string id, std::string tag) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            return contains(node->tags, tag);
        }
        return false;
    }

    bool HasAnyTag(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            return containsAny(node->tags, tags);
        }
        return false;
    }

    bool HasAllTags(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            return containsAll(node->tags, tags);
        }
        return false;
    }

    std::vector<std::string> GetActorTags(RE::StaticFunctionTag*, std::string id, int position) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (node->actors.size() > position) {
                return node->actors[position]->tags;
            }
        }
        return std::vector<std::string>();
    }

    bool HasActorTag(RE::StaticFunctionTag*, std::string id, int position, std::string tag) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (node->actors.size() > position) {
                return node->hasActorTag(position, tag);
            }
        }
        return false;
    }

    bool HasAnyActorTag(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> tags) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (node->actors.size() > position) {
                return containsAny(node->actors[position]->tags, tags);
            }
        }
        return false;
    }

    bool HasAllActorTags(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> tags) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (node->actors.size() > position) {
                return containsAll(node->actors[position]->tags, tags);
            }
        }
        return false;
    }

    bool HasActions(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            return !node->actions.empty();
        }
        return false;
    }

    int FindAction(RE::StaticFunctionTag*, std::string id, std::string type) {
        return findAction(id, [type](Graph::Action action) { return action.type == type; });
    }

    int FindAnyAction(RE::StaticFunctionTag*, std::string id, std::vector<std::string> types) {
        return findAction(id, [types](Graph::Action action) { return contains(types, action.type); });
    }

    std::vector<int> FindActions(RE::StaticFunctionTag*, std::string id, std::string type) {
        return findActions(id, [type](Graph::Action action) { return action.type == type; });
    }

    std::vector<int> FindAllActions(RE::StaticFunctionTag*, std::string id, std::vector<std::string> types) {
        return findActions(id, [types](Graph::Action action) { return contains(types, action.type); });
    }

    int FindActionForActor(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findAction(id, [position, type](Graph::Action action) { return action.actor == position && action.type == type; });
    }

    int FindAnyActionForActor(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findAction(id, [position, types](Graph::Action action) { return action.actor == position && contains(types, action.type); });
    }

    std::vector<int> FindActionsForActor(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findActions(id, [position, type](Graph::Action action) { return action.actor == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForActor(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findActions(id, [position, types](Graph::Action action) { return action.actor == position && contains(types, action.type); });
    }

    int FindActionForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findAction(id, [positions, type](Graph::Action action) { return contains(positions, action.actor) && action.type == type; });
    }

    int FindAnyActionForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findAction(id, [positions, types](Graph::Action action) { return contains(positions, action.actor) && contains(types, action.type); });
    }

    std::vector<int> FindActionsForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findActions(id, [positions, type](Graph::Action action) { return contains(positions, action.actor) && action.type == type; });
    }

    std::vector<int> FindAllActionsForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findActions(id, [positions, types](Graph::Action action) { return contains(positions, action.actor) && contains(types, action.type); });
    }

    int FindActionForTarget(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findAction(id, [position, type](Graph::Action action) { return action.target == position && action.type == type; });
    }

    int FindAnyActionForTarget(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findAction(id, [position, types](Graph::Action action) { return action.target == position && contains(types, action.type); });
    }

    std::vector<int> FindActionsForTarget(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findActions(id, [position, type](Graph::Action action) { return action.target == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForTarget(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findActions(id, [position, types](Graph::Action action) { return action.target == position && contains(types, action.type); });
    }

    int FindActionForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findAction(id, [positions, type](Graph::Action action) { return contains(positions, action.target) && action.type == type; });
    }

    int FindAnyActionForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findAction(id, [positions, types](Graph::Action action) { return contains(positions, action.target) && contains(types, action.type); });
    }

    std::vector<int> FindActionsForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findActions(id, [positions, type](Graph::Action action) { return contains(positions, action.target) && action.type == type; });
    }

    std::vector<int> FindAllActionsForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findActions(id, [positions, types](Graph::Action action) { return contains(positions, action.target) && contains(types, action.type); });
    }

    int FindActionForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findAction(id, [position, type](Graph::Action action) { return action.performer == position && action.type == type; });
    }

    int FindAnyActionForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findAction(id, [position, types](Graph::Action action) { return action.performer == position && contains(types, action.type); });
    }

    std::vector<int> FindActionsForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        return findActions(id, [position, type](Graph::Action action) { return action.performer == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        return findActions(id, [position, types](Graph::Action action) { return action.performer == position && contains(types, action.type); });
    }

    int FindActionForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findAction(id, [positions, type](Graph::Action action) { return contains(positions, action.performer) && action.type == type; });
    }

    int FindAnyActionForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findAction(id, [positions, types](Graph::Action action) { return contains(positions, action.performer) && contains(types, action.type); });
    }

    std::vector<int> FindActionsForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        return findActions(id, [positions, type](Graph::Action action) {
            return contains(positions, action.performer) && action.type == type;
        });
    }

    std::vector<int> FindAllActionsForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        return findActions(id, [positions, types](Graph::Action action) { return contains(positions, action.performer) && contains(types, action.type); });
    }

    std::vector<std::string> GetActionTypes(RE::StaticFunctionTag*, std::string id) {
        std::vector<std::string> ret;
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action->type);
            }
        }
        return ret;
    }

    std::string GetActionType(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index]->type;
            }
        }
        return "";
    }

    std::vector<int> GetActionActors(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action->actor);
            }
        }
        return ret;
    }

    int GetActionActor(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index]->actor;
            }
        }
        return -1;
    }

    std::vector<int> GetActionTargets(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action->target);
            }
        }
        return ret;
    }

    int GetActionTarget(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index]->target;
            }
        }
        return -1;
    }

    std::vector<int> GetActionPerformers(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action->performer);
            }
        }
        return ret;
    }

    int GetActionPerformer(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::LookupTable::GetNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index]->actor;
            }
        }
        return -1;
    }


	bool Bind(VM* a_vm) {
        const auto obj = "OMetadata"sv;

        BIND(GetTags);
        BIND(HasTag);
        BIND(HasAnyTag);
        BIND(HasAllTags);
        BIND(GetActorTags);
        BIND(HasActorTag);
        BIND(HasAnyActorTag);
        BIND(HasAllActorTags);

        BIND(HasActions);
        BIND(FindAction);
        BIND(FindAnyAction);
        BIND(FindActions);
        BIND(FindAllActions);
        BIND(FindActionForActor);
        BIND(FindAnyActionForActor);
        BIND(FindActionsForActor);
        BIND(FindAllActionsForActor);
        BIND(FindActionForActors);
        BIND(FindAnyActionForActors);
        BIND(FindActionsForActors);
        BIND(FindAllActionsForActors);
        BIND(FindActionForTarget);
        BIND(FindAnyActionForTarget);
        BIND(FindActionsForTarget);
        BIND(FindAllActionsForTarget);
        BIND(FindActionForTargets);
        BIND(FindAnyActionForTargets);
        BIND(FindActionsForTargets);
        BIND(FindAllActionsForTargets);
        BIND(FindActionForPerformer);
        BIND(FindAnyActionForPerformer);
        BIND(FindActionsForPerformer);
        BIND(FindAllActionsForPerformer);
        BIND(FindActionForPerformers);
        BIND(FindAnyActionForPerformers);
        BIND(FindActionsForPerformers);
        BIND(FindAllActionsForPerformers);
        BIND(GetActionTypes);
        BIND(GetActionType);
        BIND(GetActionActors);
        BIND(GetActionActor);
        BIND(GetActionTargets);
        BIND(GetActionTarget);
        BIND(GetActionPerformers);
        BIND(GetActionPerformer);

        return true;
	}
}