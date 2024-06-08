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

/*---------------------------Definirea claselor-----------------------------*/

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

class graph
{
    private:
        vector<vector<pair<double,int>>> graf;//stocheaza muchiile cu tot cu distanta
    protected:
        string regionName;
        vector<string> citiesName;
        unordered_map<string,int> cityIndex;//un hashmap pentru rapiditatea gasirii indicelui
        int numberOfCities;
        int findIndex(string cityName);//de verificat daca se mosteneste     
    public:
        string regName();
        graph(istream &in);
        virtual void addRoad(string city1, string city2, coordonate a, coordonate b);
        virtual void getDistance(string city1, string city2);
};

class tree : public graph
{
    private:
        vector<int> parent;//vector de tati
        vector<double> distanta;//vector de distanta de la fiu la tata
    public:
        tree(istream &in);
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
};

class completeGraph : public graph
{
    private:
        vector<coordonate> coordCities;
    public:
        completeGraph(istream &in);
        void addRoad(string city1, coordonate a);
        void getDistance(string city1, string city2) override;

};

class lineGraph : public graph
{
    private:
        vector<vector<pair<double,int>>> graf;
        vector<double> partialSum;
        unordered_map<string,int> indexInLine;
        vector<int> line;
        vector<bool> viz;
        void dfs(int i);
        void constructPartialSum();
    public:
        lineGraph(istream &in);
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
};

class orientedGraph : public graph
{
    private:
        vector<vector<pair<double,int>>> graf;
    public:
        orientedGraph(istream &in);
        void addRoad(string city1, string city2, coordonate a, coordonate b) override;
        void getDistance(string city1, string city2) override;
};

/*------------------------Functii pentru coordonate------------------------*/

coordonate::coordonate(istream &in)
{
    this->x = new double;
    this->y = new double;
    in>>*x>>*y;
}

coordonate::coordonate(const coordonate &other)
{
    this->x = new double;
    this->y = new double;
    *this->x = *other.x;
    *this->y = *other.y;
}
void coordonate::operator=(const coordonate &other)
{
    *this->x = *other.x;
    *this->y = *other.y;
}

double coordonate::operator-(const coordonate &other) const
{
    double dx = *this->x - *other.x;
    double dy = *this->y - *other.y;
    return sqrt(dx*dx + dy*dy);
}

coordonate::~coordonate()
{
    delete x;
    delete y;
}

/*---------------------------Functii pentru graf---------------------------*/

graph::graph(istream &in)
{
    string regionName;
    cout<<"Region name: ";
    in>>regionName;
    this->regionName = regionName;
    this->numberOfCities = 0;
}

string graph::regName()
{
    return this->regionName;
}

