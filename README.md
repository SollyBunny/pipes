# matrix
Pipes terminal screen saver made in C  
(Also see [matrix](https://github.com/SollyBunny/matrix"))

## Config
Config can be found in `config.h`  
`OPTTIME`: Time between frames  
`OPTCHANCE`: Chance for the pipe to change direction

## Compiling
Run `make`

## Installing
Run `make install`

## Usage
Run `pipes`

## Problems
At the start a few thousand ticks could be pre-computed to make it always look like it has been running  
There could be overlap detection creating + shapes where things cross  
