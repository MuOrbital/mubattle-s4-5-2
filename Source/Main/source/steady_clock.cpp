#include "stdafx.h"
#include "SkillManager.h"
#include "ZzzOpenglUtil.h"
#include "steady_clock.h"
#include "wsclientinline.h"

static int ReadMainFpsLimit()
{
	char szCurrentDir[MAX_PATH] = { 0 };
	char szIniFilePath[MAX_PATH + 32] = { 0 };

	GetCurrentDirectoryA(MAX_PATH, szCurrentDir);
	strcpy_s(szIniFilePath, szCurrentDir);

	if (szCurrentDir[strlen(szCurrentDir) - 1] == '\\')
	{
		strcat_s(szIniFilePath, "Data\\Custom\\config.ini");
	}
	else
	{
		strcat_s(szIniFilePath, "\\Data\\Custom\\config.ini");
	}

	int fpsLimit = GetPrivateProfileIntA("FPSSystem", "FpsLimit", 120, szIniFilePath);

	if (fpsLimit <= 0)
	{
		return 120;
	}

	if (fpsLimit < 25)
	{
		return 25;
	}

	if (fpsLimit > 120)
	{
		return 120;
	}

	return fpsLimit;
}

csteady_clock::csteady_clock()
{
	timeBeginPeriod(1);

	fpsLimit = ReadMainFpsLimit();
	outsideFrameTime = 0.0;
	frameTimingInitialized = false;
	ping_reg_id = 0;
	counterframe = 0;
	deltaAccumulated = 0.0;
	fpsNormalizer = REFERENCE_FPS;

	speedNormalizer = 1.0;
	visualNormalizer = 1.0;
	normal_check = true;

	frame_limit = 1.f;

	mainthread = std::chrono::high_resolution_clock::now();
	last_check_time = std::chrono::high_resolution_clock::now();

	threadTime = new CTimer();
}

csteady_clock::~csteady_clock()
{
	timeEndPeriod(1);
	SAFE_DELETE(threadTime);
}

bool csteady_clock::CheckNormalizer()
{
#ifdef PROCESS_FPS_NEW
	if (tradicional)
	{
		return true;
	}
#endif // PROCESS_FPS_NEW

	return normal_check;
}

double csteady_clock::GetNormalizerFps()
{
	return speedNormalizer;
}

double csteady_clock::GetVisualNormalizerFps()
{
	return visualNormalizer;
}

double csteady_clock::GetDeltAccumulated()
{
	return deltaAccumulated;
}

int csteady_clock::GetLimitFps()
{
	return fpsLimit;
}

void csteady_clock::SetLimitFps(int newFpsLimit)
{
	if (newFpsLimit < 25)
	{
		newFpsLimit = 25;
	}
	else if (newFpsLimit > 120)
	{
		newFpsLimit = 120;
	}

	fpsLimit = newFpsLimit;
	outsideFrameTime = 0.0;
	frameTimingInitialized = false;
}

int csteady_clock::Getframe_per_second()
{
	const int fpsLimit = this->GetLimitFps();

	return (fpsLimit <= 0) ? 40 : (1000 / fpsLimit);
}

void csteady_clock::normalizefps()
{
	if (this->GetLimitFps() == (int)REFERENCE_FPS)
	{
		normal_check = true;
		return;
	}

	auto current_time = std::chrono::high_resolution_clock::now();

	double elapsed_time = std::chrono::duration<double>(current_time - last_check_time).count();

	if (elapsed_time >= (0.04))
	{
		normal_check = true;
		last_check_time = current_time;
	}
	else
	{
		normal_check = false;
	}
}

void csteady_clock::LoadInformationFps()
{
	static bool timeinit = false;

	if (!timeinit)
	{
		timeinit = true;
		counterframe = 0;
		frame_limit = 1.0;
		FPS = this->GetLimitFps();
		FPS_AVG = FPS;
		mainthread = std::chrono::high_resolution_clock::now();
		save_time = threadTime->GetTimeElapsed();
	}

	WorldTime = threadTime->GetTimeElapsed();
	auto current_time = WorldTime;
	double difTime = (current_time - save_time);

	DeltaT = (difTime <= 0.0) ? (1.0 / GetLimitFps()) : (difTime * 0.001);
	DeltaT = max(1.0 / 1000.0, min(DeltaT, 0.2));
	fpsNormalizer = 1.0 / DeltaT;
	visualNormalizer = max(0.025, min(DeltaT * REFERENCE_FPS, 2.0));
	speedNormalizer = min(visualNormalizer, 1.0);
	save_time = current_time;
	counterframe++;

	bool finishwating = frame_limit.hasElapsed();
	//
	if (finishwating)
	{
		FPS = counterframe > this->GetLimitFps() ? this->GetLimitFps() : counterframe;
		FPS_AVG = FPS;
		counterframe = 0;
	}

	if (SceneFlag == MAIN_SCENE)
	{
		if (finishwating)
		{
			runtime_send_ping();
		}
		gSkillManager.CalcSkillDelay(static_cast<int>(difTime));
	}

	deltaAccumulated += speedNormalizer;

	this->normalizefps();
}

std::chrono::steady_clock::time_point csteady_clock::GetthreadTime()
{
	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	if (frameTimingInitialized)
	{
		const double targetFrameTime = 1.0 / static_cast<double>(GetLimitFps());
		const double measuredOutsideTime = std::chrono::duration<double>(now - lastFrameEnd).count();

		if (measuredOutsideTime >= 0.0 && measuredOutsideTime < (targetFrameTime * 0.5))
		{
			outsideFrameTime = (outsideFrameTime * 0.75) + (measuredOutsideTime * 0.25);
		}
		else
		{
			outsideFrameTime = 0.0;
		}
	}

	mainthread = now;
	return mainthread;
}

