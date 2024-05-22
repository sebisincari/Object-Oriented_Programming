#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<algorithm>
#include<variant>
#include<cmath>
#include<queue>
#include<unordered_map>
#include<typeinfo>
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
        friend ostream& operator<<(ostream &out, const coordonate &c);
        ~coordonate();
};

class RegionNotFoundException : public runtime_error {
    public:
        RegionNotFoundException(const string& regionName) : runtime_error("Regiunea " + regionName + " nu a fost găsită") {}
};


class graphStrategy{
    public:
        virtual void addRoad(string city1, string city2, coordonate a,coordonate b) = 0;
        virtual void getDistance(string city1, string city2) = 0;
        virtual string& getRegionName() = 0;
        virtual void setRegionName(const string& newName) = 0;
        virtual void addRoad(string city, coordonate a) = 0;
};

class unorientedGraphStrategy: public graphStrategy{
    private:
        vector<vector<pair<double,int>>> graf;//stocheaza muchiile cu tot cu distanta
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(const string cityName);
    public:
        unorientedGraphStrategy();
        void addRoad(const string city1, const string city2,const coordonate a,const coordonate b) override;
        void getDistance(const string city1,const string city2) override;
        string& getRegionName() override;
        void setRegionName(const string& newName) override;
        void addRoad(const string city, const coordonate a) {}
        friend istream& operator>>(istream &in, unorientedGraphStrategy &c);
        
};

class treeStrategy: public graphStrategy{
    private:
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);
        vector<int> parent;//vector de tati
        vector<double> distanta;//vector de distanta de la fiu la tata
    public:
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
        string& getRegionName() override;
        void setRegionName(const string& newName) override;
        friend istream& operator>>(istream &in, treeStrategy &c);
        void addRoad(const string city, const coordonate a) {}

};

class lineGraphStrategy: public graphStrategy{
    private:
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);
        vector<vector<pair<double,int>>> graf;
        vector<double> partialSum;
        unordered_map<string,int> indexInLine;
        vector<int> line;
        vector<bool> viz;
        void dfs(int i);
        void constructPartialSum();
    public:
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
        string& getRegionName() override;
        void addRoad(const string city, const coordonate a) {}
        void setRegionName(const string& newName) override;
        friend istream& operator>>(istream &in, lineGraphStrategy &c);
};

class orientedGraphStrategy: public graphStrategy{
    private:
        vector<vector<pair<double,int>>> graf;
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);
    public:
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
        string& getRegionName() override;
        void addRoad(const string city, const coordonate a) {}
        void setRegionName(const string& newName) override;
        friend istream& operator>>(istream &in, orientedGraphStrategy &c);
};

class completeGraphStrategy : public graphStrategy{
    private:
        vector<coordonate> coordCities;
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);
    public:
        void addRoad(string city, coordonate a) override;
        void addRoad(string city1, string city2, coordonate a, coordonate b) {}
        void getDistance(string city1, string city2) override;
        string& getRegionName() override;
        void setRegionName(const string& newName) override;
        friend istream& operator>>(istream &in, completeGraphStrategy &c);
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
        void setIstream(istream& newIn);
        istream& getIstream();
        int getIndexOfRegion(const string& regionName);
        void newRegion();
        void readRoad();
        void respondQuerie();
        
        
};


/*-------------------------Definirea functiilor pentru clasa coordonate--------------------------*/

coordonate::coordonate(){
    this->x = new double;
    this->y = new double;
}

coordonate::coordonate(const coordonate &other){
    this->x = new double;
    this->y = new double;
    *this->x = *other.x;
    *this->y = *other.y;
}
void coordonate::operator=(const coordonate &other){
    *this->x = *other.x;
    *this->y = *other.y;
}

double coordonate::operator-(const coordonate &other) const{
    double dx = *this->x - *other.x;
    double dy = *this->y - *other.y;
    return sqrt(dx*dx + dy*dy);
}

istream& operator>>(istream &in, coordonate &c){
    in>>*c.x>>*c.y;
    return in;
}

ostream& operator<<(ostream &out, const coordonate &c){
    out<<*c.x<<' '<<*c.y;
    return out;
}

coordonate::~coordonate(){
    delete x;
    delete y;
}

/*------------------Definirea functiilor pentru clasa unorientedGraphStrategy--------------------*/

