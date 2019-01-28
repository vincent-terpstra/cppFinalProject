/**
 * File : LineManager.cpp
 * OOP 345 MS3
 * Vincent Terpstra :: 140665175
 * Nov 17 2018
 * Purpose
 * 		implementation file for LineManager object
 */

#include <iostream>
#include <deque>
#include <fstream>
#include <vector>

#include "LineManager.h"
#include "CustomerOrder.h"
#include "Task.h"
#include "Utilities.h"

//	std::vector<Task*> AssemblyLine;
//	std::deque<CustomerOrder> ToBeFilled;		//Queue of Customer Orders to be filled
//	std::deque<CustomerOrder> Completed;		//Queue of Customer Orders completed
//	unsigned int CustomerOrder_Count;			//The number of customer orders the process started with

//Constructor sets the assembly line up by setting up the tasks pNextTask information
//and moves all the CustomerOrder objects into the ToBeFilled queue
using std::string;
LineManager::LineManager(
	string& str, 
	std::vector<Task*>& tasks, 
	std::vector<CustomerOrder>& orders)
{
	std::ifstream file(str);
	if(file.is_open()){
		Utilities util;
		while(!file.eof()){
			string link;
			std::getline(file, link);

			bool more = true;
			size_t pos = 0;
			try {
			string task1 = util.extractToken(link, pos, more);
			Task * ptr1 = *std::find_if(tasks.begin(), tasks.end(),
						[&](Task* task){ return task->getName().compare(task1) == 0; }
					);
			AssemblyLine.push_back(ptr1);
			if(more){
				string task2 = util.extractToken(link, pos, more);

				//find a pointer to the first Task * with names task1 and task2
				ptr1->setNextTask(
					**std::find_if(tasks.begin(), tasks.end(),
						[&](Task* task){ return task->getName().compare(task2) == 0; }
					)
				);
			}
			} catch (...){
				std::cerr << "*** Error Linking Tasks ***" << std::endl;
			}
		}
		file.close();
	} else {
		std::cerr << "*** Unable to open file: " << str << " ***" << std::endl;
	}
	
	//Validate all tasks
	//
	for(auto task : AssemblyLine)
		task->Validate(std::cout);

	//move customers into list of orders
	//
	CustomerOrder_Count = orders.size();
	for(CustomerOrder& cust : orders){
		ToBeFilled.push_front(std::move(cust));
	}
}

//Processes each assembly line task and moves the CustomerOrders through the line
//	Returning true if all items have been completed
//
bool LineManager::Run(std::ostream& os){
	//if there are any Customer Orders move the last onto
	//	the assembly line
	if(ToBeFilled.size()){
		*AssemblyLine.front() += std::move(ToBeFilled.back());
		ToBeFilled.pop_back();
	}
	//Add Items to Order in each Task
	for(auto task : AssemblyLine)
		task->RunProcess(os);

	//Move Orders onto the next task
	for(auto task : AssemblyLine)
		task->MoveTask();

	CustomerOrder completed;
	if(AssemblyLine.back()->getCompleted(completed))
		Completed.push_back(std::move(completed));

	if(Completed.size() == CustomerOrder_Count){
		os << std::endl << "COMPLETED" << std::endl;
		for(CustomerOrder& cust : Completed){
			os << std::endl;
			cust.Display(os);
		}
		return true;
	}
	return false;
}
