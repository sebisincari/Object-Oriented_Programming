#include<bits/stdc++.h>
using namespace std;
using namespace chrono;
using namespace this_thread;
ifstream fin("input.txt");
struct str
{
    char ch;
    str* next;
};

struct vectorStr
{
    int index;
    str* name;
    vectorStr *next;
};
struct roads
{
    int first;
    int second;
    double dist;
    roads *next;
};
struct vectorRegions
{
    str* regName;
    vectorStr* citiesName;
    int citiesCnt;
    roads * edges;
    bool unidirectional;
    vectorRegions *next;
};

void loading()
{
    char loading[]="[..........] .......................................................... ";
    for(int i=1;i<=10;i+=1)
    {
        
        loading[i]='#';
        cout<<loading;
        cout<<i*10<<"% processed data \n";
        sleep_for(milliseconds(250));
        //cout << "\033[2K\r" << flush;
    }

    sleep_for(seconds(1));

    cout<<"\nProcessed successfully!\n";
}

int cityInRegion(str* name,vectorRegions* start)
{
    vectorStr* cities = start->citiesName;
    while (cities!=NULL)
    {
        str * aux,*auxx;
        aux = cities->name;
        auxx = name;
        while(aux!=NULL and auxx!= NULL and aux->ch==auxx->ch)
            aux=aux->next,auxx=auxx->next;
        if (aux == NULL and auxx ==NULL)
            return cities->index;
        cities=cities->next;
    }

    return -1;
}

void addChBack(str *& start, str *& end, char c)
{
    str* chr=new str;
    chr->ch=c;
    chr->next=NULL;
    if(end==NULL)
        end=chr,start=chr;
    else
        end->next=chr,end=chr;

}

void strReadFile(str*&strStart, str*&strEnd)
{
    char c[1000];
    // if (fin.peek() != EOF)
    //     fin.ignore(numeric_limits<streamsize>::max(), '\n');
    fin.getline(c,999);
    //getline(fin,c);
    int len=strlen(c);
    //cout<<c<<endl;
    for(int i=0;i<len;i+=1)
        addChBack(strStart,strEnd,c[i]);
}

void strRead(str*&strStart, str*&strEnd)
{
    char c[1000];
    if(cin.peek() == '\n')
        cin.get();
    cin.getline(c,999);
    int len=strlen(c);
    for(int i=0;i<len;i+=1)
        addChBack(strStart,strEnd,c[i]);
}

int addCityInRegion(vectorRegions*&region,str*cityName)
{
    vectorStr * cities = region->citiesName;
    if (cities!=NULL)
    {
        //cout<<"cities!null\n";
        while (cities->next!=NULL)
            cities=cities->next;
        //cout<<"a trecut de while\n";

        vectorStr* newCity=new vectorStr;
        newCity->index = cities->index+1;
        newCity->next=NULL;
        newCity->name=cityName;
        cities->next=newCity;
        //cout<<"a ajuns la final\n";

        return newCity->index;
    }
    else
    {
        vectorStr* newCity=new vectorStr;
        newCity->index = 1;
        newCity->next=NULL;
        newCity->name=cityName;
        region->citiesName=newCity;

        return 1;
    }
        
    
}

double edgeCost(double ax, double ay, double bx, double by) 
{
    return (sqrt((bx - ax)*(bx - ax) + (by - ay)*(by - ay)));
}

void addEdge(vectorRegions*&region,int index1,int index2,double ax,double ay, double bx, double by)
{
    double cost = edgeCost(ax,ay,bx,by);
    roads* edge = new roads;
    roads*q=region->edges;
    edge->dist=cost;
    edge->first=index1;
    edge->second=index2;
    edge->next=NULL;
    if(q==NULL)
        region->edges=edge;
    else
    {
        while (q->next!=NULL)
            q=q->next;
        q->next=edge;
    }
    if(region->unidirectional == 0)
    {
        roads* edge = new roads;
        roads*q=region->edges;
        edge->dist=cost;
        edge->first=index2;
        edge->second=index1;
        edge->next=NULL;
        if(q==NULL)
            q=edge;
        else
        {
            while (q->next!=NULL)
                q=q->next;
            q->next=edge;
        }
    }
}