istream& operator>>(istream&in,unorientedGraphStrategy& graph){
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    getline(in,graph.regionName);
    int numberOfRoads;
    in>>numberOfRoads;
    
    for(int i=1;i<=numberOfRoads;i+=1){
        string city1,city2;
        coordonate a,b;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city1);
        in>>a;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city2);
        in>>b;
        graph.addRoad(city1,city2,a,b);
    }
    return in;
}


unorientedGraphStrategy::unorientedGraphStrategy(){numberOfCities=0;}

string& unorientedGraphStrategy::getRegionName(){
    return this->regionName;
}

void unorientedGraphStrategy::setRegionName(const string& name){
        this->regionName = name;
}

int unorientedGraphStrategy::findIndex(string cityName){
    if(cityIndex.count(cityName))
        return cityIndex[cityName];
    else
        return -1;

    /*Alternativa fara hash map*/
    // auto it = find(this->citiesName.begin(), this->citiesName.end(), cityName);
    // if (it != this->citiesName.end()) {
    //     return distance(this->citiesName.begin(), it);
    // }
    // return -1;
}

void unorientedGraphStrategy::addRoad(string city1, string city2,coordonate a,coordonate b){
    int city1Index = findIndex(city1);
    int city2Index = findIndex(city2);

    double distance = a - b;//oprator supraincarcat

    if(city1Index == -1){
        this->citiesName.push_back(city1);
        city1Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city1,city1Index));
    }
    if(city2Index == -1){
        this->citiesName.push_back(city2);
        city2Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city2,city2Index));
    }

    this->graf.resize(this->numberOfCities);// segf daca nu ii dam resize

    this->graf[city1Index].emplace_back(make_pair(distance, city2Index));
    this->graf[city2Index].emplace_back(make_pair(distance, city1Index));
    
}

void unorientedGraphStrategy::getDistance(string city1, string city2){
    const int dimension = (this->numberOfCities+1);
    double d[dimension];
    const int INF=1e9;
    priority_queue<pair<double,int>> heap;
    bool viz[dimension]={};

    int start = this->findIndex(city1);
    int destination =this->findIndex(city2);

    int x,y,n=this->numberOfCities;
    double c;

    for(int i=1;i<=dimension;i+=1)
        d[i]=INF;

    d[start]=0;
    heap.push({-0,start});//'-' e pentru min heap
    while (!heap.empty()){
        x=heap.top().second;
        heap.pop();
        if(!viz[x]){
            viz[x]=true;
            for(auto i:this->graf[x]){
                c=i.first;
                y=i.second;
                if(d[y]>d[x]+c)
                    d[y]=d[x]+c,heap.push({-d[y],y});
            }
        }
    }
    
    cout << "\033[A\033[2K\n";

    system("clear");

    if (d[destination]<0 or d[destination]>=INF)
            cout<<"There is no road between the 2 cities!\n";
        else
            cout<<"The minimum distance between these two cities is "<<d[destination]<<'\n';
    
}

/*-----------------------Definirea functiilor pentru clasa treeStrategy--------------------------*/

string& treeStrategy::getRegionName(){
    return this->regionName;
}

void treeStrategy::setRegionName(const string& name){
        this->regionName = name;
    }

istream& operator>>(istream&in,treeStrategy& graph){
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    string aux;
    getline(in,aux);
    graph.setRegionName(aux);
    
    int numberOfRoads;
    in>>numberOfRoads;
    
    for(int i=1;i<=numberOfRoads;i+=1){
        string city1,city2;
        coordonate a,b;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city1);
        //cout<<city1<<'\n';
        in>>a;
        //cout<<a<<'\n';
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city2);
        //cout<<city2<<'\n';
        in>>b;
        //cout<<b<<'\n';
        graph.addRoad(city1,city2,a,b);
    }
    return in;
}

int treeStrategy::findIndex(string cityName){
    if(cityIndex.count(cityName))
        return cityIndex[cityName];
    else
        return -1;
}

void treeStrategy::addRoad(string city1, string city2, coordonate a, coordonate b){
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);

    double distance = a - b;

    if(city1Index == -1){
        this->citiesName.push_back(city1);
        city1Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city1,city1Index));
    }
    if(city2Index == -1){
        this->citiesName.push_back(city2);
        city2Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city2,city2Index));
    }

    this->parent.resize(this->numberOfCities+1);// segf daca nu ii dam resize
    this->distanta.resize(this->numberOfCities+1);//elementele noi sunt 0 by default

    city1Index+=1;
    city2Index+=1;

    if (city2Index <this->numberOfCities)//este deja in vector
        swap(city1Index,city2Index);

    this->parent[city2Index] = city1Index;
    this->distanta[city2Index] = distance;
}

