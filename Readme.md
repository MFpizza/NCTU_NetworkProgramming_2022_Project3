**如果這個程式對你有幫助，希望你能夠幫我按個star**
## How to compile & exec
### compile
```
make part1 # for linux
make part2 # for windows
```
### clear executable
```
make clean # for linux
```
### exec
>[exe] [port]
```
./http_server 1234 # for linux
./cgi_server 1234 # for windows
```

## two different os http server

both of server using boost asio 1.77.0 for
    - running cgi
    - accept http request
    - connect to np_single_golden on nplinux server

### part1
    - for linux
    - have using fork exec and dup2
    - console.cpp would compile to execfile console.cgi
    - panel.cgi is generated from python


### part2
    - for windows
    - Combined http_server, panel.cgi, console.cgi to one exec file
    - doesn't have fork exec dup2
    - one bugs
      - can't be interrupted by ctrl-c with using async_accept