#include "glwidget.h"
#include "QToolTip"
#include "QPoint"

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const float MAX_DIMENSION     = 50.0f;

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent), wall_()
{
    // Reglage de la taille/position
//    setFixedSize(WIN_WIDTH, WIN_HEIGHT);
//    move(QApplication::desktop()->screen()->rect().center() - rect().center());


    // Connexion du timer
    connect(&m_AnimationTimer,  &QTimer::timeout, [&] {
        update();
    });

    m_AnimationTimer.setInterval(10);
    m_AnimationTimer.start();

    setAutoFillBackground(false);
}


// Fonction d'initialisation
void GLWidget::initializeGL()
{
    qglClearColor(Qt::black);
    glEnable(GL_DEPTH_TEST);

    keys.push_back(new Key("tse"));
    keys.at(0)->setPosition((3/2) + COEF_CELL * (rand()%(width_-1)), (3/2) + COEF_CELL * (rand()%(height_-1)));

    // Reglage de la lumiere
    GLfloat Light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, Light);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


// Fonction de redimensionnement
void GLWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    glViewport(0, 0, width, height);

    // Definition de la matrice de projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(70, WIN_WIDTH/(float)WIN_HEIGHT, 0.01, 40);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// Fonction d'affichage
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the maze
    displayMaze();

    // Draw the 2D map with the player
    maze2D->drawPlayer(player_);

    // Draw the keys
    for (int i = 0; i < keys.size(); i++) {
        keys.at(i)->display();
    }
}

void GLWidget::setMaze(Maze *maze) {
    maze_ = maze;
    width_ = maze_->getWidth();
    height_ = maze_->getHeight();
}

void GLWidget::displayMaze()
{
    player_->setCamera();

    float gray[4] = {0.8, 0.8, 0.8, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gray);

    glBegin(GL_QUADS);
        // Ground
        glNormal3f(0, 1, 0);
        glVertex3f(0, 0, 0); // Sol
        glVertex3f(0, 0, height_ * COEF_CELL);
        glVertex3f(width_ * COEF_CELL, 0, height_ * COEF_CELL);
        glVertex3f(width_ * COEF_CELL, 0, 0);

        // Ceiling
        glNormal3f(0, -1, 0);
        glVertex3f(0, WALL_HEIGHT, 0);
        glVertex3f(0, WALL_HEIGHT, height_ * COEF_CELL);
        glVertex3f(width_ * COEF_CELL, WALL_HEIGHT, height_ * COEF_CELL);
        glVertex3f(width_ * COEF_CELL, WALL_HEIGHT, 0);
    glEnd();


    float green[4] = {0, 0.8, 0, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);

    // Maze's walls
    for (int i=0; i<height_; i++) {
        for (int j=0; j<width_; j++) {
            if (maze_->getGrid(i, 0)->isFrontier(Cell::W)) {
                drawVerticalWall(QPoint(0, i), QPoint(0, i + 1));
            }
            if (maze_->getGrid(i, j)->isFrontier(Cell::E)) {
                drawVerticalWall(QPoint(j + 1, i), QPoint(j + 1, i + 1));
            }
            if (maze_->getGrid(i, j)->isFrontier(Cell::S)) {
                drawHorizontalWall(QPoint(j, i + 1), QPoint(j + 1, i + 1));
            }
            if (maze_->getGrid(0, j)->isFrontier(Cell::N)) {
                drawHorizontalWall(QPoint(j, 0), QPoint(j + 1, 0));
            }
        }
    }
    if (isChanged)
        isChanged = false;

    if (abs(player_->getPosition().x() - keys.at(0)->getPosX()) < 0.3 && abs(player_->getPosition().z() - keys.at(0)->getPosZ()) < 0.3) {
        if (!keys[0]->getIsCaught()) {
            removeWall();
        }
    }

    if (player_->getPosition().x() > maze_->getWidth() * COEF_CELL || player_->getPosition().x() < 0 ||
            player_->getPosition().z() > maze_->getHeight() * COEF_CELL || player_->getPosition().z() < 0) {
        emit gameEnd();
    }
}

