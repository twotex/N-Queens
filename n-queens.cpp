/*
Eric Lee
CECS 451
8/12/2019

Homework #2
*/

#include <iostream>
#include <conio.h>
#include <time.h>
#include <string>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

int fitnessFunction(string theString);
string geneticAlgorithm(string* states, int n, int k);
string reproduce(string x, string y);
void mutate(string &x, int n);
string randomSelection(string* states, int n, int k, int maxScore);


//vector<int> theSteps;
//int steppy = 0;


int main() 
{
	int n; // # of queens
	int k; // # of states


	cout << "Enter the number of queens: ";
	cin >> n;
	cout << "Enter the number of states: ";
	cin >> k;

	cout << endl;
	
	srand(time(NULL));
	string *states = new string[k];
	
	int temp;
	for (int count = 0; count < k; count++)
	{
		for (int i = 0; i < n; i++)
		{
			temp = rand() % n;
			states[count].append(to_string(temp));
		}
	}

	string finalAnswer;
	cout << "Loading..." << endl;
	finalAnswer = geneticAlgorithm(states, n, k);

	cout << endl;
	cout << n << "-queens resulting configuration: " << finalAnswer << endl;
	cout << "Fitness Score: " << fitnessFunction(finalAnswer) << endl;
	

	/*
	for (int count = 0; count < 100; count++)
	{
		finalAnswer = geneticAlgorithm(states, n, k);
		steppy++;
	}

	int maxSteps;
	int minSteps;
	int totalSteps = 0;
	int average;
	int median;

	for (auto it = theSteps.begin(); it != theSteps.end(); it++)
	{
		if (it == theSteps.begin())
		{
			maxSteps = *it;
			minSteps = *it;
		}

		else
		{
			if (*it > maxSteps)
			{
				maxSteps = *it;
			}

			if (*it < minSteps)
			{
				minSteps = *it;
			}
		}

		totalSteps += *it;
	}
	
	average = totalSteps / 100;

	sort(theSteps.begin(), theSteps.end());
	median = theSteps[49];
	average = totalSteps / 100;

	cout << endl << endl;
	cout << "Max Steps: " << maxSteps << endl;
	cout << "Min Steps: " << minSteps << endl;
	cout << "Total Steps: " << totalSteps << endl;
	cout << "Average: " << average << endl;
	cout << "Median: " << median << endl;
	
	fstream myFile;
	myFile.open("numbers.txt");

	for (int count = 0; count < 100; count++)
	{
		myFile << theSteps[count] << endl;
	}

	myFile.close();
	*/

	_getch();
	return 0;
}


int fitnessFunction(string theString)
{
	int score = 0;

	int currentPosition;
	char tempCurrent;

	int positionInQuestion;
	char tempPosition;

	bool madeContact;

	for (int count = 0; count < theString.length() - 1; count++)
	{
		tempCurrent = theString.at(count);
		currentPosition = (int)tempCurrent - 48; //formula for converting char numeral to an int

		for (int i = count + 1; i < theString.length(); i++)
		{
			tempPosition = theString.at(i);
			positionInQuestion = (int)tempPosition - 48; //formula for converting char numeral to an int
			madeContact = false;

			//Checking Horizontal Axis for Contact
			if (currentPosition == positionInQuestion)
			{
				madeContact = true;
			}

			//Checking Upper Diagonal for Contact
			else if (currentPosition + (i - count) == positionInQuestion)
			{
				madeContact = true;
			}

			//Checking Lower Diagonal for Contact
			else if (currentPosition - (i - count) == positionInQuestion)
			{
				madeContact = true;
			}

			//If no contact was made, add one to total score
			if (madeContact == false)
			{
				score++;
			}
		}
	}

	return score;
}

string geneticAlgorithm(string* states, int n, int k)
{
	int selections = 0;
	int crossovers = 0;
	int mutations = 0;

	srand(time(NULL));
	string* newPopulation;
	string* statesCopy = new string[k];

	for (int count = 0; count < k; count++)
	{
		statesCopy[count] = states[count];
	}

	double maxScore = 0;

	for (int count = 0; count < n - 1; count++)
	{
		for (int i = count + 1; i < n; i++)
		{
			maxScore++;
		}
	}

	bool maxValueFound;
	string maxValueString = "";
	
	do 
	{
		newPopulation = new string[k];
		maxValueFound = false;

		for (int t = 0; t < k; t++)
		{
			string firstState;
			string secondState;

			unsigned seed = chrono::system_clock::now().time_since_epoch().count();
			default_random_engine generator(seed);
			uniform_real_distribution<double> distribution(0.0, 1.0);

			firstState = randomSelection(statesCopy, n, k, maxScore);
			selections++;
			secondState = randomSelection(statesCopy, n, k, maxScore);
			selections++;
			
			string theChild = reproduce(firstState, secondState);
			crossovers++;

			double mutateProbability = 0.20;
			double randomNumber3 = distribution(generator);
			if (randomNumber3 <= mutateProbability)
			{
				mutate(theChild, n);
				mutations++;
			}
			
			newPopulation[t] = theChild;
			
			if (fitnessFunction(theChild) == maxScore)
			{
				maxValueString = theChild;
				maxValueFound = true;
			}
		}

		delete [] statesCopy;
		statesCopy = newPopulation;

	} while (maxValueFound == false);

	int numberOfSteps = selections + crossovers + mutations;

	cout << endl;
	cout << "Random Selections: " << selections << endl;
	cout << "Crossovers: " << crossovers << endl;
	cout << "Mutations: " << mutations << endl;
	cout << "Number of Steps: " << numberOfSteps << endl;

	//theSteps.push_back(numberOfSteps);

	return maxValueString;
}

string reproduce(string x, string y)
{
	double randomNumber;

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(0.0, 1.0);

	randomNumber = distribution(generator);

	int n = x.length();
	int c = rand() % n;

	string strToReturn = "";

	if (randomNumber <= 0.5)
	{
		strToReturn.append(x.substr(0, c));
		strToReturn.append(y.substr(c, n - c));
	}

	else
	{
		strToReturn.append(y.substr(0, c));
		strToReturn.append(x.substr(c, n - c));
	}
	
	return strToReturn;
}

void mutate(string &x, int n)
{
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(0.0, 1.0);

	int columnToMutate = ((int)(distribution(generator) * 10.0)) % n;
	int newMutationValue = ((int)(distribution(generator) * 10.0)) % n;
	string newMutationString = to_string(newMutationValue);

	x.replace(columnToMutate, 1, newMutationString);
}


string randomSelection(string* states, int n, int k, int maxScore)
{
	string theState;
	double totalScore = 0;
	double *score = new double[k];
	double *probability = new double[k];


	for (int count = 0; count < k; count++)
	{
		score[count] = fitnessFunction(states[count]);
	}

	for (int count = 0; count < k; count++)
	{
		totalScore += score[count];
	}

	for (int count = 0; count < k; count++)
	{
		probability[count] = score[count] / totalScore;
	}

	double randomNumber;

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_real_distribution<double> distribution(0.0, 1.0);

	randomNumber = distribution(generator);

	double tempSum;
	tempSum = 0;

	for (int count = 0; count < k; count++)
	{
		tempSum += probability[count];
		if (randomNumber <= tempSum)
		{
			theState = states[count];
			break;
		}
	}

	return theState;
}