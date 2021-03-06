#ifndef MAZE_H
/** An implementation of Prim's algorithm for generating mazes.
 * from <http://weblog.jamisbuck.org/2011/1/10/maze-generation-prim-s-algorithm>
 *
 * C++ implementation by C. Ducottet
**/

#define MAZE_H

#include "cell.h"
#include <vector>
#include <list>
#include <utility>

#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QGLWidget>
#include <iostream>

using namespace std;

using Point=pair<int,int>;

class Maze
{
    vector<vector<Cell>> grid_;

    int width_;
    int height_;

    void addFrontier(Point p,list<Point> & frontier);
    void mark(Point p,list<Point> & frontier);
    list<Point> neighbors(Point p);
    Cell::Direction direction(Point f, Point t);

public:
    Maze(int width,int height);
    void reinit();
    void generate();
    Cell* getGrid(int i, int j) {return &grid_[i][j];};
    int getWidth() {return width_;};
    int getHeight() { return height_;};

};

#endif // MAZE_H

