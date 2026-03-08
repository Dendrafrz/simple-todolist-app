#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// ─────────────────────────────────────────
//  Simple CLI To-Do List
//  Stores tasks in todo.txt (persistent)
// ─────────────────────────────────────────

const std::string FILE_NAME = "todo.txt";

struct Task
{
    std::string text;
    bool done;
};

// ── File I/O ──────────────────────────────

std::vector<Task> loadTasks()
{
    std::vector<Task> tasks;
    std::ifstream file(FILE_NAME);
    if (!file.is_open())
        return tasks;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() < 3)
            continue;
        Task t;
        t.done = (line[0] == '1');
        t.text = line.substr(2); // format: "0|task text"
        tasks.push_back(t);
    }
    return tasks;
}

void saveTasks(const std::vector<Task> &tasks)
{
    std::ofstream file(FILE_NAME);
    for (const auto &t : tasks)
        file << (t.done ? "1" : "0") << "|" << t.text << "\n";
}

// ── Display ───────────────────────────────

void listTasks(const std::vector<Task> &tasks)
{
    if (tasks.empty())
    {
        std::cout << "  (no tasks yet)\n";
        return;
    }
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        std::string status = tasks[i].done ? "[x]" : "[ ]";
        std::cout << "  " << (i + 1) << ". " << status << " " << tasks[i].text << "\n";
    }
}

void printHeader()
{
    std::cout << "\n============================\n";
    std::cout << "       CLI To-Do List\n";
    std::cout << "============================\n";
}

void printMenu()
{
    std::cout << "\n  1. List tasks\n";
    std::cout << "  2. Add task\n";
    std::cout << "  3. Mark task as done\n";
    std::cout << "  4. Delete task\n";
    std::cout << "  0. Exit\n";
    std::cout << "\n  Choice: ";
}

// ── Actions ───────────────────────────────

void addTask(std::vector<Task> &tasks)
{
    std::cout << "  Task: ";
    std::cin.ignore();
    std::string text;
    std::getline(std::cin, text);
    if (!text.empty())
    {
        tasks.push_back({text, false});
        saveTasks(tasks);
        std::cout << "  ✓ Task added.\n";
    }
}

void markDone(std::vector<Task> &tasks)
{
    listTasks(tasks);
    if (tasks.empty())
        return;
    std::cout << "  Task number to mark done: ";
    int n;
    std::cin >> n;
    if (n >= 1 && n <= (int)tasks.size())
    {
        tasks[n - 1].done = true;
        saveTasks(tasks);
        std::cout << "  ✓ Marked as done.\n";
    }
    else
    {
        std::cout << "  Invalid number.\n";
    }
}

void deleteTask(std::vector<Task> &tasks)
{
    listTasks(tasks);
    if (tasks.empty())
        return;
    std::cout << "  Task number to delete: ";
    int n;
    std::cin >> n;
    if (n >= 1 && n <= (int)tasks.size())
    {
        tasks.erase(tasks.begin() + n - 1);
        saveTasks(tasks);
        std::cout << "  ✓ Task deleted.\n";
    }
    else
    {
        std::cout << "  Invalid number.\n";
    }
}

// ── Main ──────────────────────────────────

int main()
{
    std::vector<Task> tasks = loadTasks();
    printHeader();

    int choice = -1;
    while (choice != 0)
    {
        printMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "\n";
            listTasks(tasks);
            break;
        case 2:
            addTask(tasks);
            break;
        case 3:
            markDone(tasks);
            break;
        case 4:
            deleteTask(tasks);
            break;
        case 0:
            std::cout << "  Bye!\n\n";
            break;
        default:
            std::cout << "  Unknown option.\n";
        }
    }
    return 0;
}