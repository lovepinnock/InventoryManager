#ifndef VALIDATION_H
#define VALIDATION_H
#include <iostream>
#include <string>

using namespace std;


class Validation
{
    public:
        Validation() {}

        string validate(string, char);

                //Size of arrays
        const int os = 3;
        const int logs = 3;
        const int r = 4;

        //List of options
        string options[4] = {"1) Input New Items( if you have new items to sell, use this) ",
                            "2) Modify Data( If you sold an item or made mistake, use this ) ",
                            "3) Show current inventory(shows how much of everything is left",
                            "4) Sales Data" };

        //List of genre options
        string listofgenres[4] = {"1) Book", "2) Movie", "3) Other", "4) Delete Entry" };

        //List of sales data range
        string ranges[5] = {"1) Today" , "2) This Month", "3) This year", "4) All time ", "5) Custom range" };

    private:
        string val;
        char c;
};

string Validation::validate(string v, char character)
{
    string again;

    val = v;
    c = character;

    if(val == "1" || val == "2" || val == "3" || val == "4" || val == "5" && (c == 'M' || c == 'T'))
        return val;

    else if(val == "Input" || val == "input" || val == "Modify" || val == "modify" || val == "Current" || val == "current" || val == "Sales" || val == "sales" || val == "Sale" || val == "sale" || val == "S" || val == "s")
        return val;

    else if(val == "M" || val == "m" || val == "O" || val == "o" || val == "C" || val == "c" || val == "T" || val == "t" && c == 'M')
        return val;

    else if(val == "1" || val == "2" || val == "3" && c == 'I')
        return val;

    else if(val == "Books" || val == "books" || val == "Book" || val == "book" || val == "B" || val == "b")
        return val;

    else if(val == "Movies"|| val == "Movie" || val == "movies" || val == "movie" || val == "M" || val == "m")
        return val;

    else if(val == "Other" || val == "other" || val == "o" || val == "O")
        return val;

    else if(val == "Today" || val == "today" || val == "Month" || val == "month" || val == "Year" || val == "year" || val == "All" || val == "all" || val == "A" || val == "a" && c == 'T')
        return val;

    else if(val == "T" || val == "t" || val == "M" || val == "m" || val == "Y" || val == "y" || val == "C" || val == "c" || val == "Custom" || val == "custom" || val == "custom range" || val == "Custom range" && c == 'T')
        return val;

    else if(val == "All time" || val == "all time" || val == "All Time")
        return val;
    //Get input and then validates it again
    else
    {
        cout<<"Invalid input, please enter a selection off the list: "<<endl;
        if(c == 'M')
            for(int val = 0; val <= os; val++)
                cout<<options[val]<<endl;
        else if(c == 'I')
            for(int val = 0; val <= logs; val++)
                cout<<listofgenres[val]<<endl;
        else if(c == 'T')
            for(int val = 0; val <= r; val++)
                cout<<ranges[val]<<endl;

        cin >> again;

        validate(again, c);
    }
}

#endif // VALIDATION_H
