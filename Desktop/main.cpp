//#include <SDL.h>
//#include <SDL_ttf.h>
#include <iostream>
#include <chrono>
//#include <thread>
#include <memory>
using namespace std;

//#include <Windows.h>

//#include "Core.h"
//#include "Random.h"
//#include "Logger.h"

//#include "Tensor.h"
//#include "PixelsObserver.h"
//#include "GraphObserver.h"
//#include "HistogramObserver.h"
//#include "TextObserver.h"
//#include "ValueObserver.h"

#include "DQNLearning.h"

//void Sandbox1(AI::Core* core)
//{
//	//if (AI::Core::IsMainThread())
//	//{
//		printf("Test\n");
//
//		//vector<vector<double>> values{256};
//		//AI::Core::logger->GetLog("Test values")->RegisterPixels("Values", nullptr, [&]()
//		//{
//		//	return values;
//		//});
//
//		auto randomize = [](double x) {return Normal(); };
//
//		Tensor<> tensor1({128,128});
//		//tensor1.Apply(randomize);
//		AI::Core::desktop["TENSOR1"].observers.push_back(
//			new AI::PixelsObserver([&]()->Tensor<>&
//		{
//			return tensor1;
//		}));
//
//		Tensor<> tensor2({64,128});
//		//tensor2.Apply(randomize);
//		AI::Core::desktop["TENSOR1"].observers.push_back(
//			new AI::PixelsObserver([&]()->Tensor<>&
//		{
//			return tensor2;
//		}));
//
//		Tensor<> tensor3({128,64});
//		//tensor3.Apply(randomize);
//		AI::Core::desktop["TENSOR2"].observers.push_back(
//			new AI::PixelsObserver([&]()->Tensor<>&
//		{
//			return tensor3;
//		}));
//
//		Tensor<> tensor4({32,96});
//		//tensor4.Apply(randomize);
//		AI::Core::desktop["TENSOR2"].observers.push_back(
//			new AI::PixelsObserver([&]()->Tensor<>&
//		{
//			return tensor4;
//		}));
//
//		AI::Core::desktop["TXT"].observers.push_back(
//			new AI::GraphObserver([&]()->double
//		{
//			return Normal();
//		}));
//
//		vector<double> bins(10);
//		AI::Core::desktop["TXT"].observers.push_back(
//			new AI::HistogramObserver([&]()->vector<double>
//		{
//			for (size_t i = 0; i < bins.size(); i++)
//				bins[i] = Normal();
//			return bins;
//		}));
//
//		string value = to_string(Normal());
//		AI::Core::desktop["TXT"].observers.push_back(
//			new AI::TextObserver([&]()->string
//		{
//			value = string("Value: ") + to_string(Normal());
//			return value;
//		}));
//
//		//AI::Core::desktop->OrderGroupLogs("TENSOR1");
//		//AI::Core::desktop->OrderGroupLogs("TENSOR2");
//
//		AI::Core::desktop.Order();
//		AI::Core::desktop.active_path = "";
//		//AI::Core::desktop->OrderDesktopGroups();
//
//		//Sleep(100);
//
//		while (true)
//		{
//			core->Run();
//		}
//	//}
//}

void Sandbox2()
{
	//MetaLearningAckley();

	//WorldsStateValues();

	//BanditActionSelection();

	//KBandit kbandit(10);
	//ActionSelection(kbandit, 10, 10, 100, 1, 1000, "bandit_action_selection/");

	//GridWorld<GridCoords> gridworld(5, 5);
	//ActionSelection(gridworld, 10, 10, 50, 30, 1000, "gridworld_random_init/");

	//BreakOut<PaddleBallAndForce> bopbf(8, 8, 2, 3, 1, 0, true, true);
	//ActionSelection(bopbf, 10, 10, 100, 50, 1000, "bopbf_random_init/");

	//BreakOut<PaddleAndBall> bopb(8, 8, 2, 3, 1, 0, true, true);
	//ActionSelection(bopb, 10, 10, 100, 50, 1000, "bopb_random_init/");

	//BreakOut<Pixels> bopixels(8, 8, 2, 3, 1, 0, true, true);
	//ActionSelection(bopixels, 10, 10, 100, 50, 1000, "bopixels_random_init/");

	//NeuralNetsMNIST("C:\\Gwynbleidd\\MNIST\\");

	DQNLearningSmall();
	//DQNLearningHard();
	//DQNLearningBig();

	//Tensor<> weights;
	//Tensor<> weights_delta;

	//UnbiasedPerceptronSGD<Identity> unbiased_perceptron_sgd(784, 10, 0.005);
	//UnbiasedPerceptronRMS<Identity> unbiased_perceptron_rms(784, 10, 0.005);
	//UnbiasedPerceptronAD<Identity> unbiased_perceptron_ad(784, 10);
	//BiasedPerceptron<Identity> biased_perceptron(784, 10, 0.01);

	//LearningAlgorithms(unbiased_perceptron_sgd);
	//LearningAlgorithms(unbiased_perceptron_rms);
	//LearningAlgorithms(unbiased_perceptron_ad);
	//LearningAlgorithms(biased_perceptron);
}

int main(int argc, char* argv[])
{
	//if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	//	SDL_Init(SDL_INIT_EVERYTHING);

	//if (!TTF_WasInit())
	//	TTF_Init();

	//AI::Core{Sandbox2};
	Sandbox2();

	//SDL_Quit();
	//TTF_Quit();

	return 0;
}
