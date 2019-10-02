/*

*/

#include <iostream>
#include "freeglut.h"
#define ssize_t SSIZE_T
#include "vlc/vlc.h"

GLuint gAppTexture = 0;
GLuint gTextureWidth = 0;
GLuint gTextureHeight = 0;
const GLuint gTextureDepth = 4;
GLuint gAppWindowWidth = 800;
GLuint gAppWindowHeight = 600;
unsigned char* gVideoBuffer = 0;
std::string gMediaURL = "";
libvlc_instance_t* gLibVLC = 0;
libvlc_media_t* gLibVLCMedia = 0;
libvlc_media_player_t* gLibVLCMediaPlayer = 0;
struct gVLCContext
{
    unsigned char* texture_pixels;
    libvlc_media_player_t* mp;
};
struct gVLCContext gVLCCallbackContext;

bool initVLC()
{
    char const* vlc_argv[] =
    {
        "--no-xlib"
        "",
    };

    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    gLibVLC = libvlc_new(vlc_argc, vlc_argv);
    if (!gLibVLC)
    {
        std::cout << "ERROR: Unable to initialize VLC" << std::endl;
        return false;
    }

    std::cout << "VLC initialized" << std::endl;
    return true;
}

void resetVLC()
{
    std::cout << "Closing down VLC ..." << std::endl;

    libvlc_media_player_stop(gLibVLCMediaPlayer);
    libvlc_media_player_release(gLibVLCMediaPlayer);
    libvlc_media_release(gLibVLCMedia);
    libvlc_release(gLibVLC);
    std::cout << "... finished closing down VLC" << std::endl;
}

static void* lock(void* data, void** p_pixels)
{
    struct gVLCContext* context = (gVLCContext*)data;

    *p_pixels = context->texture_pixels;

    return NULL;
}

static void unlock(void* data, void* id, void* const* raw_pixels)
{
}

static void display(void* data, void* id)
{
}

bool playMedia(const std::string url)
{
    if (gLibVLCMediaPlayer)
    {
        libvlc_media_player_stop(gLibVLCMediaPlayer);
        libvlc_media_player_release(gLibVLCMediaPlayer);
    }

    gLibVLCMedia = libvlc_media_new_location(gLibVLC, url.c_str());
    if (!gLibVLCMedia)
    {
        std::cout << "ERROR: libvlc_media_new_location failed" << std::endl;
        return false;
    }

    gLibVLCMediaPlayer = libvlc_media_player_new_from_media(gLibVLCMedia);
    if (!gLibVLCMediaPlayer)
    {
        std::cout << "ERROR: libvlc_media_player_new_from_media failed" << std::endl;
        return false;
    }

    gVLCCallbackContext.texture_pixels = gVideoBuffer;
    gVLCCallbackContext.mp = gLibVLCMediaPlayer;

    libvlc_video_set_callbacks(gLibVLCMediaPlayer, lock, unlock, display, &gVLCCallbackContext);
    libvlc_video_set_format(gLibVLCMediaPlayer, "RV32", gTextureWidth, gTextureHeight, gTextureWidth * gTextureDepth);

    libvlc_media_add_option(gLibVLCMedia, "input-repeat=10000");

    libvlc_media_player_play(gLibVLCMediaPlayer);

    return true;
}

void glutResize(int width, int height)
{
    gAppWindowWidth = width;
    gAppWindowHeight = height;

    gTextureWidth = width;
    gTextureHeight = height;

    free(gVideoBuffer);
    gVideoBuffer = new unsigned char[gTextureWidth * gTextureHeight * gTextureDepth];

    if (gLibVLCMediaPlayer)
    {
        gVLCCallbackContext.texture_pixels = gVideoBuffer;
        gVLCCallbackContext.mp = gLibVLCMediaPlayer;

        libvlc_video_set_callbacks(gLibVLCMediaPlayer, lock, unlock, display, &gVLCCallbackContext);
        libvlc_video_set_format(gLibVLCMediaPlayer, "RV32", gTextureWidth, gTextureHeight, gTextureWidth * gTextureDepth);
    }

    glViewport(0, 0, width, height);
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);

    gTextureWidth = width;
    gTextureHeight = height;

    glDeleteTextures(1, &gAppTexture);
    glGenTextures(1, &gAppTexture);
    glBindTexture(GL_TEXTURE_2D, gAppTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gTextureWidth, gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}

void glutDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2d(gAppWindowWidth, 0);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2d(0, 0);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2d(0, gAppWindowHeight);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2d(gAppWindowWidth, gAppWindowHeight);
    glEnd();

    glutSwapBuffers();
}

void glutIdle()
{
    if (gVideoBuffer != 0 && gTextureWidth != 0 && gTextureHeight != 0)
    {
        if (gLibVLCMedia == 0)
        {
            playMedia(gMediaURL);
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        0, 0,
                        gTextureWidth, gTextureHeight,
                        GL_BGRA_EXT,
                        GL_UNSIGNED_BYTE,
                        gVideoBuffer);

        glutPostRedisplay();
    }
    else
    {
        std::cout << "waiting for buffer to be ready..." << std::endl;
    }
}

void glutKeyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        glutLeaveMainLoop();
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(80, 0);
    glutInitWindowSize(gAppWindowWidth, gAppWindowHeight);

    glutCreateWindow("LibVLC-GLUT");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutDisplayFunc(glutDisplay);
    glutIdleFunc(glutIdle);
    glutKeyboardFunc(glutKeyboard);
    glutReshapeFunc(glutResize);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &gAppTexture);
    glBindTexture(GL_TEXTURE_2D, gAppTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gTextureWidth, gTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glViewport(0, 0, gTextureWidth, gTextureHeight);
    glOrtho(0.0f, gTextureWidth, gTextureHeight, 0.0f, -1.0f, 1.0f);

    if (initVLC())
    {
        gMediaURL = "http://jell.yfish.us/media/jellyfish-10-mbps-hd-h264.mkv";

        glutMainLoop();
    }

    resetVLC();
}
