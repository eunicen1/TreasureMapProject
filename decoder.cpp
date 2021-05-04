#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

// initializes variables and build solution path as a vector
// of points that corresponds to a sequence of pixels
// beginning at pathPts[0] == start, and ending with
// pathPts[pathPts.size() - 1] == end.
//
// You will want to modify the BFS algorithm you wrote for the
// treasureMap renderer in order to solve this problem.
//
// As a hint, you will want an additional working vector whose purpose
// is to encode, for every location, the location from which it
// was first discovered. This working structure will help you
// determine the path from any location to the start (and thus
// from the start to any location).
decoder::decoder(const PNG &tm, pair<int, int> s)
    : start(s), mapImg(tm)
{
   /* YOUR CODE HERE */
   //find location of treasure; farthest from start point
   //treasure is located at the last point you added to the queue last as you did level order traversal
   //need 2d parent vector; keeps track of parent of every neighbour (ie. curr)
   // copy code from treasure map and update it
   //when you reach the end, go backwards use the parent vector (while parent isn't empty)
   //as you travel backwards put it into a stack (to be popped out when in the path vector
   // PNG basecopy = base;
   int nrows = mapImg.height();
   int ncols = mapImg.width();
   vector<vector<pair<int, int>>> parent(ncols, vector<pair<int, int>>(nrows));
   vector<vector<bool>> visited(ncols, vector<bool>(nrows, false));
   vector<vector<int>> path(ncols, vector<int>(nrows));
   Queue<pair<int, int>> todo;
   visited[start.first][start.second] = true;
   todo.enqueue(start);
   pair<int, int> treasure_loc;

   while (!todo.isEmpty())
   {
      pair<int, int> curr = todo.dequeue();
      treasure_loc = curr;
      vector<pair<int, int>> nbr = neighbors(curr);
      for (int i = 0; i < (int)nbr.size(); i++)
      {
         pair<int, int> a_neighbr = nbr[i];
         if (good(visited, path, curr, a_neighbr))
         {
            parent[a_neighbr.first][a_neighbr.second] = curr;
            visited[a_neighbr.first][a_neighbr.second] = true;
            path[a_neighbr.first][a_neighbr.second] = path[curr.first][curr.second] + 1;
            todo.enqueue(a_neighbr);
         }
      }
   }

   Stack<pair<int, int>> temp_s;
   temp_s.push(treasure_loc);

   for (pair<int, int> curr = treasure_loc; curr != start; curr = parent[curr.first][curr.second])
   {
      temp_s.push(parent[curr.first][curr.second]);
   }

   while (!temp_s.isEmpty())
   {
      pathPts.push_back(temp_s.pop());
   }

   //    you don't initialize parent - done
   // and you shouldn't pushback start in pathPts - done
   // you deal with all pathPts stuff after the while loop - done
   // and you're not using parent properly
   // you're adding literally every single point in the image into pathPts

   // you should find the treasureSpot after the while loop ends
   // and then use parent to go from treasureSpot back to the start
   // adding each point into the stack in the process
}

// draws the solution path on top of the given image. The solution
// is the shortest path to the point which is farthest from the
// start in the embedded maze. Solution path is drawn in (255,0,0).
// This function assumes that the pathPts member variable has been
// populated by the constructor.
PNG decoder::renderSolution()
{
   PNG mapImg_copy = mapImg;
   int i = 0;
   while (i < (int)pathPts.size())
   {
      // cout << "line 83: PNG decoder::renderSolution()\n";
      // cout << "line 83: size of pathPts" << pathPts.size();

      if (within_image(pathPts[i]))
      {
         mapImg_copy.getPixel(pathPts[i].first, pathPts[i].second)->r = 255;
         mapImg_copy.getPixel(pathPts[i].first, pathPts[i].second)->g = 0;
         mapImg_copy.getPixel(pathPts[i].first, pathPts[i].second)->b = 0;
         i++;
      }
      // cout << "line 92 PNG decoder::renderSolution()";
   }

   return mapImg_copy;
}

// discovers and draws the entire maze on top of the given image.
// The maze is revealed by darkening each of the maze pixels: change
// each colour channel to be 50% of its treasure map value. (use the
// setGrey function from the treasureMap class.)
//
// Discovering the maze requires yet another breadth-first traversal
// of the treasureMap image. (you may be able to facilitate this
// within the constructor by adding another member variable to the
// decoder class. We leave that design decision up to you.)
//
// Note that the grey pixels in the maze rendered by this function
// may be a superset of the pixels in the original maze, because
// some neighboring pixels might have the "correct" lower
// order bits, just by happenstance.
//
// This function also draws a red 7px x 7px square at the start
// location. If the start location is near the edge of the image,
// just draw the existing portion of the square. The start
// should be at the center of the square.

