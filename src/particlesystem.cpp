#include "particle.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <sstream>
#include <iostream>

ParticleSystem::ParticleSystem( int width, int height ) :
    m_transparent( sf::Color::Transparent )
{
    m_image.resize( sf::Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height)), m_transparent );
    if (!m_texture.loadFromImage(m_image))
    {
        std::cerr << "ParticleSystem: Failed to load texture from image in constructor!" << std::endl;
        // Potentially throw an exception or set an error state
    }
    m_sprite = std::make_unique<sf::Sprite>(m_texture);
    m_position.x    = 0.5f * width;
    m_position.y    = 0.5f * height;
    m_particleSpeed = 100.0f;
    m_dissolve  = false;
    m_dissolutionRate = 4;
    m_shape     = DP::CIRCLE;
}

ParticleSystem::~ParticleSystem()
{
    for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it )
        delete *it;

    m_particles.clear();
}

void ParticleSystem::fuel( int particles )
{
    for( int i = 0; i < particles; ++i )
    {
        Particle *p = new Particle;
        p->pos.x = m_randomizer.rnd( 0.0f, static_cast<float>(m_image.getSize().x -1) );
        p->pos.y = m_randomizer.rnd( 0.0f, static_cast<float>(m_image.getSize().y -1) );
        p->vel.x = m_randomizer.rnd( -m_particleSpeed, m_particleSpeed );
        p->vel.y = m_randomizer.rnd( -m_particleSpeed, m_particleSpeed );
        p->color = sf::Color( m_randomizer.rnd(0,255), m_randomizer.rnd(0,255), m_randomizer.rnd(0,255), 255 );

        m_particles.push_back(p);
    }
}

void ParticleSystem::update()
{
    sf::Time elapsed = m_clock.restart();
    float time = elapsed.asSeconds();

    for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); )
    {
        Particle *p = *it;

        p->vel.x += m_gravity.x * time;
        p->vel.y += m_gravity.y * time;

        p->pos.x += p->vel.x * time;
        p->pos.y += p->vel.y * time;

        if( m_dissolve && p->color.a > m_dissolutionRate )
            p->color.a -= m_dissolutionRate;
        else if( m_dissolve && p->color.a <= m_dissolutionRate )
            p->color.a = 0;

        unsigned int imgWidth = m_image.getSize().x;
        unsigned int imgHeight = m_image.getSize().y;

        if( p->color.a == 0 ||
            p->pos.x >= static_cast<float>(imgWidth) || p->pos.x < 0 ||
            p->pos.y >= static_cast<float>(imgHeight) || p->pos.y < 0 )
        {
            delete p;
            it = m_particles.erase(it);
        }
        else
            ++it;
    }
}

void ParticleSystem::render()
{
    for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it )
    {
        Particle* p = *it;
        sf::Vector2u coords(static_cast<unsigned int>(p->pos.x),
                              static_cast<unsigned int>(p->pos.y));
        if (coords.x < m_image.getSize().x && coords.y < m_image.getSize().y)
        {
            m_image.setPixel( coords, p->color );
        }
    }
    m_texture.update(m_image);
}

void ParticleSystem::remove()
{
    for( ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it )
    {
        Particle* p = *it;
        sf::Vector2u coords(static_cast<unsigned int>(p->pos.x),
                              static_cast<unsigned int>(p->pos.y));
        if (coords.x < m_image.getSize().x && coords.y < m_image.getSize().y)
        {
            m_image.setPixel( coords, m_transparent );
        }
    }
}

std::string ParticleSystem::getNumberOfParticlesString()
{
    std::ostringstream oss;
    oss << m_particles.size();
    return oss.str();
}
