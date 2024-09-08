# SimpleFileSystem

A small project in which a FAT and an I-Node based file system is implemented and showcased.

## Project Overview

This project implements two types of file systems: FAT (File Allocation Table) and I-Node based file systems. The project is written in C++ and uses Qt for the graphical user interface.

### Features

- **FAT File System**: Implements a simple FAT file system.
- **I-Node File System**: Implements a simple I-Node based file system.
- **Graphical Interface**: Uses Qt to provide a graphical interface for interacting with the file systems.
- **Fragmentation and Defragmentation**: Supports fragmentation and defragmentation of both FAT and I-Node based file systems.
- **CD Burning**: Provides functionality to burn data to a CD.

## Getting Started

### Prerequisites

- **Qt Creator**: Make sure you have Qt Creator installed. You can download it from [Qt's official website](https://www.qt.io/download).
- **C++ Compiler**: Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).

### Setting Up the Project in Qt Creator

1. **Clone the Repository**:
    ```sh
    git clone https://github.com/ArthurRde/SimpleFileSystem.git
    cd SimpleFileSystem
    ```

2. **Open the Project**:
    - Launch Qt Creator.
    - Go to `File` > `Open File or Project...`.
    - Navigate to the `SimpleFileSystem` directory and open the `SimpleFileSystem.pro` file.

3. **Configure the Project**:
    - Qt Creator will prompt you to configure the project. Select the appropriate kit for your environment (e.g., Desktop Qt 5.15.2 GCC 64bit).
    - Click `Configure Project`.

4. **Build the Project**:
    - Click the `Build` button (hammer icon) or press `Ctrl+B` to build the project.

5. **Run the Project**:
    - Click the `Run` button (green play icon) or press `Ctrl+R` to run the project.

### Project Structure

- `mainwindow.ui`: Defines the main window layout and UI elements.
- `blockstatus.cpp`: Contains the implementation for block status.
- `disk.cpp`: Contains the implementation for disk operations.
- `README.md`: Project documentation.
- `blockstatus.h`: Header file for block status.
- `disk.h`: Header file for disk operations.
- `inode.cpp`: Contains the implementation for the I-Node file system.
- `inode.h`: Header file for the I-Node file system.
- `fatsystem.cpp`: Contains the implementation for the FAT file system.
- `fatsystem.h`: Header file for the FAT file system.
- `main.cpp`: Entry point of the application.
- `mainwindow.cpp`: Implementation of the main window.
- `mainwindow.h`: Header file for the main window.

## Usage

Once the project is running, you can interact with the file systems through the graphical interface. The interface provides options to create, read, write, and manage files within the FAT and I-Node based file systems.

## Contributing

If you wish to contribute to this project, please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.