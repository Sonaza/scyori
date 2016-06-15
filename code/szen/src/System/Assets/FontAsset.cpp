#include <szen/System/Assets/FontAsset.hpp>
#include <szen/System/ErrorStream.hpp>

using namespace sz;

////////////////////////////////////////////////////
FontAsset::FontAsset(const std::string &path) :
	TAsset(path)
{
	
}

////////////////////////////////////////////////////
FontAsset::~FontAsset()
{
	
}

////////////////////////////////////////////////////
bool FontAsset::loadAsset()
{
	assert(!m_loaded && "Shader has already been loaded");
	assert(!m_filename.empty() && "Must have file name");

	m_asset = new(std::nothrow) sf::Font();
	assert(m_asset && "Allocation failed");

	if(!m_asset->loadFromFile(m_filename))
	{
		szerr << "Unable to open shader file: " << m_filename << ErrorStream::error;

		return false;
	}

	m_loaded = true;

	return true;
}