//pointerul catre regiunea ce are acelasi nume cu cel citit
vectorRegions* findRegion(vectorRegions* regs, str*regName)
{
    str*aux,*auxx;
    while (regs!=NULL)
    {
        aux=regs->regName;
        auxx = regName;
        while(aux!=NULL and auxx!=NULL and aux->ch==auxx->ch)
            aux=aux->next,auxx=auxx->next;
        if (aux == NULL and auxx==NULL)
            return regs;
        regs=regs->next;
    }

    return NULL;
}


void createNewRoad(vectorRegions *& regsBegin,vectorRegions *& regsEnd,bool file)
{
    str *As,*Bs,*Rs,*Ae,*Be,*Re;
    float ax,ay,bx,by;
    vectorRegions* region;
    As=NULL,Ae=NULL,Bs=NULL,Be=NULL,Rs=NULL,Re=NULL;

    if(!file)
    {
        //citim datele de la tastatura 
        cout<<"Enter the name of the first city: ";
        strRead(As,Ae);
        cout<<"...and x coordinate: ";
        cin>>ax;
        cout<<"...and y coordinate: ";
        cin>>ay;
        //cout<<'\n';

        cout<<"Enter the name of the second city: ";
        strRead(Bs,Be);
        cout<<"...and x coordinate: ";
        cin>>bx;
        cout<<"...and y coordinate: ";
        cin>>by;
        //cout<<'\n';

        cout<<"Enter the name of the region: ";
        strRead(Rs,Re);
    }
    else
    {
        //citim datrele din fisier
        fin.ignore(numeric_limits<streamsize>::max(), '\n');
        strReadFile(As,Ae);
        fin>>ax;
        fin>>ay;

        fin.ignore(numeric_limits<streamsize>::max(), '\n');
        strReadFile(Bs,Be);
        fin>>bx;
        fin>>by;

        fin.ignore(numeric_limits<streamsize>::max(), '\n');
        strReadFile(Rs,Re);
    }

    
    
    region = findRegion(regsBegin,Rs);
    //regiunea in care se adauga muchia noua
    int index1 = cityInRegion(As,region);
    if(index1 == -1)
        region->citiesCnt+=1 ,index1 = addCityInRegion(region,As);
    //cout<<index1<<'\n';
    int index2 = cityInRegion(Bs,region);
    //cout<<index1<<'\n';
    if(index2 == -1)
        region->citiesCnt+=1 ,index2 = addCityInRegion(region,Bs);
    //cout<<index2<<'\n';
    addEdge(region,index1,index2,ax,ay,bx,by);

}

void createNewRegion(vectorRegions *& regBegin, vectorRegions *& regEnd, str* nameS,str *nameE,bool file)
{
    vectorRegions* newReg = new vectorRegions;
    vectorStr * cities = NULL;
    roads * edges = NULL;
    bool unidir;
    if(!file)
    {
        cout<<"Is it bidirectional?: [0/1] ";
        cin>>unidir;
    }
    else
    {
        fin>>unidir;
    }
    //cout<<unidir<<" unidir\n";   
    newReg->next=NULL;
    newReg->citiesName=cities;
    newReg->edges = edges;
    newReg->citiesCnt = 0;
    newReg->regName = nameS;
    newReg->unidirectional=(!unidir);
    if(regEnd == NULL)
        regEnd = newReg,regBegin = newReg;
    else
        regEnd->next=newReg,regEnd=newReg;
}

