# ZIP Viewer

ZIP Viewer is a Qt-based application that allows you to view the contents of a ZIP file in a user-friendly interface. It displays the files and directories within the ZIP archive, along with their full paths, uncompressed sizes, and compressed sizes.

## Features

- Open and view the contents of a ZIP file
- Display file information in a tree view with three columns: Full Path, Uncompressed Size, and Compressed Size
- Extract selected files from the ZIP archive to a specified output directory
- User-friendly interface with intuitive controls

## Prerequisites

Before building the ZIP Viewer application, ensure that you have the following dependencies installed:

- Qt6 (version 6.0 or later)
- CMake (version 3.10 or later)
- ZLIB library
- BZip2 library
- minizip-ng library

## Building the Application

To build the ZIP Viewer application, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/54m47/zip_viewer.git
   ```

2. Navigate to the project directory:
   ```
   cd zip_viewer
   ```

3. Create a build directory and navigate to it:
   ```
   mkdir build
   cd build
   ```

4. Generate the build files using CMake:
   ```
   cmake ..
   ```

5. Build the application:
   ```
   cmake --build .
   ```

6. Run the application:
   ```
   ./bin/zip_viewer
   ```

## Usage

1. Launch the ZIP Viewer application.

2. Click on the "Open" button to select a ZIP file from your file system.

3. The application will display the contents of the ZIP file in a tree view, showing the full path, uncompressed size, and compressed size for each file.

4. To extract a file from the ZIP archive, select the de desired file in the tree view and click on the "Extract" button.

5. Choose the output directory where you want to save the extracted file.

6. The selected file will be extracted to the specified output directory, and a success message will be displayed.

## Contributing

Contributions to the ZIP Viewer project are welcome! If you encounter any issues or have suggestions for improvements, please open an issue or submit a pull request on the GitHub repository.

## Acknowledgments

- The ZIP Viewer application utilizes the [minizip-ng](https://github.com/zlib-ng/minizip-ng) library for ZIP file manipulation.
- Special thanks to the contributors of the Qt framework and the open-source community for their valuable resources and support.
