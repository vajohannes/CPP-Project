#include <iostream>
#include <cstring>
using namespace std;

// === Region: constants and structs ===
// The constants are structs are defined in this region
// ===================================

// constants
const int MAX_CODE = 10;   // at most 10 characters (including the NULL character)
const int MAX_TITLE = 100; // at most 100 characters (including the NULL character)
struct CourseItem;         // needed because Course needs to refer to CourseItem*
// A sorted linked list of Course. List items are sorted by the course code
struct Course
{
    char code[MAX_CODE];       // course code
    char title[MAX_TITLE];     // course title
    int credit;                // course credit
    Course *next;              // a pointer pointing to the next Course
    CourseItem *exclusions;    // a sorted linked list of CourseItem
    CourseItem *prerequisites; // a sorted linked list of CourseItem
};
// A sorted linked list of CourseItem. List items are sorted by course->code
struct CourseItem
{
    Course *course;   // pointing to the course
    CourseItem *next; // a pointer pointing to the next CourseItem
};

/**
 * function ll_print_all prints the linked lists stored in the system
 * @param head pointing to the head of the linked list
 */
void ll_print_all(const Course *head); // given, you cannot make any changes

/**
 * function ll_insert_prerequisite inserts a pre-requisite
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param preCode stores the course code of the pre-requisite
 * @return true if the insertion is complete. false otherwise
 */
bool ll_insert_prerequisite(Course *head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]);

/**
 * function ll_insert_exclusion inserts an exclusion
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param excludeCode stores the course code to be excluded
 * @return true if the insertion is complete. false otherwise
 */
bool ll_insert_exclusion(Course *head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]);

/**
 * function ll_delete_prerequisite deletes a pre-requisite
 * Example: Given the input, if there is a pre-requisite, the pre-requisite will be deleted
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param preCode stores the course code of pre-requisite
 * @return true if the deletion is complete. false otherwise
 */
bool ll_delete_prerequisite(Course *head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE]);

/**
 * function ll_delete_exclusion deletes an exclusion
 * Example: Given the input, if there is an exclusion, the exclusion will be deleted
 * @param head pointing to the head of the linked list
 * @param targetCode stores the course code of the target
 * @param excludeCode stores the course code of exclusion
 * @return true if the deletion is complete. false otherwise
 */
bool ll_delete_exclusion(Course *head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]);

/**
 * function ll_insert_course inserts a course
 * Note: head is passed by reference because head may be changed during the insertion
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @param t is the course title
 * @param cred is the course credit
 * @return true if the insertion is complete. false otherwise
 */
bool ll_insert_course(Course *&head, const char c[MAX_CODE], const char t[MAX_TITLE], int cred);

/**
 * function ll_delete_course deletes an existing course
 * Note: head is passed by reference because head may be changed during the deletion
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @return true if the deletion is complete. false otherwise
 */
bool ll_delete_course(Course *&head, const char c[MAX_CODE]);

/**
 * function ll_modify_course_title modifies the course title attribute of a course
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @param t is the new course title
 * @return true if the modification is complete. false otherwise
 */
bool ll_modify_course_title(Course *head, const char c[MAX_CODE], const char t[MAX_TITLE]);

/**
 * function ll_modify_course_credit modifies the course credit attribute of a course
 * @param head pointing to the head of the linked list
 * @param c is the course code
 * @param cred is the new credit
 * @return true if the modification is complete. false otherwise
 */
bool ll_modify_course_credit(Course *head, const char c[MAX_CODE], int cred);

/**
 * function ll_cleanup_all cleans up the linked lists used in the system. It restores the original states (i.e., empty linked lists) of the system.
 * No memory leak should be found after the clean up
 * Note: head is passed by reference because head may be changed during the clean up
 * @param head pointing to the head of the linked list
 */
void ll_cleanup_all(Course *&head);

/**
 * function enter_credit: A helper function to enter a valid credit
 * ensure the credit is a non-negative integer
 */
int enter_credit()
{
    int credit;
    while (true)
    {
        cout << "Enter the credit: ";
        cin >> credit;
        if (credit >= 0)
            return credit;
        cout << "Invalid credit: " << credit << endl;
    }
}

/**
 * function main - the main function
 */
