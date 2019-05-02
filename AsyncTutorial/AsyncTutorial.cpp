//
// https://thispointer.com//c11-multithreading-part-9-stdasync-tutorial-example/
//
// AsyncTutorial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
using namespace std::chrono;
std::string fetchDataFromDB(std::string recvdData)
{
    // Make sure that function takes 5 seconds to complete
    std::this_thread::sleep_for(seconds(5));
    //Do stuff like creating DB Connection and fetching Data
    return "DB_" + recvdData;
}

std::string fetchDataFromFile(std::string recvdData)
{
    // Make sure that function takes 5 seconds to complete
    std::this_thread::sleep_for(seconds(5));
    //Do stuff like fetching Data File
    return "File_" + recvdData;
}

/*
* Function Object
*/
struct DataFetcher
{
    std::string operator()(std::string recvdData)
    {
        // Make sure that function takes 5 seconds to complete
        std::this_thread::sleep_for(seconds(5));
        //Do stuff like fetching Data File
        return "File_" + recvdData;
    }
};

int main()
{
    // Get Start Time
    system_clock::time_point start = system_clock::now();
    //std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");

    //Calling std::async with function object
    std::future<std::string> fileResult = std::async(DataFetcher(), "Data");

    ////Fetch Data from File blocking
    //std::string fileData = fetchDataFromFile("Data");

    ////Fetch Data from DB async()
    //// Will block till data is available in future<std::string> object.
    //std::string dbData = resultFromDB.get();
    
    //Calling std::async with lambda function
    std::future<std::string> resultFromDB = std::async([](std::string recvdData) {
        std::this_thread::sleep_for(seconds(5));
        //Do stuff like creating DB Connection and fetching Data
        return "DB_" + recvdData;
    }, "Data");
    
    // Get End Time
    auto end = system_clock::now();
    auto diff = duration_cast <std::chrono::seconds> (end - start).count();
    std::cout << "Async Total Time Taken = " << diff << " Seconds" << std::endl;
   
    //Combine The Data
    //std::string data = dbData + " :: " + fileResult;
    //std::string data = dbData + " :: " + fileResult.get();
    std::string data = resultFromDB.get() +" :: " + fileResult.get();
    
    // Get End Time
    end = system_clock::now();
    diff = duration_cast <std::chrono::seconds> (end - start).count();
    std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;

    //Printing the combined Data
    std::cout << "Data = " << data << std::endl;
    return 0;
}