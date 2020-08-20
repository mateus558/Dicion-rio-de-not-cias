#include <iostream>
#include <random>
#include <unistd.h>
#include <ios>
#include <chrono> 
#include "HashDictionary.hpp"
#include "TRIEDictionary.hpp"
#include "gnuplot-iostream.h"
using namespace std::chrono; 
using TermsChunks = std::vector<std::vector<std::string> >;

HashDictionary dict;
std::string strategy;

std::vector<std::string> generateRandomQueries(const std::string& path, const size_t n_queries, const size_t query_size){
    std::ifstream file(path);
    std::vector<std::string> search_terms, queries_selected(n_queries);

    if(file.is_open()){
        std::string raw_doc;
        while(std::getline(file, raw_doc)){
            auto document = json::parse(raw_doc);
            raw_doc.clear();
            std::string content = document["headline"];
            content += ' ';
            content += document["short_description"];
            content = dict.processString(content);

            std::stringstream ss(content);
            std::string term;
            while(std::getline(ss, term, ' ')){
                if(term.size() <= 2 || term.empty()) continue;
                search_terms.push_back(term);
            }
        }
    }else{
        std::cerr << "The file could not be opened." << std::endl;
    }
    file.close();

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, search_terms.size());
    for(size_t i = 0; i < n_queries; i++){
        std::string term;
        term += search_terms[distribution(generator)];
        for(size_t k = 0; k < query_size - 1; k++){
            term += std::string(" ") + search_terms[distribution(generator)];
        }
        queries_selected[i] = term;
    }

    return queries_selected;
}

std::vector<duration<float> > time_experiment(const std::string &path, const size_t n_queries, const size_t n_terms, Dictionary *_dict){
    std::clog << "Generating queries..." << std::endl;
    std::vector<std::string> queries = generateRandomQueries(path, n_queries, n_terms);
    std::vector<duration<float> > durations;
    std::vector<std::pair<double, double> > plot_data;
    Gnuplot gp;

    for(size_t i = 0; i < queries.size(); i++){
        clear();
        std::cout << "Strategy: " << strategy << std::endl;
        std::clog << "Processing query with " << n_terms << " terms [" << i+1 << "/" << queries.size() << "]" << std::endl;
        auto start = high_resolution_clock::now(); 
        _dict->findByTerms(queries[i]);
        auto stop = high_resolution_clock::now(); 
        durations.push_back(stop - start); 
    }
    std::clog << "Generating time plot for queries with " << n_terms << " terms..." << std::endl;
    int step = 1000;
    double avg = 0.0;
    for(size_t i = 0; i < durations.size(); i++){
        if(i%step == 0){
            plot_data.push_back(std::make_pair(i, avg/step));
            avg = 0.0;
        }
        avg += durations[i].count();
    }
    gp << "set terminal svg" << std::endl;
    gp << "set output 'queries_time_" << n_terms << "_terms_plot_" << strategy << ".svg'" << std::endl;
    gp << "set xlabel 'Number of queries'" << std::endl;
    gp << "set ylabel 'Time (secs)'" << std::endl;
    gp << "plot" << gp.file1d(plot_data) << "with lines title 'avg query time'" << std::endl;
    return durations;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cerr << "Available strategies:\n";
        std::cerr << "\ttrie\n";
        std::cerr << "\thash\n"; 
        std::cerr << "Usage: ./exp <strategy>" << std::endl;
        return 1;
    }else{
        strategy += argv[1];
    }
    
    clear();
    std::string path("data/News_Category_Dataset_v2.json");
    size_t n_queries = 10000;
    duration<float> time_fill_dict;
    Dictionary *to_exp;
    if(strategy == "hash"){ 
        to_exp = new HashDictionary();
    }else{
        to_exp = new TRIEDictionary();
    }
    
    double vm, rss;

    std::cout << "Strategy: " << strategy << std::endl;
    to_exp->setVerbose(false);
    std::clog << "Filling dictionary..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    to_exp->insert(path);
    auto end = std::chrono::high_resolution_clock::now();
    time_fill_dict = end - start;
    auto insert_times = to_exp->getTimeMeasurements();
    
    process_mem_usage(vm, rss);
    
    auto time_durations = time_experiment(path, n_queries, 1, to_exp);
    double avg_time, avg_time1;
    avg_time = avg_time1 = 0.0;    
    for(size_t i = 0; i < time_durations.size(); i++) {
        avg_time += time_durations[i].count();
    }
    time_durations = time_experiment(path, n_queries, 2, to_exp);
    for(size_t i = 0; i < time_durations.size(); i++) {
        avg_time1 += time_durations[i].count();
    }
    clear();
    std::cout << "Strategy: " << strategy << std::endl;
    for(size_t i = 0; i < insert_times.size(); i++) {
        std::cout << "Time to insert " << insert_times[i].first << "/" << to_exp->getNumberOfDocuments() << ": " << insert_times[i].second << " seconds" <<  std::endl;
    }
    std::clog << "-------------------------------------------------------------------------------\n";
    std::clog << "Time to construct the dictionary: " << time_fill_dict.count() << "s" << std::endl;
    std::clog << "Memory used by the dictionary VM: " << vm << " RSS: " << rss << " (Kb)" << std::endl;
    std::clog << "Average query time with 1 term: " << avg_time/n_queries << " seconds" << std::endl;
    std::clog << "Average query time with 2 terms: " << avg_time1/n_queries << " seconds" << std::endl;
    to_exp->generatePlots(strategy);
    return 0;   
}