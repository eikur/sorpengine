#pragma once
#include "Globals.hpp"
#include <string>
#include "glew-2.0.0\include\GL\glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class Module
{
	public:
		enum class Type
		{
			Audio,
			Input,
			Scene,
			Window
		};

		Module(Type type, bool active = true) : _type(type),_active(active){}
		virtual ~Module() {}

		bool isActive() const;
		bool setActive(bool active);

		virtual bool init();
		virtual bool start();
		virtual UpdateStatus preUpdate();
		virtual UpdateStatus update( float dt = 0.0f);
		virtual UpdateStatus postUpdate();
		virtual bool cleanUp();

		Module::Type getType() const;

	private:
		bool _active = true;
		Type _type;

};