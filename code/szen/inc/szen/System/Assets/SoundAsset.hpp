#ifndef SZEN_SOUND_ASSET_HPP
#define SZEN_SOUND_ASSET_HPP

#include <sfml/Audio.hpp>
#include <szen/System/Assets/TAsset.hpp>

#include <szen/System/Types.hpp>

#include <map>

namespace sz
{

	typedef uint32 SoundChannel;
	typedef std::map<const SoundChannel, sf::Sound> SoundChannels;

	class SoundAsset : public TAsset<sf::SoundBuffer>
	{
	public:
		SoundAsset(const std::string &path);
		~SoundAsset();

		bool loadAsset();

		void updateChannels();

		uint32 getActiveChannels();

		SoundChannel play(float volume = 100.f);

		void setLooping(SoundChannel channel, const bool looping);
		void setPitch(SoundChannel channel, float pitch);
		void setPanning(SoundChannel channel, const sf::Vector3f position);

		void pause(SoundChannel channel);
		void pauseAll();
		
		void stop(SoundChannel channel);
		void stopAll();

		bool isPlaying();
		bool isPlaying(SoundChannel channel);

		bool isChannelActive(SoundChannel channel);

	private:

		SoundChannels	m_soundChannels;

	};
	
}

#endif // SZEN_SOUND_ASSET_HPP