int graph::findIndex(string cityName)
{
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

void graph::addRoad(string city1, string city2,coordonate a,coordonate b)
{
    int city1Index = graph::findIndex(city1);
    int city2Index = graph::findIndex(city2);

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
    this->graf[city2Index].emplace_back(make_pair(distance, city1Index));
}

void graph::getDistance(string city1, string city2)
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

/*--------------------------Functii pentru arbore--------------------------*/

tree::tree(istream &in) : graph(in) {}

void tree::addRoad(string city1, string city2, coordonate a, coordonate b)
{
    int city1Index = this->findIndex(city1);
    int city2Index = this->findIndex(city2);

    double distance = a - b;

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

    this->parent.resize(this->numberOfCities+1);// segf daca nu ii dam resize
    this->distanta.resize(this->numberOfCities+1);//elementele noi sunt 0 by default

    city1Index+=1;
    city2Index+=1;

    if (city2Index <this->numberOfCities)//este deja in vector
        swap(city1Index,city2Index);

    this->parent[city2Index] = city1Index;
    this->distanta[city2Index] = distance;
}

void tree::getDistance(string city1, string city2)
{
    int city1Index = tree::findIndex(city1);
    int city2Index = tree::findIndex(city2);
    int x = city1Index+1;
    int y = city2Index+1;
    double distance = 0;
    const int num = this->numberOfCities+5;
    bool viz[num]={};

    while(this->parent[x]!=0)
    {
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

    if(this->parent[y]==0 and this->parent[x]==0 and x!=y)
    {
        cout << "\033[A\033[2K\n";
        system("clear");
        cout<<"There is no road between the 2 cities!\n";
        return;
    }

    while(this->parent[y]!=0)
    {
        distance-=this->distanta[y];
        y=this->parent[y];
    }
    //scadem surplusul de la intersectie pana la radacina


    cout << "\033[A\033[2K\n";
    system("clear");
    cout<<"The minimum distance between these two cities is "<<distance<<'\n';
}

/*-----------------------Functii pentru graful complet---------------------*/

completeGraph::completeGraph(istream &in) : graph(in) {}

void completeGraph::addRoad(string city1, coordonate a)
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

void completeGraph::getDistance(string city1, string city2)
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

/*-----------------------Functii pentru graful liniar----------------------*/
lineGraph::lineGraph(istream &in) : graph(in) {}

void lineGraph::dfs(int i)
{
    this->viz[i]=true;
    this->line.push_back(i);
    this->indexInLine.insert(pair<string,int>(this->citiesName[i],this->line.size()-1));
    for(auto j:this->graf[i])
        if(!this->viz[j.second])
        {
            this->partialSum[j.second]=this->partialSum[i]+j.first;
            dfs(j.second);
        }
            
}

void lineGraph::constructPartialSum()
{
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

void lineGraph::addRoad(string city1, string city2, coordonate a, coordonate b)
{
    int city1Index = graph::findIndex(city1);
    int city2Index = graph::findIndex(city2);

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
    this->partialSum.resize(this->numberOfCities);

    this->graf[city1Index].emplace_back(make_pair(distance, city2Index));
    this->graf[city2Index].emplace_back(make_pair(distance, city1Index));

    constructPartialSum();

}

void lineGraph::getDistance(string city1, string city2)
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

/*----------------------Functii pentru graful orientat---------------------*/

orientedGraph::orientedGraph(istream &in) : graph(in) {}

void orientedGraph::addRoad(string city1, string city2, coordonate a, coordonate b)
{
    int city1Index = graph::findIndex(city1);
    int city2Index = graph::findIndex(city2);

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

void orientedGraph::getDistance(string city1, string city2)
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


/*-----------------------------Functii generale----------------------------*/

bool inputType()
{
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

void addRegion(istream &in,vector<variant<graph*, tree*,completeGraph*,lineGraph*,orientedGraph*>> &regions)
{
    int opNum;
    cout<<"Choose the type of region you want to add: \n";
    cout<<"1 -> Graph\n";
    cout<<"2 -> Tree\n";
    cout<<"3 -> Complete Graph\n";
    cout<<"4 -> Line Graph\n";
    cout<<"5 -> Oriented Graph\n";
    in>>opNum;
    graph *g = nullptr;
    tree *t = nullptr;
    completeGraph *c = nullptr;
    lineGraph *l = nullptr;
    orientedGraph *o = nullptr;
    switch (opNum)
    {
        case 1:
            g=new graph(in);
            regions.push_back(g);
            break;
        case 2:
            t=new tree(in);
            regions.push_back(t);
            break;
        case 3:
            c=new completeGraph(in);
            regions.push_back(c);
            break;
        case 4:
            l=new lineGraph(in);
            regions.push_back(l);
            break;
        case 5:
            o=new orientedGraph(in);
            regions.push_back(o);
            break;
        default:
            break;
    }
}

void readCity(istream &in,string &city1,string &city2, coordonate *&a, coordonate *&b)
{
    cout<<"First city: ";
    in>>city1;
    cout<<"Coordinates of the first city: ";
    a = new coordonate(in);
    cout<<"Second city: ";
    in>>city2;
    cout<<"Coordinates of the second city: ";
    b = new coordonate(in);
}

void readCity(istream &in,string &city1,coordonate *&a)
{
    cout<<"City: ";
    in>>city1;
    cout<<"Coordinates of the city: ";
    a = new coordonate(in);
}

void addEdge(istream &in,vector<variant<graph*, tree*,completeGraph*,lineGraph*,orientedGraph*>> &regions)
{
    string city1,city2;
    string reg;
    coordonate *a,*b;

    cout<<"Region: ";
    in>>reg;

    for(auto region:regions)
    {
        if(auto p = get_if<graph*>(&region))
        {
            readCity(in,city1,city2,a,b);
            if((*p)->regName() == reg)
                (*p)->addRoad(city1,city2,*a,*b);
        }
        else if(auto p = get_if<tree*>(&region))
        {
            readCity(in,city1,city2,a,b);
            if((*p)->regName() == reg)
                (*p)->addRoad(city1,city2,*a,*b);
        }   
        else if(auto p = get_if<completeGraph*>(&region))
        {
            readCity(in,city1,a);
            if((*p)->regName() == reg)
                (*p)->addRoad(city1,*a);
        }
        else if(auto p = get_if<lineGraph*>(&region))
        {
            readCity(in,city1,city2,a,b);
            if((*p)->regName() == reg)
                (*p)->addRoad(city1,city2,*a,*b);
        }
        else if(auto p = get_if<orientedGraph*>(&region))
        {
            readCity(in,city1,city2,a,b);
            if((*p)->regName() == reg)
                (*p)->addRoad(city1,city2,*a,*b);
        }
    }
}

void distanceBetweenCities(istream &in,vector<variant<graph*, tree*,completeGraph*,lineGraph*,orientedGraph*>> &regions)
{
    string city1,city2,reg;
    cout<<"First city: ";
    in>>city1;
    cout<<"Second city: ";
    in>>city2;
    cout<<"Region: ";
    in>>reg;

    for(auto region:regions)
    {
        if(auto p = get_if<graph*>(&region))
        {
            if((*p)->regName() == reg)
                (*p)->getDistance(city1,city2);            
        }
        else if(auto p = get_if<tree*>(&region))
        {
            if((*p)->regName() == reg)
                (*p)->getDistance(city1,city2);
        }
        else if(auto p = get_if<completeGraph*>(&region))
        {
            if((*p)->regName() == reg)
                (*p)->getDistance(city1,city2);
        }
        else if(auto p = get_if<lineGraph*>(&region))
        {
            if((*p)->regName() == reg)
                (*p)->getDistance(city1,city2);
        }
        else if(auto p = get_if<orientedGraph*>(&region))
        {
            if((*p)->regName() == reg)
                (*p)->getDistance(city1,city2);
        }
            
    }
}

void menu(istream &in,vector<variant<graph*, tree*,completeGraph*,lineGraph*,orientedGraph*>> &regions)
{
    int opNum;
    cout<<"Choose an operation [1/2/3]: \n";
    in>>opNum;
    bool continueMenu = true;
    while (continueMenu)
    {
        switch (opNum)
        {
        case 1:
            addRegion(in,regions);
            break;
        case 2:
            addEdge(in,regions);
            break;
        case 3:
            distanceBetweenCities(in,regions);
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

int main()
{
    ifstream fin("input.txt");
    istream *in;
    vector<variant<graph*, tree*,completeGraph*,lineGraph*,orientedGraph*>> regions;//vector cu pointerii spre regiuni
    
    if(inputType())
        in = &cin;
    else
        in = &fin;

    menu(*in,regions);

    cout << "\033[A\033[2K";
    fin.close();
 
    return 0;
}