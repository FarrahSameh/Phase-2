//	This is an updated version of code originally
//  created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** ADT stack: Array-based implementation.
 @file ArrayStack.h */

#ifndef ARRAY_STACK_
#define ARRAY_STACK_

#include "StackADT.h"

 //Unless spesificed by the stack user, the default size is 100
template<typename T>
class ArrayStack : public StackADT<T>
{
	enum { MAX_SIZE = 100 };
protected:  
	T items[MAX_SIZE];		// Array of stack items
	int top;                   // Index to top of stack

public:

	ArrayStack()
	{
		top = -1;
	}  

	bool isEmpty() const
	{
		return top == -1;
	}  

	bool push(const T& newEntry)
	{
		if (top == MAX_SIZE - 1) return false;	

		top++;
		items[top] = newEntry;
		return true;
	}  

	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		top--;
		return true;
	}  

	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = items[top];
		return true;
	}  

	int getCount() const { return top + 1; } 

	void print() const                         
	{
		for (int i = top; i >= 0; i--)
			cout << items[i] << " ";
		cout << endl;
	}

}; 

#endif