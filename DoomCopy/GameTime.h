#pragma once

class Time
{
	friend class Game;

public:
	static Time INS;

	Time() : startTime(), deltaTime(0.0), nextGameTick(0LL)
	{ }

	double GetDeltaTime() const { return deltaTime; }
	long long GetTime() const
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
	}

private:
	std::chrono::high_resolution_clock::time_point startTime;

	double deltaTime;
	long long nextGameTick;

	long long GetGameTickCount() const
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	}
};
