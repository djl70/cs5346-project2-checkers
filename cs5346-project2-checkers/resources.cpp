#include "resources.h"

namespace resources
{
	const std::unordered_map<std::string, std::string> textureFiles = {
		{ "plain_background", "res/background.png" },
		{ "background", "res/checkers_game_field.png" },
		{ "red_king", "res/checker_piece_cream_king.png" },
		{ "black_king", "res/checker_piece_brown_king.png" },
		{ "red_man", "res/checker_piece_cream.png" },
		{ "black_man", "res/checker_piece_brown.png" },
		{ "title", "res/title.png" },
		{ "button_autoplay", "res/button_autoplay.png" },
		{ "button_autoplay_hover", "res/button_autoplay_hover.png" },
		{ "button_autoplay_press", "res/button_autoplay_press.png" },
		{ "button_multiplayer", "res/button_multiplayer.png" },
		{ "button_multiplayer_hover", "res/button_multiplayer_hover.png" },
		{ "button_multiplayer_press", "res/button_multiplayer_press.png" },
		{ "button_singleplayer", "res/button_singleplayer.png" },
		{ "button_singleplayer_hover", "res/button_singleplayer_hover.png" },
		{ "button_singleplayer_press", "res/button_singleplayer_press.png" }
	};
	std::unordered_map<std::string, sf::Texture> textures;

	const std::unordered_map<std::string, std::string> soundFiles = {
		{ "sound_move", "res/snap.wav" },
		{ "sound_jump", "res/pop_meow.wav" }
	};
	std::unordered_map<std::string, sf::SoundBuffer> sounds;

	sf::RenderWindow* pWindow;

	bool loadResources()
	{
		bool success;

		for (const auto& pair : textureFiles)
		{
			success &= textures[pair.first].loadFromFile(pair.second);
		}
		for (const auto& pair : soundFiles)
		{
			success &= sounds[pair.first].loadFromFile(pair.second);
		}

		return success;
	}
}