void GLWidget::drawVerticalWall(QPoint a, QPoint b) {
    a = a * COEF_CELL;
    b = b * COEF_CELL;
    glBegin(GL_QUADS);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(a.rx() - WALL_DEPTH, 0, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() - WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
        glVertex3f(b.rx() - WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() - WALL_DEPTH, 0, b.ry() + WALL_DEPTH);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(b.rx() - WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() - WALL_DEPTH, 0, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() - WALL_DEPTH);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() - WALL_DEPTH, 0, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() - WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
    glEnd();

    if (isChanged) {
        wall_.push_back({a.rx() - WALL_DEPTH,b.rx() + WALL_DEPTH,a.ry() + WALL_DEPTH,b.ry() + WALL_DEPTH});
    }
}

void GLWidget::drawHorizontalWall(QPoint a, QPoint b){
    a = a * COEF_CELL;
    b = b * COEF_CELL;
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() - WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() - WALL_DEPTH);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() - WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() - WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(b.rx() + WALL_DEPTH, 0, b.ry() + WALL_DEPTH);
        glVertex3f(b.rx() + WALL_DEPTH, WALL_HEIGHT, b.ry() + WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() + WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() + WALL_DEPTH);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() + WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() + WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, 0, a.ry() - WALL_DEPTH);
        glVertex3f(a.rx() + WALL_DEPTH, WALL_HEIGHT, a.ry() - WALL_DEPTH);
    glEnd();

    if (isChanged) {
        wall_.push_back({a.rx() + WALL_DEPTH,b.rx() + WALL_DEPTH,a.ry() - WALL_DEPTH,b.ry() + WALL_DEPTH});
    }
}

void GLWidget::removeWall(){
    int choice = rand()%4;
    if (choice == 1)
        maze_->getGrid(0, rand()%width_)->setFrontier(Cell::N, false);
    if (choice == 2)
        maze_->getGrid(rand()%height_, width_-1)->setFrontier(Cell::E, false);
    if (choice == 3)
        maze_->getGrid(height_-1, rand()%width_)->setFrontier(Cell::S, false);
    if (choice == 4)
        maze_->getGrid(rand()%height_, 0)->setFrontier(Cell::W, false);
    keys.at(0)->setIsCaught(true);
    isChanged = true;
    wall_.clear();
}

void GLWidget::checkCollision(bool walk) {
    QVector3D position = player_->getPosition();
    for(int i=0;i<wall_.size();i++)
    {
        if(position.x()>wall_[i][0]&& position.x()<wall_[i][1] && position.z()>wall_[i][2] && position.z()<wall_[i][3])
        {
            if (walk) {
                player_->walk(-WALK);
            }
            else
                player_->walk(WALK);
            return;
        }
    }
}

// Fonction de gestion d'interactions clavier
void GLWidget::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
        // Activation/Arret de l'animation
        case Qt::Key_Enter:
        {
            if(m_AnimationTimer.isActive())
                m_AnimationTimer.stop();
            else
                m_AnimationTimer.start();

            break;
        }
        case Qt::Key_Up:
        {
            player_->walk(WALK);
            checkCollision(true);
            break;
        }
        case Qt::Key_Down: {
            player_->walk(-WALK);
            checkCollision(false);
            break;
        }
        case Qt::Key_Right:
        {
            player_->rotate(ROTATE);
            break;
        }
        case Qt::Key_Left: {
            player_->rotate(-ROTATE);
            break;
        }

        // Sortie de l'application
        case Qt::Key_Escape:
        {
            exit(0);
            break;
        }

        // Cas par defaut
        default:
        {
            // Ignorer l'evenement
            event->ignore();
            return;
        }
    }

    // Acceptation de l'evenement et mise a jour de la scene
    event->accept();
    updateGL();
}

// Fonction de gestion d'interactions clavier
void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key())
    {

        case Qt::Key_Down:
        {
            player_->setIsWalking(false);
            break;
        }
        case Qt::Key_Up:
        {
            player_->setIsWalking(false);
            break;
        }
        case Qt::Key_Left:
        {
            player_->setIsWalking(false);
            break;
        }
        case Qt::Key_Right:
        {
            player_->setIsWalking(false);
            break;
        }

        // Cas par defaut
        default:
        {
            // Ignorer l'evenement
            event->ignore();
            return;
        }
    }
    event->accept();
}
