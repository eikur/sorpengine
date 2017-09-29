#pragma once

class Module
{
public:

	Module(bool active = true) : active(active)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return active;
	}

	bool Enable()
	{
		if (active == false)
			return active = Start();

		return true;
	}

	bool Disable()
	{
		if (active == true)
			return active = !CleanUp();

		return true;
	}

	virtual bool Init()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate()
	{
		return UpdateStatus::Continue;
	}

	virtual UpdateStatus Update()
	{
		return UpdateStatus::Continue;
	}

	virtual UpdateStatus PostUpdate()
	{
		return UpdateStatus::Continue;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadConfigFromFile(const char*)
	{
		return true;
	}

private:
	bool active = true;
};