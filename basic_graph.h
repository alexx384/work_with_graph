#ifndef __BASIC_GRAPH__
#define __BASIC_GRAPH__

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <queue>

#define UNKNOWN_MATRIX 0
#define ADJACENCY_MATR 1
#define INCIDENCE_MATR 2
#define ADJACENCY_LIST 3

class basic_graph
{
public:

	void show_count_vertex();
	void show_count_edge();
	void show_adjacency_matrix();
	void show_incidence_matrix();
	void show_adjacency_list();
	void show_pendant_vertex();
	void show_isolated_vertex();
	void show_distance(int from, int to);
	void show_eccentricity_of(int number_of_vertex);
	void show_length_of_radius();
	void show_length_of_diameter();
	void show_center_number();
	void show_peripheral_number();
	void add_vertex();
	void delete_vertex(int number_of_vertex);
	void add_edge(int from, int to);
	void delete_edge(int from, int to);
	void make_graph_addition();
	void make_graph_contraction(int from, int to);
	void make_vertex_identification(int first, int second);
	void make_vertex_dublicate(int number_of_vertex);
	void make_proliferation_with(int number_of_vertex);
	void make_union_with_graph(std::vector<std::vector<int>> *external_matrix);
	void make_connection_with_graph(std::vector<std::vector<int>> *external_matrix);
	void make_graph_product_with(std::vector<std::vector<int>> *external_matrix);

//Experimental
	void BFS(int node_num);
	void DFS(int node_num, int *used);
	void DFS_search(int node_start);
	int floyd_alg();

protected:
	
	int is_oriented;


	int get_count_vertex();
	int get_count_edge();
	int get_degree_of(int number_of_vertex);
	int get_distance(int from, int to);
	int get_eccentricity_of(int numbet_of_vertex);
	std::vector<std::vector<int>>* get_length_of_diameter();
	std::vector<std::vector<int>>* get_length_of_radius();
	void insert_matrix_to(int row, int col, std::vector<std::vector<int>> *external_matrix);
	
	//Only for debug
	void show_external_matrix(std::vector<std::vector<int>> *external_matrix);

	//Never used
	int get_count_edges_in(std::vector<std::vector<int>> *external_matrix);

	std::vector<int> * get_distance_to_all_vertex(int number_of_vertex);
	std::vector<std::vector<int>> * get_count_of_connected_component();

	//Init for childs
	void adjacency_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);
	void incidence_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);

	std::vector<std::vector <int>> matrix;
};

#endif