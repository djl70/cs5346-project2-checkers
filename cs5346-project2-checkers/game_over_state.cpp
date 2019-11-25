#include "game_over_state.h"

#include "config.h"
#include "main_menu_state.h"

GameOverState::GameOverState(ResourceManager* pResources, GameOverCondition condition)
	: m_pResources{ pResources }
{
	m_blackPiece.setColor(kBlack);
	m_blackPiece.setTextures(m_pResources->getTexture("black_man"), m_pResources->getTexture("black_king"));
	m_blackPiece.setPosition({ config::kScreenWidth * 0.5f - config::kSquareWidth - config::kSquareWidth * 0.5f, config::kScreenHeight * 0.5f - config::kSquareWidth * 0.5f });

	m_redPiece.setColor(kRed);
	m_redPiece.setTextures(m_pResources->getTexture("red_man"), m_pResources->getTexture("red_king"));
	m_redPiece.setPosition({ config::kScreenWidth * 0.5f + config::kSquareWidth - config::kSquareWidth * 0.5f, config::kScreenHeight * 0.5f - config::kSquareWidth * 0.5f });

	switch (condition)
	{
	case kRedHasNoPiecesLeft:
		m_blackPiece.promote();
		m_mainText.setString("P1 WINS");
		m_subText.setString("P2 has no pieces remaining");
		break;
	case kRedCannotMove:
		m_blackPiece.promote();
		m_mainText.setString("P1 WINS");
		m_subText.setString("P2 has no valid moves remaining");
		break;
	case kBlackHasNoPiecesLeft:
		m_redPiece.promote();
		m_mainText.setString("P2 WINS");
		m_subText.setString("P1 has no pieces remaining");
		break;
	case kBlackCannotMove:
		m_redPiece.promote();
		m_mainText.setString("P2 WINS");
		m_subText.setString("P1 has no valid moves remaining");
		break;
	case kTurnLimitReached:
		m_mainText.setString("DRAW");
		m_subText.setString("Too many turns without a capture or promotion");
		break;
	case kBoardStateRepetitionLimitReached:
		m_mainText.setString("DRAW");
		m_subText.setString("Too many repetitions of the same board state");
		break;
	}

	m_mainText.setFont(*m_pResources->getFont("font_arial"));
	m_mainText.setCharacterSize(85); // 85 px approx. 64 pt with 96 dpi
	sf::FloatRect textRect = m_mainText.getLocalBounds();
	m_mainText.setOrigin(textRect.left + textRect.width * 0.5f, textRect.top + textRect.height * 0.5f);
	m_mainText.setPosition({ config::kScreenWidth * 0.5f, config::titleRect.top - textRect.height * 0.5f });
	m_mainText.setFillColor(sf::Color::Black);

	m_subText.setFont(*m_pResources->getFont("font_arial"));
	m_subText.setCharacterSize(42); // 42 px approx. 32 pt with 96 dpi
	textRect = m_subText.getLocalBounds();
	m_subText.setOrigin(textRect.left + textRect.width * 0.5f, textRect.top + textRect.height * 0.5f);
	m_subText.setPosition({ config::kScreenWidth * 0.5f, config::titleRect.top + textRect.height });
	m_subText.setFillColor(sf::Color::Black);

	m_menuButton.setRect(config::menuButtonRect);
	m_menuButton.setTexture(kDefault, m_pResources->getTexture("button_menu"));
	m_menuButton.setTexture(kHovered, m_pResources->getTexture("button_menu_hover"));
	m_menuButton.setTexture(kPressed, m_pResources->getTexture("button_menu_press"));

	m_background.setTexture(*m_pResources->getTexture("plain_background"));
	m_background.setScale({ config::kScaling, config::kScaling });
}

GameOverState::~GameOverState()
{

}

void GameOverState::enter()
{

}

BaseState* GameOverState::event()
{
	sf::Vector2f mousePositionInWindow = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_pResources->getWindow()));

	sf::Event event;
	while (m_pResources->getWindow()->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_pResources->getWindow()->close();
			break;
		}

		if (m_menuButton.update(event, mousePositionInWindow))
		{
			m_pResources->playSound("sound_move");
			return new MainMenuState{ m_pResources };
		}
	}

	return nullptr;
}

void GameOverState::render()
{
	m_pResources->getWindow()->clear();

	m_pResources->getWindow()->draw(m_background);
	m_pResources->getWindow()->draw(m_mainText);
	m_pResources->getWindow()->draw(m_subText);
	m_blackPiece.render(m_pResources->getWindow());
	m_redPiece.render(m_pResources->getWindow());
	m_menuButton.render(m_pResources->getWindow());

	m_pResources->getWindow()->display();
}
