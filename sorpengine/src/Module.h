#pragma once
#include "Globals.h"
#include <string>

class Module
{
	public:
		Module(const std::string& id, bool active = true) : _id(id),_active(active){}
		virtual ~Module() {}

		bool isActive() const;
		bool setActive(bool active);

		virtual bool init();
		virtual bool start();
		virtual UpdateStatus preUpdate();
		virtual UpdateStatus update( float dt = 0.0f);
		virtual UpdateStatus postUpdate();
		virtual bool cleanUp();

		const std::string& getId(); 

	private:
		bool _active = true;
		std::string _id;

};