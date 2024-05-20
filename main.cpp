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

/*--------------------------------------Definirea claselor---------------------------------------*/


class coordonate
{
    private:
        double *x,*y;
    public:
        coordonate();
        coordonate(const coordonate &other);
        void operator=(const coordonate &other);
        double operator-(const coordonate &other) const;
        friend istream& operator>>(istream &in, coordonate &c);
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
        int findIndex(const string cityName);//de verificat daca se mosteneste 
    public:
        void addRoad(const string city1, const string city2,const coordonate a,const coordonate b) override;
        void getDistance(const string city1,const string city2) override;
        friend istream& operator>>(istream &in, unorientedGraphStrategy &c);
};

class map{
    private:
        graphStrategy **regions;
        istream *in;
        int numberOfRegions;
    public:
        map(graphStrategy **strategy) : regions(strategy) { numberOfRegions = 0; }
        map(const map &other);
        ~map();
        void operator=(const map &other);
        void setStrategy(graphStrategy *strategy);
        void setIstream(istream& newIn);
        istream& getIstream();
        void newRegion();
        void readRoad();
        void respondQuerie();
        
        
};
//citim operatia
//1 --> harta->readCity();
//2 --> harta->readRoads();
//3 --> harta->respondQueries();
// in harta->readCity() se citeste numele si tipul de graf
// in harta->readRoads() se citesc oras coord oras coord regiune
/*------------------Definirea functiilor pentru clasa unorientedGraphStrategy-------------------*/

istream& operator>>(istream&in,unorientedGraphStrategy &graph){
    getline(in,graph.regionName);
    in>>graph.numberOfCities;

    for(int i=0;i<graph.numberOfCities;i++)
    {
        string city1,city2;
        coordonate a,b;
        in>>city1>>a>>city2>>b;
        graph.addRoad(city1,city2,a,b);
    }
    return in;
}


/*-----------------------------Definirea functiilor pentru clasa map----------------------------*/

map::map(const map&other){}

map::~map(){}

void map::operator=(const map &other){}

void map::setIstream(istream& newIn){
    this->in = &newIn;
}

istream& map::getIstream(){
    return *this->in;
}

void map::newRegion(){
    int opNum;
    cout<<"Choose the type of region you want to add: \n";
    cout<<"1 -> Graph\n";
    cout<<"2 -> Tree\n";
    cout<<"3 -> Complete Graph\n";
    cout<<"4 -> Line Graph\n";
    cout<<"5 -> Oriented Graph\n";
    *in>>opNum;
    switch (opNum)
    {
        case 1: 
            *in >> *(static_cast<unorientedGraphStrategy*>(regions[++numberOfRegions]));
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 4:
            ;
            break;
        case 5:
            
            break;
        default:
            break;
    }
}

void map::respondQuerie(){}

/*----------------------------------Main si functiile aferente-----------------------------------*/
    
bool inputType(){
    char input_type;
    bool type_input_valid=false, keyboard;
    int caseNum;
    system("clear");
    while(!type_input_valid)
    {

        cout<<"\nSpecify the data input method: keyboard or file: [k/f] ";
        cin>>input_type;
        if (input_type == 'k' or input_type == 'K')
            type_input_valid=true,keyboard=true;
        else
            if(input_type == 'f' or input_type == 'F')
                type_input_valid=true,keyboard=false; 
            else
                cout<<"Invalid input!!\n";
    }
    return keyboard;
            
}

void interface(istream &in, map &harta){
    int opNum;
    cout<<"Choose an operation [1/2/3]: \n";
    in>>opNum;
    bool continueMenu = true;
    while (continueMenu)
    {
        switch (opNum)
        {
        case 1:
            harta.newRegion();
            break;
        case 2:
            harta.readRoad();
            break;
        case 3:
            harta.respondQuerie();
            break;
        default:
            break;
        }
        cout<<"Do you want to continue? [1/0]\n";
        in>>continueMenu;
        if(continueMenu)
        {
            cout<<"Choose an operation [1/2/3]: \n";
            in>>opNum;
        }
    }
}

int main(){
    graphStrategy **graph_strategy = new graphStrategy*[10001];
    map harta(graph_strategy);
    
    ifstream fin("input.txt");
    
    if(inputType())
        harta.setIstream(cin);
    else
        harta.setIstream(fin);

    interface(harta.getIstream(),harta);

    fin.close();
    return 0;
}
//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