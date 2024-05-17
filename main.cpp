#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include<variant>
#include<cmath>
#include<queue>
#include<unordered_map>
using namespace std;

class coordonate
{
    private:
        double *x,*y;
    public:
        coordonate(istream &in);
        coordonate(const coordonate &other);
        void operator=(const coordonate &other);
        double operator-(const coordonate &other) const;
        ~coordonate();
};

class graphStrategy{
    public:
        virtual void addRoad(string city1, string city2, coordonate a,coordonate b) = 0;
        virtual void getDistance(string city1, string city2) = 0;
};

class unorientedGraphStrategy: public graphStrategy{
    private:
        vector<vector<pair<double,int>>> graf;//stocheaza muchiile cu tot cu distanta
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);//de verificat daca se mosteneste 
    public:
        void addRoad(string city1, string city2, coordonate a,coordonate b) override;
        void getDistance(string city1, string city2) override;
        friend istream& operator>>(istream &in, coordonate &c);
};

class map{
    private:
        graphStrategy **strategy;
    public:
        map(graphStrategy **strategy) : strategy(strategy) {}
        map(const map &other);
        ~map();
        void operator=(const map &other);
        void setStrategy(graphStrategy *strategy);
        void readCity();
        void readRoads();
        void respondQueries();
        
        
};
//citim operatia
//1 --> harta->readCity();
//2 --> harta->readRoads();
//3 --> harta->respondQueries();
// in harta->readCity() se citeste numele si tipul de graf
// in harta->readRoads() se citesc oras coord oras coord regiune
int main()
{
    graphStrategy **graph_strategy = new graphStrategy*[10001];
    map harta(graph_strategy);
    return 0;
}