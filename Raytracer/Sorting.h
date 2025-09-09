#pragma once
#include <iostream>
#include <chrono>
#include <string>



template<class T>
void bubbleSort(T[], int);

template<class T>
void selectionSort(T[], int);

template<class T>
void insertionSort(T[], int);

template<class T>
void quickSort(T[], int, int, int);

template<class T>
int partition(T[], int, int, int);


template<class T>
void quickSort(T arr[], int size, int low, int high)
{

	if (low < high)
	{
		int pi = partition(arr, size, low, high);


		quickSort(arr, size, pi + 1, high);
		quickSort(arr, size, low, pi - 1);
	}

}

template<class T>
int partition(T arr[], int size, int low, int high)
{
	int pivot = high;

	int j = low - 1;

	for (int i = low; i < size; i++)
	{
		if (arr[i] < arr[pivot])
		{
			j++;
			swap(arr[j], arr[i]);
		}
	}
	swap(arr[j + 1], arr[high]);
	return (j + 1);
}

template<class T>
void insertionSort(T arr[], int size)
{
	for (int i = 1; i < size; i++)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			if (arr[j + 1] < arr[j])
				swap(arr[j + 1], arr[j]);
		}
	}

}

template<class T>
void selectionSort(T arr[], int size)
{
	int theMin = 0;
	int currentVal = 0;
	for (int j = 0; j < size; j++)
	{
		for (int i = currentVal; i < size; i++)
		{
			if (arr[i] < arr[theMin])
				theMin = i;
		}
		swap(arr[currentVal], arr[theMin]);
		currentVal++;
		theMin = currentVal;
	}

}

template<class T>
void bubbleSort(T arr1[], int size)
{
	bool swapped = false;

	while (true)
	{
		for (int i = 0; i < size; i++)
		{
			if (i + 1 != size)
			{
				if (arr1[i] > arr1[i + 1])
				{
					swap(arr1[i], arr1[i + 1]);
					swapped = true;
				}
			}
		}
		if (!swapped)
			break;
		else
			swapped = false;
	}

}