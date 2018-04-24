

#include <FL/Fl.H>
#include <stdlib.h>
#include "MyWindow.h"
#include "DisplayMesh.h"
#include "processor.h"

int main(int argc, char **argv) {
    MyWindow *window = new MyWindow();

    vector<string> args;
	cout << endl;
	for (int i = 0; i < argc; ++i) {
		args.push_back(argv[i]);
		cout << argv[i] << "ÔËÐÐÊä³öargv"<<endl;
	}
	cout << endl;
    process(args, window);
	system("pause");
    window->show();
	
    return Fl::run();
}
