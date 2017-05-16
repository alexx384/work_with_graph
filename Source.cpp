#include <iostream>
#include <fstream>
#include <cstdlib>
#include "normal_graph.h"
#include "oriented_graph.h"

using namespace std;

#define UNKNOWN_GRAPH 0
#define NORMAL_GRAPH 1
#define ORIENTED_GRAPH 2

int first(int type_of_graph);
int second(int type_of_graph);


/*##########################################################
###################### Важно! ############################
##########################################################
Что делать с отождествлением петли?

*/


int main()
{
	//normal_graph new_graph(ADJACENCY_MATR, "first_matrix.txt");

	//oriented_graph new_third(INCIDENCE_MATR, "third_matrix.txt");


	int type_of_graph = UNKNOWN_MATRIX;

	while (type_of_graph == UNKNOWN_MATRIX)
	{
		cout << setw(40) << left << "Choose type of graph" << endl;
		cout << setw(40) << "1 = NORMAL GRAPH" << endl;
		cout << setw(40) << "2 = ORIENTED GRAPH" << endl;
		cin >> type_of_graph;
		system("cls");

		switch (type_of_graph)
		{
		case NORMAL_GRAPH:		 type_of_graph = NORMAL_GRAPH;	break;
		case ORIENTED_GRAPH:	 type_of_graph = ORIENTED_GRAPH;	break;
		default:				 type_of_graph = UNKNOWN_GRAPH; break;
		}
	}

	int type_of_matrix = UNKNOWN_MATRIX;
		
	while (type_of_matrix == UNKNOWN_MATRIX)
	{
		cout << setw(40) << left << "Choose type of matrix" << endl;
		cout << setw(40) << "1 = ADJACENCY_MATR" << endl;
		cout << setw(40) << "2 = INCIDENCE_MATR" << endl;
		cout << setw(40) << "3 = ADJACENCY_LIST" << endl;
		cin >> type_of_matrix;
		system("cls");

		//REpair floyd on matrix how we make in the class
		switch (type_of_matrix)
		{
		case ADJACENCY_MATR: 
		{
			if (type_of_graph == NORMAL_GRAPH)
			{
				first(ADJACENCY_MATR);
			} else {
				second(ADJACENCY_MATR);
			}
			type_of_graph = UNKNOWN_MATRIX;
		}break;
		case INCIDENCE_MATR: 
		{
			if (type_of_graph == NORMAL_GRAPH)
			{
				first(INCIDENCE_MATR);
			} else
			{
				second(INCIDENCE_MATR);
			}
			type_of_graph = UNKNOWN_MATRIX;
		}break;
		case ADJACENCY_LIST:
		{
			if (type_of_graph == NORMAL_GRAPH)
			{
				first(ADJACENCY_LIST);
			} else
			{
				second(ADJACENCY_LIST);
			}
			type_of_graph = UNKNOWN_MATRIX;
		}break;
		}
	}

	system("pause");
	return 0;
}

