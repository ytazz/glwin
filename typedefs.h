#pragma once

#include <sbtypes.h>
#include <sbxml.h>
#include <sbimagesvg.h>
using Scenebuilder::XML;
using Scenebuilder::XMLNode;
using Scenebuilder::SVG;

#include <SprGraphics.h>
using namespace Spr;

#include <SDL.h>

#define USE_FREEGLUT
#include <GL/glew.h>
#include <GL/glut.h>
#undef CreateDialog
#undef CreateWindow
#undef GetObject
#undef TextOut
