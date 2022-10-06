#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
    this->name = "No Name Set";
    this->value = "No Value Set";
    this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
    this->name = name;
    this->value = value;
    this->next = next;
}

Contact::Contact() {
    this->first = "No First Set";
    this->last = "No Last Set";
    this->next = nullptr;
    this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
    this->first = first;
    this->last = last;
    this->next = next;
    this->headInfoList = nullptr;
}

ContactList::ContactList() {
    this->headContactList = nullptr;
    this->count = 0;
}

int ContactList::getCount() {
    return this->count;
}

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first, std::string last) {
    bool flag = 0;

    if(search(first, last) == nullptr) { // checking for element existence
        return false;
    }

    else {
        Contact *cur = search(first, last);

        if(cur->next == nullptr) { // preventing stray endline
            flag = 1;
        }
        
        os << "Contact Name: " << cur->first << " " << cur->last;

        if(cur->headInfoList != nullptr) {
            os << endl;
            for(Info *index = cur->headInfoList; index != nullptr; index = index->next) {
                os << "\t" << index->name << " | " << index->value;

                if(index->next != nullptr) {
                    os << endl;
                }
            }
        }

        if(flag == 0) {
            os << endl;
        }

        return true;
    }
 
    return true;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
    if(isEmpty()) {
        return;
    }

    else {
        for(Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
            printContact(os, cur->first, cur->last);
        }
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
    if(isEmpty()) {
        addFront(first, last, nullptr);
        count++;
        return true;
    }

    else {
        if(search(first, last) != nullptr) {
            return false;
        }

        else {
            Contact *cur = headContactList;

            while(cur->next != nullptr) {
                cur = cur->next;
            }

            Contact *add = new Contact(first, last);
            add->first = first;
            add->last = last;

            cur->next = add;
            count++;
            return true;
        }
    }

    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last, std::string infoName, std::string infoVal) {
    if(search(first, last) == nullptr) { // checking if name exists
        return false;
    }

    else {
        Contact *con = search(first, last); // getting selected contact
        Info *cur = con->headInfoList;

        if(con->headInfoList == nullptr) { // If no info in list
            Info *add = new Info(infoName, infoVal);
            con->headInfoList = add;
            return true;
        }

        else if(searchInfo(con, infoName) != nullptr) { // if info already found, just needs update
            for(cur; cur != nullptr; cur = cur->next) {
                if(infoName.compare(cur->name) == 0) {
                    cur->value = infoVal;
                    return true;
                }
            }
        }

        else {
            while(cur->next != nullptr) {
                cur = cur->next;
            }

            cur->next = new Info(infoName, infoVal);
            
            return true;

        }
    }

    return true;
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
    if(isEmpty()) { //checks if empty
        addFront(first, last, nullptr);
        count++;
        return true;
    }

    else if(search(first, last) != nullptr) { // checks for duplicate
        return false;
    }

    else if(compare(headContactList, first, last) == 1) { // checks if new node is before head
        addFront(first, last, headContactList);
        count++;
        return true;
    }

    Contact *prev = headContactList;
    // Else
    for(Contact *cur = headContactList->next; cur != nullptr; cur = cur->next) {
        if(compare(cur, first, last) == 1) {
            Contact *add = new Contact(first, last);
            add->next = prev->next;
            prev->next = add;
            count++;
            return true;
        }

        prev = prev->next;
    }

    addContact(first, last);

    return true;
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last, std::string infoName, std::string infoVal) {
    Contact *head = search(first, last);
    
    if(head == nullptr) { // checks to see if contact exists
        return false;
    }

    else if(isEmptyInfo(head->headInfoList) == true) { // if info list empty
        addFrontInfo(search(first, last), nullptr, infoName, infoVal);
        return true;
    }

    else if(searchInfo(head, infoName) != nullptr) { // if infoName is already on list
        Info *n = searchInfo(head, infoName);
        n->value = infoVal;
        return true;
    }

    else if(compareInfo(head->headInfoList, infoName) == 1) {
        addFrontInfo(head, head->headInfoList, infoName, infoVal);
        return true;
    }

    else {
        Info *prev = head->headInfoList;
        
        for(Info *cur = head->headInfoList; cur != nullptr; cur = cur->next) {
            if(compareInfo(cur, infoName) == -1) {
                Info *add = new Info(infoName, infoVal);
                add->next = prev->next;
                prev->next = add;

                return true;
            }

            prev = prev->next;
        }
        
    }
    
    return true;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
    if(search(first, last) == nullptr) { // if contact does not exist
        return false;
    }

    else if(headContactList->next == nullptr) { // if one element in list
        headContactList = nullptr;
        count--;
        return true;
    }

    else {
        Contact *cur = search(first, last);
        Contact *prev = headContactList;

        if(cur == prev) { // if deleting from head of list
            headContactList = cur->next;
            count--;
            return true;
        }
        
        if(prev->next != cur) {
            for(prev; prev->next != nullptr; prev = prev->next) {
                if(prev->next == cur) {
                    break;
                }
            }
        }

        prev->next = cur->next;
        count--;

        return true;
    }

    return true;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last, std::string infoName) {
    Contact *head = search(first, last);

    if(head == nullptr) {
        return false;
    }

    else if(searchInfo(head, infoName) == nullptr) {
        return false;
    }

    else if(infoName.compare(head->headInfoList->name) == 0) {
        head->headInfoList = nullptr;
        return true; 
    }

    else if(head->headInfoList->next == nullptr) {
        head->headInfoList = nullptr;
        return true;
    }

    Info *cur = searchInfo(head, infoName);
    Info *prev = head->headInfoList;

    while(1) {
        if(prev->next == cur) {
            break;
        }

        prev = prev->next;
    }

    prev->next = cur->next;

    return true;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    while(headContactList != nullptr) {
        removeContact(headContactList->first, headContactList->last);
    }

    count = 0;
    
    delete headContactList;
}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    headContactList = nullptr;
    deepCopy(src);
    count = src.count;
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
    if (this != &src) {
        while(headContactList != nullptr) {
            removeContact(headContactList->first, headContactList->last);
            headContactList = headContactList->next;
        }
        deepCopy(src);
    }

    return *this;
}