void treeStrategy::getDistance(string city1, string city2){
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);
    int x = city1Index+1;
    int y = city2Index+1;
    double distance = 0;
    const int num = this->numberOfCities+5;
    bool viz[num]={};

    while(this->parent[x]!=0){
        viz[x]=true;        
        distance+=this->distanta[x];
        x=this->parent[x];
    }
    //nodul x e radacina

    while(this->parent[y]!=0 and !viz[y])
    {
        viz[y]=true;
        distance+=this->distanta[y];
        y=this->parent[y];
    }
    //nodul y se afla la intersectie

    if(this->parent[y]==0 and this->parent[x]==0 and x!=y){
        cout << "\033[A\033[2K\n";
        system("clear");
        cout<<"There is no road between the 2 cities!\n";
        return;
    }

    while(this->parent[y]!=0){
        distance-=this->distanta[y];
        y=this->parent[y];
    }
    //scadem surplusul de la intersectie pana la radacina


    cout << "\033[A\033[2K\n";
    system("clear");
    cout<<"The minimum distance between these two cities is "<<distance<<'\n';
}

/*--------------------Definirea functiilor pentru clasa lineGraphStrategy------------------------*/

istream& operator>>(istream&in,lineGraphStrategy& graph){
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string aux;
    getline(in,aux);
    graph.setRegionName(aux);
    int numberOfRoads;
    in>>numberOfRoads;
    
    for(int i=1;i<=numberOfRoads;i+=1){
        string city1,city2;
        coordonate a,b;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city1);
        in>>a;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city2);
        in>>b;
        graph.addRoad(city1,city2,a,b);
    }
    return in;
}

void lineGraphStrategy::setRegionName(const string& name){
        this->regionName = name;
    }

string& lineGraphStrategy::getRegionName(){
    return this->regionName;
}

int lineGraphStrategy::findIndex(string cityName){
    if(cityIndex.count(cityName))
        return cityIndex[cityName];
    else
        return -1;
}

void lineGraphStrategy::dfs(int i){
    this->viz[i]=true;
    this->line.push_back(i);
    this->indexInLine.insert(pair<string,int>(this->citiesName[i],this->line.size()-1));
    for(auto j:this->graf[i])
        if(!this->viz[j.second]){
            this->partialSum[j.second]=this->partialSum[i]+j.first;
            dfs(j.second);
        }
            
}

void lineGraphStrategy::constructPartialSum(){
    this->viz.clear();
    this->viz.resize(this->numberOfCities);
    this->line.clear();
    this->indexInLine.clear();
    
    int start = -1;
    for(auto i : this->graf)
    {
        start+=1;
        if(i.size() == 1)
            break;
    }
    dfs(start);
}

void lineGraphStrategy::addRoad(string city1, string city2, coordonate a, coordonate b){
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);

    double distance = a - b;//oprator supraincarcat

    if(city1Index == -1){
        this->citiesName.push_back(city1);
        city1Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city1,city1Index));
    }
    if(city2Index == -1){
        this->citiesName.push_back(city2);
        city2Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city2,city2Index));
    }

    this->graf.resize(this->numberOfCities);// segf daca nu ii dam resize
    this->partialSum.resize(this->numberOfCities);

    this->graf[city1Index].emplace_back(make_pair(distance, city2Index));
    this->graf[city2Index].emplace_back(make_pair(distance, city1Index));

    constructPartialSum();

}

void lineGraphStrategy::getDistance(string city1, string city2)
{
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);

    if(this->viz[this->indexInLine[city1]] and this->viz[this->indexInLine[city2]])
    {
        cout << "\033[A\033[2K\n";
        system("clear");
        cout<<"The minimum distance between these two cities is "<<abs(this->partialSum[this->indexInLine[city1]]-this->partialSum[this->indexInLine[city2]])<<'\n';
    }
    else
    {
        cout << "\033[A\033[2K\n";
        system("clear");
        cout<<"There is no road between the 2 cities!\n";
    }
    
}

