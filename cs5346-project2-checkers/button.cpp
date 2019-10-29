#include "button.h"

#include "config.h"

Button::Button(const sf::FloatRect& rect, const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& press, const sf::SoundBuffer& clickSound)
	: m_state{ kDefault }
	, m_rect{ rect }
	, m_kDefaultTexture{ normal }
	, m_kHoveredTexture{ hover }
	, m_kPressedTexture{ press }
	, m_soundBuffer{ clickSound }
{
	m_sprite.setTexture(m_kDefaultTexture);
	m_sprite.setPosition({ m_rect.left, m_rect.top });
	m_sprite.setScale({ config::kScaling, config::kScaling });

	// For some reason, we can't call setBuffer here, otherwise it'll throw an exception on close.
	// So, we store the buffer in a variable and set it later on.
}

bool Button::update(const sf::Event& event, const sf::Vector2f& mousePositionInWindow)
{
	bool isButtonClicked = false;

	// Default state can transition to hovered if mouse position is over the button
	// Default state can transition to pressed if mouse position is over the button AND left mouse button is pressed
	if (m_state == kDefault)
	{
		if (m_rect.contains(mousePositionInWindow))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kPressed);
			}
			else
			{
				changeState(kHovered);
			}
		}
	}
	// Hovered state can transition to default if mouse position is not over the button
	// Hovered state can transition to pressed if mouse position is over the button AND left mouse button is pressed
	else if (m_state == kHovered)
	{
		if (m_rect.contains(mousePositionInWindow))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kPressed);
			}
		}
		else
		{
			changeState(kDefault);
		}
	}
	// Pressed state can transition to default if mouse position is not over the button
	// Pressed state can transition to hovered if mouse position is over the button AND left mouse button not pressed (sets clicked to true)
	else if (m_state == kPressed)
	{
		if (m_rect.contains(mousePositionInWindow))
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				changeState(kHovered);
				isButtonClicked = true;
				// We have to set the buffer outside of the constructor for some reason, so we set it here.
				m_clickSound.setBuffer(m_soundBuffer);
				m_clickSound.play();
				// We have to wait until the sound finishes, then reset the buffer for some reason. Otherwise, it'll throw an exception on close.
				// And we can't call this from the destructor.
				while (m_clickSound.getStatus() == sf::Sound::Status::Playing)
				{

				}
				m_clickSound.resetBuffer();
			}
		}
		else
		{
			changeState(kDefault);
		}
	}

	return isButtonClicked;
}

void Button::render(sf::RenderWindow* pWindow) const
{
	pWindow->draw(m_sprite);
}

void Button::changeState(ButtonState newState)
{
	m_state = newState;
	switch (newState)
	{
	case kDefault:
		m_sprite.setTexture(m_kDefaultTexture);
		break;
	case kHovered:
		m_sprite.setTexture(m_kHoveredTexture);
		break;
	case kPressed:
		m_sprite.setTexture(m_kPressedTexture);
		break;
	}
}