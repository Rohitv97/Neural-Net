// NeuralNet.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include<cstdlib>
#include<ctime>
#include<complex>
#include<vector>
#include<iostream>
using namespace std;

const int hidden_neurons = 3;


class Neuron
{
public:

	int layer;
	double lambda = 0.7;
	double w0;
	double w1[2];
	double w2[hidden_neurons];
	int in1;
	double netIn = 0;
	double output;

	void initWeight(int layer, int hidden_neurons)
	{
		float X = 1;
		if (layer == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				srand(static_cast <unsigned> (time(0)));
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
				w1[i] = r1 - 0;
				cout << "Weights of hidden layer: " << w1[i] << endl;
			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				srand(static_cast <unsigned> (time(0)));
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
				w2[i] = r1 - 0;
				cout << "Weights of output layer: " << w2[i] << endl;
			}
		}
		else
		{
			w0 = 0;
		}

	}

	double netInput(vector<Neuron*> prevLayer, int layer)
	{
		double netIn = 0;
		if (layer == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				cout << "Previous layer " << i << " output: " <<  prevLayer.at(i)->output << endl;
				netIn = netIn + (w1[i] * prevLayer.at(i)->output);
			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				cout << "Previous layer " << i << " output: " << prevLayer.at(i)->output << endl;
				netIn = netIn + (w2[i] * prevLayer.at(i)->output);
			}
		}
		else
		{
			netIn = in1;
		}
		cout << "Net Input of Neuron: " << netIn << endl;

		return netIn;
	}

	double activation(double nIn, int layer)
	{
		if (layer == 0)
		{
			output = nIn;
			return output;
		}
		else
		{
			double z = lambda * nIn;
			output = 1 / (1 + (exp(-z)));
			return output;
		}
	}

	void Input()
	{
		cout << "Enter input: ";
		cin >> in1;
	}


};


int main()
{
	Neuron x1, x2, h1, h2, h3, y1, y2;
	double x1n, x2n, h1n, h2n, h3n, y1n, y2n;
	double x1o, x2o, h1o, h2o, h3o, y1o, y2o;

	x1.initWeight(0, 3);
	x2.initWeight(0, 3);
	h1.initWeight(1, 3);
	h2.initWeight(1, 3);
	h3.initWeight(1, 3);
	y1.initWeight(2, 3);
	y2.initWeight(2, 3);

	x1.Input();
	x2.Input();

	vector<Neuron*> inLayer;
	

	//vector<Neuron>  inLayer;
	inLayer.push_back(&x1);
	inLayer.push_back(&x2);
	vector<Neuron*> hidLayer;
	hidLayer.push_back(&h1);
	hidLayer.push_back(&h2);
	hidLayer.push_back(&h3);
	vector<Neuron*> outLayer;
	outLayer.push_back(&y1);
	outLayer.push_back(&y2);

	x1n = x1.netInput(inLayer, 0);
	x2n = x2.netInput(inLayer, 0);
	x1o = x1.activation(x1n, 0);
	x2o = x2.activation(x2n, 0);

	cout << "Output of x1: " << x1o << endl;
	cout << "Output of x2: " << x2o << endl;
	

	h1n = h1.netInput(inLayer, 1);
	h2n = h2.netInput(inLayer, 1);
	h3n = h3.netInput(inLayer, 1);
	h1o = h1.activation(h1n, 1);
	h2o = h2.activation(h2n, 1);
	h3o = h3.activation(h3n, 1);
	cout << "Output of h1: " << h1o << endl;
	cout << "Output of h2: " << h2o << endl;
	cout << "Output of h3: " << h3o << endl;

	y1n = y1.netInput(hidLayer, 2);
	y2n = y2.netInput(hidLayer, 2);
	y1o = y1.activation(y1n, 2);
	y2o = y2.activation(y2n, 2);
	cout << "Output of y1: " << y1o << endl;
	cout << "Output of y2: " << y2o << endl;

	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