bool ContactList::isEmpty() {
    if(headContactList == nullptr) {
        return true;
    }

    return false;
}

bool ContactList::isEmptyInfo(Info *p) {
    if(p == nullptr) {
        return true;
    }

    return false;
}

void ContactList::addFront(std::string first, std::string last, Contact *next) {
    Contact *n = new Contact(first, last);
    n->next = next;
    headContactList = n;
}

void ContactList::addFrontInfo(Contact *p, Info *next, std::string infoName, std::string infoVal) {
    Info *add = new Info(infoName, infoVal);
    add->next = next;
    p->headInfoList = add;
}

Contact* ContactList::search(std::string first, std::string last) {
    for(Contact *cur = headContactList; cur != nullptr; cur = cur->next) {
        if(first.compare(cur->first) == 0 && last.compare(cur->last) == 0) {
            return cur;
        }
    }
    return nullptr;
}

Info* ContactList::searchInfo(Contact *head, std::string infoName) {
    for(Info *cur = head->headInfoList; cur != nullptr; cur = cur->next) {
        if(infoName.compare(cur->name) == 0) {
            return cur;
        }
    }
    return nullptr;
}

int ContactList::compare(Contact *lhs, string firstRHS, string lastRHS) {
    Contact *rhs = new Contact(firstRHS, lastRHS);

    if(lhs->last < rhs->last) return -1;
    if(lhs->last > rhs->last) return 1;
    if(lhs->first < rhs->first) return -1;
    if(lhs->first > rhs->first) return 1;
    return 0;
}

int ContactList::compareInfo(Info *lhs, string name) {
    if(lhs->name < name) return -1;
    if(lhs->name > name) return 1;
    return 0;
}

void ContactList::deepCopy(const ContactList &src) {
    for(Contact *cur = src.headContactList; cur != nullptr; cur = cur->next) {
        addContact(cur->first, cur->last);
        if(isEmptyInfo(cur->headInfoList) == false) {
            for(Info *inf = cur->headInfoList; inf->next != nullptr; inf = inf->next) {
                addInfo(cur->first, cur->last, inf->name, inf->value);
            }
        }
    }
}
