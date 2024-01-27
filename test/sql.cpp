#include <mysql_driver.h>
#include <mysql_connection.h>
#include <iostream>

int main() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "qwaszx124578DW");

    delete con;
}