int main()
{
    Course *clist = nullptr;
    enum MeunOption
    {
        OPTION_DISPLAY_CURRENT_LIST = 0,
        OPTION_INSERT_COURSE,
        OPTION_INSERT_PRE_REQUISITE,
        OPTION_INSERT_EXCLUSION,
        OPTION_DELETE_COURSE,
        OPTION_DELETE_PRE_REQUISITE,
        OPTION_DELETE_EXCLUSION,
        OPTION_MODIFY_COURSE_TITLE,
        OPTION_MODIFY_COURSE_CREDIT,
        OPTION_EXIT_WITHOUT_CLEANUP,
        OPTION_EXIT_WITH_CLEANUP,
        MAX_MENU_OPTIONS
    };
    const int MAX_MENU_OPTIONS_LENGTH = 50;
    char menuOptions[MAX_MENU_OPTIONS][MAX_MENU_OPTIONS_LENGTH] = {
        "Display the current list",
        "Insert a new course",
        "Insert a pre-requisite",
        "Insert an exclusion",
        "Delete an existing course",
        "Delete an existing pre-requisite",
        "Delete an existing exclusion",
        "Modify a course title",
        "Modify a course credit",
        "Exit without clean up",
        "Exit with clean up"};

    int option = 0;
    int i = 0;
    int credit = 0;
    // It is tricky to check the input character array, write assumptions that we don't need to check
    char code[MAX_CODE] = "";
    char title[MAX_TITLE] = "";
    char targetCode[MAX_CODE] = "";

    bool ret = false;

    while (true)
    {
        cout << "=== Menu ===" << endl;
        for (i = 0; i < MAX_MENU_OPTIONS; i++)
            cout << i + 1 << ": " << menuOptions[i] << endl; // shift by +1 when display

        cout << "Enter your option: ";
        cin >> option;

        option = option - 1; // shift by -1 after entering the option

        // The invalid menu option handling
        if (option < 0 || option >= MAX_MENU_OPTIONS)
        {
            cout << "Invalid option" << endl;
            continue;
        }

        // Exit operations handling
        if (option == OPTION_EXIT_WITHOUT_CLEANUP)
            break; // break the while loop
        else if (option == OPTION_EXIT_WITH_CLEANUP)
        {
            cout << "=== Cleanup ===" << endl;
            ll_cleanup_all(clist);
            ll_print_all(clist);
            break; //  break the while loop
        }

        switch (option)
        {
        case OPTION_DISPLAY_CURRENT_LIST:
            ll_print_all(clist);
            break;
        case OPTION_INSERT_COURSE:
            cout << "Enter a course code: ";
            cin >> code;
            cout << "Enter a course title: ";
            cin.ignore(); // necessary when mixing cin>> and cin.getline
            cin.getline(title, MAX_TITLE);
            credit = enter_credit();
            ret = ll_insert_course(clist, code, title, credit);
            if (ret == false)
            {
                cout << "Failed to insert " << code << endl;
            }
            else
            {
                cout << code << " is successfully inserted" << endl;
                ll_print_all(clist);
            }
            break;
        case OPTION_INSERT_PRE_REQUISITE:
            cout << "Enter the target course code: ";
            cin >> targetCode;
            cout << "Enter the pre-requisite course code: ";
            cin >> code;
            if (strcmp(targetCode, code) == 0)
            {
                cout << "Cannot insert a pre-requisite to the same course " << code << endl;
            }
            else
            {
                ret = ll_insert_prerequisite(clist, targetCode, code);
                if (ret == false)
                {
                    cout << "Failed to insert pre-requisite " << targetCode << ", " << code << endl;
                }
                else
                {
                    cout << "Inserted a pre-requisite " << targetCode << ", " << code << endl;
                    ll_print_all(clist);
                }
            }
            break;
        case OPTION_INSERT_EXCLUSION:
            cout << "Enter the target course code: ";
            cin >> targetCode;
            cout << "Enter the exclusion course code: ";
            cin >> code;
            if (strcmp(targetCode, code) == 0)
            {
                cout << "Cannot insert an exclusion to the same course " << code << endl;
            }
            else
            {
                ret = ll_insert_exclusion(clist, targetCode, code);
                if (ret == false)
                {
                    cout << "Failed to insert exclusion " << targetCode << ", " << code << endl;
                }
                else
                {
                    cout << "Inserted an exclusion " << targetCode << ", " << code << endl;
                    ll_print_all(clist);
                }
            }
            break;
        case OPTION_DELETE_COURSE:
            cout << "Enter the course code: ";
            cin >> code;
            ret = ll_delete_course(clist, code);
            if (ret == false)
            {
                cout << "Failed to delete course " << code << endl;
            }
            else
            {
                cout << "Deleted a course " << code << endl;
                ll_print_all(clist);
            }
            break;
        case OPTION_DELETE_PRE_REQUISITE:
            cout << "Enter the target course code: ";
            cin >> targetCode;
            cout << "Enter the pre-requisite course code: ";
            cin >> code;
            ret = ll_delete_prerequisite(clist, targetCode, code);
            if (ret == false)
            {
                cout << "Failed to delete pre-requisite " << targetCode << ", " << code << endl;
            }
            else
            {
                cout << "Deleted a pre-requisite " << targetCode << ", " << code << endl;
                ll_print_all(clist);
            }
            break;
        case OPTION_DELETE_EXCLUSION:
            cout << "Enter the target course code: ";
            cin >> targetCode;
            cout << "Enter the pre-requisite course code: ";
            cin >> code;
            ret = ll_delete_exclusion(clist, targetCode, code);
            if (ret == false)
            {
                cout << "Failed to delete exclusion " << targetCode << ", " << code << endl;
            }
            else
            {
                cout << "Deleted an exclusion " << targetCode << ", " << code << endl;
                ll_print_all(clist);
            }
            break;
        case OPTION_MODIFY_COURSE_TITLE:
            cout << "Enter the course code: ";
            cin >> code;
            cout << "Enter the new course title: ";
            cin.ignore();
            cin.getline(title, MAX_TITLE);
            ret = ll_modify_course_title(clist, code, title);
            if (ret == false)
            {
                cout << "Failed to modify the course title of " << code << endl;
            }
            else
            {
                cout << "The course title of " << code << " is modified" << endl;
                ll_print_all(clist);
            }
            break;
        case OPTION_MODIFY_COURSE_CREDIT:
            cout << "Enter the course code: ";
            cin >> code;
            credit = enter_credit();
            ret = ll_modify_course_credit(clist, code, credit);
            if (ret == false)
            {
                cout << "Failed to modify the course credit of " << code << endl;
            }
            else
            {
                cout << "The course credit of " << code << " is modified" << endl;
                ll_print_all(clist);
            }
            break;

        } // end switch

    } // end while true
    return 0;
}