void constructGraph(vectorRegions*region, str*As,str*Bs)
{
    const int dimension = (region->citiesCnt+1);
    double d[dimension];
    const int INF=1e9;
    vector<pair<double,int>> g[dimension];
    priority_queue<pair<double,int>> heap;
    bool viz[dimension]={};


    int start = cityInRegion(As,region);
    int destination = cityInRegion(Bs,region);
    int x,y,n;
    double c;

    // cout<<start<<'\n';
    // cout<<destination<<'\n';

    n=region->citiesCnt;
    roads* edges = region->edges;
    while (edges!=NULL)
    {
        x=edges->first;
        y=edges->second;
        c=edges->dist;
        g[x].emplace_back(c,y);
        edges=edges->next;
    }

    for(int i=1;i<=dimension;i+=1)
        d[i]=INF;
    // for(int i=1;i<=dimension;i+=1)
    //     cout<<d[i]<<' ';
    // cout<<'\n';
    d[start]=0;
    heap.push({-0,start});//'-' e pentru min heap
    while (!heap.empty())
    {
        x=heap.top().second;
        heap.pop();
        if(!viz[x])
        {
            viz[x]=true;
            for(auto i:g[x])
            {
                c=i.first;
                y=i.second;
                if(d[y]>d[x]+c)
                    d[y]=d[x]+c,heap.push({-d[y],y});

            }
        }
        
    }
    //for(int i=1;i<=dimension;i+=1)
    //    cout<<d[i]<<' ';
    //cout<<'\n';
    
    loading();

    if (d[destination]>=INF)
            cout<<"There is no road between the 2 cities!\n";
        else
            cout<<"The minimum distance between these two cities is "<<d[destination]<<'\n';
}

void minimumDistance(vectorRegions*&regsBegin, bool file)
{
    str *As,*Bs,*Rs,*Ae,*Be,*Re;
    As=NULL,Ae=NULL,Bs=NULL,Be=NULL,Rs=NULL,Re=NULL;
    if(!file)
    {
        cout<<"Enter the name of the start city: ";
        strRead(As,Ae);

        cout<<"Enter the name of the second city: ";
        strRead(Bs,Be);

        cout<<"Enter the name of the region: ";
        strRead(Rs,Re);
    }
    else
    {
        fin.ignore(numeric_limits<streamsize>::max(), '\n');
        strReadFile(As,Ae);
        strReadFile(Bs,Be);
        strReadFile(Rs,Re);
    }
    
    vectorRegions* region = findRegion(regsBegin,Rs);

    constructGraph(region,As,Bs);
}

void continueIntroduceingKeyboard(vectorRegions*&regsBegin,vectorRegions*&regsEnd,str*&regNameS,str*&regNameE)
{
    bool cont=true;
    int op_type;
    while (cont)
    {
        cout<<"Continue introduceing data? [0/1] ";
        cin>>cont;
        if(!cont)
            break;
        cout<<"Enter the type of operation: [1/2/3] ";
        cin>>op_type;
        switch (op_type)
        {
        case 1:
            minimumDistance(regsBegin,0);
            break;
        case 2:
            createNewRoad(regsBegin,regsEnd,0);
            break;
        case 3:
            cout<<"Introduce the name of the region: ";
            strRead(regNameS,regNameE);
            createNewRegion(regsBegin,regsEnd,regNameS,regNameE,0);
            break;
        default:
            cout<<"Invalid data! Please repeat!";
            break;
        }
    }
    
}

void continueIntroduceingFile(vectorRegions*&regsBegin,vectorRegions*&regsEnd,str*&regNameS,str*&regNameE)
{
    bool cont=true;
    int op_type;

    while (cont)
    {
        fin>>cont;
        if(!cont)
            break;
        fin>>op_type;
        switch (op_type)
        {
            case 1:
                minimumDistance(regsBegin,1);
                break;
            case 2:
                createNewRoad(regsBegin,regsEnd,1);
                break;
            case 3:
                fin.ignore(numeric_limits<streamsize>::max(), '\n');
                strReadFile(regNameS,regNameE);
                createNewRegion(regsBegin,regsEnd,regNameS,regNameE,1);
                break;
            default:
                cout<<1<<" "<<op_type<<endl;
                exit(0);
        }
    }
    
}

