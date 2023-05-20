#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

template <typename K, typename T>
class HashNode{
    private:
        K key;
        vector<T> data;
    public:
        HashNode():key(0){}
        HashNode(K key, T data){
            this->key = key;
            this->data.push_back(data);
        }
        template<typename A, typename B> friend class HashTable;
        friend std::ostream &operator<<(std::ostream &os, HashNode<K,T> const &n) { 
            os << n.key << "\t[ ";
            for(auto i: n.data) os << i << " ";
            os << "]";
            return os;
        }
};

template <typename K,typename T>
class HashTable{
    private:
        vector<HashNode<K,T>> table;
        int size;
        int hash(K key){
            return key % size;
        }
    public:
        HashTable(int size){
            this->size = size;
            table.resize(size);
        }
        void insert(int key, T value){
            int index = hash(key);
            if(!table.at(index).key) table.at(index) = HashNode<K,T>(key, value);
            else if(table.at(index).key == key) table.at(index).data.push_back(value);
        }
        void print(){
            cout << "INDEX"<<"\tKEY"<<"\tDATA\n";  
            for (int i = 0; i < size; i++) cout << i << "\t" << table.at(i) << "\n";
        }
};

class User{
private:
    string name;
    int age;
    float weight;
public:
    User(string name, int age, float weight):name(name), age(age), weight(weight){}
    friend std::ostream &operator<<(std::ostream &os, User const &u) { 
        os << "{" << u.name << ", " << u.age << ", " << u.weight << "}";
        return os;
    }
};

int main(int argc, char const *argv[]){
    //ARCHIVOS DE PRUEBA
    ifstream archivo("test");
    int size = 0;
    archivo >> size;
    
    //CREACION DE LA TABLA HASH
    HashTable<int, int> * hash = new HashTable<int, int>(size);

    //LECTURA DE LOS DATOS
    while(true){
        int key, value;
        archivo >> key >> value;
        if(archivo.eof()) break;
        hash->insert(key, value);
    }
    archivo.close();

    //IMPRIMIR LA TABLA HASH
    hash->print();
    return 0;
}

// int main(int argc, char const *argv[]){
//     //ARCHIVOS DE PRUEBA
//     ifstream archivo("test");
//     int size = 0;
//     archivo >> size;
    
//     //CREACION DE LA TABLA HASH
//     HashTable<int,User> * hash = new HashTable<int,User>(size);

//     //LECTURA DE LOS DATOS
//     while(true){
//         int key;
//         string name; int age; float weight;
//         archivo >> key >> name >> age >> weight;
//         if(archivo.eof()) break;
//         hash->insert(key, User(name, age, weight));
//     }
//     archivo.close();

//     //IMPRIMIR LA TABLA HASH
//     hash->print();
//     return 0;
// }