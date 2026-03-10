#include <iostream>
#include <string>
using namespace std;
struct Patient {
    int id;
    string name;
    int severity; // 1 (Stable) to 10 (Critical)
};
// The Node: A container to hold data and point to the next item
struct Node {
    Patient data;
    Node* next;
};
// Logic: Higher Severity = Higher Priority (served first)
class ERQueue {
    Node* front;
public:
    ERQueue() { front = NULL; }
    // INSERTION LOGIC: Sorts patients by Severity as they arrive this would put the patients 
    // Complexity: O(n) in worst case (traversing to find the spot)
    void addPatient(int id, string name, int severity) {
        Node* newNode = new Node();
        newNode->data = {id, name, severity};
        newNode->next = NULL;
        //Case 1: Queue is empty
        // If there is no one in the waiting room, this patient becomes the first one.
        if (front == NULL) {
            front = newNode;
            cout << " Patient added to empty queue.\n";
            return;
        }
        // Case 2: New patient is MORE critical than the first patient
        // We compare the new patient's severity with the front patient's severity.
        // If the new patient is in worse condition, they cut to the very front.
        if (severity > front->data.severity) {
            newNode->next = front; // Point new node to the old front
            front = newNode;       // Update front pointer to new node
            cout << "--> [Waiting Room] CRITICAL PRIORITY! Moved to front of line.\n";
            return;
        }

        // Case 3: Insert in Middle or End
        // We have to go through the queue and find the correct spot.
        // We keep moving forward as long as the NEXT patient has HIGHER or EQUAL severity.
        Node* temp = front;
        while (temp->next != NULL && temp->next->data.severity >= severity) {
            temp = temp->next;
        }
        
        // Once the loop stops, 'temp' is the patient who should be right BEFORE the new guy.
        // We squeeze the new node in between 'temp' and 'temp->next'.
        newNode->next = temp->next;
        temp->next = newNode;
        cout << " Patient added by priority.\n";
    }

    // REMOVAL LOGIC: Serves the patient at the front
    Patient servePatient() {
        if (front == NULL) return {0, "", 0}; // Return empty

        // 1. Save the node we are about to delete so we don't lose the address
        Node* temp = front;
        
        // 2. Save the data inside it so we can return it to the doctor
        Patient p = temp->data;
        
        // 3. Move the 'front' pointer to the next patient in line
        front = front->next;
        
        // 4. CRITICAL: Delete the old node from memory (Manual Memory Management)
        delete temp;
        
        return p;
    }

