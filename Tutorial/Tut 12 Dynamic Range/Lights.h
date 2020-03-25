/** Copyright (C) 2010-2012 by Jason L. McKesson **/
/** This file is licensed under the MIT License. **/



#ifndef LIGHTS_H
#define LIGHTS_H

#include <map>
#include "../framework/Timer.h"
#include <glm/glm.hpp>

typedef std::pair<float, float> MaxIntensityData;
typedef std::vector<MaxIntensityData> MaxIntensityVector;
typedef std::pair<glm::vec4, float> LightVectorData;
typedef std::vector<LightVectorData> LightVector;

namespace Framework
{
	template<typename ValueType>
	class WeightedLinearInterpolator
	{
	public:
		typedef ValueType value_type;

		size_t NumSegments() const {return m_values.empty() ? 0 : m_values.size() - 1;}

		ValueType Interpolate(float fAlpha) const
		{
			if(m_values.empty())
				return ValueType();
			if(m_values.size() == 1)
				return m_values[0].data;

			//Find which segment we are within.
			size_t segment = 1;
			for(; segment < m_values.size(); ++segment)
			{
				if(fAlpha < m_values[segment].weight)
					break;
			}

			if(segment == m_values.size())
				return m_values.back().data;

			float sectionAlpha = fAlpha - m_values[segment - 1].weight;
			sectionAlpha /= m_values[segment].weight - m_values[segment - 1].weight;

			float invSecAlpha = 1.0f - sectionAlpha;

			return m_values[segment - 1].data * invSecAlpha + m_values[segment].data * sectionAlpha;
		}

	protected:
		WeightedLinearInterpolator() {}

		struct Data
		{
			ValueType data;
			float weight;
		};

		std::vector<Data> m_values;
	};

	template<typename ValueType>
	class TimedLinearInterpolator : public WeightedLinearInterpolator<ValueType>
	{
	public:

		template<typename BidirectionalRange>
		void SetValues(const BidirectionalRange &data, bool isLooping = true)
		{
			this->m_values.clear();
			typename BidirectionalRange::const_iterator curr = data.begin();
			typename BidirectionalRange::const_iterator final = data.end();
			for(; curr != final; ++curr)
			{
				typename WeightedLinearInterpolator<ValueType>::Data currData;
				currData.data = GetValue(*curr);
				currData.weight = GetTime(*curr);

				assert(0.0f <= currData.weight && currData.weight <= 1.0f);
				this->m_values.push_back(currData);
			}

			if(isLooping && !this->m_values.empty())
				this->m_values.push_back(this->m_values[0]);

			//Ensure first is weight 0, and last is weight 1.
			if(!this->m_values.empty())
			{
				this->m_values.front().weight = 0.0f;
				this->m_values.back().weight = 1.0f;
			}
		}

		glm::vec4 GetValue(const LightVectorData &data) {return data.first;}
		float GetTime(const LightVectorData &data) {return data.second;}
		float GetValue(const MaxIntensityData &data) {return data.first;}
		float GetTime(const MaxIntensityData &data) {return data.second;}
	protected:
	};


	template<typename ValueType>
	class LinearInterpolator : public WeightedLinearInterpolator<ValueType>
	{
	public:
		typedef ValueType value_type;

		template<typename BidirectionalRange>
		void SetValues(const BidirectionalRange &data, bool isLooping = true)
		{
			this->m_values.clear();
			int iNumValues = 0;
			typename BidirectionalRange::const_iterator curr = data.begin();
			typename BidirectionalRange::const_iterator final = data.end();
			for(; curr != final; ++curr)
			{
				typename WeightedLinearInterpolator<ValueType>::Data currData;
				currData.data = *curr;
				currData.weight = 0.0f;
				this->m_values.push_back(currData);

				iNumValues++;
			}

			if(isLooping && !this->m_values.empty())
			{
				this->m_values.push_back(this->m_values.back());
				++iNumValues;
			}

			//Compute weights.
			for(size_t valIx = 0; valIx < this->m_values.size(); ++valIx)
			{
				this->m_values[valIx].weight = valIx / (float)(iNumValues - 1);
			}
		}
	private:
	};

	/**
	\brief Interpolates with a constant velocity between positions.

	This interpolator maps a range of [0, 1) onto a set of values. However, it takes the distance
	between these values. There must be a free function called "distance" which takes two ValueType's and
	returns a float distance between them.

	The idea is that, if you add 0.1 to your alpha value, you will always get a movement of the same distance.
	Not necessarily between the initial and final points, but the object will have moved at the same
	speed along the path.
	**/
	template<typename ValueType>
	class ConstVelLinearInterpolator : public WeightedLinearInterpolator<ValueType>
	{
	public:
		typedef ValueType value_type;

		explicit ConstVelLinearInterpolator()
			: m_totalDist(0.0f)
		{}

