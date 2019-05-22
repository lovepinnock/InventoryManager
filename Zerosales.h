#ifndef ZEROSALES_H
#define ZEROSALES_H
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

class Zerosales
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
        Zerosales();
};

Zerosales::Zerosales()
{
    //Variables
    fstream timesfile;
    fstream salesdata;
    string filename = "Inventory.dat";
    string filename1 = "Sales.dat";
    string filename2 = "Timestamp.dat";
    int ind;
    Data resetted;

    //Checks file status
    timesfile.open(filename2, ios::in|ios::binary);

    //For time
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    //Vector of structures to modify data
    vector <Data> resetting;
    int nowon = 0;


    //If first time opening file
    if(timesfile.fail())
    {
        ind = 1;
        timesfile.open(filename2,ios::out|ios::binary);
        timesfile << ind;
        timesfile.close();

        return;
    }

    //Opens file
    else
    {

        timesfile.open(filename2, ios::in|ios::out|ios::app|ios::binary);

        //Checks status of file
        timesfile >> ind;

        //Get current time
        if(( (timePtr->tm_mday) == 1) && (ind < 1) )
        {
            //Putting Data into vector
            salesdata.open(filename, ios::in|ios::binary);

            while( !salesdata.eof() )
            {
                salesdata.read(reinterpret_cast<char*>(&resetted), sizeof(resetted) );
                resetting.push_back(resetted);
            }

            salesdata.close();

            //Resetting sales to zero
            for(int val = 0; val < resetting.size(); val++)
                if(resetting[val].Numberofproductssold != 0)
                    resetting[val].Numberofproductssold = 0;

            //Writing Data to file
            salesdata.open(filename, ios::out|ios::binary);
            salesdata.close();

            salesdata.open(filename, ios::out|ios::app|ios::binary);
            for(int val = 0; val < resetting.size(); val++)
            {
                resetted.Genre = resetting[val].Genre;
                strncpy(resetted.Productname, resetting[val].Productname, charnum - 1);
                resetted.Morethantwo = resetting[val].Morethantwo;
                resetted.Noticeseen = resetting[val].Noticeseen;
                resetted.Numberofproducts = resetting[val].Numberofproducts;
                resetted.Numberofproductsleft = resetting[val].Numberofproductsleft;
                resetted.Numberofproductssold = resetting[val].Numberofproductssold;
                resetted.price = resetting[val].price;

                salesdata.write(reinterpret_cast<char *>(&resetted), sizeof(resetted));
            }

            salesdata.close();

            ind = 1;
            timesfile.open(filename2, ios::binary|ios::out|ios::trunc);
            timesfile << ind;
            timesfile.close();
        }

        else
        {
            if( (timePtr->tm_mday) != 1 )
            {
                ind = 0;
                timesfile.open(filename2, ios::out|ios::binary|ios::trunc);
                timesfile << ind;
                timesfile.close();
            }
        }
    }

}


#endif // ZEROSALES_H
