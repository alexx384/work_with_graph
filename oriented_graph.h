#ifndef __ORIENTED_GRAPH__
#define __ORIENTED_GRAPH__

#include "basic_graph.h"

class oriented_graph : public basic_graph
{
public:
	
	void show_degree_of(int number_of_vertex);
	void show_degree_sequence();
	void make_graph_subdivision(int from, int to);

	std::vector<std::vector <int>> *get_matrix();

	oriented_graph(int type_of_graph, char *file_name);

private:
	int get_indegree_of(int number_of_vertex);
	int get_outdegree_of(int number_of_vertex);
	void adjacency_list(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);
};

#endif