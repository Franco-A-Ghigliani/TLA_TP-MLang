[![✗](https://img.shields.io/badge/Release-v1.1.0-ffb600.svg?style=for-the-badge)](https://github.com/Franco-A-Ghigliani/TLA_TP-MLang/releases)

[![✗](https://github.com/agustin-golmar/Flex-Bison-Compiler/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/Franco-A-Ghigliani/TLA_TP-MLang/actions/workflows/pipeline.yaml)

# Flex-Bison-Compiler

A base compiler example, developed with Flex and Bison.

* [Environment](#environment)
* [CI/CD](#cicd)
* [Recommended Extensions](#recommended-extensions)
* Installation
  * [Docker](doc/readme/Docker.md)
  * [Microsoft Windows](doc/readme/Windows.md)
  * [Ubuntu](doc/readme/Ubuntu.md)

## Environment

Set the following environment variables to control and configure the behaviour of the application:

|Name|Default|Description|
|-|:-:|-|
|`LOG_IGNORED_LEXEMES`|`true`|When `true`, logs all of the ignored lexemes found with Flex at DEBUGGING level. To remove those logs from the console output set it to `false`.|
|`LOGGING_LEVEL`|`INFORMATION`|The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`.|

## Recommended Extensions

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)

## How to build

  ```bash 
      ./script/ubuntu/build.sh
  ```

## How to Execute

Once the program is compiled, you can run it as follows:

  ```bash 
    ./script/ubuntu/start.sh path_to_program_file
  ```

Where path_to_program_file is the path to the file you want to compile (a program written in your custom language).

### Importing the Resulting CSV to Google Sheets

1. After executing the program, it will generate a CSV file with the results.
2. To import this CSV file into Google Sheets, follow these steps:
  - Open Google Sheets.
  - Click on "File" > "Import".
  - Select the CSV file you want to upload.
3. When importing, **do not** select the option to automatically convert numbers to the "Number" format or dates. Keep the original formatting.

This will allow you to work with the raw CSV data as intended.

### Importing the Google Sheets to Machinations
1. Go to [Machinations](https://my.machinations.io/)
2. Click on "Menu" > "Import from Google Drive"
3. Select you Google Sheet


