#include<iostream>
#include<cmath>
#include<ctime>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<cstdlib>
#include<limits>
#include "bytes.h"
#include "Validation.h"
#include "Zerosales.h"
#undef max

using namespace std;

//Structure for data

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

struct Sales
{
    bool deleted;
    char name[charnum];
    int numberofsales;
    double price;
    int month;
    int day;
    int year;

};

struct Names
{
    char storedbooknames[charnum];
    char storedmovienames[charnum];
    char storedothernames[charnum];
    bool books;
    bool movies;
    bool others;
    int thisone;

};

void Input();
void Modify();
void Current();
void neg(int&);
void neg(double&);
void neg1(int&);
void neg2(int&);
void Tally(char, int = 0, double = 0.0, char * = nullptr);
void sum(char, int = 0, int = 0,int = 0, int = 0, int = 0, int = 0, int = 0);
void deleterec();

//List of options
string options[] = {"1) Input New Items( if you have new items to sell, use this) ",
                    "2) Modify Data( If you sold an item or made mistake, use this ) ",
                    "3) Show current inventory(shows how much of everything is left",
                    "4) Sales Data" };

//List of genre options
string listofgenres[] = {"1) Book", "2) Movie", "3) Other", "4) Delete Entry" };

//List of sales data range
string ranges[] = {"1) Today" , "2) This Month", "3) This year", "4) All time ", "5) Custom range" };

//Delete Identifier;
char d = '_';


//Sales Identifiers
char f = '#';
char s = '$';
char spo = '%';
char ft = '*';
char all = ')';
char cus = '(';

//Zero Identifiers
char done = 'z';
char notdone = 'a';

//Size of arrays
const int os = 3;
const int logs = 3;
const int r = 4;

//Global filenames
 string filename = "Inventory.dat";
 string filename1 = "Sales.dat";
 string filename2 = "Timestamp.dat";

int main()
{
    fstream File;
    Data inventory;
    Validation valid;
    Zerosales zeroing;     //Sets sales to zero first of month
    string response;
    int iterate = 0;

    //Deletion indicator
    char d;
    d = '_';


    File.open(filename, ios::in|ios::binary);
    //Creates File if it doesn't already exist
    if(File.fail())
    {
        File.open(filename, ios::in|ios::out|ios::binary);
        File.close();
        Input();
        exit(0);
    }

    else
        File.close();

    File.open(filename, ios::in|ios::binary);

    //Find Low inventory and then display it
    vector <Data> low;
    vector <int> theyat;
    int they = 0;

    while( File.read(reinterpret_cast<char *>(&inventory), sizeof(inventory)) )
    {
        low.push_back(inventory);
        if( (low[they].Morethantwo) && !(low[they].Noticeseen) && (low[they].Numberofproducts < 2))
            theyat.push_back(they);
        they++;;
    }

    File.close();

    if(theyat.size() > 0)
    {
        cout<<"Low Inventory Warning: "<<endl;
        cout<<"_____________________  "<<endl;
    }


    for(int val = 0; val < theyat.size(); val++)
    {
        int p = theyat[val];

        //Checking what products are out of stock
        if( (low[p].Numberofproducts <= 1) && !(low[p].Noticeseen) && (low[p].Morethantwo) )
        {
            cout<<"There is "<<low[p].Numberofproductsleft<<" "<<low[p].Productname<<" left."<<endl;
            //Changing Noticeseen flag to true
            low[p].Noticeseen = true;

            //Since inventory is less than two now
            low[p].Morethantwo = false;
        }

        else
            if(low[p].Numberofproductsleft == 0 && !low[p].Noticeseen)
            {
                cout<<"There is no "<<low[p].Productname<<"'s left."<<endl;
                //Changing Noticeseen flag to true
                low[p].Noticeseen = true;

                //Since inventory is less than two now
                low[p].Morethantwo = false;
            }
    }

    //Erasing Data
        File.open(filename, ios::out|ios::binary);
        File.close();

    //Rewriting
        File.open(filename, ios::out|ios::app|ios::binary);

        for(int val = 0; val < low.size(); val++)
        {
            inventory.Genre = low[val].Genre;
            strncpy(inventory.Productname, low[val].Productname, charnum - 1);
            inventory.Morethantwo = low[val].Morethantwo;
            inventory.Noticeseen = low[val].Noticeseen;
            inventory.Numberofproducts = low[val].Numberofproducts;
            inventory.Numberofproductsleft = low[val].Numberofproductsleft;
            inventory.Numberofproductssold = low[val].Numberofproductssold;
            inventory.price = low[val].price;
            File.write( reinterpret_cast<char*>(&inventory), sizeof(inventory) );
        }

        //Clearing EOF Flag
        File.clear();
        File.close();

        if(theyat.size() > 0)
            cout<<endl;

        //Ask if they would like to input data or modify data
        cout<<"What would you like to do? (Type in Input/Modify/Current/Sales or 1,2,3,4 or I,M,C,S): "<<endl<<endl;
        //Display options
        while(iterate <= os)
        {
            cout<<options[iterate]<<endl;
            iterate++;
        }

        cin >> response;

        //If invalid input is entered
        response = valid.validate(response, 'M');

        if(response == "1" || response == "Input" || response == "input" || response == "I" || response == "i")
            Input();
        else if( response == "2" || response == "Modify" || response == "modify" || response == "M" || response == "m")
            Modify();
        else if( response == "3" || response == "Current" || response == "C" || response == "current" || response == "c")
            Current();
        else
            Tally('M');


    system("pause");
    return 0;
}

