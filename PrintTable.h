/*
The following is a very simple way to print formatted tables in C++.
An example of a program using PrintTable can be seen in main.cpp in the Github repo.

There are a few basic limitations that if ignored, will result in miss-formated tables
and/or undefined behavior:
    1) No string should contain linebreaks
    2) The title string should not be longer than the all of the column names combined
    3) It is not possible to add new columns after adding the first row
    4) A row must have n elements where n is the number of columns in the table
Some basic error checking is present to alert you if you should violate some of these.

Printing the table will calculate the necessary data to print the table in the correct
format. This will induce a slight overhead depending on the size of the table as the
table format must be built. This penalty is only paid the first time the table is printed.
Consecutive prints are faster as the necessary format structure is stored. If the
following (or similar) structure is present in your program:
    Create table
    Print
    Add row
    Print
the penalty of building the table format is paid for both print statements as the table has
been changed between print statements.

Upon resetting the table, the title, columns and rows are deleted and must be set again.
This will naturally require a rebuilding of the format structure.

Items that could be interesting to implement to extend the functionality:
    - Linebreaks in strings
    - Tables within tables
    - Table customization
*/

#ifndef PRINT_TABLE_H
#define PRINT_TABLE_H

#include <string>
#include <vector>

struct PrintTable
{
    //Base data
    std::string title;
    std::vector<std::string> columnNames;
    std::vector<std::vector<std::string>> rows;
    bool startedAddingRows = false;
    bool alteredState = false;

    //Format data
    std::vector<int> maxColumnWidths;
    std::string fullDividerStr;
    std::string titleStr;
    std::string columnStr;
    std::vector<std::string> rowStrs;

    //Functions
    void SetTitle(const std::string& title);
    void AddColumn(const std::string& columnName);
    void AddRow(const std::vector<std::string>& row);
    void AddRows(const std::vector<std::vector<std::string>>& rows);
    void Print();
    void Reset();
};

void PrintTable::SetTitle(const std::string& title)
{
    this->title = title;
    alteredState = true;
}

void PrintTable::AddColumn(const std::string& columnName)
{
    if (startedAddingRows)
    {
        printf("Table '%s' already has rows added: additional columns cannot be added.\n", title.c_str());
        return;
    }
    columnNames.push_back(columnName);
    alteredState = true;
}

void PrintTable::AddRow(const std::vector<std::string>& row)
{
    if (row.size() != columnNames.size())
    {
        printf("Trying to add row with %lu elements while table '%s' requires %lu elements per row.\n", row.size(), title.c_str(), columnNames.size());
        return;
    }
    rows.push_back(row);
    startedAddingRows = true;
    alteredState = true;
}

void PrintTable::AddRows(const std::vector<std::vector<std::string>>& rows)
{
    for (const std::vector<std::string>& row : rows)
    {
        if (row.size() != columnNames.size())
        {
            printf("Trying to add row with %lu elements while table '%s' requires %lu elements per row.\n", row.size(), title.c_str(), columnNames.size());
        }
        this->rows.push_back(row);
    }
    startedAddingRows = true;
    alteredState = true;
}

void PrintTable::Print()
{
    if (title.empty() || columnNames.empty() || rows.empty())
    {
        printf("Missing some necessary data to print table:\n\tTitle: '%s' (must not be empty)\n\tNumber of columns: %lu (min=1)\n\tNumber of rows: %lu (min=1)\n", title.c_str(), columnNames.size(), rows.size());
        return;
    }
    if (alteredState)
    {
        // Find max width of each column
        maxColumnWidths.resize(columnNames.size());
        for (size_t i = 0; i < columnNames.size(); i++)
        {
            maxColumnWidths[i] = columnNames[i].length();
        }
        for (size_t r = 0; r < rows.size(); r++)
        {
            for (size_t c = 0; c < columnNames.size(); c++)
            {
                if (int(rows[r][c].length()) > maxColumnWidths[c])
                {
                    maxColumnWidths[c] = rows[r][c].length();
                }
            }
        }

        /*
        -------------------------------
        |          Test table         |
        -------------------------------
        | column0 | column1 | column2 |
        -------------------------------
        |  row0   |  row0   |  row0   |
        |-----------------------------|
        |  row1   |  row1   |  row1   |
        |-----------------------------|
        |  row2   |  row2   |  row2   |
        -------------------------------
        */
        int tableWidth = 0;
        for (const int& width : maxColumnWidths)
        {
            // +3 because it includes the space for the first |
            // and a space on each side of the column name (see above example)
            tableWidth += width + 3;
        }
        // Make space for the last |
        tableWidth += 1;
        fullDividerStr = std::string(tableWidth, '-');

        // Create string with title
        titleStr = "";
        {
            // Subtract 4 to ensure that the inital, and last, | and space are ignored
            const int lengthDiff = (tableWidth - 4) - title.length();
            // Divide by 2 to get number of pre spaces
            const int numPreSpace = lengthDiff / 2;
            // Divide by 2, but increment lengthDiff by one to round up.
            // I do this because I want extra spaces after the column name
            const int numPostSpace = (lengthDiff + 1) / 2;
            const std::string preStr(numPreSpace, ' ');
            const std::string postStr(numPostSpace, ' ');
            titleStr += "| " + preStr + title + postStr + " |";
        }

        // Create string with each column name
        columnStr = "";
        for (size_t i = 0; i < columnNames.size(); i++)
        {
            const std::string& columnName = columnNames[i];
            const int lengthDiff = maxColumnWidths[i] - columnName.length();
            if (lengthDiff > 0)
            {
                // Divide by 2 to get number of pre spaces
                const int numPreSpace = lengthDiff / 2;
                // Divide by 2, but increment lengthDiff by one to round up.
                // I do this because I want extra spaces after the column name
                const int numPostSpace = (lengthDiff + 1) / 2;
                const std::string preStr(numPreSpace, ' ');
                const std::string postStr(numPostSpace, ' ');
                columnStr += "| " + preStr + columnName + postStr + " ";
            }
            else
            {
                columnStr += "| " + columnName + " ";
            }
        }
        columnStr += "|";

        // Create string for each row and its elements
        rowStrs = std::vector<std::string>(rows.size());
        for (size_t r = 0; r < rows.size(); r++)
        {
            for (size_t e = 0; e < rows[r].size(); e++)
            {
                const std::string& elementData = rows[r][e];
                const int lengthDiff = maxColumnWidths[e] - elementData.length();
                // Divide by 2 to get number of pre spaces
                const int numPreSpace = lengthDiff / 2;
                // Divide by 2, but increment lengthDiff by one to round up.
                // I do this because I want extra spaces after the column name
                const int numPostSpace = (lengthDiff + 1) / 2;
                const std::string preStr(numPreSpace, ' ');
                const std::string postStr(numPostSpace, ' ');
                rowStrs[r] += "| " + preStr + elementData + postStr + " ";
            }
            rowStrs[r] += "|";
        }
    }

    // Print table
    printf("%s\n", fullDividerStr.c_str());
    printf("%s\n", titleStr.c_str());
    printf("%s\n", fullDividerStr.c_str());
    printf("%s\n", columnStr.c_str());
    printf("%s\n", fullDividerStr.c_str());
    for (const std::string& rowStr : rowStrs)
    {
        printf("%s\n", rowStr.c_str());
    }
    printf("%s\n", fullDividerStr.c_str());

    alteredState = false;
}

void PrintTable::Reset()
{
    title = "";
    columnNames.resize(0);
    rows.resize(0);
    maxColumnWidths.resize(0);
    startedAddingRows = false;
    alteredState = true;
}

#endif