#include <iostream>
using namespace std;
class Nodo{
public:
	int c;
	int g;
	Nodo *next;
	Nodo(){};
	~Nodo(){};
	Nodo(int coef, int grad){
		c = coef;
		g = grad;
		next = NULL;
	}
};
class Polinomio{
    Nodo * head;
public:
    Polinomio(){head = NULL;}
    void insertar(int coef, int grad){
        Nodo* newNodo = new Nodo(coef,grad);
        newNodo->next = head;
        head = newNodo;
    }
    void print(){
        Nodo * x = head;
        while(x!=NULL){
            cout<<x->c<<"x^"<<x->g<<" ";
            x=x->next;
        }
        cout<<endl;
    }
    void add (int coef, int grad){
       Nodo * x = head;
       if(x!=NULL && grad<head->g){
            while(x->next!=NULL && grad<x->g){
                x=x->next;
           }
           Nodo* newNodo = new Nodo(coef,grad);

           if(newNodo->g==x->g){
            x->c+=newNodo->c;
            if(x->c==0){
                Nodo * prev = head;
                while(x->next!=NULL && prev->next->c!=x->c){
                    prev = prev->next;
                }
                prev->next = x->next;
                delete x;
            }
           }
           else if(x->next==NULL){
            x->next=newNodo;
            newNodo->next = NULL;
           }
           else {
            newNodo->next = x->next;
            x->next = newNodo;
           }
       }
        else insertar(coef,grad);
    }
};

int main(){
    Polinomio p = Polinomio();
    p.add(4,3);
    p.add(8,7);
    p.add(-4,1);
    p.add(4,1);
    p.add(3,39);
    p.add(3,3);
    p.add(-8,7);
    p.print();
    return 0;
}
