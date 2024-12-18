User Interface Video Player
# README

## Overview

This project is a video player application built using Qt. It allows users to navigate through a collection of videos, play them, and control playback using various buttons and sliders.

## Prerequisites

- Qt 5.15.2 or later
- C++11 compatible compiler (e.g., MinGW 64-bit)
- CMake (optional, if using CMake for building)

## Project Structure

```
.gitignore
.vscode/
    settings.json
build/
    Desktop_Qt_5_15_2_MinGW_64_bit-Debug/
    Desktop_Qt_5_15_2_MinGW_64_bit-Profile/
button_navigator.cpp
button_navigator.h
next_button.cpp
next_button.h
pause_toggle_button.cpp
pause_toggle_button.h
previous_button.cpp
previous_button.h
README.md
test.txt
the_button.cpp
the_button.h
the_player.cpp
the_player.h
the.pro
the.pro.user
the.pro.user.bb3387f
timeline_slider.cpp
timeline_slider.h
tomeo.cpp
videos/
volume_button.cpp
volume_button.h
```

## Building the Project

### Using Qt Creator

1. Open `the.pro` file in Qt Creator.
2. Configure the project with the desired Qt version and compiler.
3. Build the project by clicking on the "Build" button.

### Using Command Line

1. Open a terminal and navigate to the project directory.
2. Run the following commands:

```sh
qmake
make
```

## Running the Application

### Using Qt Creator

1. After building the project, click on the "Run" button in Qt Creator.

### Using Command Line

1. Navigate to the build directory (e.g., `build/Desktop_Qt_5_15_2_MinGW_64_bit-Debug/`).
2. Run the executable:

```sh
./the.exe "path/to/videos"
```

Replace `"path/to/videos"` with the actual path to the directory containing your video files.

## Usage

- The main window will display a video player and control buttons.
- Use the "Next" and "Previous" buttons to navigate through the videos.
- Use the "Pause" button to toggle playback.
- Use the timeline slider to seek within the video.
- Use the volume button to adjust the volume.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## Contact

For any questions or issues, please open an issue on GitHub.