void neg(int &num)
{
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout<<"Invalid input(possibly entered a comma or something), please reenter a number."<<endl;
        cin >> num;
        system("pause");
    }

    if(num < 0)
    {
        cout<<"You have entered a negative number, enter the correct amount below: "<<endl;
        cin >> num;
    }

}

void neg(double &num)
{
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout<<"Invalid input(possibly entered a comma or something), please reenter a number."<<endl;
        cin >> num;
        system("pause");
    }

    if(num < 0.0)
    {
        cout<<"You entered a negative price, enter the correct amount below: "<<endl;
        cin >> num;
    }
}

void neg1(int &n)
{
    Data temp;
    int temp1, temp2;

    if(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout<<"Invalid input(possibly entered a comma or something), please reenter a number."<<endl;
        cin >> n;
        system("pause");
    }

    if(n < 0)
    {
        cout<<"You have negative inventory, you have either sold to much or entered an incorrect"<<endl;
        cout<<"amount to be sold/typed an incorrect amount of product you have in stock."<<endl;
        cout<<"Go check your inventory and type the correct amount of product below. "<<endl;
        cin >> n;
    }

}

void neg2(int &j)
{
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout<<"Invalid input(possibly entered a comma or something), please reenter a number."<<endl;
        cin >> j;
        system("pause");
    }

    if(j  == 0)
    {
        cout<<"You have entered invalid option, please enter valid list entry: "<<endl;
        cin  >> j;
    }


}

//Inputs data into file
void Input()
{
    Data put;
    Validation valid1;
    int choice;
    double temp;
    string genrechosen;
    ofstream F;

    do
    {
        int count = 0;

        cout<<"What genre is the item? (Type the option or enter 1,2, or 3): "<<endl;
        while(count < logs)
        {
            cout<<listofgenres[count]<<endl;
            count++;
        }

        cin >> genrechosen;

        genrechosen = valid1.validate(genrechosen, 'I');

        if(genrechosen == "1" || genrechosen == "Books" || genrechosen == "books" || genrechosen == "Book" || genrechosen == "book" || genrechosen == "B" || genrechosen == "b")
        {
            F.open(filename, ios::out|ios::app|ios::binary);
            put.Genre = '~';

            cout<<"What is the name of the book: "<<endl;
            cin.ignore();
            cin.getline(put.Productname, charnum - 1);

            cout<<"How many "<<put.Productname<<" do you have: "<<endl;
            cin >> put.Numberofproducts;
            neg(put.Numberofproducts);

            cout<<"How much are you selling "<<put.Productname<<" for? (If you don't know , just put 0. Don't put a $): "<<endl;
            cin >> temp;
            put.price = static_cast<int> (temp);
            neg(put.price);

            put.Numberofproductssold = 0;
            put.Numberofproductsleft = put.Numberofproducts;

            if(put.Numberofproducts >= 2)
                put.Morethantwo = true;
            else
                put.Morethantwo = false;

            put.Noticeseen = false;

             F.write( (const char *)&put, sizeof(put));
             F.close();
        }

        if(genrechosen == "2" || genrechosen == "Movies" || genrechosen == "movies" || genrechosen == "Movie" || genrechosen == "movie" || genrechosen == "M" || genrechosen == "m")
        {
            F.open(filename, ios::out|ios::binary|ios::app);
            put.Genre = '!';

            cout<<"What is the name of the movie: "<<endl;
            cin.ignore();
            cin.getline(put.Productname, charnum - 1);

            cout<<"How many "<<put.Productname<<" do you have: "<<endl;
            cin >> put.Numberofproducts;
            neg(put.Numberofproducts);

            cout<<"How much are you selling "<<put.Productname<<" for? (If you don't know , just put 0. Don't put a $): "<<endl;
            cin >> temp;
            put.price = static_cast<int> (temp);
            neg(put.price);

            put.Numberofproductssold = 0;
            put.Numberofproductsleft = put.Numberofproducts;

            if(put.Numberofproducts >= 2)
                put.Morethantwo = true;
            else
                put.Morethantwo = false;

            put.Noticeseen = false;

             F.write( (const char *)&put, sizeof(put));
            F.close();
        }

        if(genrechosen == "3" || genrechosen == "Other" || genrechosen == "other" || genrechosen == "O" || genrechosen == "o")
        {
            F.open(filename, ios::out|ios::binary|ios::app);
            put.Genre = '=';

            cout<<"What is the name of the item: "<<endl;
            cin.ignore();
            cin.getline(put.Productname, charnum - 1);

            cout<<"How many "<<put.Productname<<" do you have: "<<endl;
            cin >> put.Numberofproducts;
            neg(put.Numberofproducts);

            cout<<"How much are you selling "<<put.Productname<<" for? (If you don't know , just put 0. Don't put $): "<<endl;
            cin >> temp;
            put.price = static_cast<int> (temp);
            neg(put.price);

            put.Numberofproductssold = 0;
            put.Numberofproductsleft = put.Numberofproducts;

            if(put.Numberofproducts >= 2)
                put.Morethantwo = true;
            else
                put.Morethantwo = false;

            put.Noticeseen = false;

            F.write( (const char *)&put, sizeof(put));
            F.close();
        }

        system("pause");

        cout<<"Do you need to input anything else(Put 0 if you are done, put 1 if you are not): "<<endl;
        cin >> choice;
        system("pause");


    }while(choice != 0);
}