uintmax_t csteady_clock::thread_sleep(const std::chrono::steady_clock::time_point thread_tick)
{
	const double targetFrameTime = 1.0 / static_cast<double>(GetLimitFps());
	const double compensatedFrameTime = max(targetFrameTime * 0.5, targetFrameTime - outsideFrameTime);
	const std::chrono::duration<double> targetFrame(compensatedFrameTime);
	const std::chrono::steady_clock::time_point deadline =
		thread_tick + std::chrono::duration_cast<std::chrono::steady_clock::duration>(targetFrame);
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	if (now < deadline)
	{
		const std::chrono::microseconds sleepMargin(750);
		while (now < deadline)
		{
			const std::chrono::steady_clock::duration remaining = deadline - now;
			if (remaining > sleepMargin)
			{
				std::this_thread::sleep_for(remaining - sleepMargin);
			}
			else
			{
				std::this_thread::yield();
			}

			now = std::chrono::steady_clock::now();
		}
	}

	mainthread = now;
	lastFrameEnd = now;
	frameTimingInitialized = true;
	return static_cast<uintmax_t>(
		std::chrono::duration_cast<std::chrono::milliseconds>(now - thread_tick).count());
}

bool csteady_clock::rand_calc_check(int fr)
{
	static std::random_device rd;  // a seed source for the random number engine
	static std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	static std::uniform_real_distribution<> distrib(0.0, 1.0);

	const auto rand_value = distrib(gen);
	const auto chance = (fr == 1) ? speedNormalizer : (1.0 / fr) * speedNormalizer;

	return rand_value <= chance;
}

void csteady_clock::runtime_send_ping()
{
	int ping_id = ping_reg_id++;

	SendPing(ping_id);

	pingMap[ping_id] = std::chrono::high_resolution_clock::now();
}

void csteady_clock::runtime_recv_ping(int ping_id)
{
	if (pingMap.find(ping_id) != pingMap.end())
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		auto startTime = pingMap[ping_id];
		pingMap.erase(ping_id);
		ping_time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	}
}

int csteady_clock::get_ping_time()
{
	return ping_time;
}

csteady_clock* csteady_clock::Instance()
{
	static csteady_clock sInstance;
	return &sInstance;
}

uint32_t standlimit(uint32_t x)
{
#ifdef PROCESS_FPS_NEW
	if (gsteady_clock->tradicional)
	{
		return x;
	}
#endif // PROCESS_FPS_NEW

	if (gsteady_clock->GetLimitFps() == (int)REFERENCE_FPS)
		return x;

	return (gsteady_clock->GetLimitFps() / REFERENCE_FPS * x);
}

double ReduceWithDelta(double x)
{
#ifdef PROCESS_FPS_NEW
	if (gsteady_clock->tradicional)
	{
		return x;
	}
#endif // PROCESS_FPS_NEW

	if (gsteady_clock->GetLimitFps() == (int)REFERENCE_FPS)
		return x;

	//return pow(x, gsteady_clock->GetNormalizerFps());
	return pow(x, DeltaT * REFERENCE_FPS);
}

double AdvanceByDelta(double x)
{
#ifdef PROCESS_FPS_NEW
	if (gsteady_clock->tradicional)
	{
		return x;
	}
#endif // PROCESS_FPS_NEW

	if (gsteady_clock->GetLimitFps() == (int)REFERENCE_FPS)
		return x;

	//return (x * gsteady_clock->GetNormalizerFps());
	return (x * DeltaT * REFERENCE_FPS);
}

double timeNormalizer(double x)
{
#ifdef PROCESS_FPS_NEW
	if (gsteady_clock->tradicional)
	{
		return x;
	}
#endif // PROCESS_FPS_NEW

	if (gsteady_clock->GetLimitFps() == (int)REFERENCE_FPS)
		return x;

	return (x * gsteady_clock->GetNormalizerFps());
}

bool steady_clock_::numeral(int element) const
{
	if (_runvalueback >= _runvalue)
		return (_runvalueback >= element && _runvalue <= element);
	else
		return (_runvalue >= element && _runvalueback <= element);
}

bool steady_clock_::duration(int element)
{
	if (_runvalueback >= _runvalue)
	{
		return _runvalue <= (std::floor((_runvalueback / element)) * element);
	}
	else
	{
		return _runvalue >= (std::ceil((_runvalueback / element)) * element);
	}
}

bool steady_clock_::residual_duration(int element, int time)
{
	int multiplo;
	double residual_memory1, residual_memory2;

	if (_runvalueback >= _runvalue)
	{
		multiplo = (std::floor((_runvalueback / element)) * element);

		residual_memory1 = (_runvalue - multiplo);
		residual_memory2 = (_runvalueback - multiplo);

		return (residual_memory2 >= time && residual_memory1 <= time);
	}
	else
	{
		multiplo = (std::floor((_runvalue / element)) * element);

		residual_memory1 = (_runvalue - multiplo);
		residual_memory2 = (_runvalueback - multiplo);

		return (residual_memory2 <= time && residual_memory1 >= time);
	}
}

int steady_clock_::factor_res(int time)
{
	int rounded_back = 0;
	int rounded_value = 0;

	if (_runvalueback >= _runvalue)
	{
		rounded_back = static_cast<int>(std::floor(_runvalueback));
		rounded_value = static_cast<int>(std::ceil(_runvalue));
	}
	else
	{
		rounded_back = static_cast<int>(std::ceil(_runvalueback));
		rounded_value = static_cast<int>(std::floor(_runvalue));
	}

	if (rounded_back == rounded_value)
	{
		return (rounded_value % time);
	}
	return -1;
}
