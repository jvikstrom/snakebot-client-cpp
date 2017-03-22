# Snakebot Client for C++
Do you find a bug or something that doesn't work as expected? Make an issue, or even better: fix it and make a pull request!

## Editing
First, change the name of your Snakebot in `src/snake.h`. The name
of your Snakebot must be unique!

## Game modes
In `src/main.cpp` in the following code snippet:
```C
static const std::string host = "snake.cygni.se";
static const std::string port = "80";
static const std::string venue = "training";
```
you may change the venue to either 'training' or 'tournament'.

## Coding
Edit the snake at `src/snake.cpp`, primarily the `get_next_move` function.

## Building
Ensure you have the necessary websocket package:
```
git submodule init            
git submodule update --recursive
```

Then make the project:
```
make
```

## Running
```
bin/runner
```

## Run the project via docker (if you have problems with compiling):
```
make clean
docker-compose run build
docker-compose up snakebot
```
