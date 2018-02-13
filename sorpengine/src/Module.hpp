#pragma once
#include "Globals.hpp"
#include <string>
#include "GL/glew.h"
#include "SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Module
{
	public:
		Module(bool active = true) : _active(active){}
		virtual ~Module() {}

		bool isActive() const;
		bool setActive(bool active);

		virtual bool init();
		virtual bool start();
		virtual UpdateStatus preUpdate();
		virtual UpdateStatus update( float dt = 0.0f);
		virtual UpdateStatus postUpdate();
		virtual bool cleanUp();

	private:
		bool _active = true;
};