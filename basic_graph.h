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
#include <stack>

#define ERROR (-1)
#define NO_ERROR (0)
#define UNKNOWN_MATRIX (0)
#define ADJACENCY_MATR (1)
#define INCIDENCE_MATR (2)
#define ADJACENCY_LIST (3)
#define INF 200000000

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
	void add_vertex(int number_of_vertex);
	void delete_vertex(int number_of_vertex);
	void add_edge(int from, int to);
	void delete_edge(int from, int to);
	void make_graph_addition();
	void make_graph_contraction(std::vector<int> *user_arr);
	void make_vertex_identification(int first, int second);
	void make_vertex_dublicate(int number_of_vertex);
	void make_proliferation_with(int number_of_vertex);
	void make_union_with_graph(std::vector<std::vector<int>> *external_matrix);
	void make_connection_with_graph(std::vector<std::vector<int>> *external_matrix);
	void make_graph_product_with(std::vector<std::vector<int>> *external_matrix);

//Experimental
	void BFS(int node_num, std::vector<std::vector<int>> *temp_matrix,
		std::vector<int> *used);
	void algorithm_BFS(int node_num);
	void DFS(int node_num, int *used, std::queue<int> *path);
	void DFS_search(int node_start);
	int floyd_alg(int start_n, int end_n, int key);
	std::vector<std::vector<int>> * topological_sort(int *start_node, int *end_node);
	bool DFS_for_topological(int node_num, int *color, std::stack<int> *stack_vert);
	void non_loop_search(int start_num, int end_num);

	int basic_graph::DFS_for_ford_fulkers(int node_num, int end_node, int cmin, int *used,
		std::vector<std::vector<int>> *temp_matrix, int back_vert);
	void ford_fulkerson();
	int get_unused_user_num();

	struct
	{
		int weight;
		int first;
		int second;
	}typedef edge_struct;

	int basic_graph::DFS_for_kruskal(int node_num, int *used, int search_edge,
		std::vector<std::vector<int>> *temp_matrix);
	std::deque<edge_struct> * get_edges();
	void kruskal_algorithm();
	void DFS_another_non_loop(int node_num, int prev, int prev_weight, int *used, int *dist,
		int *path, int target);

protected:
	
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

	/*Distance*/
	std::vector<int> * get_distance_to_all_vertex(int number_of_vertex);
	std::vector<std::vector<int>> * get_count_of_connected_component();

  /*Init apropriate matrix for normal and oriented graph*/	
	void adjacency_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);
	void incidence_matrix(std::ifstream *work_file, std::vector<std::vector <int>> *matrix);

	/*That section is response for labels of vertices*/
	struct
	{
		int num;			//Number in user space
		int cur_num;		//Number in program space
	}typedef num_vert;

	int is_oriented;
	int transform_num(int num);
	int untransform_num(int cur_num);
	int add_user_num(int num, int matrix_num);
	int del_user_num(int num);

	/*initialized in constructor of appropriate matrix*/
	std::vector<num_vert> user_vert;
	std::vector<std::vector <int>> matrix;
};

#endif