
// Assignment 2 - Luna Dana (260857641)

#include <iostream>
using namespace std;

class Store {
public:
    Store();
    Store(int id, int ssize, int* store_data, Store* next);
    void SetData(int* store_data){
        this->store_data=store_data;
    }
    int id;
    int ssize;
    int* store_data;
    Store* next;
};

Store::Store(){
    this->id = -1;
    this->ssize = 0;
    this->store_data = NULL;
    this->next = nullptr;
}

Store::Store(int id, int ssize, int* store_data, Store* next){
    this->id = id;
    this->ssize = ssize;
    this->store_data = store_data;
    this->next = next;
}

Store* datastore = nullptr;

// Question 1
int newstore(int ssize){ 
    int* data = (int*)calloc(ssize,sizeof(int));
    if (datastore == nullptr){
        Store* newstore = new Store(0, ssize, data, nullptr);
        datastore = newstore;
        return 0;
    }

    Store* current = datastore;
    while(current->next != NULL){
        current = current->next;
    }
    Store* newstore = new Store(current->id +1, ssize, data, nullptr);
    current->next = newstore;
    return newstore->id;
    }


// Question 2
int add_element_to_store(int id, int val, int idx=-1){
    Store* current = datastore;
    while (current != NULL){
        if (current->id == id) {
            if (idx == -1){
                for (int j = 0; j < current->ssize; j++){
                    if (current->store_data[j] == 0){
                        current->store_data[j] = val;
                        return 0;
                    }
                }
                return -1; 
            }
            if (current->ssize - idx < 1){
                return -1;
            }
            current->store_data[idx] = val;
            return 0;
        }
        current = current->next; 
    }  
    return -1; 
}

// Question 3
void print_debug(){
    int n_elements = 0;
    string datastore_string = "data store : ";
    Store* current = datastore;
    while(current != NULL){
        for (int i = 0; i<current->ssize;i++){
            datastore_string = datastore_string + std::to_string(current->store_data[i]) + " ";
            n_elements = n_elements+1;
        }
        current = current->next;
    }
    cout << "elements in the datastore: " << n_elements << endl << endl;
    cout << datastore_string << endl << endl;
    current = datastore;
    while(current != NULL){
        cout << "Store " << current->id << ": ";    
        for (int i = 0; i<current->ssize;i++){
            cout << current->store_data[i] << " ";    
        }
        cout << endl << endl;
        current = current->next;
    } 
    cout << "#####################" << endl << endl;
}


// Question 4
void delete_element_from_store_by_value(int id, int val){
Store* current = datastore;
    while (current != NULL){
        if (current->id == id) {
            for (int i = 0; i<current->ssize;i++){
                if (current->store_data[i] == val){
                    current->store_data[i] = 0;
                    return;
                }
            }
        }
        current = current->next;
    }   
}

void delete_element_from_store_by_index(int id, int idx){
Store* current = datastore;
    while (current != NULL){  
        if (current->id == id) {
            if(idx >= current->ssize){
                return;
            }
            current->store_data[idx] = 0;
        } 
        current = current->next;
    }       
}

// Question 5
void which_stores_have_element(int val){
    string s = "Element " + to_string(val) + " is present in store IDs: "; // Create a string for output
    int found = false;
    Store* current = datastore;
    while(current != NULL){
        for (int i = 0; i<current->ssize;i++){
                if (current->store_data[i] == val){
                    found = true;
                    s = s + to_string(current->id) + ", ";
                    break;
                }
            } 
        current = current->next;
    }
    if (found == true){
        s.pop_back();
        s.pop_back();
        cout << s << endl;
        return;
    }
    cout << "Element " << val << " is not available in any store" << endl << endl;
}

// Question 6
void delete_store(int id)
{
    Store* current = datastore;
    if(current == NULL){
        return;
    }
    if (current->id == id){
        if(current->next != NULL){
            datastore = current->next;
            return;
        }
        datastore = nullptr;
    }
    while(current != NULL){
        if(current->next != NULL){
            if (current->next->id == id){
                current->next = current->next->next;
                return;
            }
        } 
        current = current->next;
    }
}

// Question 7
int resize_store(int id, int newsize){
    int* new_data = (int*)calloc(newsize,sizeof(int));
    Store* current = datastore;
    while(current != NULL){
        if(current->id == id){
            for (int i = 0; i<newsize;i++){
                if(i>current->ssize){
                    break;
                }
                new_data[i] = current->store_data[i];
            }
            current->ssize=newsize;
            current->SetData(new_data);
            return 0; 
        }
        current = current->next;
    }
    return -1;
}

// DO NOT ADD ANY LOGIC TO THE MAIN FUNCTION.
// YOU MAY MODIFY FOR YOUR OWN TESTS ONLY BUT THE ORIGINAL MAIN
// FUNCTION SHOULD BE SUBMITTED AS IS
int main(){

    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1){
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }
    
    if (s2 != -1){
        add_element_to_store(s2, 7, 2);
        add_element_to_store(s2, 15, 0);
        add_element_to_store(s2, 22, 1);
    }

    print_debug();

    delete_element_from_store_by_value(s1, 13);
    delete_element_from_store_by_value(s1, 71);
    delete_element_from_store_by_index(s2, 2);
    delete_element_from_store_by_index(s1, 5);
    print_debug();

    which_stores_have_element(15);
    which_stores_have_element(29);

    delete_store(s1);
    print_debug();

    resize_store(s2, 20);
    int s3 = newstore(40);
    print_debug();

    s3 = newstore(30);
    add_element_to_store(s3, 7, 29);
    print_debug();

    return -1;

}