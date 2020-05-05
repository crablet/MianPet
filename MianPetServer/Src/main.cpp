﻿#include <iostream>
#include <exception>

#include "Server.h"


#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

#define OTL_ODBC
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
#define OTL_ODBC_UNIX
#include "otlv4.h"

otl_connect db;

void insert()
// insert rows into table
{
    otl_stream o(1, // buffer size should be == 1 always on INSERT
        "insert into test_tab values "
        " (:f1<int>,:f2<char[31]>), "
        " (:f12<int>,:f22<char[31]>), "
        " (:f13<int>,:f23<char[31]>), "
        " (:f14<int>,:f24<char[31]>), "
        " (:f15<int>,:f25<char[31]>) ",
        // INSERT statement. Multiple sets of values can be used
        // to work around the lack of the bulk interface
        db // connect object
    );

    // If the number of rows to be inserted is not known in advance,
    // another stream with the same INSERT can be opened
    otl_stream o2(1, // buffer size should be == 1 always on INSERT
        "insert into test_tab values "
        " (:f1<int>,:f2<char[31]>)",
        db // connect object
    );

    char tmp[32];
    int i;
    for (i = 1; i <= 100; ++i) {
        sprintf(tmp, "Name%d", i);
        o << i << tmp;
    }
    for (i = 101; i <= 103; ++i) {
        sprintf(tmp, "Name%d", i);
        o2 << i << tmp;
    }
}
void update(const int af1)
// insert rows into table
{
    otl_stream o(1, // buffer size should be == 1 always on UPDATE
        "UPDATE test_tab "
        "   SET f2=:f2<char[31]> "
        " WHERE f1=:f1<int>",
        // UPDATE statement
        db // connect object
    );
    o << "Name changed" << af1;
    o << otl_null() << af1 + 1; // set f2 to NULL

}
void select(const int af1)
{
    otl_stream i(50, // buffer size may be > 1
        "select * from test_tab "
        "where f1>=:f11<int> "
        "  and f1<=:f12<int>*2",
        // SELECT statement
        db // connect object
    );
    // create select stream

    int f1;
    char f2[31];

    i << af1 << af1; // :f11 = af1, :f12 = af1

#if (defined(_MSC_VER) && _MSC_VER>=1600) || defined(OTL_CPP_11_ON)
// C++11 (or higher) compiler
    for (auto &it : i) {
        it >> f1;
        cout << "f1=" << f1 << ", f2=";
        it >> f2;
        if (it.is_null())
            cout << "NULL";
        else
            cout << f2;
        cout << endl;
    }
#else
// C++98/03 compiler
    while (!i.eof()) { // while not end-of-data
        i >> f1;
        cout << "f1=" << f1 << ", f2=";
        i >> f2;
        if (i.is_null())
            cout << "NULL";
        else
            cout << f2;
        cout << endl;
    }
#endif

}

int main()
{
    otl_connect::otl_initialize(); // initialize ODBC environment
    try {

        db.rlogon("UID=scott;PWD=tiger;DSN=mysql35"); // connect to ODBC
      //  db.rlogon("scott/tiger@mysql35"); // connect to ODBC, alternative format
                                          // of connect string 

        otl_cursor::direct_exec
        (
            db,
            "drop table test_tab",
            otl_exception::disabled // disable OTL exceptions
        ); // drop table

        otl_cursor::direct_exec
        (
            db,
            "create table test_tab(f1 int, f2 varchar(30)) type=innoDB"
        );  // create table

        insert(); // insert records into the table
        update(10); // update records in the table
        select(8); // select records from the table

    }

    catch (otl_exception &p) { // intercept OTL exceptions
        cerr << p.msg << endl; // print out error message
        cerr << p.stm_text << endl; // print out SQL that caused the error
        cerr << p.sqlstate << endl; // print out SQLSTATE message
        cerr << p.var_info << endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from ODBC

    return 0;
}

//int main()
//{
//    try
//    {
//        Server server;
//        server.Run();
//    }
//    catch (std::exception &e)
//    {
//        std::cerr << "exception: " << e.what() << '\n';
//    }
//
//    return 0;
//}
