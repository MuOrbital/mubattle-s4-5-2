#pragma once
#ifndef MU_REFERENCE_FPS_DEFINED
#define MU_REFERENCE_FPS_DEFINED
constexpr double REFERENCE_FPS = 25.0;
#endif
#define FPS_ANIMATION_FACTOR				(gsteady_clock->GetNormalizerFps())
#define FPS_VISUAL_FACTOR					(gsteady_clock->GetVisualNormalizerFps())
#define checkNormalizer						(gsteady_clock->CheckNormalizer())
#define rand_fps_check(reference)			(gsteady_clock->rand_calc_check(reference))
#define gsteady_clock						(csteady_clock::Instance())



class CheckerTime
{
public:
	CheckerTime(float intervalInSeconds){
		intervalSet(intervalInSeconds);
	}
	CheckerTime() {
		intervalSet(0.0f);
	}

	CheckerTime& operator=(float _interval) {

		intervalSet(_interval);
		return *this;
	}

	void intervalSet(float intervalInSeconds) {
		if (intervalInSeconds == 0.0f) {
			initialize = false;
		}
		else {
			initialize = true;
			interval = std::chrono::duration<float>(intervalInSeconds);
			lastCheckTime = std::chrono::steady_clock::now();
		}
	}

	// Retorna true si ha pasado el intervalo especificado.
	bool now() {
		return this->hasElapsed();
	}
	float work_duration() {
		if (!initialize) return 0.0f;
		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration<float>(currentTime - lastCheckTime).count();
		return min((elapsedTime / interval.count()), 1.0f);
	}

	bool hasElapsed() {
		if (initialize)
		{
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = currentTime - lastCheckTime;

			if (elapsedTime >= interval) {
				lastCheckTime = currentTime; // Reinicia el tiempo
				return true;
			}
		}
		return false;
	}
	bool CheckElapsed() {
		if (initialize)
		{
			auto currentTime = std::chrono::steady_clock::now();
			auto elapsedTime = currentTime - lastCheckTime;

			return (elapsedTime >= interval);
		}
		return false;
	}

private:
	bool initialize;
	std::chrono::duration<float> interval;
	std::chrono::time_point<std::chrono::steady_clock> lastCheckTime;
};

class steady_clock_
{
public:
	double _runvalue;
	double _runvalueback;

	// Constructor
	steady_clock_() : _runvalue(0.0), _runvalueback(0.0) {}
	steady_clock_(double value) : _runvalue(value), _runvalueback(value) {}

	steady_clock_& operator=(double value) {
		_runvalueback = value;
		_runvalue = value;
		return *this;
	}

	operator double() const
	{
		return _runvalue;
	}

	explicit operator float() const {
		return static_cast<float>(_runvalue);
	}

	explicit operator int() const {
		return static_cast<int>(_runvalue);
	}


	// Operadores aritméticos
	template<typename T>
	steady_clock_ operator/(T val) const {
		return steady_clock_(_runvalue / static_cast<double>(val));
	}

	template<typename T>
	steady_clock_ operator*(T val) const {
		return steady_clock_(_runvalue * static_cast<double>(val));
	}

	template<typename T>
	steady_clock_ operator+(T val) const {
		return steady_clock_(_runvalue + static_cast<double>(val));
	}

	template<typename T>
	steady_clock_ operator-(T val) const {
		return steady_clock_(_runvalue - static_cast<double>(val));
	}

	// Adición y Asignación
	template<typename T>
	steady_clock_& operator+=(T value) {
		_runvalueback = _runvalue;
		_runvalue += static_cast<double>(value);
		return *this;
	}

	// Sustracción y Asignación
	template<typename T>
	steady_clock_& operator-=(T value) {
		_runvalueback = _runvalue;
		_runvalue -= static_cast<double>(value);
		return *this;
	}

	// Multiplicación y Asignación
	template<typename T>
	steady_clock_& operator*=(T value) {
		_runvalueback = _runvalue;
		_runvalue *= static_cast<double>(value);
		return *this;
	}

	// División y Asignación
	template<typename T>
	steady_clock_& operator/=(T value) {
		_runvalueback = _runvalue;
		_runvalue /= static_cast<double>(value);
		return *this;
	}

	// Incremento
	steady_clock_& operator++() { // Pre-incremento
		_runvalueback = _runvalue;
		++_runvalue;
		return *this;
	}

	steady_clock_ operator++(int) {
		steady_clock_ temp = *this;
		_runvalueback = _runvalue;
		++_runvalue;
		return temp;
	}

	// Incremento
	steady_clock_& operator--() { // Pre-incremento
		_runvalueback = _runvalue;
		--_runvalue;
		return *this;
	}

	steady_clock_ operator--(int) {
		steady_clock_ temp = *this;
		_runvalueback = _runvalue;
		--_runvalue;
		return temp;
	}

	// Módulo
	double operator%(int element) const
	{
		return std::fabs(fmod(_runvalue, (double)element));
	}

	// Igualdad
	template<typename T>
	bool operator==(T value) const {
		return numeral(value);
	}

	// Igualdad
	template<typename T>
	bool operator!=(T value) const {
		return !numeral(value);
	}

	// Menor o igual que
	template<typename T>
	bool operator<=(T value) const {
		return _runvalue <= value;
	}

	// Mayor o igual que
	template<typename T>
	bool operator>=(T value) const {
		return _runvalue >= value;
	}

	// Menor que
	template<typename T>
	bool operator<(T value) const {
		return _runvalue < value;
	}

	// Mayor que
	template<typename T>
	bool operator>(T value) const {
		return _runvalue > value;
	}

	bool numeral(int element) const;
	bool duration(int element);
	bool residual_duration(int element, int time);

	int factor_res(int time);
};

class CTimer;
class csteady_clock
{
public:
	csteady_clock();
	virtual~csteady_clock();

public:
#ifdef PROCESS_FPS_NEW
	bool tradicional;
	void setTradicional(bool set) {
		tradicional = set;
	};
#endif // PROCESS_FPS_NEW


	bool CheckNormalizer();
	int GetLimitFps();
	void SetLimitFps(int fpsLimit);
	int Getframe_per_second();
	double GetNormalizerFps();
	double GetVisualNormalizerFps();
	double GetDeltAccumulated();
	
	void normalizefps();
	void LoadInformationFps();
	std::chrono::steady_clock::time_point GetthreadTime();
	uintmax_t thread_sleep(const std::chrono::steady_clock::time_point thread_tick);

	bool rand_calc_check(int fr);
	void runtime_send_ping();
	void runtime_recv_ping(int ping_id);
	int get_ping_time();
	static csteady_clock* Instance();
private:
	int fpsLimit;
	int32_t counterframe;
	bool normal_check;

	double fpsNormalizer;
	double speedNormalizer;
	double visualNormalizer;
	double deltaAccumulated;
	CheckerTime frame_limit;
	std::chrono::steady_clock::time_point mainthread;
	std::chrono::steady_clock::time_point lastFrameEnd;
	double outsideFrameTime;
	bool frameTimingInitialized;

	double save_time;
	CTimer* threadTime;
	//std::chrono::steady_clock::time_point last_save_time;
	std::chrono::steady_clock::time_point last_check_time;

	int ping_reg_id;
	long long ping_time;
	std::unordered_map<int, std::chrono::high_resolution_clock::time_point> pingMap;
};


extern uint32_t standlimit(uint32_t x);
extern double ReduceWithDelta(double x = 1.0);
extern double AdvanceByDelta(double x = 1.0);
extern double timeNormalizer(double x = 1.0);
