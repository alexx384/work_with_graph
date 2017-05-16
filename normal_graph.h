#ifndef __NORMAL_GRAPH__
#define __NORMAL_GRAPH__

#include "basic_graph.h"

class normal_graph : public basic_graph
{
public:
	
	void show_degree_of(int number_of_vertex);
	void show_degree_sequence();
	void make_graph_subdivision(int from, int to);

	std::vector< std::vector<int>> *get_matrix();

	normal_graph(int type_of_graph, char *file_name);
	void delete_edge(int from, int to);
	void add_edge(int from, int to);
	void show_length_of_radius();
	void show_length_of_diameter();

	void build_ostov_prims();

private:
	void adjacency_list(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);
};

#endif