    // SYNC LOGIC: Find and remove a specific ID (for Undo)
    // This function searches the entire list to find a specific patient ID and removes them.
    bool removeByID(int id) {
        if (front == NULL) return false;

        // Special Case: If the patient to remove is at the very front
        if (front->data.id == id) {
            Node* temp = front;
            front = front->next;
            delete temp;
            return true;
        }

        // General Case: Search the rest of the list
        // We use a 'current' pointer to look ahead at 'current->next'
        Node* current = front;
        while (current->next != NULL) {
            if (current->next->data.id == id) {
                // Found them! Now unlink them from the chain.
                Node* temp = current->next;
                current->next = temp->next; // Bypass the node
                delete temp;                // Free the memory
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool isEmpty() { return front == NULL; }

    void showQueue() {
        cout << "\n  ER WAITING ROOM (Priority Queue) \n";
        Node* temp = front;
        while (temp != NULL) {
            string status = "";
            if (temp->data.severity >= 8) status = " [CRITICAL]";
            else if (temp->data.severity <= 3) status = " (Stable)";

            cout << "ID: " << temp->data.id 
                 << " | Name: " << temp->data.name 
                 << " | Severity: " << temp->data.severity << status << endl;
            temp = temp->next;
        }
        cout << "----------------------------------------\n";
    }
};


// COMPONENT B: Patient History (Linked List)
// Logic: Append to End (Chronological Log)

class History {
    Node* head;

public:
    History() { head = NULL; }

    // Adds a treated patient to the permanent record
    void addRecord(Patient p) {
        Node* newNode = new Node();
        newNode->data = p;
        newNode->next = NULL;

        // Case 1: The history is empty
        if (head == NULL) {
            head = newNode;
        } 
        else {
            // Case 2: The history has records
            // We traverse to the LAST node (where next is NULL)
            Node* temp = head;
            while (temp->next != NULL) { 
                temp = temp->next;
            }
            // Link the last node to the new record
            temp->next = newNode; 
        }
    }

    // Removes a record (in case of Undo)
    // Same logic as ERQueue::removeByID, but acting on the History list
    bool removeByID(int id) {
        if (head == NULL) return false;

        if (head->data.id == id) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        Node* current = head;
        while (current->next != NULL) {
            if (current->next->data.id == id) {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void showHistory() {
        cout << "\n--- TREATED PATIENTS (Archive) ---\n";
        Node* temp = head;
        while (temp != NULL) {
            cout << "ID: " << temp->data.id 
                 << " | Name: " << temp->data.name 
                 << " | Severity: " << temp->data.severity << endl;
            temp = temp->next;
        }
        cout << "----------------------------------\n";
    }
};


// COMPONENT C: Undo Stack
// Logic: LIFO (Last In, First Out) for error correction

class UndoStack {
    Node* top;

public:
    UndoStack() { top = NULL; }

    // Push: Add a new "Action" to the top 
    void push(Patient p) {
        Node* newNode = new Node();
        newNode->data = p;
        newNode->next = top; // New node points to current top
        top = newNode;       // Top becomes new node
    }

    // Pop: Remove the most recent "Action"
    Patient pop() {
        if (top == NULL) return {0, "", 0};

        Node* temp = top;
        Patient p = temp->data;
        top = top->next; // Move top down one level
        
        delete temp;     // Delete the stack node
        return p;
    }

    bool isEmpty() { return top == NULL; }
};


// MAIN SYSTEM

int main() {
    // Variable renamed to 'waitingRoom' to better reflect the hospital environment
    ERQueue waitingRoom; 
    History records;
    UndoStack undoStack;

    int choice;
    int idCounter = 1; // Auto-incrementing ID unique to each patient

    while (true) {
        cout << "\n SMART ER MANAGEMENT SYSTEM \n";
        cout << "1. Register New Patient (Add to Waiting Room)\n";
        cout << "2. Serve Next Patient\n";
        cout << "3. Undo Last Registration\n";
        cout << "4. View Status (Waiting Room & History)\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int severity;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Severity (1-10): ";
            cin >> severity;

            // 1. Add to Waiting Room (Sorts automatically by Severity)
            waitingRoom.addPatient(idCounter, name, severity);
            
            // 2. Add to Undo Stack (So we can reverse this specific ID later)
            undoStack.push({idCounter, name, severity});
            
            idCounter++;
        }
        else if (choice == 2) {
            if (!waitingRoom.isEmpty()) {
                // Serve the patient with the HIGHEST severity (Front of Queue)
                Patient p = waitingRoom.servePatient();
                
                // Move them to the permanent records
                records.addRecord(p); 
                cout << ">>> DOCTOR IS SEEING: " << p.name << endl;
            } else {
                cout << "Waiting room is empty.\n";
            }
        }
        else if (choice == 3) {
            // THE SYNC LOGIC [Task 3]
            if (undoStack.isEmpty()) {
                cout << "Nothing to Undo.\n";
            } else {
                // Get the last registered patient from the stack
                Patient p = undoStack.pop();
                
                // We don't know if the patient is still waiting or if they were served.
                // So, we try to remove them from BOTH places to be safe.
                
                // Attempt to remove from Waiting Room (if waiting)
                bool removedFromQueue = waitingRoom.removeByID(p.id);
                
                // Attempt to remove from History (if served)
                bool removedFromHistory = records.removeByID(p.id);

                cout << "--> UNDO SUCCESSFUL: Removed " << p.name << " from system.\n";
                if(removedFromQueue) cout << "    (Removed from Waiting Room)\n";
                if(removedFromHistory) cout << "    (Removed from Medical Records)\n";
            }
        }
        else if (choice == 4) {
            waitingRoom.showQueue();
            records.showHistory();
        }
        else if (choice == 0) {
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
