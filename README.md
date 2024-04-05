# pipes
Pipes terminal screen saver made in C  
(Also see [matrix](https://github.com/SollyBunny/matrix))

## Config
Config can be found in `config.h`  
`OPTTBOLD`: Whether the characters bold  
`OPTTIME`: Time between frames in us  
`OPTCHANCE`: Chance for the pipe to change direction  
`OPTERASERIT`: Iterations between each eraser tick (undefine to disable)  
`OPTPRESIM`: Number of iterations to presimulate  
You can also set the color using the `THEME_COLOR` environment variable

## Compiling
Run `make`

## Installing
Run `make install` to install  
Run `make uninstall` to uninstall

## Usage
Run `pipes`

## TODO
* There could be overlap detection creating + shapes where things cross  
* Command line args
