#pragma once
#include "Globals.h"
#include <string>

class Module
{
	public:
		Module(const std::string& id, bool active = true) : _id(id),_active(active){}
		virtual ~Module() {}

		bool IsActive() const;
		bool SetActive(bool active);

		virtual bool Init();
		virtual bool Start();
		virtual UpdateStatus PreUpdate();
		virtual UpdateStatus Update();
		virtual UpdateStatus PostUpdate();
		virtual bool CleanUp();

		const std::string& getId(); 

	private:
		bool _active = true;
		std::string _id;

};