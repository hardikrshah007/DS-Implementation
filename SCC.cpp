#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
struct Node{
	struct Node* next=NULL;
	int num_Vertex, leader_num;
	bool explored=false;
};



class linked_list{
public:
	Node *head,*tail; // Tail is required for example in LIFO systems such as Stack, head is required in FIFO systems
	linked_list(){
		head	=	NULL,tail 	=	NULL;
	}
	int leader;
	void add_Node(Node* start);
	void print_list();
	void delete_list();
};


void linked_list::add_Node(Node* start)
{
	if(head==NULL && tail==NULL)
	{
		head = start;
		tail=start;
	}
	else
	{
		tail->next=start;
		tail=tail->next;
	}
}

void linked_list::print_list()
{
	Node* pos;
	cout<<" Link is"<<endl;
	//cout<<head->num_Vertex<<"->";
	if(head!=NULL)
	{
		pos = head;//->next;
		while(pos->next!=NULL)
		{
			cout<<pos->num_Vertex<<"->";
			pos=pos->next;
		}
		cout<<pos->num_Vertex<<endl;
	}
	else 
		cout<<"empty"<<endl;
}
void linked_list::delete_list()
{
	while(head!=NULL)
	{
		Node *n1 = head;
		head=head->next;
		n1=NULL;
	}
}

std::vector<linked_list> reverseGraph(std::vector<linked_list> v)
{
	std::vector<linked_list> rev_graph;
	for(int i=0;i<v.size();i++)
	{
		linked_list l1;
		Node *start = new Node();
		start->num_Vertex=v[i].head->num_Vertex;
		l1.add_Node(start);
		rev_graph.push_back(l1);
	}
	
	for(int i=0;i<v.size();i++)
	{
		if(v[i].head!=NULL)
		{
			Node *pos=v[i].head->next;
			while(pos!=NULL)
			{
				int head_index = pos->num_Vertex;
				int tail_index = v[i].head->num_Vertex;
				Node *n1 = new Node();
				n1->num_Vertex=tail_index;
				rev_graph[head_index-1].add_Node(n1);
				pos=pos->next;
			}
		}
	}
	return rev_graph;
}

void DFS_recursive(std::vector<linked_list> &network, int &current_label,int vertex)
{
	Node *pos = network[vertex].head;
	if(network[vertex].head->explored==false)
	{
		network[vertex].head->explored=true;
		while(pos->next!=NULL)
		{
			pos = pos->next;
			if(network[(pos->num_Vertex)-1].head->explored==false)
				DFS_recursive(network,current_label,(pos->num_Vertex)-1);
		}

		// cout<<"Current Label is: "<<current_label<<endl;
		// cout<<"vertex is: "<<vertex<<endl;
		network[vertex].head->leader_num = current_label;
		current_label--;
		//cout<<"The current label is: "<<current_label<<endl;
	}

}

void DFS_main_Loop(std::vector<linked_list>& network)
{
	int current_label = network.size();

	for(int i=network.size()-1;i>=0;i--)//<network.size();i++)
	{
		if(network[i].head->explored==false)
			DFS_recursive(network,current_label,i);
	}
}

void DFS_second_recursive(std::vector<linked_list> &network, int &current_label, int vertex)
{
	Node *pos = network[vertex].head;
	if(network[vertex].head->explored==false)
	{
		network[vertex].head->explored=true;
		network[vertex].head->leader_num=current_label;
		while(pos->next!=NULL)
		{
			pos = pos->next;
			if(network[pos->num_Vertex-1].head->explored==false)
				DFS_second_recursive(network,current_label,pos->num_Vertex-1);
		}
		//network[pos->num_Vertex-1].head->leader_num=current_label;
		// cout<<"Current Label is: "<<current_label<<endl;
		// cout<<"vertex is: "<<vertex<<endl;
		//
		//current_label++;//--;current_label++;
	}
		
}
void DFS_second_main_Loop(std::vector<linked_list> &rev_network,std::vector<linked_list> &network)
{
	int current_label = 1;//network.size();
	for(int i=0;i<rev_network.size();i++)//<network.size();i++)
	{
		if(network[rev_network[i].head->num_Vertex-1].head->explored==false)
		{
			DFS_second_recursive(network,current_label,rev_network[i].head->num_Vertex-1);
			current_label++;
		}
	}
	// for(int i=0;i<network.size();i++)
	// 	std::cout<<"Vertex is: "<<network[i].head->num_Vertex<<" and the leader is: "<<network[i].head->leader_num<<endl;
}

