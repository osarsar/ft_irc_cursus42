#include <iostream>
#include <vector>
#include "./inc/channel.hpp" // Include the header file for the channel class

int main() {
    std::vector<channel*> channel_vec;

    // Create channel objects
    channel* channel1 = new channel("Channel 1"); // Assuming the channel class has a constructor that takes a name parameter
    channel* channel2 = new channel("Channel 2");

    // Add channel pointers to the vector
    channel_vec.push_back(channel1);
    channel_vec.push_back(channel2);

	std::vector<channel *>::iterator it;
	for (it = channel_vec.begin(); it != channel_vec.end();it++) {
		std::cout << *it << std::endl;
	}

    return 0;
}