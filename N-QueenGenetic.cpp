// Max Base
// https://github.com/BaseMax/N-QueenGenetic
#include <iostream>
#include <algorithm> // sort, random_shuffle
#include <sstream> // ostringstream
// #include <string>
#include <map>
#include <queue> // vector, .push_back()

using namespace std;

typedef struct {
	string way;
	int cost;
} individual;

vector<individual*> population;
int chessBoardSize;
int maxSolutions;
int initialPopulationCount = 10;

int fitValue(string way) {
	int fitness=(chessBoardSize*(chessBoardSize-1))/2;
	for(int i=0; i<chessBoardSize; i++)
		for(int j=i+1; j<chessBoardSize; j++)
			if((way[i] == way[j]) ||  (i-way[i] == j-way[j]) || (i+way[i] == j+way[j]))
				fitness--;
	return fitness;
}

individual* reproduce(individual *a, individual *b) {
	individual *item = new individual;
	int c = rand()%chessBoardSize;
	item->way =
				(a->way).substr(0, c) +
				(b->way).substr(c, chessBoardSize-c+1)
	;
	item->cost = fitValue(item->way);
	return item;
}

bool compare(individual *a, individual*b) {
	return(a->cost > b->cost);
}

individual* GA() {
	int random1, random2;
	individual *individual1, *individual2, *item;
	bool found=0;
	while(!found) {
		vector<individual*> populationNew;
		for(unsigned int i=0; i<population.size(); i++) {
			sort(population.begin(), population.end(), compare);

			random1 = rand()%population.size() %2;
			random2 = rand()%population.size() %2;

			individual1 = population[random1];
			individual2 = population[random2];

			item = reproduce(individual1, individual2);

			if(rand()%2==0)
				item->way[rand()%(chessBoardSize)+1] = (rand()%(chessBoardSize)+1)+48;

			if(fitValue(item->way)==((chessBoardSize*(chessBoardSize-1))/2)) {
				found=1;
				return item;
			}
			populationNew.push_back(item);
		}
		//
		population = populationNew;
	}
	return item;
}

int main() {
	srand(time(0));
	chessBoardSize=8;
	maxSolutions=92;
	map<string, int> solutions;
	int numFound=0;
	while(numFound!=maxSolutions) {
		string tempWay="";
		for(int i=1; i<=chessBoardSize; i++) {
			ostringstream ostr;
			ostr<<i;
			tempWay += ostr.str();
		}

		individual *temp;
		for(int i=0; i<initialPopulationCount; i++) {
			random_shuffle(tempWay.begin(), tempWay.end());
			temp   = new individual;
			temp->way = tempWay;
			temp->cost = fitValue(tempWay);
			population.push_back(temp);
		}

		individual *solution = GA();
		if(!solutions[solution->way]) {
			solutions[solution->way]=1;
			cout<<"Possible Solution #"<<(++numFound)<<":\t"<<solution->way<<endl;
		}
	}
	return 0;
}