//Modifies and deletes data
void Modify()
{
    bytes Numof;
    Validation valid2;
    Data change;
    Data change2;
    string answer;
    char genrechosen;
    int sold;
    int continuee;
    int continued;
    int passedrecords = 0;
    vector <int> position;
    int trup = 0;

    //Genre Identifiers
    char bs;
    char ms;
    char orr;

    bs = '~';
    ms = '!';
    orr = '=';

    //Modify variables
    char n[charnum];
    char temp[charnum];
    char t;
    int tempNP, tempNPS, tempNPL;
    double tempPrice;
    bool MT2, NS;
    string response;

    //File
    ofstream f;
    ifstream f1;

    //Vector of Structures
    vector <Data> changing;

    //Display options
    do
    {
        f1.open(filename, ios::binary|ios::in);

        int count = 1;

        cout<<"What genre is the item you are looking for? Otherwise enter 4, D/d, or Delete/delete or Delete Entry"<<endl;
        for(int val = 0; val <= logs; val++)
            cout<<listofgenres[val]<<endl;

        cin >> answer;

        //Validates input
        if(answer != "4" || answer != "Delete Entry" || answer != "delete entry" || answer != "Delete" || answer != "delete" || answer != "D" || answer != "d" )
            genrechosen = d;

        else
            answer = valid2.validate(answer, 'I'); // Uses I for the char variable because they both use same list, so there is no point in creating new char for it.

        //Allows for list of only that genre to be shown
        if(answer == "1" || answer == "Books" || answer == "books" || answer == "Book" || answer == "book" || answer == "B" || answer == "b")
            genrechosen = bs;

        if(answer == "2" || answer == "Movies" || answer == "movies" || answer == "Movie" || answer == "movie" || answer == "M" || answer == "m")
            genrechosen = ms;

        if(answer == "3" || answer == "Other" || answer == "other" || answer == "O" || answer == "o")
            genrechosen = orr;

        //Displays List of items in that genre
        if(genrechosen == bs)
            cout<<"Which book's data will you be modifying? (Type in the number):"<<endl;
        if(genrechosen == ms)
            cout<<"Which movie's data will you be modifying? (Type in the number):"<<endl;
        if(genrechosen == orr)
            cout<<"What item's data will you be modifying? (Type in the number):"<<endl;
        if(genrechosen == d)
        {
            deleterec();
            return;
        }

        //Read first record
        while( f1.read( reinterpret_cast<char *>(&change), sizeof(change)) )
        {
           if(change.Genre == genrechosen)
           {
                cout<<count<<") "<<change.Productname<<endl;
                count++;
                position.push_back(passedrecords);
           }

           /* We need to know exactly what record in the file matches that genre, that way when the users chooses a number of the list, we can go and find
           that exact record. We don't want only 2 records to show up and then conclude the user wants the second record when they actually want the 37th record.*/

           passedrecords++;

        }

        if(position.size() > 0)
        {
            cout<<"Enter the number of the item you want to modify "<<endl;
            cin >> continuee;
        }

        else
        {
            cout<<"There are no items of that genre in stock. Program restarting"<<endl;
            return;
        }

        //Clear eof
        f1.clear();

        //Display data
        f1.seekg(Numof.byteNum(position[continuee - 1]), ios::beg);
        f1.read(reinterpret_cast<char*>(&change), sizeof(change));
        cout<<"Product Name is: "<<change.Productname<<endl;
        cout<<"Current inventory is: "<<change.Numberofproducts<<endl;
        f1.close();


        cout<<"If you sold this product, just type the number of items sold or else put -1 to continue: "<<endl;
        cin >> sold;

        if(sold == -1)
        {
            f1.open(filename, ios::in|ios::binary);

            for(int val = 0 ; val < passedrecords; val++)
            {
                f1.read(reinterpret_cast<char*>(&change), sizeof(change));
                changing.push_back(change);
            }

            f1.close();

            cout<<"Here you can change the name, number of products in inventory, etc. Hit -1 if you don't want to change a value: "<<endl;
            cout<<"What will you be modifying today: "<<endl;
            cout<<"1) Genre"<<endl;
            cout<<"2) Name"<<endl;
            cout<<"3) Current inventory(Type in 3 or Current)"<<endl;
            cout<<"4) # of items sold(Type in 4 or #)"<<endl;
            cout<<"5) Price"<<endl;
            cin >> response;

            //Changes genre

            if(response == "Genre" || response == "genre" || response == "1" )
            {
                cout<<"What genre is the product(Type 1, 2 or 3)!: "<<endl;
                for(int val = 0; val <= logs; val++)
                    cout<<listofgenres[val]<<endl;
                cin >> temp;

                if(temp == "1")
                    changing[position[continuee - 1] ].Genre = bs;
                if(temp == "2")
                    changing[position[continuee - 1]].Genre = ms;
                if(temp == "3")
                    changing[position[continuee - 1]].Genre = orr;
            }

            //Changes name of product
            if(response == "Name" || response == "name" || response == "2")
            {
                cout<<"Name: "<<endl;
                cin.ignore();
                cin.getline(n, charnum - 1);
                strncpy(changing[position[continuee - 1]].Productname, n, charnum - 1);
            }

            //Changes number of products left in stock
            if(response == "Current" || response == "current" || response == "3")
            {
                cout<<"Number of products in stock: "<<endl;
                cin >> tempNP;
                neg(tempNP);
                changing[position[continuee - 1]].Numberofproducts = tempNP;
                changing[position[continuee - 1]].Numberofproductsleft = tempNP;
                changing[position[continuee - 1]].Numberofproductssold = 0;
            }


            //Changes number of products sold
            if(response == "#" || response == "4")
            {
                cout<<"Number of items sold: "<<endl;
                cin >> tempNPS;

                neg1(tempNPS);
                changing[position[continuee - 1]].Numberofproductssold = tempNPS;
                changing[position[continuee - 1]].Numberofproductsleft -= tempNPS;
            }

            //Changes price
            if(response == "Price" || response == "price" || response == "5")
            {
                cout<<"Price: "<<endl;
                cin >> tempPrice;
                tempPrice = static_cast<double>(tempPrice);

                neg(tempPrice);
                changing[position[continuee - 1]].price = tempPrice;
            }

            //Checks if there is still more than two items
            if(changing[position[continuee - 1]].Numberofproducts >= 2)
                changing[position[continuee - 1]].Morethantwo = true;
            else
                changing[position[continuee - 1]].Morethantwo = false;

            changing[position[continuee - 1]].Noticeseen = false;

            //Erases data
            f.open(filename, ios::out|ios::binary);
            f.close();

            //Rewrites data
            f.open(filename, ios::out|ios::app|ios::binary);
            for(int val = 0; val < changing.size(); val++)
            {
                strncpy(change.Productname, changing[val].Productname, charnum - 1);
                change.Genre = changing[val].Genre;
                change.Morethantwo = changing[val].Morethantwo;
                change.Noticeseen = changing[val].Noticeseen;
                change.Numberofproducts = changing[val].Numberofproducts;
                change.Numberofproductsleft = changing[val].Numberofproductsleft;
                change.Numberofproductssold = changing[val].Numberofproductssold;
                change.price = changing[val].price;

                f.write(reinterpret_cast<char *>(&change) , sizeof(change));
            }

            f.close();
            cout<<"Done!"<<endl;

        }

        //Item was sold
        else
        {
            f1.open(filename, ios::in|ios::binary);

            for(int val = 0 ; val < passedrecords; val++)
            {
                f1.read(reinterpret_cast<char*>(&change), sizeof(change));
                changing.push_back(change);
            }

            f1.close();

            neg1(sold);


            if(changing[position[continuee - 1] ].Numberofproducts < sold)
            {
                cout<<"You now have negative inventory, please reenter number of items sold: "<<endl;
                cin >> sold;
                changing[position[continuee - 1]].Numberofproducts -= sold;
                changing[position[continuee - 1]].Numberofproductsleft -= sold;
                changing[position[continuee - 1]].Numberofproductssold += sold;
            }

            else
            {
                changing[position[continuee - 1]].Numberofproducts -= sold;
                changing[ position[continuee - 1] ].Numberofproductsleft -= sold;
                changing[position[continuee - 1]].Numberofproductssold += sold;
            }

            int CNPS = changing[position[continuee - 1]].Numberofproductssold;
            double CP = changing[position[continuee - 1]].price;
            char CPN[charnum];
            int p = position[continuee - 1];
            strncpy(CPN, changing[p].Productname , charnum - 1);

            Tally('2',CNPS, CP, CPN);

             //Erases data
            f.open(filename, ios::out|ios::binary);
            f.close();

            //Rewrites data
            f.open(filename, ios::out|ios::app|ios::binary);

            for(int val = 0; val < changing.size(); val++)
            {
                strncpy(change.Productname, changing[val].Productname, charnum - 1);
                change.Genre = changing[val].Genre;
                change.Morethantwo = changing[val].Morethantwo;
                change.Noticeseen = changing[val].Noticeseen;
                change.Numberofproducts = changing[val].Numberofproducts;
                change.Numberofproductsleft = changing[val].Numberofproductsleft;
                change.Numberofproductssold = changing[val].Numberofproductssold;
                change.price = changing[val].price;

                f.write(reinterpret_cast<char *>(&change) , sizeof(change));
            }

            f.close();
            cout<<"Done!"<<endl;
        }

        system("pause");


        cout<<"Do you want to modify anymore data? (Enter 0 if no): "<<endl;
        cin >> continued;

        system("pause");

    }while(continued != 0);

}

