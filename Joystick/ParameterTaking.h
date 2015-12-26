#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>

using namespace std;

class ParameterTaking {
public:
	/*static ParameterTaking *GetSerialPort(){
	static ParameterTaking sp;
	return &sp;
	}*/
	ParameterTaking();
	void Update();
	int *GetJoystickData();
	int GetSelectionType();
	int BuffertoInt(char *);

private:
	//ParameterTaking();
	int joystickData[3]; // 0. index for X, 1. index for Y, 2. index for button 
	int fd;
	int selectionType;
};
