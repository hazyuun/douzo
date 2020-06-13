# douzo
![C/C++ CI](https://github.com/A-Rain-Lover/douzo/workflows/C/C++%20CI/badge.svg) <br />
A simple HTTP server written in C
## Compiling
```console
$ mkdir bin
$ gcc src/*.c -o bin/douzo
```
## Usage
```console
Usage: ./bin/main [-r] [-p port]
        -r : Print http requests
        -p : Use the specified port instead of default value (8080)
```
### Example
```console
$ ./bin/douzo -p 5600 
```
When the server is up, you can test it by visiting `http://localhost:5600/somepage.html` in your browser.
Currently the pages must be in the same directory, but it will be changed in the future.

## TODO
* [ ] Add the possibility to set custom folder
* [ ] Support the other methods (POST, PUT, etc..)
* [ ] Support the other HTTP headers
* [ ] Add a GUI

## Disclaimer
I didn't focus on security on this server, so it is NOT for general use !
