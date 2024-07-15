#pragma once
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <core/utils.h>

class RTVar {
   public:
    class Pair {
       public:
        std::string m_key{};
        std::vector<std::string> m_values{};
        std::string m_value;
        Pair() {
        }
        Pair(std::string key, std::initializer_list<std::string> values) : m_key(key), m_values(values) {
        }
        bool operator==(const RTVar::Pair& rst) {
            return m_key == rst.m_key && m_values[0] == rst.m_values[0];
        }
        static Pair parse(std::string str) {
            Pair ret{};
            if (!str.length()) { // we dont want to parse empty string so m_pairs vector wont make the app crash
                ret.m_value.append("PARSE ERROR");
                ret.m_values.push_back("PARSE ERROR");
                return ret;
            }
            std::string token;
            std::stringstream ss(str);
            bool key = true, first = true;
            while (std::getline(ss, token, '|')) {
                if (key) {
                    ret.m_key = token;
                    key = false;
                }
                else {
                    if (!first)
                        ret.m_value.append("|");
                    ret.m_value.append(token);
                    ret.m_values.push_back(token);
                    first = false;
                }
            }
            return ret;
        }
        std::string serialize() {
            std::string ret{};
            ret.append(m_key);
            for (auto& val : m_values) {
                ret.append("|");
                ret.append(val);
            }
            return ret;
        }
    };
    RTVar() {
    }
    RTVar(std::initializer_list<Pair> pairs) : m_pairs(pairs) {
    }
    static RTVar parse(std::string str) {
        RTVar ret{};
        std::stringstream ss(str);
        std::string token{};
        while (std::getline(ss, token, '\n'))
            ret.append(token);
        return ret;
    }
    Pair& append(std::string str) {
        Pair p = Pair::parse(str);
        m_pairs.push_back(p);
        return m_pairs.back();
    }
    Pair& get(int i) {
        if ((unsigned)i >= m_pairs.size())
            return m_pairs[0];
        return m_pairs[i];
    }
    bool valid() {
        if (m_pairs.size() < 1)
            return false;

        if (m_pairs[0].m_values.size() < 1)
            return false;

        return true;
    }
    Pair* find(const std::string& key) {
        int inx = 0;
        for (auto pair : m_pairs) {
            if (pair.m_key == key) //we cant return local pairs addr
                return &m_pairs[inx];
            inx++;
        }
        return nullptr;
    }

    std::string get(const std::string& key) {
        auto pair = find(key);
        if (pair)
            return pair->m_value;
        return "";
    }
    void set(const std::string& key, std::string value) {
        auto pair = find(key);
        if (pair && pair->m_values.size() >= 1)
            pair->m_values[0] = value;
    }
    std::string serialize() {
        std::string ret{};
        for (auto& val : m_pairs) {
            ret.append(val.serialize());
            ret.append("\n");
        }
        if (ret != "")
            ret.erase(ret.end());
        return ret;
    }
    bool validate_ints(std::vector<std::string> vals) {
        for (auto str : vals) {
            auto pair = this->find(str);
            if (!pair)
                return false;
            if (!utils::is_number(pair->m_value))
                return false;
        }
        return true;
    }
    bool validate_int(std::string str) {
        auto pair = this->find(str);
        if (!pair)
            return false;
        if (!utils::is_number(pair->m_value))
            return false;
        return true;
    }
    inline int get_int(const std::string& key) { //this does not chekc if it exists, it assumes validate_ints has been consulated beforehand
        return atoi(find(key)->m_value.c_str());
    }
    inline long long get_long(const std::string& key) { //assumes validate_ints
        return atoll(find(key)->m_value.c_str());
    }
    size_t size() const {
        return m_pairs.size();
    }
    void remove(const std::string& key) {
        auto pair = find(key);
        if (pair) {
            auto& ref = *pair;
            m_pairs.erase(std::remove(m_pairs.begin(), m_pairs.end(), ref), m_pairs.end());
        }
    }

   private:
    //i could use std::map but for the sake of simplicity i dont, i want the code to be as readable as possible
    std::vector<Pair> m_pairs{};
};