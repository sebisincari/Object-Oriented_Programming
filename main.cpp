#include <iostream>
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
    public:
        void addRoad(string city1, string city2, coordonate a,coordonate b) override;
        void getDistance(string city1, string city2) override;
};

class map{
    private:
        graphStrategy *strategy;
    public:
        map(graphStrategy *strategy) : strategy(strategy) {}
        map(const map &other);
        ~map();
        void operator=(const map &other);
        friend istream& operator>>(istream &in, coordonate &c);
        
        
};

int main()
{
    graphStrategy *graph_strategy = new unorientedGraphStrategy();
    map harta(graph_strategy);
    return 0;
}