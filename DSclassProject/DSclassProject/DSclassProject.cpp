#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct LinkedList {
	string name;
	int age;
	LinkedList *next, *prev;

	LinkedList(string name1, int age1, LinkedList *prev1 = nullptr, LinkedList *next1 = nullptr) {	// Constructor
		name = name1;
		age = age1;
		prev = prev1;
		next = next1;
	}
};
LinkedList *head = nullptr;

// Prototypes
void print_list(LinkedList*);
void save_list_to_file(LinkedList*, fstream*);
void remove_from_list(LinkedList*, string);
void add_to_list(LinkedList*, string, int);
void list_sort(LinkedList*, LinkedList*);
LinkedList* partition(LinkedList*, LinkedList*);
bool is_in_order(string*, string*, int, int);
void l_swap(LinkedList*, LinkedList*);
void menu_system(fstream*);

int main()
{
	fstream in_out_file;
	in_out_file.open("C://Users//James Vera//Desktop//data.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!in_out_file.is_open()) cout << "File not opened!" << endl;

	string name;
	int age;
	LinkedList *record_ptr = nullptr;

	// Push data to list
	while (in_out_file >> name && in_out_file >> age) {
		record_ptr = new LinkedList(name, age, record_ptr);
		if (record_ptr->prev != nullptr) record_ptr->prev->next = record_ptr;
		else head = record_ptr;	 // set head ptr
	}
	// Sort List
	list_sort(head, record_ptr);

	// Menu
	menu_system(&in_out_file);

	system("pause");
	return 0;
}

void print_list(LinkedList *ptr) {
	if (ptr != nullptr) {
		cout << ptr->name << " " << ptr->age << endl;
		print_list(ptr->next);
	}
}

void save_list_to_file(LinkedList *list_ptr, fstream *f_ptr) {
	f_ptr->close();
	f_ptr->open("C://Users//James Vera//Desktop//data.txt", std::fstream::out);
	if (!f_ptr->is_open()) cout << "File did not open!" << endl;
	while (list_ptr != nullptr) {
		*f_ptr << list_ptr->name << " " << list_ptr->age << endl;
		list_ptr = list_ptr->next;
	}
}

void remove_from_list(LinkedList *list_ptr, string name1) {
	while (list_ptr != nullptr) {
		if (list_ptr->name == name1) break;
		else list_ptr = list_ptr->next;
	}
	if (list_ptr != nullptr && list_ptr->name == name1) {
		if (list_ptr == head) head = head->next;
		LinkedList *temp = list_ptr;
		list_ptr = list_ptr->next;
		if (temp->prev != nullptr) {
			temp->prev->next = list_ptr;
			if (list_ptr != nullptr)list_ptr->prev = temp->prev;
		}
		else list_ptr->prev = nullptr;
		delete temp;
	}
}

void add_to_list(LinkedList *list_ptr, string name1, int age1) {
	// Base Case: if list is empty
	if (list_ptr == nullptr) {
		head = new LinkedList(name1, age1);
		return;
	}

	while (list_ptr != nullptr) {
		if (is_in_order(&name1, &list_ptr->name, age1, list_ptr->age)) {
			LinkedList *added = new LinkedList(name1, age1, list_ptr->prev, list_ptr);
			if (list_ptr->prev != nullptr) {
				list_ptr->prev->next = added;
				list_ptr->prev = added;
			}
			else {
				list_ptr->prev = added;
				head = added;
			}
			return;
		}
		if (list_ptr->next == nullptr) {
			LinkedList *added = new LinkedList(name1, age1, list_ptr);
			list_ptr->next = added;
			return;
		}
		list_ptr = list_ptr->next;
	}

}

void l_swap(LinkedList *l_ptr1, LinkedList *l_ptr2) {
	string name_temp = l_ptr1->name;
	int age_temp = l_ptr1->age;
	l_ptr1->name = l_ptr2->name;
	l_ptr1->age = l_ptr2->age;
	l_ptr2->name = name_temp;
	l_ptr2->age = age_temp;
}

bool is_in_order(string *str1, string *str2, int age1, int age2) {
	int i = 0;
	while ((*str1)[i] != '\0' && (*str2)[i] != '\0') {
		if (tolower((*str1)[i]) < tolower((*str2)[i])) return true;
		if (tolower((*str1)[i]) > tolower((*str2)[i])) return false;
		i++;
	}
	if (age1 <= age2) return true;
	else return false;
}

void list_sort(LinkedList* start, LinkedList* end) {
	if (end != nullptr && start != end && start != end->next) {
		LinkedList *partition_index = partition(start, end);

		list_sort(start, partition_index->prev);
		list_sort(partition_index->next, end);
	}
}

LinkedList* partition(LinkedList* start, LinkedList* end) {
	int pivot_age = end->age;
	string pivot = end->name;
	LinkedList *partition_index = start->prev;

	for (LinkedList *i_ptr = start; i_ptr != end; i_ptr = i_ptr->next) {
		if (is_in_order(&i_ptr->name, &pivot, i_ptr->age, pivot_age)) {
			partition_index = (partition_index == nullptr) ? start : partition_index->next;

			l_swap(partition_index, i_ptr);
		}
	}
	partition_index = (partition_index == nullptr) ? start : partition_index->next;
	l_swap(partition_index, end);
	return partition_index;
}

void menu_system(fstream *in_out_file) {
	int option;
	string name;
	int age;
	do {
		option = 0;
		cout << "             ***** Menu Options *****\n"
			<< "(1)List Data (2)Add Data (3)Delete Data (4)Save Data (5)Quit\n";
		cout << "Enter option number: ";
		cin >> option;
		switch (option) {
		case 1:
			print_list(head);
			break;
		case 2:
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter age: ";
			cin >> age;
			add_to_list(head, name, age);
			break;
		case 3:
			cout << "Enter name to delete: ";
			cin >> name;
			remove_from_list(head, name);
			break;
		case 4:
			save_list_to_file(head, in_out_file);
			break;
		case 5:
			cout << "Do you want to save before exiting? (y/n) ";
			char save;
			cin >> save;
			if (toupper(save) == 'Y') save_list_to_file(head, in_out_file);
			break;
		default:
			cout << "ERROR: Invalid option\n";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
	} while (option != 5);
}