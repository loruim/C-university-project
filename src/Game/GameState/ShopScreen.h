#pragma once

#include "IGameState.h"

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Sprite;
}

class ShopScreen : public IGameState
{
public:
	static constexpr unsigned int BLOCK_SIZE = 16;
	static constexpr unsigned int STARTSCREEN_WIDTH = 15 * BLOCK_SIZE * 2;
	static constexpr unsigned int STARTSCREEN_HEIGHT = 14 * BLOCK_SIZE * 2;

	ShopScreen(const std::vector<std::string>& shopScreenDescription);
	virtual void render() const override;
	virtual void update(const double delta) override;

	virtual unsigned int getStateWidth() const override;
	virtual unsigned int getStateHeight() const override;

private:
	std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> m_sprites;
};