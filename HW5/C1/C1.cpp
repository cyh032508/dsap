#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random> // For Test

struct IMetro {
    virtual void AddConnection(std::string station_name_a, std::string station_name_b) = 0;
    virtual bool IsConnected(std::string station_name_a, std::string station_name_b) = 0;
    virtual bool HasPath(std::string station_name_a, std::string station_name_b) = 0;
    virtual std::vector<std::string> ShortestPath(std::string station_name_a, std::string station_name_b) = 0;
    virtual ~IMetro() {}
};


#include <bits/stdc++.h>
class Metro : public IMetro {
public:
    void AddConnection(std::string station_name_a, std::string station_name_b) override;
    bool IsConnected(std::string station_name_a, std::string station_name_b) override;
    bool HasPath(std::string station_name_a, std::string station_name_b) override;
    std::vector<std::string> ShortestPath(std::string station_name_a, std::string station_name_b) override;
    bool DFS(const std::string& current_station, const std::string& target_station, std::unordered_set<std::string>& visited);
private:
    std::unordered_map<std::string, std::vector<std::string>> adjacencyList;
};
void Metro::AddConnection(std::string station_name_a, std::string station_name_b) {
    adjacencyList[station_name_a].push_back(station_name_b);
    adjacencyList[station_name_b].push_back(station_name_a);
}

bool Metro::IsConnected(std::string station_name_a, std::string station_name_b) {
    if (station_name_a == station_name_b) {
        return false;
    }

    for (const std::string& neighbor : adjacencyList[station_name_a]) {
        if (neighbor == station_name_b) {
            return true;
        }
    }

    return false;
}


bool Metro::HasPath(std::string station_name_a, std::string station_name_b) {
    if (station_name_a == station_name_b) {
        return true;
    }

    std::unordered_set<std::string> visited;
    return DFS(station_name_a, station_name_b, visited);
}

bool Metro::DFS(const std::string& current_station, const std::string& target_station, std::unordered_set<std::string>& visited) {
    visited.insert(current_station);

    if (current_station == target_station) {
        return true;
    }

    for (const std::string& neighbor : adjacencyList[current_station]) {
        if (visited.find(neighbor) == visited.end()) {
            if (DFS(neighbor, target_station, visited)) {
                return true;
            }
        }
    }

    return false;
}


std::vector<std::string> Metro::ShortestPath(std::string station_name_a, std::string station_name_b) {
    if(station_name_a == station_name_b) {
        std::vector<std::string> result;
        result.push_back(station_name_a);
        return result;
    }
    
    std::unordered_map<std::string, int> distance;
    std::unordered_map<std::string, bool> visited;
    
    distance[station_name_b] = 0; 
    
    std::queue<std::string> q;
    q.push(station_name_b);
    
    bool find = false;
    int step = 0;
    
    while (!q.empty()) {
        std::string current_station = q.front();
        q.pop();
        
        for (const auto& neighbor : adjacencyList[current_station]) {
            if (!visited[neighbor]) { 
                if(neighbor == station_name_a) {
                    find = true;
                    step = distance[current_station];
                    break;
                } 
                q.push(neighbor);
                visited[neighbor] = true;
                distance[neighbor] = distance[current_station] + 1;
            }
        }
        if(find) {
            break;
        }
    }
    std::vector<std::string> shortest_path;
    if(!find) {
        return shortest_path;
    }
    shortest_path.push_back(station_name_a);
    std::string current = station_name_a;
    for(int i = step; i > 0; i--) {
        std::string next = "~";
        for(const auto& neighbor : adjacencyList[current])
            if(distance[neighbor] == i && neighbor < next) 
                next = neighbor;
        current = next;
        shortest_path.push_back(current);
    }
    shortest_path.push_back(station_name_b);
    
    return shortest_path;
}


// [YOUR CODE WILL BE PLACED HERE] 

void Dump(const std::vector<std::string>&);

void Test1(); // Sample1
void Test2(); // AddConnection, IsConnected 
void Test3(); // AddConnection, IsConnected 
void Test4(); // AddConnection, HasPath 
void Test5(); // AddConnection, HasPath
void Test6(); // AddConnection, ShortestPath 
void Test7(); // AddConnection, ShortestPath

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6, Test7};
    f[id-1]();
}