PNG decoder::renderMaze()
{
   PNG mapImg_copy = mapImg;
   int nrows = mapImg_copy.height();
   int ncols = mapImg_copy.width();
   vector<vector<bool>> visited(ncols, vector<bool>(nrows, false));
   vector<vector<int>> path(ncols, vector<int>(nrows, -1));
   Queue<pair<int, int>> todo;
   visited[start.first][start.second] = true;
   setGrey(mapImg_copy, start);
   todo.enqueue(start);
   path[start.first][start.second] = 0;

   while (!todo.isEmpty())
   {
      pair<int, int> curr = todo.dequeue();
      vector<pair<int, int>> nbr = neighbors(curr);
      for (int i = 0; i < (int)nbr.size(); i++)
      {
         pair<int, int> a_neighbr = nbr[i];
         if (good(visited, path, curr, a_neighbr))
         {
            visited[a_neighbr.first][a_neighbr.second] = true;
            setGrey(mapImg_copy, a_neighbr);
            path[a_neighbr.first][a_neighbr.second] = path[curr.first][curr.second] + 1;
            todo.enqueue(a_neighbr);
         }
      }
   }

   // draws a red square
   pair<int, int> temp = make_pair(start.first - 3, start.second - 3); //move start to top left of red square

   for (int x = temp.first; (x < temp.first + 7); x++)
   {
      for (int y = temp.second; (y < temp.second + 7); y++)
      {
         if (within_image(make_pair(x, y)))
         {
            (*mapImg_copy.getPixel(x, y)).r = 255;
            (*mapImg_copy.getPixel(x, y)).b = 0; //remove the blue and green at the pixel locations
            (*mapImg_copy.getPixel(x, y)).g = 0;
         }
      }
   }
   return mapImg_copy;
}

// changes the pixel at position loc to consist of colour channel
// values which are 50% of their original. Note that because of
// our encoding scheme which essentially invalidates the lower
// order bits, you should compute each value as 2*(x/4), where
// x is the value of a colour channel.
void decoder::setGrey(PNG &im, pair<int, int> loc)
{
   if (within_image(make_pair(loc.first, loc.second)))
   {
      RGBAPixel *pix = im.getPixel(loc.first, loc.second);
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
}

// returns the location of the treasure
pair<int, int> decoder::findSpot()
{
   return pathPts.back(); // the last thing added on to the vector
}

// returns the length of the path to the treasure, in pixels, using
// manhattan distance. (no diagonal steps)
int decoder::pathLength()
{
   return pathPts.size();
}

// tests a neighbor (adjacent vertex) to see if it is
// 1. within the image, 2. unvisited, and 3. coloured so that
// lower order bits fit the pattern prescribed by the encoder.
// An entry in table v is true if a cell has previously been
// visited. table d contains the shortest distance from each location
// back to the start. the (r,g,b) colour of position next must have lower order
// bits (XX,XX,XX) that, when interpreted as an integer between 0 and 63,
// inclusive, is d[curr.second][curr.first] + 1 (mod 64).
bool decoder::good(vector<vector<bool>> &v, vector<vector<int>> &d, pair<int, int> curr, pair<int, int> next)
{
   //needs to call compare
   bool is_within_image = within_image(curr) && within_image(next);
   bool visited = false;
   bool coloured = false;
   if (is_within_image)
   {
      visited = v[next.first][next.second];
      coloured = compare(*(mapImg.getPixel(next.first, next.second)), d[curr.first][curr.second]);
   }
   return is_within_image && !visited && coloured;
}

//checks if a point is within the image
bool decoder::within_image(pair<int, int> point)
{
   return point.first >= 0 && point.first < (int)mapImg.width() && point.second >= 0 && point.second < (int)mapImg.height();
}

// builds a vector containing the locations of the
// four vertices adjacent to curr:
// left, below, right, above.
// does not pay any attention to whether or not the neighbors are
// valid (in the image, previously visited, or the right colour).
vector<pair<int, int>> decoder::neighbors(pair<int, int> curr)
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

// tests whether p's lower order bits, when interpreted as a
// number, is d+1 (mod 64).
bool decoder::compare(RGBAPixel p, int d)
{
   int maze_val_r = (int)(p.r % 4); //mod 4 to get last 2 bits
   int maze_val_g = (int)(p.g % 4);
   int maze_val_b = (int)(p.b % 4);

   int maze_val_r_6bits = maze_val_r * 16; //multiply by 16 create 4 zeros at the right
   int maze_val_g_4bits = maze_val_g * 4;  //multiply by 4 create 2 zeros at the right
   int pix_LOB = maze_val_r_6bits + maze_val_g_4bits + maze_val_b;
   return pix_LOB == (d + 1) % 64;
}