/*---------------------Definirea functiilor pentru clasa orientedGraphStrategy-------------------*/

istream& operator>>(istream&in,orientedGraphStrategy& graph){
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string aux;
    getline(in,aux);
    graph.setRegionName(aux);
    int numberOfRoads;
    in>>numberOfRoads;
    
    for(int i=1;i<=numberOfRoads;i+=1)
    {
        string city1,city2;
        coordonate a,b;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city1);
        in>>a;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city2);
        in>>b;
        graph.addRoad(city1,city2,a,b);
    }
    return in;
}

void orientedGraphStrategy::setRegionName(const string& name){
        this->regionName = name;
    }

string& orientedGraphStrategy::getRegionName(){
    return this->regionName;
}

int orientedGraphStrategy::findIndex(string cityName){
    if(cityIndex.count(cityName))
        return cityIndex[cityName];
    else
        return -1;
}

void orientedGraphStrategy::addRoad(string city1, string city2, coordonate a, coordonate b)
{
    int city1Index = findIndex(city1);
    int city2Index = findIndex(city2);

    double distance = a - b;//oprator supraincarcat

    if(city1Index == -1)
    {
        this->citiesName.push_back(city1);
        city1Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city1,city1Index));
    }
    if(city2Index == -1)
    {
        this->citiesName.push_back(city2);
        city2Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city2,city2Index));
    }

    this->graf.resize(this->numberOfCities);// segf daca nu ii dam resize

    this->graf[city1Index].emplace_back(make_pair(distance, city2Index));
}

void orientedGraphStrategy::getDistance(string city1, string city2)
{
    const int dimension = (this->numberOfCities+1);
    double d[dimension];
    const int INF=1e9;
    priority_queue<pair<double,int>> heap;
    bool viz[dimension]={};

    int start = this->findIndex(city1);
    int destination =this->findIndex(city2);

    int x,y,n=this->numberOfCities;
    double c;

    for(int i=1;i<=dimension;i+=1)
        d[i]=INF;

    d[start]=0;
    heap.push({-0,start});//'-' e pentru min heap
    while (!heap.empty())
    {
        x=heap.top().second;
        heap.pop();
        if(!viz[x])
        {
            viz[x]=true;
            for(auto i:this->graf[x])
            {
                c=i.first;
                y=i.second;
                if(d[y]>d[x]+c)
                    d[y]=d[x]+c,heap.push({-d[y],y});
            }
        }
    }
    
    cout << "\033[A\033[2K\n";

    system("clear");

    if (d[destination]<0)
            cout<<"There is no road between the 2 cities!\n";
        else
            cout<<"The minimum distance between these two cities is "<<d[destination]<<'\n';
    
}

/*----------------Definirea functiilor pentru clasa completeGraphStrategy-----------------------*/

istream& operator>>(istream&in,completeGraphStrategy& graph){
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    string aux;
    getline(in,aux);
    graph.setRegionName(aux);
    int numberOfRoads;
    in>>numberOfRoads;
    
    for(int i=1;i<=numberOfRoads;i+=1)
    {
        string city;
        coordonate a;
        in.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(in,city);
        in>>a;
        //in.ignore(numeric_limits<streamsize>::max(), '\n');

        graph.addRoad(city,a);
    }
    return in;
}

void completeGraphStrategy::setRegionName(const string& name){
        this->regionName = name;
    }

string& completeGraphStrategy::getRegionName(){
    return this->regionName;
}

int completeGraphStrategy::findIndex(string cityName){
    if(cityIndex.count(cityName))
        return cityIndex[cityName];
    else
        return -1;
}

void completeGraphStrategy::addRoad(string city1, coordonate a)
{
    int city1Index = this->findIndex(city1);

    if(city1Index == -1)
    {
        this->citiesName.push_back(city1);
        city1Index = this->citiesName.size()-1;
        this->numberOfCities+=1;
        this->cityIndex.insert(pair<string,int>(city1,city1Index));
        this->coordCities.push_back(a);
    }
}

void completeGraphStrategy::getDistance(string city1, string city2)
{
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);
    coordonate a = this->coordCities[city1Index];;
    coordonate b = this->coordCities[city2Index];
    double distance = a - b;

    cout << "\033[A\033[2K\n";
    system("clear");
    cout<<"The minimum distance between these two cities is "<<distance<<'\n';
}

