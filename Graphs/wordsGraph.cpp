#include <iostream>
#include<list>
#include<map>
#include <utility>
using namespace std;

template <typename T>
class Vertex{
    private:
        bool visited;
        int id;
        T data;
    public:
        Vertex(int id, T data): id(id), data(data){}
        void addAdy(Vertex *v){
            ady.push_back(v);
        }
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
            v1.addAdy(&v2);
            v2.addAdy(&v1);
            aristas.push_back(make_pair(v1, peso));
            aristas.push_back(make_pair(v2, peso));
        }
        void print(){
            cout << "Nodo"<< "\tData" << "\tPeso" << endl;
            for (auto i : aristas){
                cout << i.first.id << "\t" << i.first.data << "\t" << i.second << endl;
                for(auto j : i.first.ady)
                    cout << j->id << "\t" << j->data << endl;
            }
        }
};

int main(int argc, char const *argv[])
{
    Grafo<int> g;
    g.addEdge(1, 2, 2, 3, 4);
    g.addEdge(2, 3, 3, 4, 5);
    g.print();
    return 0;
}