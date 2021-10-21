# osu-ds-convert

Convert osu beatmaps (with the latest [.osu file format](https://osu.ppy.sh/wiki/en/osu%21_File_Formats/Osu_%28file_format%29)) to the format [osu ds](https://github.com/echojc/osu-ds) wants

It's currently hardcoded to convert [*Miracle Sugite Yabai (feat. shully)*](https://osu.ppy.sh/beatmapsets/682290/#osu/1442771). 
**NOTE**: It's currently meant to be for this fork of [osu-ds](https://github.com/KonPet/osu-ds), since it's currently the only one that works. It might change in the future.

## How to use
Compile the code and run the `.exe`. **NOTE**: The converter will look for the `.osu` file in the parent folder of the working directory. There it will also output the `.ods` file. To play, create a folder called `osuds` on your SD-Card and put the `.ods` into that folder. To activate sound you'll have to edit the `.ods` file. Change `.mp3` to `.raw` and convert the `.mp3` file so it's mono and at 22050 Hz. Also make sure that the output has no header etc and that the output is encoded into Signed 8-bit PCM. A good program for that would be Audacity. Put that `.raw` file into your `osuds` folder. When you run osu-ds with SD-Card access now it should show your beatmap on the bottom screen. Tap it and have fun.

## Features:
* Title
* Author
* Creator
* Version
* Audio File Name


* HP Drain
* Circle Size
* Overall Difficulty
* Slider Multiplier
* Slider Tick Rate


* Peppy Stars
* Eyup Stars (Neither of them are converted. They're hardcoded to be 0)


* Timing points
* Breaks


* Hit Points
* Sliders (They don't work correctly)

## To do:
* Fix sliders
* Convert spinners