int first(int type_of_graph)
{
	normal_graph new_graph(type_of_graph, "first_matrix.txt");
	normal_graph new_second(INCIDENCE_MATR, "second_matrix.txt");

	int user_number;

	while (true)
	{
		cout << setw(40) << left << "Choose one" << endl;
		cout << setw(40) << "1  = Get count vertex" << endl;
		cout << setw(40) << "2  = Get count_edge" << endl;
		cout << setw(40) << "3  = Get degree of" << endl;

		cout << setw(40) << "4  = Get degree sequence" << endl;
		cout << setw(40) << "5  = Show adjacency matrix" << endl;
		cout << setw(40) << "6  = Show incidence matrix" << endl;
		cout << setw(40) << "7  = Show adjacency list" << endl;
		cout << setw(40) << "8  = Get pendant vertex(aka visyachie)" << endl;
		cout << setw(40) << "9  = Get isolated vertex" << endl;
		cout << setw(40) << "10 = Get distance" << endl;

		cout << setw(40) << "11 = Get eccentricity of" << endl;

		cout << setw(40) << "12 = Get diameter" << endl;
		cout << setw(40) << "13 = Get radius" << endl;
		cout << setw(40) << "14 = Get center" << endl;
		cout << setw(40) << "15 = Get peripheral number" << endl;
		cout << setw(40) << "16 = Add vertex" << endl;
		cout << setw(40) << "17 = Delete vertex" << endl;

		cout << setw(40) << "18 = Add edge" << endl;

		cout << setw(40) << "19 = Delete edge" << endl;

		cout << setw(40) << "20 = Make graph addition(aka dopolnenie)" << endl;
		cout << setw(40) << "21 = Make graph contraction(aka styagivanie)" << endl;
		cout << setw(40) << "22 = Make graph subdivision(aka podrazbienie)" << endl;

		cout << setw(40) << "23 = Make vertex identification(aka otogdestvlenie)" << endl;

		cout << setw(40) << "24 = Make vertex dublicate" << endl;

		cout << setw(40) << "25 = Make proliferation with(aka razmogenie)" << endl;

		cout << setw(40) << "26 = Make union(aka ob'edinenie)" << endl;
		cout << setw(40) << "27 = Make connection(aka soedinenie)" << endl;
		cout << setw(40) << "28 = Make graph product(aka proizvedenie)" << endl;
		cout << setw(40) << "29 = Exit" << endl;
		cout << setw(40) << "30 = Build ostov(Prim's algorithm)" << endl;
		cout << setw(40) << "31 = BFS" << endl;
		cout << setw(40) << "32 = DFS" << endl;
		cout << setw(40) << "33 = Topological sort" << endl;

		cin >> user_number;
		system("cls");
		int from, to;
		switch (user_number)
		{
		case 1:  new_graph.show_count_vertex();	break;
		case 2:	 new_graph.show_count_edge();	break;
		case 3:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.show_degree_of(user_number);	break;
		}
		case 4:  new_graph.show_degree_sequence();	break;
		case 5:  new_graph.show_adjacency_matrix();	break;
		case 6:  new_graph.show_incidence_matrix();	break;
		case 7:  new_graph.show_adjacency_list();	break;
		case 8:  new_graph.show_pendant_vertex();	break;
		case 9:  new_graph.show_isolated_vertex();	break;
		case 10:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.show_distance(from, to);	break;
		}
		case 11:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.show_eccentricity_of(user_number);	break;
		}
		case 12: new_graph.show_length_of_diameter();	break;
		case 13: new_graph.show_length_of_radius();	break;
		case 14: new_graph.show_center_number();	break;
		case 15: new_graph.show_peripheral_number();	break;
		case 16: new_graph.add_vertex();	break;
		case 17:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.delete_vertex(user_number);	break;
		}
		case 18:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.add_edge(from, to);	break;
		}
		case 19:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.delete_edge(from, to);	break;
		}
		case 20: new_graph.make_graph_addition();	break;
		case 21:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_graph_contraction(from, to);	break;
		}
		case 22:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_graph_subdivision(from, to);	break;
		}
		case 23:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_vertex_identification(from, to);	break;
		}
		case 24:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.make_vertex_dublicate(user_number);	break;
		}
		case 25:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.make_proliferation_with(user_number);	break;
		}
		case 26: new_graph.make_union_with_graph(new_second.get_matrix());	break;
		case 27: new_graph.make_connection_with_graph(new_second.get_matrix());	break;
		case 28: new_graph.make_graph_product_with(new_second.get_matrix());	break;
		case 29: exit(0);	break;
		case 30: new_graph.build_ostov_prims(); break;
		case 31:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.BFS(user_number);	break;
		}
		case 32:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.DFS_search(user_number);	break;
		}
		case 33: new_graph.topological_sort(); break;
		default:
			break;
		}
		system("pause");
	}
}

