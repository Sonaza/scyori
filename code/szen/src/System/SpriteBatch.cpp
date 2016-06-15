#include <szen/System/SpriteBatch.hpp>

#include <szen/System/Assets/TextureAsset.hpp>
#include <szen/System/Assets/ShaderAsset.hpp>

#include <thor/Particles.hpp>

using namespace sz;

BatchDataList	SpriteBatch::m_batchData;

////////////////////////////////////////////////////
SpriteBatch::SpriteBatch()
{
	
}

////////////////////////////////////////////////////
SpriteBatch::~SpriteBatch()
{
	
}

////////////////////////////////////////////////////
BatchData::BatchData(size_t layer, const sf::Texture* texture, const sf::Shader* shader, sf::BlendMode blendmode) :
	layer		(layer),
	texture		(texture),
	shader		(shader),
	particles	(NULL),
	blendmode	(blendmode),

	type		(Vertices)
{
}

////////////////////////////////////////////////////
BatchData::BatchData(size_t layer, thor::ParticleSystem* system, const sf::Shader* shader, sf::BlendMode blendmode) :
	layer		(layer),
	texture		(NULL),
	shader		(shader),
	particles	(system),
	blendmode	(blendmode),

	type		(Particles)
{
}

////////////////////////////////////////////////////
BatchData::BatchData(size_t layer, sf::Text* text, sf::BlendMode blendmode) :
	layer		(layer),
	texture		(NULL),
	shader		(NULL),
	particles	(NULL),
	text		(sf::Text(*text)),
	blendmode	(blendmode),

	type		(Text)
{
}

////////////////////////////////////////////////////
void SpriteBatch::init()
{
	m_batchData.reserve(100);
}

////////////////////////////////////////////////////
void SpriteBatch::clear()
{
	m_batchData.clear();
}

////////////////////////////////////////////////////
void SpriteBatch::append(const sf::Vertex* quad, sf::Uint32 layer, const sf::Texture* textureasset, ShaderAsset* shaderasset, sf::BlendMode blendmode)
{
	const sf::Texture* texture = textureasset != NULL ? textureasset : NULL;
	const sf::Shader* shader = shaderasset != NULL ? shaderasset->getAsset() : NULL;

	BatchDataList::iterator data = std::find_if(m_batchData.begin(), m_batchData.end(),
		[&](BatchData& d)
		{
			return d.particles == NULL && d.layer == layer && d.texture == texture && d.shader == shader && d.blendmode == blendmode;
		});
	
	if(data != m_batchData.end())
	{
		BatchData* d = &*data;

		d->vertices.append(quad[0]);
		d->vertices.append(quad[1]);
		d->vertices.append(quad[2]);
		d->vertices.append(quad[3]);
	}
	else
	{
		BatchData d(layer, texture, shader, blendmode);

		d.vertices.setPrimitiveType(sf::Quads);

		d.vertices.append(quad[0]);
		d.vertices.append(quad[1]);
		d.vertices.append(quad[2]);
		d.vertices.append(quad[3]);

		m_batchData.push_back(d);
	}
}

////////////////////////////////////////////////////
void SpriteBatch::append(thor::ParticleSystem* system, sf::Uint32 layer, ShaderAsset* shader, sf::BlendMode blendmode)
{
	m_batchData.push_back(
		BatchData(layer, system, shader ? shader->getAsset() : NULL, blendmode)
	);
}

////////////////////////////////////////////////////
void SpriteBatch::append(sf::Text* text, sf::Uint32 layer, sf::BlendMode blendmode)
{
	m_batchData.push_back(
		BatchData(layer, text, blendmode)
	);
}

////////////////////////////////////////////////////
void SpriteBatch::draw(sf::RenderTarget& target)
{
	for(BatchDataList::iterator it = m_batchData.begin(); it != m_batchData.end(); ++it)
	{
		//BatchData* data = &it->second;
		BatchData* data = &*it;

		sf::RenderStates states;
		if(data->texture)	states.texture	= data->texture;
		if(data->shader)	states.shader	= data->shader;
		states.blendMode	= data->blendmode;
		//states.blendMode	= sf::BlendAlpha;

		switch(data->type)
		{
			case BatchData::Vertices:	target.draw(data->vertices, states);	break;
			case BatchData::Particles:	target.draw(*data->particles, states);	break;
			case BatchData::Text:		target.draw(data->text, states);		break;
		}
	}
}