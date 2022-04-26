
// Assignment 1 - Luna Dana (260857641)

#include <iostream>
using namespace std;

#define ARRSIZE 50
int datastore[ARRSIZE] = {};
int ARRSIZE_remaining = ARRSIZE;
int store_counter = -1;
int store_capacity[ARRSIZE] = {};

// Question 1
int newstore(int ssize)
{
    // Check if there is enough space in the data store
    if (ARRSIZE_remaining-ssize < 0){
        return -1;
    }
    store_counter = store_counter+1; // Increment the store counter
    ARRSIZE_remaining = ARRSIZE_remaining-ssize; // Update the remaining spot in the datastore
    // Explicit the store capacity at the index of the new store
    store_capacity[store_counter] = ssize;
    return store_counter;
}

// Question 2
int add_element_to_store(int id, int val, int idx=-1)
{
    int id_capacity = store_capacity[id];
    int which_index = 0;
    for (int i=0; i<id; i++){
        which_index = which_index+store_capacity[i];
    }
    // Case where index is -1 and val is added at the next free spot
    if (idx == -1){
        for (int j=which_index; j<which_index+id_capacity; j++){
            if (datastore[j]==0){
                datastore[j]=val;
                return 0;
            }
        }
        return -1;
    }
    // Case where index is out of bound
    else if (id_capacity-idx <= 0){
        return -1;
    }
    else {
        datastore[which_index+idx]=val;
        return 0;
    }
    return -1;
}

// Question 3
void print_debug()
{
    // Print the elements in the datastore
    cout << "available element in the datastore: " << ARRSIZE_remaining << endl << endl << "datastore: ";
    for(int i = 0; i<ARRSIZE;i++){
        cout << datastore[i] << " ";
    }
    cout << endl << endl;
    int counter = 0;

    // Print the elements store by store
    if(ARRSIZE_remaining != 50){
        for(int i=0;i<store_counter+1;i++){
            if(store_capacity[i]==0){
                continue;
            }
            cout << "store " << i << ": ";
            for(int j=0; j<store_capacity[i]; j++){
                cout << datastore[j+counter] << " ";
            }
            cout << endl << endl;
            counter = counter + store_capacity[i];
        }
    }
    cout << "#####################" << endl << endl;
}


// Question 4
void delete_element_from_store_by_value(int id, int val)
{
    int which_index = 0;
    // Check in the datastore at which index the store begins
    for (int i=0; i<id; i++){
        which_index = which_index+store_capacity[i];
    }
    for(int i = which_index; i<store_capacity[id];i++){
        // Remove the element when found
        if (datastore[i] == val){
            datastore[i] = 0;
            return;
        }
    }
}

void delete_element_from_store_by_index(int id, int idx)
{
    int id_capacity = store_capacity[id]; // Capactiy of the store
    int which_index = 0;
    for (int i=0; i<id; i++){
        which_index = which_index+store_capacity[i];
    }
    // Case where the index is out of bound
    if(idx >= id_capacity){
        return;
    }
    // Remove the element
    datastore[which_index+idx] = 0;
    
}

// Question 5
void which_stores_have_element(int val)
{
    string s = "Element " + to_string(val) + " is present in store IDs: "; // Create a string for output
    int found = false; // Check if there is at least one val in the datastore
    for(int j=0;j<ARRSIZE;j++){
        if(datastore[j] == val){ found = true;}
    }
    // Case where the element is not in any store
    if(found == false){
        cout << "Element " << val << " is not available in any store" << endl << endl;
        return;
    }    
    for(int i=0; i<store_counter+1; i ++){
        int which_index = 0;
        for (int j=0; j<i; j++){
            which_index = which_index+store_capacity[j];
        }
        for(int k=which_index; k<store_capacity[i]; k++){
            // If the element is found in the store, print it
            if(datastore[k] == val){
                s = s + to_string(i) + ", ";
                break;
            }
        }
    }
    // Remove the coma and the space at the end of the string.
    s.pop_back();
    s.pop_back();
    cout << s << endl << endl;
}

// Question 6
void delete_store(int id)
{
    // Update the remaining element variables
    int capacity = store_capacity[id];
    ARRSIZE_remaining = ARRSIZE_remaining + capacity;

    // Decay all the other stores element by store capacity
    int which_index = 0;
        for (int j=0; j<id; j++){
            which_index = which_index+store_capacity[j];
        }
    for(int k=which_index+capacity; k<ARRSIZE; k++){
        datastore[k-capacity] = datastore[k];
        datastore[k] = 0;
    }
    // Update the store capacity array at index id
    store_capacity[id] = 0;
}

// Question 7
int resize_store(int id, int newsize)
{
    int old_capacity = store_capacity[id];
    if(newsize-old_capacity > ARRSIZE_remaining){
        return -1;
    }
    int which_index = 0;
        for (int j=0; j<id; j++){
            which_index = which_index+store_capacity[j];
        }

    // Case where you want to make the store smaller
    if(newsize<old_capacity){
        int decay = old_capacity-newsize;
        for(int k=which_index+old_capacity; k<ARRSIZE; k++){
            datastore[k-decay] = datastore[k];
            datastore[k] = 0;
        }
    }
    // Case where you want to make the store bigger
    if(newsize>old_capacity){
        int decay = newsize-old_capacity;
        for(int k=ARRSIZE; k>which_index+newsize; k--){
            datastore[k] = datastore[k-decay];
            datastore[k-decay] = 0;
        }
    }
    store_capacity[id] = newsize;
    ARRSIZE_remaining = ARRSIZE_remaining-(newsize-old_capacity);
    return 0;

}


// DO NOT ADD ANY LOGIC TO THE MAIN FUNCTION.
// YOU MAY MODIFY FOR YOUR OWN TESTS ONLY BUT THE ORIGINAL MAIN
// FUNCTION SHOULD BE SUBMITTED AS IS
int main()
{
    int s1 = newstore(3); // create new empty data store of size 3
    int s2 = newstore(5); // create new empty data store of size 5

    if (s1 != -1)
    {
        add_element_to_store(s1, 13);
        add_element_to_store(s1, 15);
        add_element_to_store(s1, 21);
        add_element_to_store(s1, 42); // this should return -1
    }
    
    
    if (s2 != -1)
    {
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
