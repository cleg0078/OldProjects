#!/usr/make
SHELL := /usr/bin/bash

.PHONY: compile

compile:
	gcc -o mandelbrot mandelbrot.c -lpthread -lX11 -lm -O2
	gcc -o julia julia.c -lpthread -lX11 -lm -O2