bool sortLeader( linked_list l1,linked_list l2)
{
	return l1.head->leader_num < l2.head->leader_num;
}

// bool sortLeaderColumn(linked_list l1,linked_list l2)
// {

// }

void SCC_final_loop(std::vector<linked_list> &rev_network, std::vector<linked_list> &network)
{
	// for(int i=0;i<rev_network.size();i++)
	// 	std::cout<<"The vertex is: "<<rev_network[i].head->num_Vertex<<" and the finishing time is: "<<rev_network[i].head->leader_num<<endl;
	std::sort(rev_network.begin(),rev_network.end(),sortLeader);
	//std::cout<<"in sorted array , the first head has vertex"<<rev_network[0].head->num_Vertex<<" and the leader number is: "<<rev_network[0].head->leader_num<<endl;
	DFS_second_main_Loop(rev_network,network);

}



int main ()
{
	vector <linked_list> Graph,Graph1;
	int n;
	string line;
	//cout<<"Enter the number of nodes in the network"<<endl;
	//cin>>n;
	for (int i=1;i<=320000;i++)
	// for (int i=1;i<=9;i++)
	{
		linked_list l1;
		Node *start = new Node();
		start->num_Vertex =i;
		l1.add_Node(start);
		Graph.push_back(l1);
	}
	ifstream SCC_file;
	//SCC_file.open("SCC.txt");
	SCC_file.open("input_mostlyCycles_68_320000.txt");
	if(SCC_file.is_open())
	{
		while(!SCC_file.eof())
		{
			getline(SCC_file,line);
			istringstream ssi(line);
			string token;
			vector<string> token_array;
			while(std::getline(ssi,token,' '))
			{
				token_array.push_back(token);
			}

			int head_index = stoi(token_array[0]);
			int tail_index = stoi(token_array[1]);
			Node *edge = new Node();
			edge->num_Vertex = tail_index;
			Graph[head_index-1].add_Node(edge);
		}
	}
	

	SCC_file.close();
	cout<<"Finished generating the graph"<<endl;
	// for (int i=0;i<Graph.size();i++)
	// 	Graph[i].print_list();
	//Graph[875713].print_list();
	Graph1 = reverseGraph(Graph);
	cout<<"Finished reversing the graph"<<endl;
	DFS_main_Loop(Graph1);
	// for(int i=0;i<Graph1.size();i++)
	// {
	// 	Graph1[i].print_list();
	// 	cout<<"The current labels are: "<<Graph1[i].head->leader_num<<endl;
	// }
	cout<<"Finished DFS on the reverse graph"<<endl;
	SCC_final_loop(Graph1,Graph);

	sort(Graph.begin(),Graph.end(),sortLeader);
	cout<<"Finished SCC on the  graph"<<endl;
	// for(int i=0;i<Graph.size();i++)
	// 	cout<<"The leader for vertex: "<<Graph[i].head->num_Vertex<<" is: "<<Graph[i].head->leader_num<<endl;

	std::vector<int> LeaderCount;
	for(int i=0;i<5;i++)
		LeaderCount.push_back(0);

	int count =0;
	int oldLeader=Graph[0].head->leader_num;
	bool reset =false;
	for(int i=0;i<Graph.size();i++)
	{
		int newLeader= Graph[i].head->leader_num;
		if( newLeader == oldLeader)
		{
			count++;
			reset = false;
		}
		else 
		{
			LeaderCount.push_back(count);
			count =1;
			oldLeader=newLeader;
			reset =true;
		}
	}

	if(reset ==false)
		LeaderCount.push_back(count);

	sort(LeaderCount.begin(),LeaderCount.end());
	for(int i=LeaderCount.size()-1;i>LeaderCount.size()-6;i--)
		cout<<"Leader count is: "<<LeaderCount[i]<<endl;

	return 0;
}