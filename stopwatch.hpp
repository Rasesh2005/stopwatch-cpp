#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <x86intrin.h>

namespace stopwatch
{

    struct rdtscp_clock
    {
        std::uint64_t sum; // Elapsed Cycle
        std::uint64_t now;
        std::uint32_t id;
        void start()
        {
            now = rdtscp(&id);
        }
        void stop()
        {
            auto t = rdtscp(&id);
            sum += (t - now);
        }
        uint64_t elapsed()
        {
            std::ofstream data("difference_data.txt", std::ios_base::app);
            data<<"Time elapsed "<<sum<<"\n";
            return sum;
        }
    };

    template <class Clock = rdtscp_clock>
    struct timer
    {
        std::string name;
        Clock clock;
        std::uint32_t count; // Number of calls
        timer(const char *s)
        {
            name = s;
            count = 0;
        }
        void start()
        {
            count++;
            clock.start();
        }
        void stop()
        {
            clock.stop();
        }
        ~timer()
        {
            auto e = clock.elapsed();
            std::cerr << name << ": elapsed " << e;
            std::cerr << " (" << count << " times)";
            std::cerr << " Average " << static_cast<double>(e) / count << std::endl;
        }
    };
}
