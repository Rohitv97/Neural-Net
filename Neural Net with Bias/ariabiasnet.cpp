#include "aria.h"
#include <iostream>
#include<stdlib.h>
#include<math.h>
#include<cstdlib>
#include<ctime>
#include<complex>
#include<vector>
#include<fstream>
#include<math.h>
using namespace std;

const int hidden_neurons = 3;
const double lambda = 0.85;
const double eta = 0.9;
const double alpha = 0.7;

class Neuron
{
public:
	int layer;
	//double lambda = 0.6;
	double w0;
	double w1[3];
	double w2[hidden_neurons + 1];
	double in1;
	double netIn = 0;
	double output;
	double local_gradient;
	//double eta = 0.65;
	double deltaweight1[3];
	double deltaweight2[hidden_neurons + 1];
	//double alpha = 0.1;

	void initWeight(int layer, int hidden_neurons)
	{
		float X = 1;
		if (layer == 1)
		{
			for (int i = 0; i <= 2; i++)
			{
				float r1 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / X));
				w1[i] = r1 - 0;
				deltaweight1[i] = 0;
				cout << "Weights of hidden layer: " << w1[i] << endl;
			}
		}
		else if (layer == 2)
		{
			for (int i = 0; i <= hidden_neurons; i++)
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
			netIn = netIn + w1[2];
		}
		else if (layer == 2)
		{
			for (int i = 0; i < hidden_neurons; i++)
			{
				//cout << "Previous layer " << i << " output: " << prevLayer.at(i)->output << endl;
				netIn = netIn + (w2[i] * prevLayer.at(i)->output);
			}
			netIn = netIn + w2[hidden_neurons];
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

	void weightUpdate(int layer, vector<Neuron*> prevLayer, vector<Neuron*> biasLayer)
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
			deltaweight1[2] = (eta * local_gradient * biasLayer[0]->output) + (alpha * deltaweight1[2]);
			w1[2] += deltaweight1[2];
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
			deltaweight2[hidden_neurons] = (eta * local_gradient * biasLayer[1]->output) + (alpha * deltaweight2[hidden_neurons]);
			w2[hidden_neurons] += deltaweight2[hidden_neurons];
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
		for (int j = 0; j <= 2; j++)
		{
			getline(model, line);
			value = stod(line);
			hidLayer[i]->w1[j] = value;
			cout << hidLayer[i]->w1[j] << endl;

		}
	}

	for (unsigned int i = 0; i < outLayer.size(); i++)
	{
		for (int j = 0; j <= hidden_neurons; j++)
		{
			getline(model, line);
			value = stod(line);
			outLayer[i]->w2[j] = value;
			cout << outLayer[i]->w2[j] << endl;
		}
	}
}

int main(int argc, char **argv)
{
	//Initialise
	//create instances
	Aria::init();
	ArRobot robot;

	//parse command line arguments
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();

	//Connect to robot
	ArRobotConnector robotConnector(&argParser, &robot);

	if (robotConnector.connectRobot())
		std::cout << "Robot connected!" << std::endl;

	robot.runAsync(false);
	robot.lock();
	robot.enableMotors();
	robot.unlock();

	// Initialise Neural Network and load weights
	cout << "Creating a brain..." << endl;
	cout << "Remembering old stuff..." << endl;
	cout << "I think I'm done..." << endl;

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

	Neuron b1, b2;
	vector<Neuron*> biasLayer;
	biasLayer.push_back(&b1);
	biasLayer.push_back(&b2);

	x1.initWeight(0, 3);
	x2.initWeight(0, 3);
	b1.initWeight(0, 3);
	b2.initWeight(0, 3);

	b1.output = 1;
	b2.output = 1;

	model_load(hidLayer, outLayer);

	double x1n, x2n, h1n, y1n, y2n;
	double x1o, x2o, h1o, y1o, y2o;


	robot.setVel2(50, 50);

	while (true)
	{
		//----------Get Sonar Readings----------

		ArSensorReading *sonarSensor[8];

		int sonarRange[8];
		for (int i = 0; i < 8; i++)
		{
			sonarSensor[i] = robot.getSonarReading(i);
			sonarRange[i] = sonarSensor[i]->getRange();
		}
		
		double min0 = sonarRange[0];

		for (int i = 0; i < 5; i++)
		{
			double val0 = robot.getSonarReading(0)->getRange();
			if (min0 > val0)
			{
				min0 = val0;
			}
		}

		double min1 = sonarRange[1];

		for (int i = 0; i < 5; i++)
		{
			double val1 = robot.getSonarReading(1)->getRange();
			if (min1 > val1)
			{
				min1 = val1;
			}
		}

		double min2 = sonarRange[2];

		for (int i = 0; i < 5; i++)
		{
			double val2 = robot.getSonarReading(2)->getRange();
			if (min2 > val2)
			{
				min2 = val2;
			}
		}

		double min3 = sonarRange[3];

		for (int i = 0; i < 5; i++)
		{
			double val3 = robot.getSonarReading(3)->getRange();
			if (min3 > val3)
			{
				min3 = val3;
			}
		}

		double min4 = sonarRange[4];

		for (int i = 0; i < 5; i++)
		{
			double val4 = robot.getSonarReading(4)->getRange();
			if (min4 > val4)
			{
				min4 = val4;
			}
		}

		double min5 = sonarRange[5];

		for (int i = 0; i < 5; i++)
		{
			double val5 = robot.getSonarReading(5)->getRange();
			if (min5 > val5)
			{
				min5 = val5;
			}
		}
		
		

		double rbs = min(min0, min1);
		double v34 = (min3 + min4) / 2;
		double rfs = min(min2, min5);
		rfs = min(rfs, v34);


		x1.in1 = (rbs / 5000);
		x2.in1 = (rfs / 5000);

		x1n = x1.netInput(inLayer, 0);
		x2n = x2.netInput(inLayer, 0);
		x1o = x1.activation(x1n, 0);
		x2o = x2.activation(x2n, 0);

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


		//---------Setting speed----------

		robot.setVel2(y1o, y2o);


		ArUtil::sleep(100);

	}

	//termination
	//stop the robot
	robot.lock();
	robot.stop();
	robot.unlock();

	//terminate all threads
	Aria::exit();


	return 0;
}
