#include "treasureMap.h"
#include "queue.h"
#include<iostream>
#include<cmath>
using namespace std;

treasureMap::treasureMap(const PNG &baseim, const PNG &mazeim, pair<int, int> s)
{   /* YOUR CODE HERE */
    start = s;
    base = baseim;
    maze = mazeim;
}

void treasureMap::setGrey(PNG &im, pair<int, int> loc)
{
    /* YOUR CODE HERE */
    RGBAPixel * pix = im.getPixel(loc.first, loc.second);
    int xr = pix->r;
    int xg = pix->g;
    int xb = pix->b;
    int red = 2 * (xr / 4);
    int gre = 2 * (xg / 4);
    int blu = 2 * (xb / 4);
    pix->r = red;
    pix->g = gre;
    pix->b = blu;
}

// This function takes an image, a location, and an integer value.
// Its effect is to change the colour channels in position loc of the image
// so that they encode the maze-value d. Here's how it's done: Consider
// the value of d%64 to be a 6 bit integer and the pixel in location
// loc to be (r,g,b). The two most significant
// bits of d%64 will be used to set the value of the red channel by
// changing r. The two least significant bits of d%64 will be used
// to set the value of the blue channel by changing b. And the remaining
// two bits of d%64 will be used to set the value of the green channel
// by changing g.
//
// Now consider the value of any colour channel, x, represented as an
// 8 bit binary number. To encode d, simply replace the 2 lower order
// bits in x with the corresponding bits from d, as described above.
void treasureMap::setLOB(PNG &im, pair<int, int> loc, int d)
{
    /* YOUR CODE HERE */
    RGBAPixel colour_channel = *im.getPixel(loc.first, loc.second);
    int maze_val = d % 64;                                          // -- red, -- green, -- blue
    int maze_val_get_red = maze_val / 16;                               // first 2 bits
    int maze_val_get_green = (maze_val / 4) % 4;                        // middle 2 bits; "/" removes first 2 bits, mod return mid 2 bits
    int maze_val_get_blue = maze_val % 4;                               // last 2 bits
    colour_channel.r = (int) ((colour_channel.r / 4) * 4 + maze_val_get_red); // divide to remove last 2 bits, * to add 2 bits as zeros, + the corresponding colours.
    colour_channel.g = (int) ((colour_channel.g / 4) * 4 + maze_val_get_green);
    colour_channel.b = (int) ((colour_channel.b / 4) * 4 + maze_val_get_blue);
    *im.getPixel(loc.first, loc.second) = colour_channel;
}

PNG treasureMap::renderMap()
{
    /* YOUR CODE HERE */
    PNG basecopy(base);
    int nrows = basecopy.height();
    int ncols = basecopy.width();
    vector<vector<bool>> visited(ncols, vector<bool>(nrows, false));
    vector<vector<int>> path(ncols, vector<int>(nrows, 0));

    Queue<pair<int, int>> todo;
    visited[start.first][start.second] = true;
    path[start.first][start.second] = 0;
    setLOB(basecopy, start, 0);
    todo.enqueue(start);
    while (!todo.isEmpty())
    { 
        pair<int, int> curr = todo.dequeue();
        vector<pair<int, int>> nbr = neighbors(curr);

        for (int i = 0; i < (int) nbr.size(); i++)
        {
            pair<int, int> a_neighbr = nbr[i];
            // cout<< a_neighbr.first<<","<< a_neighbr.second<<endl;
            if (good(visited, curr, a_neighbr))
            {
                visited[a_neighbr.first][a_neighbr.second] = true;
                path[a_neighbr.first][a_neighbr.second] = path[curr.first][curr.second] + 1;
                setLOB(basecopy, a_neighbr, path[a_neighbr.first][a_neighbr.second]);
                todo.enqueue(a_neighbr);
            }
        }
    }
    return basecopy;
}

