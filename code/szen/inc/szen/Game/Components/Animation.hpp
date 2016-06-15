#ifndef SZEN_ANIMATION_HPP
#define SZEN_ANIMATION_HPP

#include <string>
#include <map>
#include <cassert>

#include <SFML/Graphics.hpp>
#include <szen/Game/Component.hpp>

#include <szen/System/PausableClock.hpp>

#include <szen/System/Types.hpp>

namespace sz
{

	class Renderer;

	struct AnimationSequence
	{
		AnimationSequence();
		AnimationSequence(int32 start, int32 end, int32 frametime);

		int32	start, end;
		int32	frametime;
		bool	looping;

		std::string	nextAnimation;
	};

	typedef std::map<const std::string, AnimationSequence>	SequenceList;

	class Animation : public Component
	{
	public:
		Animation(sf::Vector2u framesize);
		virtual ~Animation();

		bool isPlaying();
		bool isPaused();
		bool isStopped();

		std::string getAnimationTag();

		float getCurrentFPS();

		void setCurrentFrame(int32);
		int32 getCurrentFrame();

		void attached();
		void update();
		void parsePrefab(json::Value&);

		void updateRegion();

		void play();
		void play(const std::string &sequence);
		void play(int32 start, int32 end, int32 fps);

		void pause();

		void stop();

		void defineAnimation(const std::string &tag, int32 start, int32 end, int32 fps, bool looping = false, const std::string &nextAnimation = "");

	private:

		Renderer*			p_renderer;

		enum PlayState
		{
			Stopped,
			Paused,
			Playing
		};

		PlayState			m_playState;

		AnimationSequence	m_currentSequence;
		SequenceList		m_animations;

		int32				m_frameID;
		int32				m_framesPerRow;
		sf::Vector2u		m_framesize;

		PausableClock		m_frametimer;

		std::string			m_animationTag;

	};
	
}

#endif // SZEN_ANIMATION_HPP
