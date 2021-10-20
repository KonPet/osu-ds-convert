#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "osu!parser.h"

std::string sTypes[] = { "linear", "perfect", "bezier", "catmull" };

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
    while (in && (i < 4)) {
        data.back() |= 0x80;
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

int main() {
    std::vector<char> data;

    std::ifstream file("../Hige Driver - Miracle Sugite Yabai (feat. shully) (Milan-) [Easy].osu");

    if (!file) {
        std::cout << "Couldn't read .osu file" << std::endl;
        return 1;
    }

    osuParser::OsuParser p(&file);
    p.Parse();

    std::cout << p.title << std::endl;

    // File magic
    addStringN("ODS", data);
    data.push_back(0);

    addStringS(p.title.c_str(), data);
    addStringS(p.artist.c_str(), data);
    addStringS(p.creator.c_str(), data);
    addStringS(p.version.c_str(), data);
    addStringS(p.audioFilename.c_str(), data);

    data.push_back((uint8_t) p.hpDrainRate);
    data.push_back((uint8_t) p.circleSize);
    data.push_back((uint8_t) p.overallDifficulty);

//    data.push_back(1); // HP Drain Rate
//    data.push_back(3); // CircleSize
//    data.push_back(2); // Overall Difficulty

    float a = p.sliderMultiplier;
    addData(&a, 4, data);
    a = p.sliderTickRate;
    addData(&a, 4, data);
    a = 1;
    addData(&a, 4, data);

//    addFloat(0.7, data); // Slider Multiplier
//    addFloat(1, data);   // Slider Tick Rate

    data.push_back(0); // Peppy Stars
    int b = 0;
    addData(&b, 4, data);

    addVarInt(p.timingPoints.size(), data);

    for (auto & timingPoint : p.timingPoints) {
        addData(&timingPoint.offset, 4, data);
        float msPerBeat = timingPoint.msPerBeat;
        addData(&msPerBeat, 4, data);
        data.push_back(timingPoint.sampleSet);
    }

    std::vector<osuParser::Event> breaks;

    for (auto & event : p.events) {
        if (event.type == osuParser::eBreak) {
            breaks.push_back(event);
        }
    }

    addVarInt(breaks.size(), data);
    for (auto & eBreak : breaks) {
        addData(&eBreak.begin, 4, data);
        addData(&eBreak.end, 4, data);
    }


    std::vector<osuParser::HitObject> objects;

    for (auto & hitObject : p.hitObjects) {
        if (hitObject.type == osuParser::oCircle || hitObject.type == osuParser::oSlider) {
            objects.push_back(hitObject);
        }

//        switch (hitObject.type) {
//            case osuParser::oCircle:
//                printf("C: (%i, %i), %i, %x, %x", hitObject.x, hitObject.y, hitObject.time, hitObject.type, hitObject.soundMask);
//            case osuParser::oSlider:
//                printf("C: (%i, %i), %i, %x, %x, %c", hitObject.x, hitObject.y, hitObject.time, hitObject.type, hitObject.soundMask, sTypes[hitObject.slider.type].c_str(), );
//            case osuParser::oSpinner:
//                printf("C: (%i, %i), %i, %x, %x", hitObject.x, hitObject.y, hitObject.time, hitObject.type, hitObject.soundMask);
//        }
    }

    addVarInt(objects.size(), data);

    for (auto & object : objects) {
        addData(&object.time, 4, data);
        data.push_back(1 << object.type);
        addData(&object.x, 2, data);
        addData(&object.y, 2, data);
        data.push_back(object.soundMask);

        if (object.type == osuParser::oSlider) {
            data.push_back(object.slider.nRepeats);
            data.push_back(0);

            addData(&object.slider.duration, 4, data);
            addVarInt(object.slider.curvePoints.size(), data);

            for (auto & curvePoint : object.slider.curvePoints) {
                addData(&curvePoint.x, 2, data);
                addData(&curvePoint.y, 2, data);

                b = 1;
                addData(&b, 4, data);
                if (object.slider.type == osuParser::sBezier) {
                    std::cout << (signed short) curvePoint.x << ", " << (signed short) curvePoint.y << std::endl;
                }
            }

            if (object.slider.type == osuParser::sBezier) {
                std::cout << std::endl;
            }

            addVarInt(1, data);
            b = 200;
            addData(&b, 2, data);
            addData(&b, 2, data);
        }
    }

    std::ofstream output("../Hige Driver - Miracle Sugite Yabai (feat. shully) (Milan-) [Easy].ods", std::ios::binary);

    if (!output) {
        std::cout << "Couldn't open output-file" << std::endl;
        return 2;
    }

    output.write(data.data(), data.size());

    return 0;
}