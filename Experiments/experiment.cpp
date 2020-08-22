#include <iostream>
#include <random>
#include <unistd.h>
#include <ios>
#include <chrono> 
#include "HashDictionary.hpp"
#include "TRIEDictionary.hpp"
#include "gnuplot-iostream.h"

using namespace std::chrono; 

HashDictionary dict;
std::string strategy;
double avg_comparisons = 0.0;
double avg_term_size = 0.0;

void generatePlots(Dictionary* dictionary, const std::string &strategy){
	Gnuplot gp;
	// plot the memory consumption of the dictionary at each 20000 documents insertion
	gp << "set terminal svg" << std::endl;
    gp << "set output 'insertion_mem_plot_" << strategy << ".svg'" << std::endl;
    gp << "set xlabel 'Number of documents'" << std::endl;
    gp << "set ylabel 'Memory (Mb)'" << std::endl;
    gp << "plot" << gp.file1d(dictionary->mem_measures) << "with lines title 'Virtual memory'" << std::endl;
	
	// plot the average time of the dictionary at each 20000 documents insertion
    gp << "set output 'insertion_time_plot_" << strategy << ".svg'" << std::endl;
    gp << "set xlabel 'Number of documents'" << std::endl;
    gp << "set ylabel 'Time (ms)'" << std::endl;
    gp << "plot" << gp.file1d(dictionary->time_measures) << "with lines title 'avg insertion time'" << std::endl;
    
    dictionary->mem_measures.clear();
    dictionary->time_measures.clear();
}

// function for queries generation with n terms
std::vector<std::string> generateRandomQueries(const std::string& path, const size_t n_queries, const size_t query_size){
	std::ifstream file(path);
	std::vector<std::string> search_terms, queries_selected(n_queries);

	if(file.is_open()){
		std::string raw_doc;
		// at each line of the file
		while(std::getline(file, raw_doc)){
			// parse the document
			auto document = json::parse(raw_doc);
			raw_doc.clear();
			// create the sentence for terms generation
			std::string content = document["headline"];
			content += ' ';
			content += document["short_description"];
			// normalize the string
			content = dict.normalizeString(content);

			std::stringstream ss(content);
			std::string term;
			// split the sentence in search terms and compute average term size
			while(std::getline(ss, term, ' ')){
				if(!dict.isValidTerm(term)) continue;
				avg_term_size += term.size();
				search_terms.push_back(term);
			}
		}
	}else{
		std::cerr << "The file could not be opened." << std::endl;
	}
	file.close();
	avg_term_size /= search_terms.size();

	// generate random queries with the given number of terms
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<size_t> distribution(0, search_terms.size());
	for(size_t i = 0; i < n_queries; i++){
		std::string query;
		// select random term
		query += search_terms[distribution(generator)];
		// select random terms up to the required number of terms
		for(size_t k = 0; k < query_size - 1; k++){
			query += std::string(" ") + search_terms[distribution(generator)];
		}
		// save query
		queries_selected[i] = query;
	}

	return queries_selected;
}

// run queries time experiment with random queries of a given size
std::vector<duration<float> > time_experiment(const std::string &path, const size_t &n_queries, const size_t &n_terms, const size_t &step, Dictionary *_dict, double *avg_comparisons){
	std::clog << "Generating queries..." << std::endl;
	// generate random queries with the given number of terms
	std::vector<std::string> queries = generateRandomQueries(path, n_queries, n_terms);
	std::vector<duration<float> > durations;
	std::vector<std::pair<double, double> > plot_data;
	Gnuplot gp;

	// measure the time for each query and compute the average number of comparisons
	for(size_t i = 0; i < queries.size(); i++){
		auto start = high_resolution_clock::now(); 
		_dict->findByTerms(queries[i], 20);
		auto stop = high_resolution_clock::now();
		(*avg_comparisons) += _dict->numberComparisons(); 
		durations.push_back(stop - start); 
	}
	(*avg_comparisons) /= queries.size();
	
	std::clog << "Generating time plot for queries with " << n_terms << " terms..." << std::endl;
	std::clog << "Average number of comparisons: " << (*avg_comparisons) << std::endl;
	// prepare time data for ploting and compute the average query time
	double avg = 0.0;
	for(size_t i = 0; i < durations.size(); i++){
		if(i%step == 0){
			plot_data.push_back(std::make_pair(i, (avg/step)*1E6));
			avg = 0.0;
		}
		avg += durations[i].count();
	}
	// plot the average query time at each n steps
	gp << "set terminal svg" << std::endl;
	gp << "set output 'queries_time_" << n_terms << "_terms_plot_" << strategy << ".svg'" << std::endl;
	gp << "set xlabel 'Number of queries'" << std::endl;
	gp << "set ylabel 'Time (\xC2\xB5s)'" << std::endl;
	gp << "plot" << gp.file1d(plot_data) << "with lines title 'avg query time'" << std::endl;
	return durations;
}

