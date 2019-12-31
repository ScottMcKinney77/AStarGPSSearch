/* myGraphics.h
*	Header file I created to make easy implementation of rudimentary graphics
*	Not intended to be a sophisticated graphics implementation: just for most basically visualizing some algorithms
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

//Note: compile with: -std=c++11 -F/Library/Frameworks/SDL2.framework/Headers -framework SDL2
//for SDL2 Graphics framework to work

//If compiling (on a different computer) change path. Will not work unless SDL framework can be found

using namespace std;

class Color{
protected:
	int r, g, b;
public:
	Color(int r, int g, int b) : r(r), g(g), b(b){}
	Color(string c){ //Constructor which allows you to specify basic colors as strings instead of rgb
		if (c == "BLACK"){
			r = 0;
			g = 0;
			b = 0;
		}else if (c == "RED"){
			r = 255;
			g = 0;
			b = 0;
		}else if (c == "GREEN"){
			r = 0;
			g = 255;
			b = 0;
		}else if (c == "BLUE"){
			r = 0;
			g = 0;
			b = 255;
		}else if (c == "WHITE"){
			r = 255;
			g = 255;
			b = 255;
		}else{
			cout << "Desired color not found: set to white\n";
			r = 255;
			g = 255;
			b = 255;
		}
	}
	int getR(){
		return this->r;
	}
	int getG(){
		return this->g;
	}
	int getB(){
		return this->b;
	}
};

class GraphicsWindow {
protected:
	int width;
	int height;
	Color* backgroundColor;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

public:
	GraphicsWindow(int width, int height, Color* backgroundColor): backgroundColor(backgroundColor),height(height),width(width){
		//Attempt to initialize SDL framework
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			cout << "Error Intitializing SDL\n";
		}
		if(SDL_CreateWindowAndRenderer(width,height,0, &window, &renderer) != 0){
	        cout << "Creation error : " << SDL_GetError() << std::endl;
	        SDL_Quit();
	    }
	    SDL_SetRenderDrawColor(renderer, backgroundColor->getR(), backgroundColor->getG(), backgroundColor->getB(),SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
	}

	int createWindow(){
	    SDL_Event events;
	    bool quit(false);

	    while(!quit) {
	        SDL_WaitEvent(&events);
	        if(events.window.event == SDL_WINDOWEVENT_CLOSE){
	            quit = true;
	        }
	        SDL_RenderPresent(renderer);
	    }
	    if (renderer){
	    	SDL_DestroyRenderer(renderer);
	    }
	    if (window){
	    	SDL_DestroyWindow(window);
	    }
	    SDL_Quit();
	    return 0;
	}
	void drawLine(int x1, int y1, int x2, int y2, Color* color){
		y1 = height-y1;
		y2 = height-y2;
		SDL_SetRenderDrawColor(renderer, color->getR(), color->getG(), color->getB(), SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer,x1,y1,x2,y2);
	}
};