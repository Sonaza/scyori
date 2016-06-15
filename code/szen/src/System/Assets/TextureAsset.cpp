#include <szen/System/Assets/TextureAsset.hpp>
#include <szen/System/ErrorStream.hpp>

using namespace sz;

////////////////////////////////////////////////////
TextureAsset::TextureAsset(const std::string &path) :
	TAsset(path)
{
}

////////////////////////////////////////////////////
TextureAsset::~TextureAsset()
{
}

////////////////////////////////////////////////////
bool TextureAsset::loadAsset()
{
	assert(!m_loaded && "Texture has already been loaded");
	assert(!m_filename.empty() && "Must have file name");

	m_asset = new(std::nothrow) sf::Texture();
	assert(m_asset && "Allocation failed");

	if(!m_asset->loadFromFile(m_filename))
	{
		szerr << "Unable to open texture: " << m_filename << ErrorStream::error;

		return false;
	}
	
	// Choose linear filtering, looks a lot better!
	m_asset->setSmooth(true);

	m_loaded = true;

	return true;
}

////////////////////////////////////////////////////
void TextureAsset::setSmooth(const bool v)
{
	m_asset->setSmooth(v);
}
