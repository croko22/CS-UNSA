#include <iostream>
#include<map>
#include<list>
#include <utility>
#include <string>
using namespace std;

bool stringDifmax1(string s1, string s2){
    if(s1.size() != s2.size()) return false;
    int dif = 0;
    for(int i = 0; i < s1.size(); i++){
        if(s1[i] != s2[i]) dif++;
        if(dif > 1) return false;
    }
    return true;
}

template <typename T>
class Vertex{
    private:
        int id;
        T data;
    public:
        Vertex(int id, T data): id(id), data(data){}
        template <typename A> friend class Grafo;
};

//Adyacent list graph
template <typename T>
class Grafo{
    private:
       map<Vertex<T>, list<Vertex<T>>> vertices;
    public:
        Grafo(){}
        void addEdge(int id1, T data1,int id2, T data2, int peso){
            Vertex<T> v1(id1, data1);
            Vertex<T> v2(id2, data2);   
            if(typeid(T) == typeid(string)){
                if(stringDifmax1(data1, data2)) {
                    vertices[v1].push_back(v2);
                    vertices[v2].push_back(v1);
                }
            }
        }
        void print(){
            cout << "Nodo"<< "\tData" << "\tPeso" << "\tAdyacentes" << endl;
            for(auto v: vertices){
                cout << v.first.id << "\t" << v.first.data << "\t" << "\t";
                for(auto ady: v.second){
                    cout << ady.id << " " << ady.data << " ";
                }
                cout << endl;
            }
        }
};

int main(int argc, char const *argv[])
{
    Grafo<string> dif1;
    dif1.addEdge(1, "abc", 2, "abd", 1);
    dif1.addEdge(2, "abd", 3, "acd", 1);
    dif1.addEdge(3, "acd", 4, "ace", 1);

    dif1.print();
    return 0;
}