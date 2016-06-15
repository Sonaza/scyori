#include <szen/Game/Components/Animation.hpp>

#include <szen/Game/Components/Renderer.hpp>

using namespace sz;

////////////////////////////////////////////////////
Animation::Animation(sf::Vector2u framesize) :
	m_frameID		(0),
	m_framesPerRow	(0),
	m_framesize		(framesize),
	m_playState		(Stopped),
	p_renderer		(NULL)
{
	m_componentOrder = 15;
}

////////////////////////////////////////////////////
Animation::~Animation()
{
}

////////////////////////////////////////////////////
AnimationSequence::AnimationSequence() :
	start		(0),
	end			(0),
	frametime	(0),
	looping		(false)
{
}

////////////////////////////////////////////////////
AnimationSequence::AnimationSequence(int32 start, int32 end, int32 frametime) :
	start		(start),
	end			(end),
	frametime	(frametime),
	looping		(false)
{
}

////////////////////////////////////////////////////
void Animation::attached()
{
	p_renderer = m_entity->getComponent<Renderer>();

	if(!p_renderer)
	{
		szerr << "Animation component depends on renderer component." << ErrorStream::error;
	}

	if(p_renderer->getTextureSize().x % m_framesize.x == 0)
	{
		m_framesPerRow = p_renderer->getTextureSize().x / m_framesize.x;

		p_renderer->setTextureRect(sf::IntRect(0, 0, m_framesize.x, m_framesize.y));
		//p_renderer->m_sprite->setTextureRect(sf::IntRect(0, 0, m_framesize.x, m_framesize.y));
		//p_renderer->m_sprite->setOrigin(m_framesize.x / 2.f, m_framesize.y / 2.f);
	}
}

////////////////////////////////////////////////////
void Animation::parsePrefab(json::Value& val)
{
	if(val.isMember("sequences") && !val["sequences"].empty())
	{
		json::Value sequences = val["sequences"];

		for(json::Value::iterator it = sequences.begin(); it != sequences.end(); ++it)
		{
			if(!(*it).isMember("start") || !(*it).isMember("end") || !(*it).isMember("fps"))
			{
				szerr << "Animation sequence definition must have start and end frame and fps value." << ErrorStream::error;
				continue;
			}

			sf::Uint32 start	= (*it)["start"].asUInt();
			sf::Uint32 end		= (*it)["end"].asUInt();
			sf::Uint32 fps		= (*it)["fps"].asUInt();

			bool looping		= (*it).get("looping", 0).asBool();
			std::string next	= (*it).get("next", "").asString();

			defineAnimation(it.memberName(), start, end, fps, looping, next);
		}
	}

	if(val.isMember("autoplay"))
	{
		if(val["autoplay"].isString())
		{
			play(val["autoplay"].asString());
		}
	}
}

////////////////////////////////////////////////////
float Animation::getCurrentFPS()
{
	return m_currentSequence.frametime > 0 ? 1000.f / (float)m_currentSequence.frametime : 0.f;
}

////////////////////////////////////////////////////
bool Animation::isPlaying()
{
	return m_playState == Playing;
}

////////////////////////////////////////////////////
bool Animation::isPaused()
{
	return m_playState == Paused;
}

////////////////////////////////////////////////////
bool Animation::isStopped()
{
	return m_playState == Stopped;
}

////////////////////////////////////////////////////
void Animation::setCurrentFrame(int32 frame)
{
	m_frameID = frame;

	updateRegion();
}

////////////////////////////////////////////////////
int32 Animation::getCurrentFrame()
{
	return m_frameID;
}

////////////////////////////////////////////////////
void Animation::update()
{
	if(m_playState == Playing)
	{
		// Advance frames if it's the time
		if(m_frametimer.getElapsedTime().asMilliseconds() >= m_currentSequence.frametime)
		{
			m_frameID += 1;
			
			// Check if animation has reached the end
			if(m_frameID > m_currentSequence.end)
			{
				// Check for looping
				if(m_currentSequence.looping)
				{
					m_frameID = m_currentSequence.start;
				}
				else
				{
					// No looping but is there a next animation?
					if(!m_currentSequence.nextAnimation.empty())
					{
						play(m_currentSequence.nextAnimation);
					}
					else
					{
						m_frameID = m_currentSequence.end;

						// Okay, animation can be stopped
						m_playState = Stopped;
					}
				}
			}

			m_frametimer.restart();
		}
	}

	updateRegion();
}

////////////////////////////////////////////////////
void Animation::updateRegion()
{
	// Calculate texture region offsets
	sf::Vector2i grid(
		m_frameID % m_framesPerRow,
		(int)(m_frameID / static_cast<float>(m_framesPerRow))
		);

	sf::IntRect region(
		grid.x * m_framesize.x,	grid.y * m_framesize.y,
		m_framesize.x, m_framesize.y
		);

	p_renderer->setTextureRect(region);
}

////////////////////////////////////////////////////
void Animation::play()
{
	if(m_playState == Stopped)
	{
		m_frameID = m_currentSequence.start;
		m_frametimer.restart();
	}
	else if(m_playState == Paused)
	{
		m_frametimer.resume();
	}

	m_playState = Playing;
}

////////////////////////////////////////////////////
void Animation::play(int32 start, int32 end, int32 fps)
{
	assert(fps != 0);

	m_playState = Playing;

	m_frameID = start;
	
	// Update current sequence
	// We don't care if we lose a bit of precision by not doing float division
	m_currentSequence = AnimationSequence(start, end, 1000 / fps);

	m_frametimer.restart();

	m_animationTag.clear();
}

////////////////////////////////////////////////////
void Animation::play(const std::string &sequence)
{
	assert(!sequence.empty());
	assert(!m_animations.empty());

	SequenceList::iterator anim = m_animations.find(sequence);
	if(anim != m_animations.end())
	{
		m_playState = Playing;

		m_currentSequence = anim->second;
		m_frameID = m_currentSequence.start;

		m_frametimer.restart();

		m_animationTag = sequence;
	}
	else
	{
		szerr << "Playing undefined animation: " << sequence << ErrorStream::error;
	}
}

////////////////////////////////////////////////////
void Animation::pause()
{
	m_playState = Paused;
	m_frametimer.pause();
}

////////////////////////////////////////////////////
void Animation::stop()
{
	m_playState = Stopped;
}

////////////////////////////////////////////////////
void Animation::defineAnimation(const std::string &tag, int32 start, int32 end, int32 fps, bool looping, const std::string &nextAnimation)
{
	AnimationSequence animation;

	animation.start			= start;
	animation.end			= end;
	animation.frametime		= 1000 / fps;
	animation.looping		= looping;
	animation.nextAnimation = nextAnimation;

	m_animations.insert(std::make_pair(tag, animation));
}

////////////////////////////////////////////////////
std::string Animation::getAnimationTag()
{
	return !isStopped() ? m_animationTag : "";
}