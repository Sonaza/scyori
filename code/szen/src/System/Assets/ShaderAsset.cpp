#include <szen/System/Assets/ShaderAsset.hpp>
#include <szen/System/ErrorStream.hpp>

#include <szen/System/Window.hpp>

using namespace sz;

////////////////////////////////////////////////////
ShaderAsset::ShaderAsset(const std::string &path) :
	TAsset(path)
{
}

////////////////////////////////////////////////////
ShaderAsset::~ShaderAsset()
{
}

////////////////////////////////////////////////////
bool ShaderAsset::loadAsset()
{
	assert(!m_loaded && "Shader has already been loaded");
	assert(!m_filename.empty() && "Must have file name");

	m_asset = new(std::nothrow) sf::Shader();
	assert(m_asset && "Allocation failed");

	if(!m_asset->loadFromFile(m_filename, sf::Shader::Fragment))
	{
		szerr << "Unable to open shader file: " << m_filename << ErrorStream::error;

		return false;
	}

	m_asset->setParameter("u_resolution", static_cast<sf::Vector2f>(Window::getSize()));
	m_asset->setParameter("u_vresolution", static_cast<sf::Vector2f>(Window::getVirtualSize()));

	m_asset->setParameter("u_texture", sf::Shader::CurrentTexture);

	m_loaded = true;

	return true;
}