void Dump(const std::vector<std::string>& path) {
    bool isFirst = true;
    for (const std::string& s : path) {
        if (!isFirst) {
            std::cout << " -> ";
        } else {
            isFirst = false;
        }
        std::cout << s;
    }
    std::cout << std::endl;
}

namespace Feis {}

void Test1() {
    Metro m;
    m.AddConnection("TaipeiMainStation", "ShandoTemple");
    m.AddConnection("TaipeiMainStation", "Zhongshan");
    m.AddConnection("TaipeiMainStation", "Ximen");
    m.AddConnection("Shuanglian", "Zhongshan");
    m.AddConnection("Beimen", "Zhongshan");
    m.AddConnection("Beimen", "Ximen");
    m.AddConnection("MinquanWRd", "Shuanglian");
    m.AddConnection("MinquanWRd", "Yuanshan");
    m.AddConnection("MinquanWRd", "ZhongshangElementarySchool");
    m.AddConnection("XingtianTample", "ZhongshangElementarySchool");
    m.AddConnection("XingtianTample", "SongjianNanjing");
    m.AddConnection("ZhongxiaoXinsheng", "SongjianNanjing");
    m.AddConnection("ZhongxiaoXinsheng", "Dongmen");
    m.AddConnection("ShandoTemple", "ZhongxiaoXinsheng");
    m.AddConnection("ZhongxiaoXinsheng", "ZhongxiaoFuxing");
    m.AddConnection("ZhongxiaoFuxing", "SunYatSenMemorialHall");
    m.AddConnection("SunYatSenMemorialHall", "TaipeiCityHall");
    m.AddConnection("TaipeiCityHall", "Yongchun");
    m.AddConnection("Yongchun", "Houshanpi");
    m.AddConnection("Houshanpi", "Kunyang");
    m.AddConnection("Kunyang", "Nanggong");

    std::cout << "01) " << std::boolalpha << m.IsConnected("TaipeiMainStation", "Ximen") << std::endl;

    std::cout << "02) " << std::boolalpha << m.IsConnected("TaipeiMainStation", "Beimen") << std::endl;

    std::cout << "03) " << std::boolalpha << m.IsConnected("TaipeiMainStation", "TaipeiMainStation") << std::endl;

    std::cout << "04) " << std::boolalpha << m.HasPath("TaipeiMainStation", "Beimen") << std::endl;

    std::cout << "05) " << std::boolalpha << m.HasPath("TaipeiMainStation", "FuJenUniversity") << std::endl;

    std::cout << "06) " << std::boolalpha << m.HasPath("TaipeiMainStation", "TaipeiMainStation") << std::endl;

    std::cout << "07) ";
    Dump(m.ShortestPath("TaipeiMainStation", "TaipeiCityHall"));

    std::cout << "08) ";
    Dump(m.ShortestPath("TaipeiMainStation", "Nanggong"));

    std::cout << "09) ";
    Dump(m.ShortestPath("TaipeiMainStation", "SongjianNanjing"));

    m.AddConnection("Zhongshan", "SongjianNanjing");

    std::cout << "10) ";
    Dump(m.ShortestPath("TaipeiMainStation", "SongjianNanjing"));

    std::cout << "11) ";
    Dump(m.ShortestPath("TaipeiMainStation", "Beimen"));
}

void Test2() {}
void Test3() {}
void Test4() {}
void Test5() {}
void Test6() {}
void Test7() {}
/*
01) true
02) false
03) false
04) true
05) false
06) true
07) TaipeiMainStation -> ShandoTemple -> ZhongxiaoXinsheng -> ZhongxiaoFuxing -> SunYatSenMemorialHall -> TaipeiCityHall
08) TaipeiMainStation -> ShandoTemple -> ZhongxiaoXinsheng -> ZhongxiaoFuxing -> SunYatSenMemorialHall -> TaipeiCityHall -> Yongchun -> Houshanpi -> Kunyang -> Nanggong
09) TaipeiMainStation -> ShandoTemple -> ZhongxiaoXinsheng -> SongjianNanjing
10) TaipeiMainStation -> Zhongshan -> SongjianNanjing
11) TaipeiMainStation -> Ximen -> Beimen
*/