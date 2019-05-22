#ifndef BYTES_H
#define BYTES_H
#include <iostream>

using namespace std;

const int charnum = 150;

class bytes
{
    private:
        struct Data
        {
            char Genre;
            char Productname[charnum];
            int Numberofproducts;
            int Numberofproductsleft;
            int Numberofproductssold;
            bool Morethantwo;
            bool Noticeseen;
            double price;
        };

    public:
        bytes() {}

        long byteNum(int &recordnum)
        { return sizeof(Data) * recordnum; }
};

#endif // BYTES_H