//draws the entire maze on a copy of the base image by darkening the locations
// so it's visible as a shadow. This can be done by iterating over every
// pixel, but it will be faster to begin from the start position
// and find the points in the maze from there. Returns an image whose
// non-maze pixels are the same as the base image, and whose maze pixels
// have colour values that are 50% of those in the base image, for each of
// red, green and blue. (We specify the computation of the colours in the
// description of a setGrey function, below.)
//
// This function also draws a red 7px x 7px square at the start
// location. If the start location is near the edge of the image,
// just draw the existing portion of the square. The start
// should be at the center of the square.
PNG treasureMap::renderMaze()
{
     // This function also draws a red 7px x 7px square at the start                                                                                    
    // location. If the start location is near the edge of the image,
    // just draw the existing portion of the square. The start 
    // should be at the center of the square.
    PNG basecopy(base); 
    int nrows = basecopy.height();
    int ncols = basecopy.width();
    cout<<nrows<<ncols;
    vector<vector<bool>> visited(ncols, vector<bool>(nrows, false));
    Queue<pair<int, int>> todo;
    visited[start.first][start.second] = true;
    setGrey(basecopy, start);
    todo.enqueue(start);
    while (!todo.isEmpty())
    { 
        pair<int, int> curr = todo.dequeue();
        vector<pair<int, int>> nbr = neighbors(curr);

        for (int i = 0; i < (int) nbr.size(); i++)
        {
            pair<int, int> a_neighbr = nbr[i];
            if (good(visited, curr, a_neighbr))
            {
                visited[a_neighbr.first][a_neighbr.second] = true;
                setGrey(basecopy, a_neighbr);
                todo.enqueue(a_neighbr);
            }
        }
    } 

    for(int a = start.first-3; a <= start.first+3; a++){
        for(int b = start.second-3; b <= start.second+3; b++){
            pair<int,int> toCheck = make_pair(a,b);
            if(within_image(toCheck, maze)){
                (basecopy.getPixel(a, b))->r = 255;
                (basecopy.getPixel(a, b))->b = 0;
                (basecopy.getPixel(a, b))->g = 0; 
            }
        }
    }
    return basecopy;
}

// tests next, a neighbor (adjacent vertex) of curr to see if it is
// 1. within the image, 2. unvisited, and 3. the same colour as curr in
// the maze image (member var).  An entry in table v is true if a cell
// has previously been visited, and false, otherwise. Colour sameness is
// measured using the == operator on RGBAPixel values.

bool treasureMap::good(vector<vector<bool>> &v, pair<int, int> curr, pair<int, int> next)
{
    /* YOUR CODE HERE */
    bool within = within_image(next, base);
    if (within) {
        RGBAPixel *n = maze.getPixel(next.first, next.second);
        RGBAPixel *c = maze.getPixel(curr.first, curr.second);
        bool same_color = ((n->r == c->r) && (n->g == c->g) && (n->b == c->b));
        bool visited = !(v[next.first][next.second] == true);
        if (visited && same_color){
                return true;
        }
        return false;
    }
    return false;
}

bool treasureMap::within_image(pair<int,int> point, PNG &im){
    return (point.first < (int) im.width()) && (point.second < (int) im.height()) && (point.first >= 0) && (point.second >= 0);
}

// builds a vector containing the locations of the
// four vertices adjacent to curr:
// left, below, right, above.
// does not pay any attention to whether or not the neighbors are
// valid (in the image, previously visited, or the right colour).
vector<pair<int, int>> treasureMap::neighbors(pair<int, int> curr)
{
    vector<pair<int, int>> v;
    pair<int, int> left = make_pair(curr.first - 1, curr.second);
    pair<int, int> above = make_pair(curr.first, curr.second - 1);
    pair<int, int> right = make_pair(curr.first + 1, curr.second);
    pair<int, int> below = make_pair(curr.first, curr.second + 1);
    v.push_back(left);
    v.push_back(below);
    v.push_back(right);
    v.push_back(above);
    return v;
}