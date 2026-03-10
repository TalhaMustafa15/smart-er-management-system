
# Smart ER Management System

## Project Overview
The Smart ER Management System is a C++ based project that simulates the working of a hospital emergency room. In a real hospital environment, patients are treated based on the severity of their medical condition rather than the order of arrival. This project models that behavior using fundamental data structures.
The system allows hospital staff to register patients, prioritize them according to their severity level, treat patients in order of priority, maintain patient history, and undo incorrect registration
## Objective
The main objective of this project is to demonstrate the practical implementation of data structures in a real-world scenario. The system improves patient management by ensuring that critically ill patients receive treatment first while maintaining a record of treated patients.
## Data Structures Used
### 1. Linked List
A linked list is used to store patient records in the waiting room and history list. Each patient is represented as a node containing patient details and a pointer to the next node. Linked lists allow efficient insertion and deletion operations.
### 2. Priority Queue
The waiting room operates as a priority queue where patients with higher severity levels receive higher priority. Patients with more critical conditions are placed at the front of the queue and served first.
### 3. Stack
A stack is used to implement the undo functionality. The stack follows the Last In First Out (LIFO) principle. This allows the system to remove the most recently registered patient if a mistake occurs during registration.
## System Features
### Patient Registration
New patients can be registered in the system by providing their name and severity level. Each patient receives a unique identification number.
### Priority-Based Treatment
Patients are automatically sorted based on their severity level. Patients with higher severity are treated before patients with lower severity.
### Serve Patient
When a doctor becomes available, the system serves the patient with the highest priority from the waiting queue.
### Patient History
After treatment, the patient's information is moved to the medical history section where records of treated patients are stored.
### Undo Functionality
If a mistake occurs during patient registration, the system allows the last action to be undone by removing the most recently registered patient.
## Real-World Application
This project represents how emergency departments in hospitals prioritize patient care. By using data structures such as linked lists, priority queues, and stacks, the system efficiently manages patient flow and maintains accurate medical records.
## Learning Outcomes
This project helps in understanding the practical implementation of:
* Linked Lists
* Priority Queues
* Stack operations
* Dynamic memory management in C++
* Real-world problem solving using data structures