//Shows current inventory
void Current ()
{
    Data C;
    Names N;
    ifstream i;

    char bs;
    char ms;
    char orr;

    bs = '~';
    ms = '!';
    orr = '=';

    //Static total inventory
    static int total_inventory = 0;
    static int total_books = 0;
    static int total_movies = 0;
    static int total_other = 0;

    //Name of items
    vector <Names> containnames;


    char d;
    d = '_';

    i.open(filename, ios::binary|ios::in);

    if(!i.fail())
        i.read(reinterpret_cast<char *>(&C) , sizeof(C));


    while( !i.eof() )
    {
        while( C.Genre == d && !i.eof() )
            i.read((char *)&C , sizeof(C));



        if(C.Genre != d)
        {
            //Calculates total inventory
            total_inventory += C.Numberofproductsleft;

            //Stores individual data on each genre
            if(C.Genre == bs)
            {
                total_books += C.Numberofproductsleft;
                strncpy(N.storedbooknames, C.Productname, charnum - 1);
                N.thisone = C.Numberofproductsleft;
                N.books = true;
                N.movies = false;
                N.others = false;
                containnames.push_back(N);
            }

            if(C.Genre == ms)
            {
                total_movies += C.Numberofproductsleft;
                strncpy(N.storedmovienames, C.Productname, charnum - 1);
                N.thisone = C.Numberofproductsleft;
                N.books = false;
                N.movies = true;
                N.others = false;
                containnames.push_back(N);
            }

            if(C.Genre == orr)
            {
                total_other += C.Numberofproductsleft;
                strncpy(N.storedothernames, C.Productname, charnum - 1);
                N.thisone = C.Numberofproductsleft;
                N.books = false;
                N.movies = false;
                N.others = true;
                containnames.push_back(N);
            }

            i.read((char *)&C , sizeof(C));
        }
    }

    cout<<endl<<"There is in total "<<total_inventory<<" items currently in stock "<<endl;

    cout<<endl;

    cout<<"Out of the "<<total_inventory<<" items, there is "<<total_books<<" books: "<<endl;
    for(int val = 0; val < containnames.size(); val++)
    {
        //Load a structure into memory and read data

        if( containnames[val].books )
          cout<<"There is currently "<<(containnames[val].thisone)<<" "<<(containnames[val].storedbooknames)<<" in stock."<<endl;
    }

    cout<<endl;

    //Number of Movies
    cout<<"Out of the "<<total_inventory<<" items, there is "<<total_movies<<" movies: "<<endl;
    for(int val = 0; val < containnames.size(); val++)
    {
        //Load a structure into memory and read data

        if( containnames[val].movies )
            cout<<"There is currently "<<(containnames[val].thisone)<<" "<<(containnames[val].storedmovienames)<<" in stock."<<endl;
    }

    cout<<endl;

    //Number of Other
    cout<<"Out of the "<<total_inventory<<" items, there is "<<total_other<<" other items. "<<endl;
    for(int val = 0; val < containnames.size(); val++)
    {
        //Load a structure into memory and read data

        if( containnames[val].others )
            cout<<"There is currently "<<(containnames[val].thisone)<<" "<<(containnames[val].storedothernames)<<" in stock."<<endl;
    }

    system("pause");

    i.close();
}