void keybordRead(vectorRegions*&regsBegin,vectorRegions*&regsEnd,str*&regNameS,str*&regNameE)
{
    int op_type;
    bool valid_op_type=false;
    system("clear");

    cout<<"We will code the operation of querying the minimum distance between region A and B with 1,\n";
    cout<<"the introduction of the road between region A and B with 2 si ";
    cout<<"creating a new region with 3.\n\n";
    
    while(!valid_op_type)
    {
        cout<<"Enter the type of operation: [1/2/3] ";
        cin>>op_type;
        if(op_type != 1 and op_type != 2 and op_type != 3)
            cout<<"Invalid number!\n";
        else
            valid_op_type = true;
    }
    switch (op_type)
    {
        case 1:
            minimumDistance(regsBegin,0);
            break;
        case 2:
            createNewRoad(regsBegin,regsEnd,0);
            break;
        case 3:
            cout<<"Introduce the name of the region: ";
            strRead(regNameS,regNameE);
            createNewRegion(regsBegin,regsEnd,regNameS,regNameE,0);
            break;
        default:
            cout<<"Invalid data! Please repeat!";
            break;
    }
    continueIntroduceingKeyboard(regsBegin,regsEnd,regNameS,regNameE);
}

void fileRead(vectorRegions*&regsBegin,vectorRegions*&regsEnd,str*&regNameS,str*&regNameE)
{
    int op_type;
    bool valid_op_type=false;
    system("clear");
    cout<<"We will code the operation of querying the minimum distance between region A and B with 1,\n";
    cout<<"the introduction of the road between region A and B with 2 si ";
    cout<<"creating a new region with 3.\n\n";

    cout<<"Operation 1 is followed by a string representing the name of the region, then if it is a \n";
    cout<<"region with bidirectional roads (0/1).\n\n";

    cout<<"Operation 2 is followed by the names of 2 cities each (after each city its coordinates) and at the end\n";
    cout<<"the name of the region they belong to, each on one line.\n\n";
    
    cout<<"Operation 3 is followed by 3 strings, each on one line, representing the city of origin, the city of\n";
    cout<<"destination and the region of which they belong.\n\n";

    cout<<"After each entry of the operation with all the related data, it must be entered if more data follows \n";
    cout<<"or the reading is ready (0/1).\n\n";
    
    cout<<"Continue? [0/1] ";
    bool cnt;
    cin>>cnt;

    if(cnt)
    {
        while(!valid_op_type)
        {
            fin>>op_type;
            //cout<<op_type<<"fileread optype\n";
            if(op_type != 1 and op_type != 2 and op_type != 3)
            {
                cout<<"Invalid operation type!\n";
                exit(0);
            }
                
            else
                valid_op_type = true;
        }
        switch (op_type)
        {
            case 1:
                minimumDistance(regsBegin,1);
                break;
            case 2:
                createNewRoad(regsBegin,regsEnd,1);
                break;
            case 3:
                fin.ignore(numeric_limits<streamsize>::max(), '\n');
                strReadFile(regNameS,regNameE);
                createNewRegion(regsBegin,regsEnd,regNameS,regNameE,1);
                break;
            default:
                //cout<<2;
                exit(0);
        }
        //cout<<"Aici";
        continueIntroduceingFile(regsBegin,regsEnd,regNameS,regNameE);
    }
    else
    {
        //cout<<3;
        exit(0);
    }
        
    
}

void initialInterface(vectorRegions*&regsBegin,vectorRegions*&regsEnd,str*&regNameS,str*&regNameE)
{
    char input_type;
    bool type_input_valid=false;
    int caseNum;
    system("clear");
    while(!type_input_valid)
    {

        cout<<"\nSpecify the data input method: keyboard or file: [k/f] ";
        cin>>input_type;
        if (input_type == 'k' or input_type == 'K')
        {
            type_input_valid=true;
            keybordRead(regsBegin,regsEnd,regNameS,regNameE);
        }
                
        else
            if(input_type == 'f' or input_type == 'F')
            {
                type_input_valid=true; 
                //cout<<"Format\n";
                fileRead(regsBegin,regsEnd,regNameS,regNameE);
                
            }
                  
            else
                cout<<"Invalid input!!\n";
            
    }
}

int main()
{
    int caseNum;
    vectorRegions *regsBegin = NULL,* regsEnd =NULL;
    str *regNameS=NULL,*regNameE=NULL;
    bool reading = true;
    initialInterface(regsBegin,regsEnd,regNameS,regNameE);
    
   
    return 0;
}