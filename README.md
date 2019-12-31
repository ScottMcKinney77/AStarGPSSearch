__An independent project I did to implement a* search in C++__
* Reads intersection/road data from CSV files to create map
*	CSV files must be of the format:
 * i,intersection-name,intersection-latitude,intersection-longitude  //For intersections
 *	r,road-name,intersection-1,intersection-2   //For roads
* Will draw map with path highlighted in red from specified start intersection to specified goal intersections

* Files:
  *	AStarGPSSearch.cc 	//Contains all code specific to this project (Main function)
  *	ChainingStringHashMap.h 	//Helper header file I created with an implementation of string->template hash map which uses chaining to deal with collisions
  *	CSVRead.h 	//Helper header file I created to read from CSV files
  *	myGraphics.h 	//Helper header file I created with super rudimentary graphics implementation using SDL2 framework
  *	monroe.txt, nys.txt, ur.txt 	//Sample data sets
