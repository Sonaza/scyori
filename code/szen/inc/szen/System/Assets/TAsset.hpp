#ifndef SZEN_TASSET_HPP
#define SZEN_TASSET_HPP

#include <string>
#include <cassert>

namespace sz
{
	
	namespace priv
	{
		extern size_t	m_nextGuid;
	}

	template <class Type>
	class TAsset
	{
	public:
		explicit TAsset(const std::string &path) :
		m_asset(NULL),
			m_loaded(false),
			m_boundToScene(false),
			m_filename(path)
		{
			m_guid = priv::m_nextGuid++;
		}

		~TAsset()
		{
			if(m_loaded) unloadAsset();
		}


		virtual bool loadAsset() =0;
		void unloadAsset();

		inline Type* getAsset() const { return m_asset; }

		inline void setSceneBound(const bool v) { m_boundToScene = v; }
		inline bool isSceneBound() const { return m_boundToScene; }

		inline size_t getGuid() const { return m_guid; }

	protected:

		Type*			m_asset;

		// Tells if asset is loaded and ready to use
		bool			m_loaded;

		std::string		m_filename;

		// States whether an asset is only needed for one scene
		// and can be freed once scene is changed
		bool			m_boundToScene;

		// Asset global unique id
		size_t			m_guid;

	};
	
	////////////////////////////////////////////////////
	template <class Type>
	void TAsset<Type>::unloadAsset()
	{
		if(!m_loaded) return;

		delete m_asset;
		m_asset = NULL;

		m_loaded = false;
	}

}

#endif // SZEN_TASSET_HPP
