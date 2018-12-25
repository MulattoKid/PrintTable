#define PRINT_TABLE_IMPLEMENTATION
#include "PrintTable.h"

int main(int argc, char**argv)
{
    PrintTable pt;
    pt.SetTitle("My Friends' Gaming GPUs");
    pt.AddColumn("Vendor");
    pt.AddColumn("GPU Name");
    pt.AddColumn("Release Year");
    std::vector<std::vector<std::string>> rows = {
        {
            "Nvidia", "GTX 980 Ti", "2015"
        },
        {
            "Nvidia", "GTX 1070", "2016"
        },
        {
            "Nvidia", "GTX 1080", "2016"
        },
        {
            "Nvidia", "RTX 2080", "2018"
        }
    };
    pt.AddRows(rows);
    pt.Print();
    pt.Reset();
    pt.Print();

    return 0;
}