#pragma once

/*
Akil Dixon

A class using a doubly Linked List to store data, with a vector that stores pointers to every node for easy retrieval via index, 
and incorporating threads to partition the list and search concurrently

Things I might want to do later for fun:
	use a different structure instead of a Linked List
	make different types of searches
	make a sorting algorithm for the partitions
	use threads to partition and concurrently delete nodes


Note: this was slightly rewritten from the original to accomodate the members of the structs in this project (data containing type, data->type, will not always be true, etc.,)
*/
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
using namespace std;




template<typename T>
struct Node
{

	T data;

	Node<T>* next;
	Node<T>* previous;
};



template<typename T>
void lSearch(int, Node<T>**, int, vector<Node<T>*>, int, string);

template<typename T>
void partitionWork(int, Node<T>**, int, vector<Node<T>*>, int);

template<typename T>
void linearSearch(int, Node<T>**, int, string);

void checkDiv(int, int, int*);

template<class T>
class MyList
{
public:
	Node<T>* head;
	Node<T>* tail;

	vector<Node<T>*> Master_List;

	int size = 0;

	MyList<T>();
	MyList<T>(int);
	~MyList<T>();

	T operator[](int);

	Node<T>* getNode(int id);

	
	void addNode(T);

	void deleteNode(int);

	void editNode(int id, T data);
	void swapNode(int, int);

	void destroy();
	void empty();
	void search(string, int);


private:

};




template<typename T>
MyList<T>::MyList()
{
	head = new Node<T>;

	tail = new Node<T>;
	head->next = tail;
	tail->previous = head;


}

template<typename T>
MyList<T>::~MyList()
{
	destroy();
}



template<typename T>
T MyList<T>::operator[](int id)
{
	if (id >= size)
		return NULL;

	return Master_List[id]->data;
}

template<typename T>
Node<T>* MyList<T>::getNode(int id)
{
	return Master_List[id];
}


/*
* Takes an initial size
*/
template<typename T>
MyList<T>::MyList(int s)
{
	head = new Node<T>;

	tail = new Node<T>;
	head->next = tail;
	tail->previous = head;


	Master_List.reserve(s);

}

//O(1)
template<typename T>
void MyList<T>::addNode(T data)
{

	Node<T>* newNode = new Node<T>;

	newNode->data = data;
	newNode->previous = tail->previous;
	newNode->next = tail;
	tail->previous->next = newNode;
	tail->previous = newNode;


	Master_List.push_back(newNode);
	size++;


}

//O(1)
template<typename T>
void MyList<T>::deleteNode(int id)
{
	Node<T>* removeThis = Master_List[id];

	Node<T>* left = removeThis->previous;
	Node<T>* right = removeThis->next;

	left->next = right;
	right->previous = left;



	removeThis->previous = NULL;
	removeThis->next = NULL;

	Master_List.erase(Master_List.begin() + id);

	delete removeThis;
	size--;


}




//O(1)
template<typename T>
void MyList<T>::editNode(int id, T data)
{		

	Master_List[id]->data = data;
}


//O(1)
template<typename T>
void MyList<T>::swapNode(int id1, int id2)
{
	Node<T>* tempNode1 = Master_List[id1];
	Node<T>* tempNode2 = Master_List[id2];


	Node<T>* tempPrevious = tempNode1->previous;
	Node<T>* tempNext = tempNode1->next;

	tempNode1->previous = tempNode2->previous;
	tempNode1->next = tempNode2->next;

	tempNode2->previous = tempPrevious;
	tempNode2->next = tempNext;

	swap(Master_List[id1], Master_List[id2]);


}
//O(n)
template<typename T>
void MyList<T>::empty()
{
	for (int i = size - 1; i > -1; i--)
		deleteNode(i);

	//Master_List.erase(Master_List.begin(), Master_List.end());
}




//O(n)
template<typename T>
void MyList<T>::destroy()
{
	if (size > 0)
	{
		for (int i = size - 1; i > -1; i--)
			deleteNode(i);

		Master_List.erase(Master_List.begin(), Master_List.end());


		head->next = NULL;
		tail->previous = NULL;

		delete head;
		delete tail;
		head = NULL;
		tail = NULL;

		size--;
	}

}


template<typename T>
void MyList<T>::search(string searchFor, int threadCount)
{
	int* sizes;
	sizes = new int[threadCount] {0};
	int* adjSizes = new int[threadCount] {0};
	int* cumulSizes = new int[threadCount] {0};

	auto partitions = new Node<T>**[threadCount];

	checkDiv(size, threadCount, sizes);

	for (int i = 0; i < threadCount; i++)
	{
		adjSizes[i] = (size / threadCount) + sizes[i];
		if (i > 0)
			cumulSizes[i] = cumulSizes[i - 1] + adjSizes[i - 1];
	}
	for (int i = 0; i < threadCount; i++)
		partitions[i] = new Node<T>*[adjSizes[i]];

	thread* threads = new thread[threadCount];

	for (int i = 0; i < threadCount; i++)
		threads[i] = thread(lSearch<T>, i, partitions[i], adjSizes[i], Master_List, cumulSizes[i], searchFor);


	auto start = chrono::system_clock::now();
	for (int i = 0; i < threadCount; i++)
		threads[i].join();
	auto end = chrono::system_clock::now();

	chrono::duration<double> elapsed_seconds = end - start;
	cout << "\n\nTime Elapsed: " << elapsed_seconds.count() << endl;


	for (int i = 0; i < threadCount; i++)
	{
		for (int j = 0; j < adjSizes[i]; j++)
			partitions[i][j] = NULL;

		delete[] partitions[i];
	}
	delete[] cumulSizes;
	delete[] sizes;
	delete[] adjSizes;
	delete[] partitions;
	delete[] threads;
}



template<typename T>
void lSearch(int id, Node<T>* arr1[], int size, vector<Node<T>*> arr2, int base, string searchFor)
{

	partitionWork(id, arr1, size, arr2, base);

	linearSearch(id, arr1, size, searchFor);

}

template<typename T>
void partitionWork(int id, Node<T>* arr1[], int size, vector<Node<T>*> arr2, int base)
{

	for (int i = base, j = 0; i < base + size; i++, j++)
	{

		arr1[j] = arr2[i];

		this_thread::yield();
		this_thread::yield();
	}
	//someSort(arr1,size);

}

template<typename T>
void linearSearch(int id, Node<T>* arr[], int size, string searchFor)
{

	for (int i = 0; i < size; i++)
	{

		if (arr[i]->data->type == searchFor)
			cout << "\n\nThread " << (id+1) << " has found " << searchFor << " in Master_List[" << (id*size)+i << "]\n";

		this_thread::yield();
		this_thread::yield();
	}
}