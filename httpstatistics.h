#ifndef HTTPSTATISTICS_H
#define HTTPSTATISTICS_H

#include "headers.h"
#include <sstream>
#include <map>
#include "HttpStataEntry.h"
#include "time.h"
#include <deque>

#include <iostream>

namespace HttpSniffer
{

class HttpStatistics
{
public:
    HttpStatistics();
    ~HttpStatistics();
    void get(std::stringstream& stream, int user_id);
    void update(const std::string& url);
    void clear() { _statistics.clear(); }

//    template <typename T>
//    HttpStatistics& operator<<(const T & data)
//    {
//        //std::cout << "in mutex1\n";

//        _mutex.acquire();
//        _stata_stream << data;
//        _mutex.release();

//        //std::cout << "in mutex2\n";

//        return *this;
//    }



    //template<>
//    HttpStatistics& operator<<(const HttpStataEntry & entry)
//    {
//        _mutex.acquire();
//        _mutex.release();

//        return *this;
//    }

    void client_init(int client_id)
    {
        _mutex.acquire();

        if (_client_start.empty())
        {
            _client_start[client_id] = 0;
        }
        else
        {
            _client_start[client_id] = ::time(0);
        }

        _mutex.release();
    }

    void client_exit(int client_id)
    {
        _mutex.acquire();
        _client_start.erase(client_id);
        _mutex.release();
    }

private:
    Mutex _mutex;
    std::stringstream _stata_stream;
    std::deque<HttpStataEntry> _statistics;

    template <typename T>
    std::map<std::string, HttpStataEntry> get_aggregate(T& entries)
    {
        std::map<std::string, HttpStataEntry> stata;
        for (typename T::iterator it = entries.begin(); it != entries.end(); ++it)
        {
            HttpStataEntry& entry = *it;
            std::map<std::string, HttpStataEntry>::iterator _it = stata.find(entry.url);
            if (_it == stata.end())
            {
                stata[entry.url] = entry;
            }
            else
            {
                stata[entry.url].timestamp = entry.timestamp;
                stata[entry.url].count++;
            }
        }
        return stata;
    }

    void unsafe_clean_up()
    {
        //_mutex.acquire();

        if (!_client_start.empty())
        {
            //std::cout << "C 1\n";

            time_t min_start_time = _client_start.begin()->second;
            for (std::map<int, time_t>::iterator it = _client_start.begin();
                 it != _client_start.end(); ++it)
            {
                if (it->second < min_start_time)
                    min_start_time = it->second;
            }

            //std::cout << "C 2\n";

            for (std::deque<HttpStataEntry>::iterator it = _statistics.begin();
                 it != _statistics.end();)
            {
                HttpStataEntry& entry = *it;
                if (min_start_time >= entry.timestamp)
                    it = _statistics.erase(it);
                else
                    break;
            }

            //std::cout << "C 3\n";
        }

        //_mutex.release();
    }

    HttpStatistics(const HttpStatistics&);

private:

    std::map<int, time_t> _client_start;

};

}

#endif // HTTPSTATISTICS_H