// === Region: function definitions ===
// You should implement the functions below
// ====================================

void delete_courseItem(CourseItem *p)
{
    // base
    if (p == nullptr)
    {
        return;
    }
    else
    {
        // recursion
        delete_courseItem(p->next);
        delete p;
        p = nullptr;
    }
}

void ll_print_all(const Course *head)
{
    const Course *p;
    const CourseItem *q;
    int count;

    cout << "=== Course List (Code[Credits]) ===" << endl;
    count = 0;
    for (p = head; p != nullptr; p = p->next)
    {
        cout << p->code << "[" << p->credit << "]";
        if (p->next != nullptr)
            cout << " -> ";
        count++;
    }
    if (count == 0)
    {
        cout << "No courses in the list";
    }
    cout << endl;

    cout << "=== Course titles ===" << endl;
    count = 0;
    for (p = head; p != nullptr; p = p->next)
    {
        cout << p->code << ": " << p->title << endl;
        count++;
    }
    if (count == 0)
    {
        cout << "No course titles" << endl;
    }

    cout << "=== Pre-requisites ===" << endl;
    count = 0;
    for (p = head; p != nullptr; p = p->next)
    {
        q = p->prerequisites;
        if (q != nullptr)
        {
            cout << p->code << ": ";
            for (; q != nullptr; q = q->next)
            {
                cout << q->course->code;
                if (q->next != nullptr)
                    cout << ", ";
            }
            count++;
            cout << endl;
        }
    }
    if (count == 0)
    {
        cout << "No pre-requisites" << endl;
    }

    cout << "=== Exclusions ===" << endl;
    count = 0;
    for (p = head; p != nullptr; p = p->next)
    {
        q = p->exclusions;
        if (q != nullptr)
        {
            cout << p->code << ": ";
            for (; q != nullptr; q = q->next)
            {
                cout << q->course->code;
                if (q->next != nullptr)
                    cout << ", ";
            }
            count++;
            cout << endl;
        }
    }
    if (count == 0)
    {
        cout << "No pre-exclusions" << endl;
    }
}

