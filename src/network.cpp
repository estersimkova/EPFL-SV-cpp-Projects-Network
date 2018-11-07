#include "network.h"
#include "random.h"
#include <algorithm>


void Network :: resize(const size_t& n)
{
	RandomNumbers ran;
	values.clear();
	for (unsigned int i(0); i< n; ++i) {
		values.push_back(ran.normal(0.0, 1.0));
	}
}

bool Network:: add_link(const size_t& i, const size_t& j)
{
	if ((values.size() > i) and (values.size() > j)) {
		if (i!=j) {					
				std::vector<size_t> neighbours = neighbors(i);
				if (std:: find(neighbours.begin(), neighbours.end(), j) !=neighbours.end()) return false;	//if j is in the vector, doesn't add the link
				else {
					links.insert(std::make_pair(i,j));
					links.insert(std::make_pair(j,i));
					return true;
			}
		}	
	}	
	return false;
}

size_t Network:: random_connect(const double& mean)	
{
	links.clear();
	int sum(0);
	size_t nb_connect(0);
	std::vector<int> random_nodes;
	random_nodes.clear();
	
	for (unsigned int i(0); i< values.size(); ++i) {
		nb_connect = RNG.poisson(mean);
		if (nb_connect > (values.size() -1)) nb_connect = (values.size() -1);
		random_nodes.clear();
		random_nodes.resize(nb_connect);
		RNG.uniform_int(random_nodes, 0, values.size() -1);
		
		for(auto j: random_nodes) {
			if (add_link(i, j)) ++ sum; 	
			else { 							//in case it can't add a link, to add the right number of links
				for (unsigned int a(0); a< values.size(); ++a) {
					if (add_link(i,a)) break;
				} 
			++sum;
			}
		}
	}
	return sum;
}

size_t Network:: set_values(const std::vector<double>& new_val)
{
	if (new_val.size() < values.size()) {
		for(unsigned int i(0); i< new_val.size(); ++i) values[i] = new_val[i];
		return new_val.size();
	}
	
	else if (values.size() == new_val.size()) {
		values.clear();
		for(unsigned int i(0); i< new_val.size(); ++i) values.push_back(new_val[i]);
		return values.size();
	}
	else {
		for(unsigned int i(0); i< values.size(); ++i) values[i] = new_val[i];
		return values.size();
	}
}

size_t Network:: size() const
{
	return values.size();
}

size_t Network:: degree(const size_t &_n) const
{
	if (values.size() > _n) return links.count(_n);   
	else throw (std::string ("Error: node not found.")) ;	
}

double Network:: value(const size_t &_n) const
{	
	if (values.size() > _n) return values[_n];		
	else throw (std::string ("Error: node not found.")) ;		
}

std::vector<double> Network:: sorted_values() const
{
	std::vector<double> sorted_val;
	sorted_val = values;
	std::sort(sorted_val.begin(), sorted_val.end());		
	std:: reverse(sorted_val.begin(), sorted_val.end());	
	return sorted_val;
}

std::vector<size_t> Network:: neighbors(const size_t& n) const
{
	std::vector<size_t> neighbours;
	for (auto val: links) {
		if (val.first == n) neighbours.push_back(val.second);
	}
	return neighbours;
}