int main(int argc, char* argv[]){
	std::string path("data/News_Category_Dataset_v2.json");
	size_t n_queries = 10000, steps = 1000;
	duration<float> time_fill_dict;
	double vm, rss, avg_comparisons1 = 0.0, avg_comparisons2 = 0.0;
	double avg_time, avg_time1;
	avg_time = avg_time1 = 0.0;

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
	
	Dictionary *to_exp;
	if(strategy == "hash"){ 
		to_exp = new HashDictionary();
	}else{
		to_exp = new TRIEDictionary();
	}

	std::cout << "Strategy: " << strategy << std::endl;
	to_exp->setVerbose(false);
	
	std::clog << "Filling dictionary..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	// insert the dataset into the dictionary
	to_exp->insert(path);
	auto end = std::chrono::high_resolution_clock::now();
	time_fill_dict = end - start;
	
	// get the array of time measurements
	auto insert_times = to_exp->getTimeMeasurements();
	std::clog << "Generating plots for insertion time at each 20000 documents..." << std::endl;
	generatePlots(to_exp, strategy);
	// get the process memory usage
	process_mem_usage(vm, rss);
	
	// run time experiments with random queries with 1 term
	auto time_durations = time_experiment(path, n_queries, 1, steps, to_exp, &avg_comparisons1);
	// compute the average query time with 1 term
	for(size_t i = 0; i < time_durations.size(); i++) {
		avg_time += time_durations[i].count();
	}
	
	// run time experiments with random queries with 2 terms
	time_durations = time_experiment(path, n_queries, 2, steps, to_exp, &avg_comparisons2);
	// compute the average query time with 2 terms
	for(size_t i = 0; i < time_durations.size(); i++) {
		avg_time1 += time_durations[i].count();
	}
	avg_time /= n_queries;
	avg_time1 /= n_queries;
	
	clear();
	std::cout << "Strategy: " << strategy << std::endl;
	
	// compute the average insertion time
	double avg_insert_time = 0.0;
	for(size_t i = 0; i < insert_times.size(); i++) {
		std::cout << "Time to insert " << insert_times[i].first << "/" << to_exp->getNumberOfDocuments() << ": " << insert_times[i].second << " ms" <<  std::endl;
		avg_insert_time += insert_times[i].second;
	}
	avg_insert_time /= (double)insert_times.size();
	
	std::clog << "-------------------------------------------------------------------------------\n";
	std::clog << "Average insertion time: " << avg_insert_time << "ms" << std::endl;
	std::clog << "Time to construct the dictionary: " << time_fill_dict.count() << "s" << std::endl;
	std::clog << "Memory used by the dictionary VM: " << vm/1024 << " RSS: " << rss/1024 << " (Mb)" << std::endl;
	std::clog << "Average query time with 1 term: " << avg_time * 1E6 << " \xC2\xB5s" << std::endl;
	std::clog << "Average query time with 2 terms: " << avg_time1 * 1E6 << " \xC2\xB5s" << std::endl;
	std::clog << "Average comparisons with 1 term: " << avg_comparisons1  << std::endl;
	std::clog << "Average comparisons with 2 terms: " << avg_comparisons2 << std::endl;
	std::clog << "Average term size: " << avg_term_size << std::endl;
	
	delete to_exp;
	return 0;   
}