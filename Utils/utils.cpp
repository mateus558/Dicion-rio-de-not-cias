#include "utils.hpp"
#include <cstring>
#include <limits>


void waitUserAction(){
    std::cout << "\nPress any key to finish..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cin.get();
}

void clear(){
#ifdef __unix__
    system("clear");
#elif _WIN32
    system("CLS");
#else
    int n;
        for (n = 0; n < 10; n++)
          printf( "\n\n\n\n\n\n\n\n\n\n" );
#endif
}

void process_mem_usage(double& vm_usage, double& resident_set){
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vm_usage     = 0.0;
    resident_set = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat",ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage     = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

bool save_dataset(MeasureIt begin, MeasureIt end, std::string fname){
    std::ofstream dataset(fname+".plt");
    auto it = begin;

    if(!dataset.good() || !dataset.is_open()) return false;

    for(; it != end; it++){
        auto measure = (*it);
        dataset << measure.first << "\t" << measure.second << std::endl;
    }
    dataset.close();
    return (it == end);
}