		template<typename BidirectionalRange>
		void SetValues(const BidirectionalRange &data, bool isLoop = true)
		{
			this->m_values.clear();

			typename BidirectionalRange::const_iterator curr = data.begin();
			typename BidirectionalRange::const_iterator last = data.end();
			for(; curr != last; ++curr)
			{
				typename WeightedLinearInterpolator<ValueType>::Data currData;
				currData.data = *curr;
				currData.weight = 0.0f;
				this->m_values.push_back(currData);
			}

			if(isLoop)
			{
				typename WeightedLinearInterpolator<ValueType>::Data currData;
				currData.data = *data.begin();
				currData.weight = 0.0f;
				this->m_values.push_back(currData);
			}

			//Compute the distances of each segment.
			m_totalDist = 0.0f;
			for(size_t iLoop = 1; iLoop < this->m_values.size(); ++iLoop)
			{
				m_totalDist += distance(this->m_values[iLoop - 1].data,
				    this->m_values[iLoop].data);
				this->m_values[iLoop].weight = m_totalDist;
			}

			//Compute the alpha value that represents when to use this segment.
			for(size_t iLoop = 1; iLoop < this->m_values.size(); ++iLoop)
			{
				this->m_values[iLoop].weight /= m_totalDist;
			}
		}
		
		float distance(const glm::vec3 &lhs, const glm::vec3 &rhs)
		{
			return glm::length(rhs - lhs);
		}

		float Distance() const {return m_totalDist;}

	private:
		float m_totalDist;
	};
}



struct PerLight
{
	glm::vec4 cameraSpaceLightPos;
	glm::vec4 lightIntensity;
};

const int NUMBER_OF_LIGHTS = 5;
const int NUMBER_OF_POINT_LIGHTS = NUMBER_OF_LIGHTS - 2;

struct LightBlock
{
	glm::vec4 ambientIntensity;
	float lightAttenuation;
	float padding[3];
	PerLight lights[NUMBER_OF_LIGHTS];
};

struct LightBlockHDR
{
	glm::vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float padding[2];
	PerLight lights[NUMBER_OF_LIGHTS];
};

struct LightBlockGamma
{
	glm::vec4 ambientIntensity;
	float lightAttenuation;
	float maxIntensity;
	float gamma;
	float padding;
	PerLight lights[NUMBER_OF_LIGHTS];
};

struct SunlightValue
{
	float normTime;
	glm::vec4 ambient;
	glm::vec4 sunlightIntensity;
	glm::vec4 backgroundColor;
};

struct SunlightValueHDR
{
	float normTime;
	glm::vec4 ambient;
	glm::vec4 sunlightIntensity;
	glm::vec4 moonlightIntensity;
	glm::vec4 backgroundColor;
	float maxIntensity;
};

enum TimerTypes
{
	TIMER_SUN,
	TIMER_LIGHTS,
	TIMER_ALL,
	NUM_TIMER_TYPES,
};

class LightManager
{
public:
	LightManager();

	void SetSunlightValues(SunlightValue *pValues, int iSize);
	void SetSunlightValues(SunlightValueHDR *pValues, int iSize);

	void UpdateTime();
	void TogglePause(TimerTypes eTimer);
	void SetPause(TimerTypes eTimer, bool pause = true);
	bool IsPaused(TimerTypes eTimer) const;

	void RewindTime(TimerTypes eTimer, float secRewind);
	void FastForwardTime(TimerTypes eTimer, float secRewind);

	LightBlock GetLightInformation(const glm::mat4 &worldToCameraMat) const;
	LightBlockHDR GetLightInformationHDR(const glm::mat4 &worldToCameraMat) const;
	LightBlockGamma GetLightInformationGamma(const glm::mat4 &worldToCameraMat) const;

	glm::vec4 GetBackgroundColor() const;
	float GetMaxIntensity() const;

	glm::vec4 GetSunlightDirection() const;
	glm::vec4 GetSunlightIntensity() const;
	glm::vec4 GetMoonlightIntensity() const;

	int GetNumberOfPointLights() const;
	glm::vec3 GetWorldLightPosition(int iLightIx) const;
	void SetPointLightIntensity(int iLightIx, const glm::vec4 &intensity);
	glm::vec4 GetPointLightIntensity(int iLightIx) const;

	void CreateTimer(const std::string &timerName, Framework::Timer::Type eType, float fDuration);
	float GetTimerValue(const std::string &timerName) const;
	float GetSunTime() const;

private:
	typedef Framework::ConstVelLinearInterpolator<glm::vec3> LightInterpolator;
	typedef std::map<std::string, Framework::Timer> ExtraTimerMap;

	Framework::Timer m_sunTimer;
	Framework::TimedLinearInterpolator<glm::vec4> m_ambientInterpolator;
	Framework::TimedLinearInterpolator<glm::vec4> m_backgroundInterpolator;
	Framework::TimedLinearInterpolator<glm::vec4> m_sunlightInterpolator;
	Framework::TimedLinearInterpolator<glm::vec4> m_moonlightInterpolator;
	Framework::TimedLinearInterpolator<float> m_maxIntensityInterpolator;

	std::vector<LightInterpolator> m_lightPos;
	std::vector<glm::vec4> m_lightIntensity;
	std::vector<Framework::Timer> m_lightTimers;
	ExtraTimerMap m_extraTimers;
};

#endif //LIGHTS_H