void sum( char m, int time, int time2, int time3, int time4, int time5, int time6, int time7)
{
    fstream filed;
    Sales summation;
    int total = 0;
    double rev = 0.0;
    bool there = false;

    //No overwritten data exist any more, it is simply erased.

    //For calculating hottest item
    long highestpos;
    int highest;

    filed.open(filename1, ios::in|ios::binary);

    //Today
    if(m == f)
    {
      while(!there && !filed.eof())
      {
          filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

        //Prevents overwritten data from being included in data calculations
        if(summation.deleted)
            continue;

        if(summation.day == time)
            there == true;
        //Checks if any sales were made today
        else
        {
            cout<<"There are no sales yet!"<<endl;
            return;
        }

      }

        if(!summation.deleted)
        {
            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            highestpos = filed.tellg();
            highest = summation.numberofsales;
        }

        cout<<"The total number of sales today was: "<<total<<", which brought in $"<<rev<<" of revenue!"<<endl;
        filed.seekg(highestpos, ios::beg);
        filed.read(reinterpret_cast<char*>(&summation), sizeof(summation));
        cout<<"The hottest selling item was: "<<summation.name<<", which sold "<<summation.numberofsales<<" items."<<endl;
    }

    //Month
    if(m == s)
    {
      while(!there && !filed.eof())
      {
          filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

        //Prevents overwritten data from being included in data calculations
          if(summation.deleted)
            continue;

          if(summation.month == time)
            there == true;
      }

         if(!summation.deleted)
        {
            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            highestpos = filed.tellg();
            highest = summation.numberofsales;
        }

      while(!filed.eof() && (summation.month == time) )
      {
            filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

            //Prevents overwritten data from being included in data calculations
            if(summation.deleted)
                continue;

            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            if(summation.numberofsales > highest)
            {
                highestpos = filed.tellg();
                highest = summation.numberofsales;
            }
        }

            cout<<"The total number of sales this month was: "<<total<<", which brought in $"<<rev<<" of revenue!"<<endl;
            filed.seekg(highestpos, ios::beg);
            filed.read(reinterpret_cast<char*>(&summation), sizeof(summation));
            cout<<"The hottest selling item was: "<<summation.name<<", which sold "<<summation.numberofsales<<" items."<<endl;
    }

    //Yearly
    if(m == spo)
    {
          while(!there && !filed.eof())
          {
              filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

              //Prevents overwritten data from being included in data calculations
              if(summation.deleted)
                continue;

              if(summation.year == time)
                there == true;
          }
        if(!summation.deleted)
        {
            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            highestpos = filed.tellg();
            highest = summation.numberofsales;
        }

          while(!filed.eof() && summation.year == time)
          {
                filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

                //Prevents overwritten data from being included in data calculations
                if(summation.deleted)
                    continue;

                total += summation.numberofsales;
                rev += (summation.numberofsales * summation.price);
                if(summation.numberofsales > highest)
                {
                    highestpos = filed.tellg();
                    highest = summation.numberofsales;
                }
          }

            cout<<"The total number of sales this year was: "<<total<<", which brought in $"<<rev<<" of revenue!"<<endl;
            filed.seekg(highestpos, ios::beg);
            filed.read(reinterpret_cast<char*>(&summation), sizeof(summation));
            cout<<"The hottest selling item was: "<<summation.name<<", which sold "<<summation.numberofsales<<" items."<<endl;
    }

    //All time
    if(m == all)
    {


        filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation));

        //Prevents overwritten data from being included in data calculations
        if(!summation.deleted)
        {
              total += summation.numberofsales;
              rev += (summation.numberofsales * summation.price);
              highestpos = filed.tellg();
              highest = summation.numberofsales;
        }


          while( filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation)) )
          {

            //Prevents overwritten data from being included in data calculations
            if(summation.deleted)
                continue;

            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            if(summation.numberofsales > highest)
            {
                highestpos = filed.tellg();
                highest = summation.numberofsales;
            }
          }

            cout<<"The total amount of sales your business has made is : "<<total<<", which brought in a lifetime revenue of $"<<rev<<" of revenue!"<<endl;
            filed.seekg(highestpos, ios::beg);
            filed.read(reinterpret_cast<char*>(&summation), sizeof(summation));
            cout<<"The hottest selling item you have ever sold was : "<<summation.name<<", which sold "<<summation.numberofsales<<" items."<<endl;
    }

    //Custom
    if(m == cus)
    {
      while(!there && filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation)) )
      {
        /*Since range provided may be a overwritten data date, just set that day's data to 0*/
         if(summation.deleted)
            {
                summation.numberofsales = 0;
                summation.price = 0.0;
            }

          if(summation.month == time2 || summation.day == time3 || summation.year == time4 ) //month, day ,year
            there == true;
      }

      total += summation.numberofsales;
      rev += (summation.numberofsales * summation.price);
      highestpos = filed.tellg();
      highest = summation.numberofsales;


      while(filed.read(reinterpret_cast<char*>(&summation) , sizeof(summation)) && !there)
      {

            /*Since range provided may be a overwritten data date, just set that day's data to 0*/
            if(summation.deleted)
            {
                summation.numberofsales = 0;
                summation.price = 0.0;
            }

            if(summation.month == time5 && !summation.day == time6 && !summation.year == time7)
                  there = true;

            total += summation.numberofsales;
            rev += (summation.numberofsales * summation.price);
            if(summation.numberofsales > highest)
            {
                highestpos = filed.tellg();
                highest = summation.numberofsales;
            }
      }

        cout<<"The total number of sales in this range was: "<<total<<", which brought in $"<<rev<<" of revenue!"<<endl;
        filed.seekg(highestpos, ios::beg);
        filed.read(reinterpret_cast<char*>(&summation), sizeof(summation));
        cout<<"The hottest selling item was: "<<summation.name<<", which sold "<<summation.numberofsales<<" items."<<endl;
    }

    system("pause");

    filed.close();
}
void Tally(char a, int numsold, double pricing, char * id)
{
    fstream timefile;
    Sales download;
    Validation valid3;
    string ans;
    string takeans;

    //To get time
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    //Functions for time
    int currenttime;
    int month1, day1, year1, month2, day2, year2;

    do
    {
        if(a == 'M')
        {
            //Checks if any sales exist
            timefile.open(filename1, ios::in|ios::binary);
            if(timefile.fail())
            {
                cout<<"No items have been sold!"<<endl;
                return;
            }
            timefile.close();

            for(int val = 0; val <= r; val++)
                cout<<ranges[val]<<endl;
            cin >> ans;
            ans = valid3.validate(ans, 'T');

            if(ans == "1")//Calculating data today
            {
                currenttime = (timePtr->tm_mday);
                sum(f, currenttime);
            }

            if(ans == "2")//Calculating data this month
            {
                currenttime = (timePtr->tm_mon);
                sum(s, currenttime);
            }

            if(ans == "3")//Calculating data this year
            {
                currenttime = (timePtr->tm_year);
                sum(spo, currenttime);
            }

            if(ans == "4")//Calculates data all time
            {
                sum(all);
            }

            if(ans == "5") //Calculates data for custom range
            {
                cout<<"Enter the range (From Month/Day/Year to Month/Day/Year)"<<endl;
                cout<<"From this month: "<<endl;
                cin >> month1;
                cout<<"From this day: "<<endl;
                cin >> day1;
                cout<<"From this year: "<<endl;
                cin >> year1;
                cout<<"To this month: "<<endl;
                cin >> month2;
                cout<<"To this day: "<<endl;
                cin >> day2;
                cout<<"To this year: "<<endl;
                cin >> year2;

                if(year2 <= ( (timePtr->tm_year)+1900 ))
                {
                    if(month2 <= ( (timePtr->tm_mon)+1 ) )
                    {
                        if(day2 <= ( (timePtr->tm_mday) ) )
                        {

                        }

                         else
                        {
                            cout<<"Invalid day, please reenter a valid time frame "<<endl;
                            cin >> day2;
                        }
                    }

                    else
                    {
                        cout<<"Invalid month, please reenter a valid time frame "<<endl;
                        cin >> month2;
                    }
                }

                else
                {
                    cout<<"Invalid year, please reenter a valid time frame "<<endl;
                    cin >> year2;
                }
                sum(cus, 0, month1, day1, year1, month2, day2, year2);
            }

        }

        else
        {
            //Check if sales file exists, if it doesn't, then there are zero sales
            timefile.open(filename1, ios::out|ios::app|ios::binary);
            strncpy(download.name, id, charnum - 1);
            download.deleted = false;
            download.price = pricing;
            download.numberofsales = numsold;
            download.month = (timePtr->tm_mon)+1;
            download.day = (timePtr->tm_mday);
            download.year = (timePtr->tm_year)+1900;

            timefile.write(reinterpret_cast<char *>(&download), sizeof(download));

            timefile.close();
        }


        if(a == 'M')
        {
            cout<<"Would you like to run this again? (Hit enter if you do not, otherwise hit any other key) "<<endl;
            cin.ignore();
            getline(cin, takeans);
            if(takeans.length() == 0)
                takeans = "\n";
        }

        else
            takeans = "\n";


    }while(takeans != "\n");



}

