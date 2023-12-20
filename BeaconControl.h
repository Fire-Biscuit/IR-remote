#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// for the serial I/O
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

/// @brief typedef used to define the code needed to turn the beacon into a colour
typedef enum {
    OFF_COMMANDO = '0',
    ON_COMMANDO = '1',
    RED_COMMANDO = '2',
    GREEN_COMMANDO = '3',
    BLUE_COMMANDO = '4',
    YELLOW_COMMANDO = '5'
}BeaconColour_t;

/// @brief class to control the beacons and the beacon colour
class BeaconControl
{
private:
    /// @brief sets up the connection wit the usb port
    /// @param name path name to usb port
    /// @return file descriptor that is used to access the port
    int open_serial_port(const char* name) {
        int fd, bits;
        struct termios term;
        struct serial_struct kernel_serial_settings;

        if ((fd = open(name, O_RDWR | O_NONBLOCK | O_NOCTTY )) == -1) {
            fprintf(stderr, "open(%s) failed: %s\n", name, strerror(errno));
            return -1;
        }

        
        if (ioctl(fd, TIOCMGET, &bits) < 0) {
            close(fd);
            perror("ioctl(TCIOMGET)");
            return -1;
        }
        bits &= ~(TIOCM_DTR | TIOCM_RTS);
        if (ioctl(fd, TIOCMSET, &bits) < 0) {
            close(fd);
            perror("ioctl(TIOCMSET)");
            return -1;
        }
        sleep(1);
        tcflush(fd, TCIFLUSH);
        bits &= TIOCM_DTR;
        if (ioctl(fd, TIOCMSET, &bits) < 0) {
            close(fd);
            perror("ioctl(TIOCMSET)");
            return -1;
        }

        memset(&term, 0, sizeof(term));
        term.c_iflag = IGNBRK | IGNPAR;
        term.c_cflag = CS8 | CREAD | HUPCL | CLOCAL;
        cfsetospeed(&term, B115200);
        cfsetispeed(&term, B115200);
        if (tcsetattr(fd, TCSANOW, &term) < 0) {
            perror("tcsetattr()");
            return -1;
        }
        if (ioctl(fd, TIOCGSERIAL, &kernel_serial_settings) == 0) {
            kernel_serial_settings.flags |= ASYNC_LOW_LATENCY;
            ioctl(fd, TIOCSSERIAL, &kernel_serial_settings);
        }
        tcflush(fd, TCIFLUSH);

        return fd;
    }
    
    /// @brief tells the arduino what colour to turn the beacon into
    /// @param colour colour that the beacon should be
    void sendMessageToIR(BeaconColour_t colour){
        if ((n = write(this->fd, &colour, sizeof(BeaconColour_t))) == -1) {
            fprintf(stderr, "write() failed: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    int fd;
	ssize_t n;

public:
    /// @brief initializes the BeaconControl object
    BeaconControl(){
	    if ((fd = open_serial_port("/dev/ttyACM0")) == -1) {
            printf("couldnt open arduino");
            exit(EXIT_FAILURE);
        }
        printf("fd = %d", fd);
        ToggleBeacon(true);
    }
    /// @brief turns the beacon on or off
    /// @param onOrOff true to turn the beacon on and false to turn the beacon off
    void ToggleBeacon(bool onOrOff){
        this->sendMessageToIR(onOrOff ? ON_COMMANDO : OFF_COMMANDO);
    }

    /// @brief turns the beacon into a new colour
    /// @param beaconColour colour the beacon should become
    /// @note 4 second delay needed after this function to account for the slowness of the arduino
    void ToggleColour(BeaconColour_t beaconColour){
        this->sendMessageToIR(beaconColour);
    }
};