bool ll_insert_prerequisite(Course *head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE])
{

    // TODO: Implementation of inserting a pre-requisite

    if (head == nullptr)
    {
        return false;
    }

    Course *temp = head;

    for (; head != nullptr;)
    {
        if (strcmp(temp->code, preCode) == 0)
        {
            break;
        }
        else if (temp->next == nullptr)
        {
            return false;
        }
        temp = temp->next;
    }

    while (strcmp(head->code, targetCode) != 0)
    {
        if (head->next == nullptr)
        {
            return false;
        }
        else
        {
            head = head->next;
        }
    }

    if (strcmp(head->code, targetCode) == 0)
    {
        CourseItem *courseitem = head->prerequisites;

        if (courseitem == nullptr ||
            strcmp(courseitem->course->code, preCode) > 0)
        {
            CourseItem *newcourseitem = new CourseItem;
            newcourseitem->course = temp;
            newcourseitem->next = courseitem;
            head->prerequisites = newcourseitem;
            return true;
        }
        else
        {
            for (courseitem; courseitem != nullptr; courseitem = courseitem->next)
            {
                if ((courseitem->next == nullptr && strcmp(courseitem->course->code, preCode) < 0) ||
                    (courseitem->next != nullptr && strcmp(courseitem->course->code, preCode) < 0 && strcmp(courseitem->next->course->code, preCode) > 0))
                {
                    CourseItem *newcourseitem = new CourseItem;
                    newcourseitem->course = temp;
                    newcourseitem->next = courseitem->next;
                    courseitem->next = newcourseitem;

                    return true;
                }
            }
        }
    }

    return false;
}

bool ll_insert_exclusion(Course *head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]) // ok
{

    // TODO: Implementation of inserting an exclusion

    if (head == nullptr)
    {
        return false;
    }

    Course *temp = head;
    for (; head != nullptr;)
    {
        if (strcmp(temp->code, excludeCode) == 0)
        {
            break;
        }
        else if (temp->next == nullptr)
        {
            return false;
        }
        temp = temp->next;
    }

    while (strcmp(head->code, targetCode) != 0)
    {
        if (head->next == nullptr)
        {
            return false;
        }
        else
        {
            head = head->next;
        }
    }

    if (strcmp(head->code, targetCode) == 0)
    {

        CourseItem *courseitem = head->exclusions;

        if (courseitem == nullptr || strcmp(courseitem->course->code, excludeCode) > 0)
        {
            CourseItem *newcourseitem = new CourseItem;
            newcourseitem->course = temp;
            newcourseitem->next = courseitem;
            head->exclusions = newcourseitem;

            return true;
        }
        else
        {
            for (courseitem; courseitem != nullptr; courseitem = courseitem->next)
            {
                if ((courseitem->next == nullptr && strcmp(courseitem->course->code, excludeCode) < 0) ||
                    (courseitem->next != nullptr && strcmp(courseitem->course->code, excludeCode) < 0 && strcmp(courseitem->next->course->code, excludeCode) > 0))
                {
                    CourseItem *newcourseitem = new CourseItem;
                    newcourseitem->course = temp;
                    newcourseitem->next = courseitem->next;
                    courseitem->next = newcourseitem;

                    return true;
                }
            }
        }
    }

    return false;
}

bool ll_delete_prerequisite(Course *head, const char targetCode[MAX_CODE], const char preCode[MAX_CODE])
{

    // TODO: Implementation of deleting a pre-requisite

    if (head == nullptr)
    {
        return false;
    }

    while (strcmp(targetCode, head->code) != 0)
    {
        if (head->next == nullptr)
        {
            return false;
        }
        else
        {
            head = head->next;
        }
    }
    CourseItem *prevcourse = nullptr;
    CourseItem *tempcourseitem;

    if (strcmp(head->code, targetCode) == 0)
    {
        for (head; head->prerequisites != nullptr; head->prerequisites = head->prerequisites->next)
        {
            if (strcmp(head->prerequisites->course->code, preCode) == 0)
            {
                tempcourseitem = head->prerequisites->next;
                if (prevcourse != nullptr)
                {
                    tempcourseitem = head->prerequisites->next;
                    delete head->prerequisites;
                    prevcourse->next = tempcourseitem;
                    return true;
                }
                else
                {
                    prevcourse = head->prerequisites;
                    delete head->prerequisites;
                    prevcourse->next = tempcourseitem;
                    return true;
                }
            }
            prevcourse = head->prerequisites;
        }
    }
    return false;
}