void deleterec()
{
    //Genre Identifiers
    char bs;
    char ms;
    char orr;

    bs = '~';
    ms = '!';
    orr = '=';

    //Takes user input
    char genrechose;
    string response;

    //Opens Data Records
    ifstream J;
    ofstream J1;

    //Opens Sales Record
    ifstream K;
    ofstream K1;

    //Looks for name
    char tempname[charnum];

    //Indicates if name was found
    bool found = false;

    //Used to find position of deleted record
    int passedrecord = 0;

    //Vector position
    int here = 0;
    int numtimes = 0;
    int position = 0;
    int val = 0;

    //User input
    int choice;

    //Used to display list
    int count = 1;

    //Vector of structures
    vector <Data> changing1;
    vector <Sales> changing2;

    J.open(filename, ios::in|ios::binary);
    Data change1;
    Sales also;

    cout<<"What genre is the data you want to delete( Enter number )? "<<endl;
    for(int val = 0; val < logs; val++)
        cout<<listofgenres[val]<<endl;
    cin >> response;


    if(response == "1")
        genrechose = bs;

    if(response == "2")
        genrechose = ms;

    if(response == "3")
        genrechose = orr;

     while( J.read(reinterpret_cast<char *>(&change1), sizeof(change1)) )
    {
           changing1.push_back(change1);

           if(change1.Genre == genrechose)
           {
                cout<<count<<") "<<change1.Productname<<endl;
                count++;
                here++;
           }

           /* We need to know exactly what record in the file matches that genre, that way when the users chooses a number of the list, we can go and find
           that exact record. We don't want only 2 records to show up and then conclude the user wants the second record when they actually want the 37th record.*/

           passedrecord++;
    }

    if(count <= 0)
    {
        cout<<"No items of that genre in stock."<<endl;
        return;
    }

    cout<<"Enter number of the item you want to delete, enter -1 if you don't want to delete anything. "<<endl;
    cin >> choice;

    //Checks if user doesn't want to delete anything
    if(choice == -1)
        return;

    //Validates input
    neg2(choice);

    cout<<"Enter the number again, this is to make sure you delete right file. "<<endl;
    cin >> choice;

    //Clear eof
    J.clear();
    J.close();

    //Erasing Data
    J1.open(filename, ios::out|ios::trunc|ios::binary);
    J1.close();

    //Rewriting Data
    J1.open(filename, ios::out|ios::app|ios::binary);

    while(numtimes != here)
    {
        if(changing1[val].Genre == genrechose)
        {
            numtimes++;
            position = val;

            if(numtimes != here)
                val++;
        }

        else
            val++;
    }


    strncpy(tempname, changing1[position].Productname, charnum -1 );

     for(int val = 0; val < changing1.size(); val++)
    {
        if(val == position)
            val++;

        change1.Genre = changing1[val].Genre;
        strncpy(change1.Productname, changing1[val].Productname, charnum - 1);
        change1.Morethantwo = changing1[val].Morethantwo;
        change1.Noticeseen = changing1[val].Noticeseen;
        change1.Numberofproducts = changing1[val].Numberofproducts;
        change1.Numberofproductsleft = changing1[val].Numberofproductsleft;
        change1.Numberofproductssold = changing1[val].Numberofproductssold;
        change1.price = changing1[val].price;
        J1.write( reinterpret_cast<char*>(&change1), sizeof(change1) );
    }

    J1.close();


    //Overwriting sales data
    K.open(filename1, ios::in|ios::binary);
    int on = 0;

    while( K.read(reinterpret_cast<char*>(&also), sizeof(also)) && !found )
    {
        changing2.push_back(also);

        if( !strncmp(changing2[on].name, tempname, charnum - 1) )
        {
            found = true;
            position = on;
        }

        else
            on++;
    }

    K.close();

    //If file is found , delete it
    if(found)
    {
        K1.open(filename1, ios::out|ios::binary);
        K1.close();

        K1.open(filename1, ios::out|ios::app|ios::binary);


         for(int val = 0; val < changing2.size(); val++)
        {
            if(val == position)
                val++;

            also.day = changing2[val].day;
            also.deleted = changing2[val].deleted;
            also.month = changing2[val].month;
            strncpy(also.name, changing2[val].name, charnum - 1);
            also.numberofsales = changing2[val].numberofsales;
            also.price = changing2[val].price;
            also.year = changing2[val].year;

            K1.write(reinterpret_cast<char*>(&also), sizeof(also));
        }

        K1.close();
    }

    cout<<"File deleted!"<<endl;
    system("pause");

}
