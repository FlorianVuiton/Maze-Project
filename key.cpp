#include "key.h"

Key::Key(string keyName)
{    
    keyQuad_ = gluNewQuadric();
    QImage tex = QGLWidget::convertToGLFormat(QImage(QString(":/res/%1.jpg").arg(QString::fromStdString(keyName))));

    glGenTextures(1, &TextureID);

    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, 4, tex.width(), tex.height(), 0, GL_RGBA , GL_UNSIGNED_BYTE, tex.bits());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    gluQuadricTexture(keyQuad_, GL_TRUE);
    gluQuadricNormals(keyQuad_, GLU_SMOOTH);
}

Key::~Key() {
    gluDeleteQuadric(keyQuad_);
    glDeleteTextures(1, &TextureID);
}

void Key::setPosition(float posX, float posZ) {
    posX_ = posX;
    posZ_ = posZ;
}

void Key::display() {
    if (!isCaught_) {
        glPushMatrix();

        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.0f, 0.0f);
        gluQuadricDrawStyle(keyQuad_, GLU_FILL);
        glEnable(GL_LIGHTING);

        glTranslated(posX_, POS_Y, posZ_);
        glRotatef(-90, 1, 0, 0);

        GLfloat emissionColor[] = {0.1f, 0.1f, 0.1f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissionColor);

        GLfloat color[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, TextureID);
        gluSphere(keyQuad_, RADIUS, 32, 32);
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    //cout << "x " << posX_ <<" +++ y = " << posZ_ << endl;
}
