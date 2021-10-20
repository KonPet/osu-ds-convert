#include <iostream>
#include <fstream>
#include <vector>

void addStringN(const char* str, std::vector<char>& data) {
	int p = 0;
	while (str[p]) {
		data.push_back(str[p]);
		p++;
	}
}

void addVarInt(unsigned int in, std::vector<char>& data) {
    data.push_back(in & 0x7f);
    in >>= 7;
    int i = 1;
    while (in && i < 4) {
        data.back() += 0x80;
        data.push_back(in & 0x7f);
        in >>= 7;
        i++;
    }
}

void addStringS(const char* str, std::vector<char>& data) {
	unsigned int p = 0;
	while (str[p]) {
		p++;
	}
    addVarInt(p, data);
    addStringN(str, data);
}

void addData(void* dataIn, size_t length, std::vector<char>& data) {
    char* cdata = (char*) dataIn;
    for (size_t i = 0; i < length; i++) {
        data.push_back(cdata[i]);
    }
}

void addFloat(float fl, std::vector<char>& data) {
    addData(&fl, 4, data);
}

int main() {
    std::vector<char> data;
    
    // File magic
    addStringN("ODS", data);
    data.push_back(0);

    addStringS("Miracle Sugite Yabai (feat. shully)", data);    // Title
    addStringS("Hige Driver", data);                            // Artist
    addStringS("Milan-", data);                                 // Creator
    addStringS("Easy", data);                                   // Version
    addStringS("audio.mp3", data);                              // Audio filename

    data.push_back(1); // HP Drain Rate
    data.push_back(3); // CircleSize
    data.push_back(2); // Overall Difficulty

    addFloat(0.7, data); // Slider Multiplier
    addFloat(1, data);   // Slider Tick Rate
    
    data.push_back(0); // Peppy Stars
    addFloat(0, data); // Eyup Stars

    addVarInt(43, data);

    std::ofstream output("C:\\Users\\gercu\\Desktop\\Misc\\Programs\\NDS\\osu-ds-master\\test.osuds", std::ios::binary);

    if (!output) {
        std::cout << "Couldn't open output-file" << std::endl;
        return 2;
    }

    output.write(data.data(), data.size());

    return 0;
}