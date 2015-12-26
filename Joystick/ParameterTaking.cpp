#include "ParameterTaking.h"

enum { x = 0, y = 1, button = 2 };
const int LIMIT = 495;

ParameterTaking::ParameterTaking()
{
	joystickData[0] = 0;
	joystickData[1] = 0;
	joystickData[2] = 0;
	selectionType = 0;
}

void ParameterTaking::Update()
{
	/* Initializing part */
	struct termios toptions;

	/* open serial port */
	int fd;
	/* file descriptor */
	fd = open("/dev/ttyACM1", O_RDWR | O_NOCTTY);
	//cout << "fd opened as " << fd << endl;
	
	if(fd < 1){
		cout << "Check the device path" << endl;
		exit(-1);
	}

	/* wait for the Arduino to reboot */
	usleep(3000); // microseconds

	/* get current options for serial port */
	tcgetattr(fd, &toptions);
	/* set 9600 baud both ways */
	cfsetispeed(&toptions, B9600);
	cfsetospeed(&toptions, B9600);
	/* 8 bits, no parity, no stop bits */
	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;
	toptions.c_cflag |= (CLOCAL | CREAD);
	/* Canonical mode */
	toptions.c_lflag |= ICANON;
	/* commit the serial port settings */
	tcsetattr(fd, TCSANOW, &toptions);
	
	/* Reading part */
	int n, numberofRead = 0;
	/*----B-Y----*/
	char *buf = new char[11];
	float totalX = 0, totalY = 0;
	bool seektoX;

	tcflush(fd, TCIOFLUSH);

	for (int i = 0; i < 10; i++) {
		
		seektoX = true;
		/* seeking to 'X' */
		do {
			/* reading 1 byte for each loop until 'X' */
			n = read(fd, buf, 1);
			if (n <= 0) {
				seektoX = false;
			}
		} while (n > 0 && buf[0] != 'X');

		if (seektoX) {
			/* reading 4 byte X value */
			n = read(fd, buf, 11);
			cout << i << ". x buffer: " << buf << endl;
			
			totalX += BuffertoInt(buf);
			
			/* button state clicked: 1 */
			joystickData[2] = (int)buf[5] - '0';
			
				/* seeking to 'Y' */
				buf = buf + 7;
			
				cout << i << ". y buffer: " << buf << endl;
				
				totalY += BuffertoInt(buf);
				
				/* if X and Y are read succesfully, then increment numberofRead*/
				numberofRead++;	
				
				/* consuming 1 byte '\n' */
			    n = read(fd, buf, 1);
		}
		/* flush buffer then wait for new coming data */
		tcflush(fd, TCIOFLUSH);
		usleep(20000);
	}

	cout << "\nTotal x: " << totalX << endl;
	cout << "Total y: " << totalY << endl;
	cout << "Number of read: " << numberofRead << endl;
	cout << "Average x: " << (totalX / numberofRead) << endl;
	cout << "Average y: " << (totalY / numberofRead) << endl;
	joystickData[0] = totalX / numberofRead;
	joystickData[1] = totalY / numberofRead;
	cout << "Joystick x = " << joystickData[0] << endl;
	cout << "Joystick y = " << joystickData[1] << endl;
	cout << "Button state = " << joystickData[2] << endl;
	cout << "Direction: ";
	
	/* Joystick X values */
	if (joystickData[0] < LIMIT) {
		selectionType = 1;
		cout << "Left";
	}
	else if (joystickData[0] > LIMIT) {
		selectionType = 2;
		cout << "Right";
	}
	else {
		selectionType = 0;
		cout << "error" << endl;
	}
	
	/* Joystick Y values */
	if (joystickData[1] < LIMIT) {
		cout << " Down" << endl;
	}
	else if (joystickData[1] > LIMIT) {
		cout << " Up" << endl;
	}
	else {
		cout << "error" << endl;
	}
}

int *ParameterTaking::GetJoystickData()
{
	return joystickData;
}

int ParameterTaking::GetSelectionType()
{
	return selectionType;
}

int ParameterTaking::BuffertoInt(char* buf)
{
	int temp[4] = { 0 }, index = 0;
	int toReturn = 0;
	int i = 0;

	while (i < 4) {
		/* Checking number or not */
		if ((int)buf[i] >= 48 && (int)buf[i] <= 57) {
			if (buf[i] == '0') {
				i++;
				if (i == 4) toReturn = 0;
			}
			else {
				for (int j = i; j < 4; j++) {
					toReturn += (buf[j] - '0')*pow(10, 4 - j - 1);
				}
				break;
			}
		}
	}
	return toReturn;
}

int main() {
	ParameterTaking pt;
	pt.Update();

	return 0;
}