int second(int type_of_graph)
{
	oriented_graph new_graph(type_of_graph, "first_matrix.txt");
	oriented_graph new_second(INCIDENCE_MATR, "second_matrix.txt");
	int user_number;

	while (true)
	{
		cout << setw(40) << left << "Choose one" << endl;
		cout << setw(40) << "1  = Get count vertex" << endl;
		cout << setw(40) << "2  = Get count_edge" << endl;
		cout << setw(40) << "3  = Get degree of" << endl;

		cout << setw(40) << "4  = Get degree sequence" << endl;
		cout << setw(40) << "5  = Show adjacency matrix" << endl;
		cout << setw(40) << "6  = Show incidence matrix" << endl;
		cout << setw(40) << "7  = Show adjacency list" << endl;
		cout << setw(40) << "8  = Get pendant vertex(aka visyachie)" << endl;
		cout << setw(40) << "9  = Get isolated vertex" << endl;
		cout << setw(40) << "10 = Get distance" << endl;

		cout << setw(40) << "11 = Get eccentricity of" << endl;

		cout << setw(40) << "12 = Get diameter" << endl;
		cout << setw(40) << "13 = Get radius" << endl;
		cout << setw(40) << "14 = Get center" << endl;
		cout << setw(40) << "15 = Get peripheral number" << endl;
		cout << setw(40) << "16 = Add vertex" << endl;
		cout << setw(40) << "17 = Delete vertex" << endl;

		cout << setw(40) << "18 = Add edge" << endl;

		cout << setw(40) << "19 = Delete edge" << endl;

		cout << setw(40) << "20 = Make graph addition(aka dopolnenie)" << endl;
		cout << setw(40) << "21 = Make graph contraction(aka styagivanie)" << endl;
		cout << setw(40) << "22 = Make graph subdivision(aka podrazbienie)" << endl;

		cout << setw(40) << "23 = Make vertex identification(aka otogdestvlenie)" << endl;

		cout << setw(40) << "24 = Make vertex dublicate" << endl;

		cout << setw(40) << "25 = Make proliferation with(aka razmogenie)" << endl;

		cout << setw(40) << "26 = Make union(aka ob'edinenie)" << endl;
		cout << setw(40) << "27 = Make connection(aka soedinenie)" << endl;
		cout << setw(40) << "28 = Make graph product(aka proizvedenie)" << endl;
		cout << setw(40) << "29 = Exit" << endl;
		cout << setw(40) << "31 = BFS" << endl;
		cout << setw(40) << "32 = DFS" << endl;

		cin >> user_number;
		system("cls");
		int from, to;
		switch (user_number)
		{
		case 1:  new_graph.show_count_vertex();	break;
		case 2:	 new_graph.show_count_edge();	break;
		case 3:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.show_degree_of(user_number);	break;
		}
		case 4:  new_graph.show_degree_sequence();	break;
		case 5:  new_graph.show_adjacency_matrix();	break;
		case 6:  new_graph.show_incidence_matrix();	break;
		case 7:  new_graph.show_adjacency_list();	break;
		case 8:  new_graph.show_pendant_vertex();	break;
		case 9:  new_graph.show_isolated_vertex();	break;
		case 10:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.show_distance(from , to);	break;
		}
		case 11:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.show_eccentricity_of(user_number);	break;
		}
		case 12: new_graph.show_length_of_diameter();	break;
		case 13: new_graph.show_length_of_radius();	break;
		case 14: new_graph.show_center_number();	break;
		case 15: new_graph.show_peripheral_number();	break;
		case 16: new_graph.add_vertex();	break;
		case 17:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.delete_vertex(user_number);	break;
		}
		case 18:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.add_edge(from, to);	break;
		}
		case 19:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.delete_edge(from, to);	break;
		}
		case 20: new_graph.make_graph_addition();	break;
		case 21:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_graph_contraction(from, to);	break;
		}
		case 22:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_graph_subdivision(from, to);	break;
		}
		case 23:
		{
			cout << "From ";
			cin >> from;
			cout << endl << "To ";
			cin >> to;
			new_graph.make_vertex_identification(from, to);	break;
		}
		case 24:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.make_vertex_dublicate(user_number);	break;
		}
		case 25:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.make_proliferation_with(user_number);	break;
		}
		case 26: new_graph.make_union_with_graph(new_second.get_matrix());	break;
		case 27: new_graph.make_connection_with_graph(new_second.get_matrix());	break;
		case 28: new_graph.make_graph_product_with(new_second.get_matrix());	break;
		case 29: exit(0);	break;
		case 31:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.BFS(user_number);	break;
		}
		case 32:
		{
			cout << "Enter the vertex number: ";
			cin >> user_number;
			new_graph.DFS_search(user_number);	break;
		}
		default:
			break;
		}
		system("pause");
	}
}