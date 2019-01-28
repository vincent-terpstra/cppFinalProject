/**
 * File : Task.cpp
 * OOP 345 MS3
 * Vincent Terpstra :: 140665175
 * Nov 17 2018
 * Purpose
 * 	implementation file for Task object
 * 		inherits from a Item
 * 		   
 */

#include <iostream>
#include <deque>
#include "Task.h"
#include "Item.h"
#include "CustomerOrder.h"


//	std::deque<CustomerOrder> Orders;				//Current orders at this station	
//	Task* pNextTask;								//Reference to the next task
//Constructor
//		receives a string from Inventory.txt
//		sets to safe empty state
Task::Task(std::string& str) : Item(str), pNextTask(nullptr) {}								

//Modifier function to fill last order in the queue
//
void Task::RunProcess(std::ostream& os){
	if(Orders.size() && !Orders.back().getItemFillState(getName()))
			Orders.back().fillItem(*this, os);
}

//This function moves the completed task to the next part of the assembly line
//
bool Task::MoveTask(){
	if(Orders.size() == 0) return false;
	if(pNextTask && Orders.back().getItemFillState(getName())){
		*pNextTask += std::move(Orders.back());
		Orders.pop_back();
	}
	return true;

}

//Sets up the pointer to the next task if the tasks names line up
//
void Task::setNextTask(Task& task){
	pNextTask = &task;
}

//removes the last CustomerOrder from the queue & places it in src
//		returns false if Orders is empty or the Item hasn't been filled
bool Task::getCompleted(CustomerOrder & src){
	if(Orders.size() && Orders.back().getItemFillState(getName())){
		src = std::move(Orders.back());
		Orders.pop_back();
		return true;
	} 
	return false;
}

void Task::Validate(std::ostream& os){
	os << getName();
	if(pNextTask)
		os << " - " << pNextTask->getName();
	os << std::endl;
}

//Move a CustomerOrder into the orders queue
//
Task& Task::operator+=(CustomerOrder&& newOrder){
	Orders.push_front(std::move(newOrder));
	return *this;
}				
