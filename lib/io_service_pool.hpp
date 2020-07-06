#ifndef __IO_SERVICE_POOL_HPP__
#define __IO_SERVICE_POOL_HPP__

#include "inc.hpp"

class io_service_pool : private boost::noncopyable
{
public:
    explicit io_service_pool(std::size_t pool_size, std::size_t thread_count = 0) 
        : _next_io_service(0)
    {
        if (pool_size == 0)
        {
            throw std::runtime_error("io_service_pool size is 0");
        }
        if (thread_count == 0 )
        {
            thread_count = pool_size;
        }
        std::cout << "[io_service_pool:" << std::this_thread::get_id() << "] " << "pool size : " << pool_size << " .. thread_count : " << thread_count << std::endl;
        _thread_count = thread_count;

        // Give all the io_services work to do so that their run() functions will not
        // exit until they are explicitly stopped.
        for (std::size_t i = 0; i < pool_size; ++i)
        {
            io_service_ptr io_service(new boost::asio::io_service(1));
            _worksptr work(new boost::asio::io_service::work(*io_service));
            _io_services.push_back(io_service);
            _works.push_back(work);
        }
    }


    void run()
    {
        // Create a pool of threads to run all of the io_services.
        
        for (std::size_t i = 0; i < _thread_count; ++i)
        {
            std::shared_ptr<boost::thread> thread(new boost::thread(
                boost::bind(&boost::asio::io_service::run, _io_services[i])));
            threads.push_back(thread);
        }

    }


    void stop()
    {
        std::cout << "[io_service_pool:" << std::this_thread::get_id() << "] " << "io_service_pool stop" << std::endl;
        for(std::size_t i = 0 ; i < _io_services.size() ; i++)
        {
            _io_services[i]->stop();
        }
    }

    boost::asio::io_service & get_io_service()
    {
        // Use a round-robin scheme to choose the next io_service to use.
        boost::asio::io_service& io_service = *_io_services[_next_io_service];
        ++_next_io_service;
        if (_next_io_service == _io_services.size())
            _next_io_service = 0;
        return io_service;
    }

    std::vector<std::shared_ptr<boost::asio::io_service>> & get_io_services()
    {
        return _io_services;
    }

public:
    using io_service_ptr = std::shared_ptr<boost::asio::io_service>;
    using _worksptr = std::shared_ptr<boost::asio::io_service::work>;
    std::size_t _thread_count;
    std::vector<std::shared_ptr<boost::thread> > threads;
    std::vector<io_service_ptr> _io_services;
    std::vector<_worksptr> _works;
    std::size_t _next_io_service;
};

#endif