/*-----------------------------Definirea functiilor pentru clasa map-----------------------------*/

map::map(const map&other){
    this->regions = new graphStrategy*[10001];

    this->in = other.in;
    for(int i=1;i<=numberOfRegions;i+=1)
        *regions[i] = *other.regions[i];
}

void map::operator=(const map &other){
    this->in = other.in;
    for(int i=1;i<=numberOfRegions;i+=1)
        *regions[i] = *other.regions[i];
}

istream& map::getIstream(){
    return *this->in;
}

map::~map(){
    delete[] regions;
}

void map::setIstream(istream& newIn){
    this->in = &newIn;
}


int map::getIndexOfRegion(const string& regionName){
    for(int i=1;i<=numberOfRegions;i+=1)
        if(regions[i]->getRegionName() == regionName)
            return i;
    throw RegionNotFoundException(regionName);
}

void map::newRegion(){
    int opNum;
    cout<<"Choose the type of region you want to add: \n";
    cout<<"1 -> Graph\n";
    cout<<"2 -> Tree\n";
    cout<<"3 -> Line Graph\n";
    cout<<"4 -> Oriented Graph\n";
    cout<<"5 -> Complete Graph\n";
    *in>>opNum;
    cout<<"Introduce the entire region: [region name, number of roads and the roads]\n";
    switch (opNum)
    {
        case 1: 
            regions[++numberOfRegions] = new unorientedGraphStrategy;
            *in>>*static_cast<unorientedGraphStrategy*>(regions[numberOfRegions]);
            break;
        case 2:
            regions[++numberOfRegions] = new treeStrategy;
            *in>>*static_cast<treeStrategy*>(regions[numberOfRegions]);
            break;
        case 3:
            regions[++numberOfRegions] = new lineGraphStrategy;
            *in>>*static_cast<lineGraphStrategy*>(regions[numberOfRegions]);
            break;
        case 4:
            regions[++numberOfRegions] = new orientedGraphStrategy;
            *in>>*static_cast<orientedGraphStrategy*>(regions[numberOfRegions]);
            break;
        case 5:
            regions[++numberOfRegions] = new completeGraphStrategy;
            *in>>*static_cast<completeGraphStrategy*>(regions[numberOfRegions]);
            break;
        default:
            break;
    }
}

void map::readRoad(){
    string city1,city2;
    coordonate a,b;
    string regionName;
    (*in).ignore(numeric_limits<streamsize>::max(), '\n');
    getline(*in,regionName);
    //cout<<regionName<<'\n';
    getline(*in,city1);
    //cout<<city1<<'\n';
    *in>>a;
    //cout<<a<<'\n';
    
    if(typeid(*regions[getIndexOfRegion(regionName)]) != typeid(completeGraphStrategy)){
        (*in).ignore(numeric_limits<streamsize>::max(), '\n');
        getline(*in,city2);
        //cout<<city2<<'\n';
        *in>>b;
        //cout<<b<<'\n';
    }

    try{
        const int regionIndex = getIndexOfRegion(regionName);
        if(typeid(*regions[getIndexOfRegion(regionName)]) == typeid(completeGraphStrategy))
            regions[regionIndex]->addRoad(city1,a);
        else
            regions[regionIndex]->addRoad(city1,city2,a,b);

    } catch (const RegionNotFoundException& e){
        cerr << "Eroare: " << e.what() << std::endl;
    }
}

void map::respondQuerie(){
    string city1,city2;
    string regionName;
    (*in).ignore(numeric_limits<streamsize>::max(), '\n');
    getline(*in,regionName);
    //cout<<regionName<<'\n';
    getline(*in,city1);
    //cout<<city1<<'\n';
    //(*in).ignore(numeric_limits<streamsize>::max(), '\n');
    getline(*in,city2);
    //cout<<city2<<'\n';

    try{
        const int regionIndex = getIndexOfRegion(regionName);
        regions[regionIndex]->getDistance(city1,city2);

    } catch (const RegionNotFoundException& e){
        cerr << "Eroare: " << e.what() << std::endl;
    }
}

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
    
    ifstream fin("oriented_graph.txt");
    
    if(inputType())
        harta.setIstream(cin);
    else
        harta.setIstream(fin);

    interface(harta.getIstream(),harta);

    cout << "\033[A\033[2K\n";
    fin.close();
    return 0;
}