bool ll_delete_exclusion(Course *head, const char targetCode[MAX_CODE], const char excludeCode[MAX_CODE]) // ok
{

    // TODO: Implementation of deleting an exclusion
    if (head == nullptr)
    {
        return false;
    }

    Course *temp = head;

    while (strcmp(head->code, targetCode) != 0)
    {
        if (head->next == nullptr)
        {
            return false;
        }
        else
        {
            head = head->next;
        }
    }

    if (strcmp(head->code, targetCode) == 0)
    {
        for (head; head->exclusions != nullptr; head->exclusions = head->exclusions->next)
        {
            if (head->exclusions != nullptr)
            {
                if (strcmp(head->exclusions->course->code, excludeCode) == 0)
                {
                    CourseItem *c = head->exclusions->next;
                    delete head->exclusions;
                    head->exclusions = c;
                    return true;
                }
            }
            else if (head->exclusions->next != nullptr)
            {
                if (strcmp(head->exclusions->next->course->code, excludeCode) == 0)
                {
                    CourseItem *c = head->exclusions->next->next;
                    delete head->exclusions->next;
                    head->exclusions->next = c;
                    return true;
                }
            }
        }
    }
    return false;
}

bool ll_insert_course(Course *&head, const char c[MAX_CODE], const char t[MAX_TITLE], int cred) // ok
{

    // TODO: Implementation of inserting a course
    Course *temp = head;

    if (head == nullptr || strcmp(head->code, c) > 0)
    {
        head = new Course;
        strcpy(head->code, c);
        strcpy(head->title, t);
        head->credit = cred;
        head->next = temp;
        head->exclusions = head->prerequisites = nullptr;
        return true;
    }

    for (head; head != nullptr; head = head->next)
    {
        if ((head->next == nullptr && strcmp(head->code, c) < 0) ||
            (head->next != nullptr && strcmp(head->code, c) < 0 && strcmp(head->next->code, c) > 0))
        {
            Course *course = new Course;
            strcpy(course->code, c);
            strcpy(course->title, t);
            course->credit = cred;
            course->next = head->next;
            course->exclusions = course->prerequisites = nullptr;
            head->next = course;
            head = temp;
            return true;
        }
    }

    head = temp;
    return false;
}

bool ll_delete_course(Course *&head, const char c[MAX_CODE])
{

    // TODO: Implementation of deleting a course
    Course *prevcourse = nullptr;
    Course *temp = head;

    if (head == nullptr)
    {
        return false;
    }

    while (temp != nullptr)
    {
        if (strcmp(temp->code, c) == 0)
        {
            break;
        }
        prevcourse = temp;
        temp = temp->next;
    }
    if (temp == nullptr)
    {
        return false;
    }

    for (Course *p = head; p != nullptr; p = p->next)
    {
        if (strcmp(p->code, c) != 0)
        {
            ll_delete_prerequisite(head, p->code, c);
            ll_delete_exclusion(head, p->code, c);
        }
    }
    delete_courseItem(temp->prerequisites);
    delete_courseItem(temp->exclusions);

    if (strcmp(head->code, c) != 0)
    {
        prevcourse->next = temp->next;
        delete temp;
        temp = nullptr;
    }
    else
    {
        temp = head;
        head = head->next;
        delete temp;
        temp = nullptr;
    }
    return true;
}

bool ll_modify_course_title(Course *head, const char c[MAX_CODE], const char t[MAX_TITLE]) // ok
{

    // TODO: Implementation of modifying a course title
    if (head == nullptr)
    {
        return false;
    }
    for (head; head != nullptr; head = head->next)
    {
        if (strcmp(head->code, c) == 0)
        {
            strcpy(head->title, t);
            return true;
        }
    }
    return false;
}

bool ll_modify_course_credit(Course *head, const char c[MAX_CODE], int cred) // ok
{

    // TODO: Implementation of modifying a course credit
    if (head == nullptr)
    {
        return false;
    }
    for (head; head != nullptr; head = head->next)
    {
        if (strcmp(head->code, c) == 0)
        {
            head->credit = cred;
            return true;
        }
    }
    return false;
}

void ll_cleanup_all(Course *&head) // ok
{

    // TODO: Implementation of clean up all
    if (head == nullptr)
    {
        return;
    }
    else
    {
        ll_cleanup_all(head->next);
        delete_courseItem(head->prerequisites);
        delete_courseItem(head->exclusions);
        delete head;
        head = nullptr;
    }
}
