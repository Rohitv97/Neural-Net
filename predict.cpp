// RoNet predict.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<complex>
#include<vector>
#include<fstream>
#include<math.h>
using namespace std;

const int hidden_neurons = 3;

class Neuron
{
public:
	int layer;
	double lambda = 0.6;
	double w0;
	double w1[2];
	double w2[hidden_neurons];
	double in1;
	double netIn = 0;
	double output;
	double local_gradient;
	double eta = 0.65;
	double deltaweight1[2];
	double deltaweight2[hidden_neurons];
	double alpha = 0.1;

	void initWeight(int layer, int hidden_neurons)
	{
		float X = 1;
		if (layer == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
				w1[i] = r1 - 0;
				deltaweight1[i] = 0;
				cout << "Weights of hidden layer: " << w1[i] << endl;
			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
				w2[i] = r1 - 0;
				deltaweight2[i] = 0;
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
				//cout << "Previous layer " << i << " output: " << prevLayer.at(i)->output << endl;
				netIn = netIn + (w1[i] * prevLayer.at(i)->output);
			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				//cout << "Previous layer " << i << " output: " << prevLayer.at(i)->output << endl;
				netIn = netIn + (w2[i] * prevLayer.at(i)->output);
			}
		}
		else
		{
			netIn = in1;
		}
		//cout << "Net Input of Neuron: " << netIn << endl;

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

	void gradient(int layer, double error, vector<Neuron*> nextLayer, int neuron)
	{
		double sum_of_gw = 0.0;
		if (layer == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				sum_of_gw += nextLayer.at(i)->w2[neuron] * nextLayer.at(i)->local_gradient;
			}
			local_gradient = lambda * output * (1 - output) * sum_of_gw;
			//cout << "Sum of (Gradient of next * Weight) :  " << sum_of_gw << endl;
			//cout << "Local gradient of hidden neuron " << neuron << " : " << local_gradient << endl;
		}
		else if (layer == 2)
		{
			local_gradient = error * output * (1 - output) * lambda;
			//cout << "Local gradient of output neuron " << neuron << " : " << local_gradient << endl;
		}
	}

	void weightUpdate(int layer, vector<Neuron*> prevLayer)
	{
		if (layer == 1)
		{
			for (int i = 0; i < 2; i++)
			{
				deltaweight1[i] = (eta * local_gradient * prevLayer[i]->output) + (alpha * deltaweight1[i]);
				w1[i] += deltaweight1[i];

				//cout << "Delta W of hidden neurons: " << deltaweight1[i] << endl;
				//cout << "New weight : " << w1[i] << endl;

			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				deltaweight2[i] = (eta * local_gradient * prevLayer[i]->output) + (alpha * deltaweight2[i]);
				w2[i] += deltaweight2[i];

				//cout << "Delta W of output neurons: " << deltaweight2[i] << endl;
				//cout << "New weight : " << w2[i] << endl;

			}
		}
	}
};

void model_load(vector<Neuron*> hidLayer, vector<Neuron*> outLayer)
{
	ifstream model("model.txt");
	string line;
	double value;

	for (unsigned int i = 0; i < hidLayer.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			getline(model, line);
			value = stod(line);
			hidLayer[i]->w1[j] = value;
			//cout << hidLayer[i]->w1[j] << endl;

		}
	}

	for (unsigned int i = 0; i < outLayer.size(); i++)
	{
		for (int j = 0; j < hidden_neurons; j++)
		{
			getline(model, line);
			value = stod(line);
			outLayer[i]->w2[j] = value;
			//cout << outLayer[i]->w2[j] << endl;
		}
	}
}

int main()
{
	Neuron x1, x2, y1, y2;
	vector<Neuron*> inLayer;
	inLayer.push_back(&x1);
	inLayer.push_back(&x2);

	vector<Neuron*> outLayer;
	outLayer.push_back(&y1);
	outLayer.push_back(&y2);

	vector<Neuron*> hidLayer;
	for (int i = 0; i < hidden_neurons; i++)
	{
		Neuron* h = new Neuron();
		hidLayer.push_back(h);
	}

	x1.initWeight(0, 3);
	x2.initWeight(0, 3);

	model_load(hidLayer, outLayer);
	for (int i = 0; i < hidLayer.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cout << hidLayer[i]->w1[j] << endl;
		}
	}
	for (int i = 0; i < outLayer.size(); i++)
	{
		for (int j = 0; j < hidden_neurons; j++)
		{
			cout << outLayer[i]->w2[j] << endl;
		}
	}

	double x1n, x2n, h1n, y1n, y2n;
	double x1o, x2o, h1o, y1o, y2o;

	double t1;

	cout << "Input 1: ";
	cin >> t1;
	x1.in1 = (t1 / 5000);

	cout << "Input 2: ";
	cin >> t1;
	x2.in1 = (t1 / 5000);

	x1n = x1.netInput(inLayer, 0);
	x2n = x2.netInput(inLayer, 0);
	x1o = x1.activation(x1n, 0);
	x2o = x2.activation(x2n, 0);

	//cout << "Output of x1: " << x1o << endl;
	//cout << "Output of x2: " << x2o << endl;

	for (unsigned int j = 0; j < hidLayer.size(); j++)
	{
		h1n = hidLayer[j]->netInput(inLayer, 1);
		h1o = hidLayer[j]->activation(h1n, 1);
		//cout << "Output of h" << j+1 << ": " << h1o << endl;
	}

	y1n = y1.netInput(hidLayer, 2);
	y2n = y2.netInput(hidLayer, 2);
	y1o = y1.activation(y1n, 2);
	y2o = y2.activation(y2n, 2);

	y1o = (y1o * 250);
	y2o = (y2o * 250);

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
