#include <string>
#include <iostream>

using namespace std;

typedef void(*ActionListener)(unsigned char *imageData, int len);
static ActionListener g_actionListener = 0;

typedef void(*ResultActionListener)(char** results, int size);
static ResultActionListener g_resultActionListener = 0;

unsigned char *imageData;


//Will send byte[](actually char[]) back to C#
extern "C" __declspec(dllexport) void setupListener(void(*listener)(unsigned char *imageData, int len)) {
	g_actionListener = listener;
}
extern "C" __declspec(dllexport) void call() {

	unsigned char tempData[10] = { '1','2','3','4','5','\0','6','7','8','9' };

	imageData = new unsigned char[10];

	memcpy(imageData, (const char *)tempData, 10);

	g_actionListener(imageData, 10);
}

//Will send string[] back to C#
extern "C" __declspec(dllexport) void setupResultListener(void(*listener)(char** results, int size)) {
	g_resultActionListener = listener;
}
extern "C" __declspec(dllexport) void callResult() {
	char* results[2];
	results[0] = "abc";
	results[1] = "123";

	g_resultActionListener(results, 2);
}