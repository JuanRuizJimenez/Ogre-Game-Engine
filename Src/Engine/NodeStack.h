#include <iostream>
using namespace std;

#ifndef NODESTACK_H_
#define NODESTACK_H_

template <class T>  //  Template, parametros genericos
class NodeStack {

private:

	struct StackNode  // Struct nodos
	{
		T value;		  // Valor nodos
		StackNode *next;  // Puntero -> siguiente nodo
	};

	StackNode *top;  // Puntero -> nodo "top"

public:
	
	NodeStack()  // Constructora
	{
		top = 0;
	}

	~NodeStack();  // Destructora

	/***Métodos***/
	void push(T);
	void pop(T &);
	bool isEmpty();
};

/***CODIGO DE LOS METODOS INCLUIDO EN EL .h***/
                     
template < class T >    
void NodeStack<T>::push(T value) // Añade nodo a la pila 
{
	StackNode *newNode;
	newNode = new StackNode;
	newNode->value = value;

	if (isEmpty())
	{
		top = newNode;
		newNode->next = 0;
		cout << "Insertado primer nodo con valor: " << value << endl;
	}
	else
	{
		newNode->next = top;
		top = newNode;
		cout << "Insertado nuevo nodo con valor: " << value << endl;
	}
}

template < class T >  // Devuelve el valor del "top" de la pila y lo elimina
void NodeStack<T>::pop(T &value)
{
	StackNode *temp;

	if (isEmpty())
	{
		cout << "La pila está vacía" << endl;
	}
	else
	{
		value = top->value;
		temp = top->next;
		delete top;
		top = temp;
		cout << "Pop de: " << value << " realizado con exito" << endl;
	}
}

template < class T > 
bool NodeStack<T>::isEmpty() //bool existen nodos en la pila
{
	bool state;

	if (!top)
		state = true;
	else
		state = false;

	return state;
}

template < class T >
NodeStack<T>::~NodeStack() // Destructora
{
	StackNode *nodePtr, *nextNode;

	nodePtr = top;

	while (nodePtr != 0) // recorrido de la pila eliminando cada elemento
	{
		nextNode = nodePtr->next;
		delete nodePtr;
		nodePtr = nextNode;
	}
}

#endif

